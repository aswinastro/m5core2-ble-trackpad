#include <Arduino.h>
#include <M5Core2.h>
#include <BleMouse.h>
#include <esp_bt.h>
#include "Config.h"
#include "CyberUI.h"

// =============================================================================
// GLOBAL OBJECTS & STATE
// =============================================================================
BleMouse bleMouse(BLE_DEVICE_NAME, BLE_MANUFACTURER_NAME, 100);

AppMode currentMode = MODE_TRACKPAD;
bool bleEnabled = true;
bool lastBleConnectionState = false;

// Touch tracking & Tap-to-click state
int lastTouchX = -1;
int lastTouchY = -1;
int touchStartX = -1;
int touchStartY = -1;
uint32_t touchStartTime = 0;
bool isFingerDown = false;
bool hasMovedBeyondTap = false;

// Center Button B Hold Tracker
uint32_t btnB_pressStartTime = 0;
bool btnB_isHolding = false;
bool btnB_longPressHandled = false;

// Battery & Telemetry
int currentBatPct = 100;
float currentBatVolt = 4.1f;
bool isCharging = false;
char btMacStr[20] = "XX:XX:XX:XX:XX:XX";

// Refresh timers
uint32_t lastTelemetryCheck = 0;
uint32_t lastTopBarUpdate = 0;

// =============================================================================
// HARDWARE HELPERS
// =============================================================================

// Triggers the Core2 internal vibration motor on AXP192 LDO3
void triggerHaptic(int durationMs) {
    M5.Axp.SetLDOEnable(3, true);
    delay(durationMs);
    M5.Axp.SetLDOEnable(3, false);
}

// Read Bluetooth MAC address
void readBtMacAddress() {
    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_BT);
    snprintf(btMacStr, sizeof(btMacStr), "%02X:%02X:%02X:%02X:%02X:%02X",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

// Update battery level from AXP192
void updateBatteryTelemetry() {
    currentBatVolt = M5.Axp.GetBatVoltage();
    // Calculate battery percentage from LiPo discharge curve
    if (currentBatVolt >= 4.15f) currentBatPct = 100;
    else if (currentBatVolt <= 3.40f) currentBatPct = 0;
    else currentBatPct = (int)((currentBatVolt - 3.40f) / (4.15f - 3.40f) * 100.0f);

    isCharging = M5.Axp.isCharging();

    // Sync battery level with host computer over BLE HID
    if (bleMouse.isConnected()) {
        bleMouse.setBatteryLevel(currentBatPct);
    }
}

// Switch between Trackpad and Settings modes
void switchToMode(AppMode newMode) {
    currentMode = newMode;
    triggerHaptic(40);

    if (currentMode == MODE_TRACKPAD) {
        CyberUI::init();
        CyberUI::drawCyberGrid();
        CyberUI::drawSciFiCenterTitle();
        CyberUI::drawTopBar(bleMouse.isConnected(), bleEnabled, currentBatPct, isCharging, DEFAULT_SENSITIVITY);
        CyberUI::drawBottomButtonHUD(false, false, false, 0.0f);
        lastTouchX = -1;
        lastTouchY = -1;
        touchStartX = -1;
        touchStartY = -1;
        touchStartTime = 0;
        isFingerDown = false;
        hasMovedBeyondTap = false;
    } else if (currentMode == MODE_SETTINGS) {
        updateBatteryTelemetry();
        CyberUI::drawSettingsScreen(bleEnabled, bleMouse.isConnected(), btMacStr, currentBatPct, currentBatVolt);
    }
}

// Execute safe system power off via AXP192
void executePowerOff() {
    M5.Lcd.fillScreen(0x0000);
    CyberUI::drawCyberBox(10, 80, SCREEN_WIDTH - 20, 80, 8, COLOR_CYBER_RED, 0x0822);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextColor(COLOR_CYBER_RED, 0x0822);
    M5.Lcd.drawString("// POWERING OFF //", SCREEN_WIDTH / 2, 110, 4);
    M5.Lcd.setTextColor(COLOR_CYBER_YELLOW, 0x0822);
    M5.Lcd.drawString("CORE2 TERMINATING...", SCREEN_WIDTH / 2, 140, 2);

    triggerHaptic(150);
    delay(600);
    M5.Axp.PowerOff();
}

// Toggle Bluetooth state
void toggleBluetooth() {
    bleEnabled = !bleEnabled;
    triggerHaptic(50);

    if (bleEnabled) {
        bleMouse.begin();
    } else {
        bleMouse.end();
    }

    CyberUI::drawSettingsScreen(bleEnabled, bleMouse.isConnected(), btMacStr, currentBatPct, currentBatVolt);
}

// =============================================================================
// TRACKPAD INPUT ENGINE (FULL SCREEN TOUCH + TAP-TO-CLICK)
// =============================================================================
void handleTrackpadTouch() {
    Point p = M5.Touch.getPressPoint();
    bool touchDown = (p.x >= 0 && p.y >= 0);

    if (touchDown) {
        // Full screen trackpad touch area (0 <= x < SCREEN_WIDTH, TRACKPAD_Y_MIN <= y < TRACKPAD_Y_MAX)
        if (p.x >= 0 && p.x < SCREEN_WIDTH && p.y >= TRACKPAD_Y_MIN && p.y < TRACKPAD_Y_MAX) {
            if (!isFingerDown) {
                // Initial touch down
                isFingerDown = true;
                lastTouchX = p.x;
                lastTouchY = p.y;
                touchStartX = p.x;
                touchStartY = p.y;
                touchStartTime = millis();
                hasMovedBeyondTap = false;
            } else {
                // Continuous drag movement
                int dx = p.x - lastTouchX;
                int dy = p.y - lastTouchY;

                // Track total displacement from initial touch point to distinguish tap vs drag
                int totalDx = p.x - touchStartX;
                int totalDy = p.y - touchStartY;
                if ((totalDx * totalDx + totalDy * totalDy) > (TAP_MAX_DISTANCE_PX * TAP_MAX_DISTANCE_PX)) {
                    hasMovedBeyondTap = true;
                }

                // Deadband filter to prevent idle jitter
                if (abs(dx) >= DEAD_ZONE_PX || abs(dy) >= DEAD_ZONE_PX) {
                    float dist = sqrtf((float)(dx * dx + dy * dy));
                    float speedMultiplier = DEFAULT_SENSITIVITY;

                    // Cyberpunk kinetic acceleration curve
                    if (dist > ACCEL_THRESHOLD) {
                        speedMultiplier *= ACCEL_FACTOR;
                    }

                    int moveX = (int)(dx * speedMultiplier);
                    int moveY = (int)(dy * speedMultiplier);

                    if (bleMouse.isConnected()) {
                        bleMouse.move(moveX, moveY);
                    }

                    lastTouchX = p.x;
                    lastTouchY = p.y;
                }
            }
        }
    } else {
        if (isFingerDown) {
            uint32_t pressDuration = millis() - touchStartTime;

            // Tap-to-click detection (short touch without significant drag)
            if (!hasMovedBeyondTap && 
                pressDuration >= TAP_MIN_DURATION_MS && 
                pressDuration <= TAP_MAX_DURATION_MS) {
                if (bleMouse.isConnected()) {
                    bleMouse.click(MOUSE_LEFT);
                }
                triggerHaptic(20);
            }

            isFingerDown = false;
            lastTouchX = -1;
            lastTouchY = -1;
            touchStartX = -1;
            touchStartY = -1;
            hasMovedBeyondTap = false;
        }
    }
}

// =============================================================================
// TOUCH BUTTONS (BTN A, BTN B, BTN C) ENGINE
// =============================================================================
void handleCapacitiveButtons() {
    // -------------------------------------------------------------------------
    // BUTTON A: LEFT CLICK
    // -------------------------------------------------------------------------
    if (M5.BtnA.wasPressed()) {
        triggerHaptic(20);
        if (bleMouse.isConnected()) {
            bleMouse.press(MOUSE_LEFT);
        }
        CyberUI::drawBottomButtonHUD(true, M5.BtnB.isPressed(), M5.BtnC.isPressed(), 0.0f);
    }
    if (M5.BtnA.wasReleased()) {
        if (bleMouse.isConnected()) {
            bleMouse.release(MOUSE_LEFT);
        }
        CyberUI::drawBottomButtonHUD(false, M5.BtnB.isPressed(), M5.BtnC.isPressed(), 0.0f);
    }

    // -------------------------------------------------------------------------
    // BUTTON C: RIGHT CLICK
    // -------------------------------------------------------------------------
    if (M5.BtnC.wasPressed()) {
        triggerHaptic(20);
        if (bleMouse.isConnected()) {
            bleMouse.press(MOUSE_RIGHT);
        }
        CyberUI::drawBottomButtonHUD(M5.BtnA.isPressed(), M5.BtnB.isPressed(), true, 0.0f);
    }
    if (M5.BtnC.wasReleased()) {
        if (bleMouse.isConnected()) {
            bleMouse.release(MOUSE_RIGHT);
        }
        CyberUI::drawBottomButtonHUD(M5.BtnA.isPressed(), M5.BtnB.isPressed(), false, 0.0f);
    }

    // -------------------------------------------------------------------------
    // BUTTON B: CENTER (LONG PRESS -> SETTINGS SCREEN)
    // -------------------------------------------------------------------------
    if (M5.BtnB.wasPressed()) {
        btnB_pressStartTime = millis();
        btnB_isHolding = true;
        btnB_longPressHandled = false;
        CyberUI::drawBottomButtonHUD(M5.BtnA.isPressed(), true, M5.BtnC.isPressed(), 0.1f);
    }

    if (btnB_isHolding && M5.BtnB.isPressed()) {
        uint32_t elapsed = millis() - btnB_pressStartTime;
        float progress = (float)elapsed / (float)BTN_HOLD_SETTINGS_MS;

        // Render live charging progress bar in bottom center button HUD
        CyberUI::drawBottomButtonHUD(M5.BtnA.isPressed(), true, M5.BtnC.isPressed(), progress);

        if (elapsed >= BTN_HOLD_SETTINGS_MS && !btnB_longPressHandled) {
            btnB_longPressHandled = true;
            btnB_isHolding = false;
            switchToMode(MODE_SETTINGS);
            return;
        }
    }

    if (M5.BtnB.wasReleased()) {
        if (btnB_isHolding && !btnB_longPressHandled) {
            // Short tap on center button: can act as middle click or reconnect
            triggerHaptic(25);
            if (bleMouse.isConnected()) {
                bleMouse.click(MOUSE_MIDDLE);
            }
        }
        btnB_isHolding = false;
        btnB_longPressHandled = false;
        CyberUI::drawBottomButtonHUD(M5.BtnA.isPressed(), false, M5.BtnC.isPressed(), 0.0f);
    }
}

// =============================================================================
// SETTINGS SCREEN TOUCH HANDLER
// =============================================================================
void handleSettingsTouch() {
    // Also allow pressing BtnB or BtnA/BtnC to return
    if (M5.BtnB.wasPressed() || M5.BtnA.wasPressed()) {
        switchToMode(MODE_TRACKPAD);
        return;
    }

    // Handle touch on screen settings buttons
    if (M5.Touch.ispressed()) {
        Point p = M5.Touch.getPressPoint();
        if (p.x >= 0 && p.y >= 0) {
            // Button 1: Toggle Bluetooth (y: 110 to 144)
            if (p.y >= 105 && p.y <= 145) {
                toggleBluetooth();
                while (M5.Touch.ispressed()) { M5.update(); delay(10); } // Debounce touch release
                return;
            }

            // Button 2: Power Down (y: 150 to 184)
            if (p.y >= 148 && p.y <= 188) {
                executePowerOff();
                return;
            }

            // Button 3: Return to Trackpad (y: 192 to 235)
            if (p.y >= 190 && p.y <= 238) {
                switchToMode(MODE_TRACKPAD);
                while (M5.Touch.ispressed()) { M5.update(); delay(10); } // Debounce touch release
                return;
            }
        }
    }
}

// =============================================================================
// SETUP & MAIN LOOP
// =============================================================================
void setup() {
    // 1. Initialize M5Core2 (LCD, Touch FT6336U, AXP192 PMU)
    M5.begin(true, true, true, true); // LCD, SD, Serial, I2C
    Serial.println(F("[SYSTEM] M5Stack Core2 BLE Cyberpunk Mouse Initializing..."));

    // 2. Initialize haptic motor (AXP192 LDO3)
    M5.Axp.SetLDOVoltage(3, 3300); // 3.3V for motor
    M5.Axp.SetLDOEnable(3, false);
    triggerHaptic(80);

    // 3. Obtain BT MAC address & battery stats
    readBtMacAddress();
    updateBatteryTelemetry();

    // 4. Start BLE Mouse
    if (bleEnabled) {
        bleMouse.begin();
        Serial.printf("[BLE] Advertising as '%s' (MAC: %s)\n", BLE_DEVICE_NAME, btMacStr);
    }

    // 5. Initialize Cyberpunk UI
    switchToMode(MODE_TRACKPAD);
}

void loop() {
    M5.update(); // Poll capacitive buttons and touch state

    uint32_t now = millis();

    // -------------------------------------------------------------------------
    // TELEMETRY & BLE STATE MONITORING
    // -------------------------------------------------------------------------
    if (now - lastTelemetryCheck > 2500) {
        lastTelemetryCheck = now;
        updateBatteryTelemetry();

        // Refresh settings screen stats if open
        if (currentMode == MODE_SETTINGS) {
            CyberUI::drawSettingsScreen(bleEnabled, bleMouse.isConnected(), btMacStr, currentBatPct, currentBatVolt);
        }
    }

    // Refresh top bar in trackpad mode on connection change or every 500ms for blinking
    if (currentMode == MODE_TRACKPAD) {
        bool isConnected = bleMouse.isConnected();
        if (isConnected != lastBleConnectionState || (now - lastTopBarUpdate > 600)) {
            lastTopBarUpdate = now;
            lastBleConnectionState = isConnected;
            CyberUI::drawTopBar(isConnected, bleEnabled, currentBatPct, isCharging, DEFAULT_SENSITIVITY);
        }
    }

    // -------------------------------------------------------------------------
    // MODE-SPECIFIC LOGIC
    // -------------------------------------------------------------------------
    if (currentMode == MODE_TRACKPAD) {
        handleTrackpadTouch();
        handleCapacitiveButtons();
    } else if (currentMode == MODE_SETTINGS) {
        handleSettingsTouch();
    }

    delay(8); // High-rate smooth 120Hz polling loop
}
