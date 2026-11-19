#pragma once
#include <Arduino.h>
#include <M5Core2.h>
#include "Config.h"

// Forward declaration of states
enum AppMode {
    MODE_TRACKPAD,
    MODE_SETTINGS
};

class CyberUI {
public:
    static void init() {
        M5.Lcd.setRotation(1); // Landscape
        M5.Lcd.fillScreen(COLOR_CYBER_BG);
        M5.Lcd.setTextWrap(false);
    }

    // =========================================================================
    // CYBERPUNK HUD PRIMITIVES
    // =========================================================================

    // Draws an angled sci-fi chamfered corner box
    static void drawCyberBox(int x, int y, int w, int h, int chamfer, uint16_t borderColor, uint16_t fillColor = COLOR_CYBER_BG) {
        if (fillColor != COLOR_CYBER_BG) {
            M5.Lcd.fillRect(x + chamfer, y, w - 2 * chamfer, h, fillColor);
            M5.Lcd.fillRect(x, y + chamfer, w, h - 2 * chamfer, fillColor);
            M5.Lcd.fillTriangle(x, y + chamfer, x + chamfer, y, x + chamfer, y + chamfer, fillColor);
            M5.Lcd.fillTriangle(x + w - 1, y + chamfer, x + w - chamfer - 1, y, x + w - chamfer - 1, y + chamfer, fillColor);
            M5.Lcd.fillTriangle(x, y + h - chamfer - 1, x + chamfer, y + h - 1, x + chamfer, y + h - chamfer - 1, fillColor);
            M5.Lcd.fillTriangle(x + w - 1, y + h - chamfer - 1, x + w - chamfer - 1, y + h - 1, x + w - chamfer - 1, y + h - chamfer - 1, fillColor);
        }

        // Chamfered borders
        M5.Lcd.drawLine(x + chamfer, y, x + w - chamfer - 1, y, borderColor);
        M5.Lcd.drawLine(x + chamfer, y + h - 1, x + w - chamfer - 1, y + h - 1, borderColor);
        M5.Lcd.drawLine(x, y + chamfer, x, y + h - chamfer - 1, borderColor);
        M5.Lcd.drawLine(x + w - 1, y + chamfer, x + w - 1, y + h - chamfer - 1, borderColor);

        // Chamfer corners
        M5.Lcd.drawLine(x, y + chamfer, x + chamfer, y, borderColor);
        M5.Lcd.drawLine(x + w - 1, y + chamfer, x + w - chamfer - 1, y, borderColor);
        M5.Lcd.drawLine(x, y + h - chamfer - 1, x + chamfer, y + h - 1, borderColor);
        M5.Lcd.drawLine(x + w - 1, y + h - chamfer - 1, x + w - chamfer - 1, y + h - 1, borderColor);
    }

    // Draws sci-fi corner bracket accents
    static void drawCornerBrackets(int x, int y, int w, int h, int len, uint16_t color) {
        // Top-Left
        M5.Lcd.drawLine(x, y, x + len, y, color);
        M5.Lcd.drawLine(x, y, x, y + len, color);
        // Top-Right
        M5.Lcd.drawLine(x + w - 1, y, x + w - 1 - len, y, color);
        M5.Lcd.drawLine(x + w - 1, y, x + w - 1, y + len, color);
        // Bottom-Left
        M5.Lcd.drawLine(x, y + h - 1, x + len, y + h - 1, color);
        M5.Lcd.drawLine(x, y + h - 1, x, y + h - 1 - len, color);
        // Bottom-Right
        M5.Lcd.drawLine(x + w - 1, y + h - 1, x + w - 1 - len, y + h - 1, color);
        M5.Lcd.drawLine(x + w - 1, y + h - 1, x + w - 1, y + h - 1 - len, color);
    }

    // Draws subtle cyber background grid & tech markings
    static void drawCyberGrid() {
        M5.Lcd.fillRect(0, TRACKPAD_Y_MIN, SCREEN_WIDTH, TRACKPAD_Y_MAX - TRACKPAD_Y_MIN, COLOR_CYBER_BG);

        // Fine dotted matrix grid
        for (int x = 20; x < SCREEN_WIDTH; x += 20) {
            for (int y = TRACKPAD_Y_MIN + 10; y < TRACKPAD_Y_MAX; y += 20) {
                M5.Lcd.drawPixel(x, y, COLOR_CYBER_GRID);
            }
        }

        // Screen perimeter tech accents
        drawCornerBrackets(4, TRACKPAD_Y_MIN + 2, SCREEN_WIDTH - 8, TRACKPAD_Y_MAX - TRACKPAD_Y_MIN - 4, 12, COLOR_CYBER_DARKCYAN);

        // Center crosshair reference dots
        int cx = SCREEN_WIDTH / 2;
        int cy = (TRACKPAD_Y_MIN + TRACKPAD_Y_MAX) / 2;
        M5.Lcd.drawFastHLine(cx - 15, cy, 30, COLOR_CYBER_GRID);
        M5.Lcd.drawFastVLine(cx, cy - 15, 30, COLOR_CYBER_GRID);
    }

    // =========================================================================
    // SCI-FI CENTER TITLE: "CORE2 TRACKPAD"
    // =========================================================================
    static void drawSciFiCenterTitle() {
        int cx = SCREEN_WIDTH / 2;
        int cy = (TRACKPAD_Y_MIN + TRACKPAD_Y_MAX) / 2;

        // Outer Cyber HUD Shield
        int boxW = 210;
        int boxH = 68;
        int boxX = cx - boxW / 2;
        int boxY = cy - boxH / 2;

        // Faint glow backing
        drawCyberBox(boxX, boxY, boxW, boxH, 8, COLOR_CYBER_DARKCYAN, COLOR_CYBER_BG);
        drawCyberBox(boxX + 2, boxY + 2, boxW - 4, boxH - 4, 6, COLOR_CYBER_GRID);

        // Sci-Fi Tech Badges on top & bottom rails
        M5.Lcd.drawFastHLine(boxX + 20, boxY, 30, COLOR_CYBER_CYAN);
        M5.Lcd.drawFastHLine(boxX + boxW - 50, boxY, 30, COLOR_CYBER_CYAN);
        M5.Lcd.drawFastHLine(boxX + 35, boxY + boxH - 1, 40, COLOR_CYBER_MAGENTA);

        // Header Tech Code
        M5.Lcd.setTextDatum(MC_DATUM);
        M5.Lcd.setTextColor(COLOR_CYBER_GRAY, COLOR_CYBER_BG);
        M5.Lcd.setTextSize(1);
        M5.Lcd.drawString("// PROTOCOL: BLE-HID //", cx, boxY + 11);

        // MAIN SCI-FI TITLE: "CORE2 TRACKPAD"
        // Rendered with high-tech dual-layer neon glow effect
        // Line 1: CORE 2
        M5.Lcd.setTextColor(COLOR_CYBER_DARKCYAN, COLOR_CYBER_BG);
        M5.Lcd.drawString("CORE 2", cx + 1, boxY + 27, 2);
        M5.Lcd.setTextColor(COLOR_CYBER_CYAN, COLOR_CYBER_BG);
        M5.Lcd.drawString("CORE 2", cx, boxY + 26, 2);

        // Line 2: TRACKPAD
        M5.Lcd.setTextColor(COLOR_CYBER_DARKMAG, COLOR_CYBER_BG);
        M5.Lcd.drawString("TRACKPAD", cx + 1, boxY + 47, 4);
        M5.Lcd.setTextColor(COLOR_CYBER_MAGENTA, COLOR_CYBER_BG);
        M5.Lcd.drawString("TRACKPAD", cx, boxY + 46, 4);

        // Tech subline
        M5.Lcd.setTextColor(COLOR_CYBER_GRAY, COLOR_CYBER_BG);
        M5.Lcd.drawString("< T O U C H   S U R F A C E >", cx, boxY + boxH + 8, 1);
    }

    // =========================================================================
    // TOP STATUS BAR (BATTERY, BLE TELEMETRY, SENSITIVITY)
    // =========================================================================
    static void drawTopBar(bool bleConnected, bool bleEnabled, int batteryPct, bool isCharging, float sensitivity) {
        M5.Lcd.fillRect(0, 0, SCREEN_WIDTH, TOP_BAR_HEIGHT, 0x0000);
        M5.Lcd.drawFastHLine(0, TOP_BAR_HEIGHT - 1, SCREEN_WIDTH, COLOR_CYBER_CYAN);
        M5.Lcd.drawFastHLine(0, TOP_BAR_HEIGHT - 2, SCREEN_WIDTH, COLOR_CYBER_DARKCYAN);

        M5.Lcd.setTextDatum(ML_DATUM);
        M5.Lcd.setTextSize(1);

        // 1. BLE Status indicator
        if (!bleEnabled) {
            // Radio disabled
            M5.Lcd.fillCircle(12, 13, 4, COLOR_CYBER_RED);
            M5.Lcd.setTextColor(COLOR_CYBER_RED, 0x0000);
            M5.Lcd.drawString("BLE: OFF", 22, 14);
        } else if (bleConnected) {
            // Connected & Synced
            M5.Lcd.fillCircle(12, 13, 4, COLOR_CYBER_GREEN);
            M5.Lcd.drawCircle(12, 13, 6, COLOR_CYBER_GREEN);
            M5.Lcd.setTextColor(COLOR_CYBER_GREEN, 0x0000);
            M5.Lcd.drawString("BLE: SYNCED", 22, 14);
        } else {
            // Searching / Pairing
            static bool blink = false;
            blink = !blink;
            uint16_t pairColor = blink ? COLOR_CYBER_YELLOW : COLOR_CYBER_GRAY;
            M5.Lcd.fillCircle(12, 13, 4, pairColor);
            M5.Lcd.setTextColor(COLOR_CYBER_YELLOW, 0x0000);
            M5.Lcd.drawString("BLE: PAIRING...", 22, 14);
        }

        // 2. Sensitivity readout (Center)
        M5.Lcd.setTextDatum(MC_DATUM);
        M5.Lcd.setTextColor(COLOR_CYBER_CYAN, 0x0000);
        char sensBuf[20];
        snprintf(sensBuf, sizeof(sensBuf), "SENS: x%.1f", sensitivity);
        M5.Lcd.drawString(sensBuf, SCREEN_WIDTH / 2, 14);

        // 3. Battery Segmented Meter (Right)
        int batX = SCREEN_WIDTH - 65;
        int batY = 8;
        int batW = 28;
        int batH = 12;

        // Battery outer casing
        M5.Lcd.drawRect(batX, batY, batW, batH, COLOR_CYBER_CYAN);
        M5.Lcd.fillRect(batX + batW, batY + 3, 3, 6, COLOR_CYBER_CYAN); // Anode tip

        // Battery level fill
        int fillW = map(constrain(batteryPct, 0, 100), 0, 100, 0, batW - 4);
        uint16_t batColor = (batteryPct > 50) ? COLOR_CYBER_GREEN :
                            (batteryPct > 20) ? COLOR_CYBER_YELLOW : COLOR_CYBER_RED;
        
        M5.Lcd.fillRect(batX + 2, batY + 2, batW - 4, batH - 4, 0x0000);
        if (fillW > 0) {
            M5.Lcd.fillRect(batX + 2, batY + 2, fillW, batH - 4, batColor);
        }

        // Percentage text
        M5.Lcd.setTextDatum(MR_DATUM);
        M5.Lcd.setTextColor(isCharging ? COLOR_CYBER_YELLOW : COLOR_CYBER_WHITE, 0x0000);
        char pctBuf[16];
        snprintf(pctBuf, sizeof(pctBuf), "%s%d%%", isCharging ? "+" : "", batteryPct);
        M5.Lcd.drawString(pctBuf, batX - 4, 14);
    }

    // =========================================================================
    // BOTTOM CAPACITIVE BUTTON HUD (BTN A, BTN B, BTN C)
    // =========================================================================
    static void drawBottomButtonHUD(bool btnA_down, bool btnB_down, bool btnC_down, float holdProgress) {
        int y = SCREEN_HEIGHT - BOTTOM_BAR_HEIGHT;
        M5.Lcd.fillRect(0, y, SCREEN_WIDTH, BOTTOM_BAR_HEIGHT, 0x0000);
        M5.Lcd.drawFastHLine(0, y, SCREEN_WIDTH, COLOR_CYBER_MAGENTA);

        int slotW = (SCREEN_WIDTH - 12) / 3;

        // --- BUTTON A: LEFT CLICK ---
        int aX = 4;
        uint16_t aBorder = btnA_down ? COLOR_CYBER_CYAN : COLOR_CYBER_DARKCYAN;
        uint16_t aFill   = btnA_down ? COLOR_CYBER_CYAN : 0x0822;
        drawCyberBox(aX, y + 3, slotW, BOTTOM_BAR_HEIGHT - 6, 4, aBorder, aFill);
        M5.Lcd.setTextDatum(MC_DATUM);
        M5.Lcd.setTextColor(btnA_down ? 0x0000 : COLOR_CYBER_CYAN, aFill);
        M5.Lcd.drawString("[ L-CLICK ]", aX + slotW / 2, y + BOTTOM_BAR_HEIGHT / 2);

        // --- BUTTON B: CENTER (HOLD FOR SETTINGS) ---
        int bX = aX + slotW + 2;
        uint16_t bBorder = btnB_down ? COLOR_CYBER_YELLOW : COLOR_CYBER_GRAY;
        uint16_t bFill   = 0x0822;
        drawCyberBox(bX, y + 3, slotW, BOTTOM_BAR_HEIGHT - 6, 4, bBorder, bFill);

        // Hold Progress Bar inside Center Box
        if (holdProgress > 0.05f) {
            int progW = (int)((slotW - 6) * constrain(holdProgress, 0.0f, 1.0f));
            M5.Lcd.fillRect(bX + 3, y + BOTTOM_BAR_HEIGHT - 7, progW, 3, COLOR_CYBER_YELLOW);
        }

        M5.Lcd.setTextColor(btnB_down ? COLOR_CYBER_YELLOW : COLOR_CYBER_WHITE, bFill);
        M5.Lcd.drawString(holdProgress > 0.8f ? "! RELEASING !" : "[ HOLD: CFG ]", bX + slotW / 2, y + BOTTOM_BAR_HEIGHT / 2 - 2);

        // --- BUTTON C: RIGHT CLICK ---
        int cX = bX + slotW + 2;
        uint16_t cBorder = btnC_down ? COLOR_CYBER_MAGENTA : COLOR_CYBER_DARKMAG;
        uint16_t cFill   = btnC_down ? COLOR_CYBER_MAGENTA : 0x0822;
        drawCyberBox(cX, y + 3, slotW, BOTTOM_BAR_HEIGHT - 6, 4, cBorder, cFill);
        M5.Lcd.setTextColor(btnC_down ? 0x0000 : COLOR_CYBER_MAGENTA, cFill);
        M5.Lcd.drawString("[ R-CLICK ]", cX + slotW / 2, y + BOTTOM_BAR_HEIGHT / 2);
    }

    // =========================================================================
    // DYNAMIC TOUCH RETICLE / TRACKING TRACE (DISABLED)
    // =========================================================================
    static void drawTouchReticle(int x, int y, int lastX, int lastY, bool isDown) {
        // Tracking trace printing on display disabled to prevent display artifacts and jitter
        (void)x; (void)y; (void)lastX; (void)lastY; (void)isDown;
    }

    // =========================================================================
    // SETTINGS SCREEN UI
    // =========================================================================
    static void drawSettingsScreen(bool bleEnabled, bool bleConnected, const char* macStr, int batPct, float batVolt) {
        M5.Lcd.fillScreen(COLOR_CYBER_BG);

        // Header Banner
        drawCyberBox(4, 4, SCREEN_WIDTH - 8, 34, 6, COLOR_CYBER_MAGENTA, 0x0000);
        M5.Lcd.setTextDatum(MC_DATUM);
        M5.Lcd.setTextColor(COLOR_CYBER_YELLOW, 0x0000);
        M5.Lcd.drawString("// SYSTEM MATRIX CONFIG //", SCREEN_WIDTH / 2, 16, 2);
        M5.Lcd.setTextColor(COLOR_CYBER_GRAY, 0x0000);
        M5.Lcd.drawString("CORE2 HID v2.4.0", SCREEN_WIDTH / 2, 28, 1);

        // Card 1: Pairing & Connection Status Card
        int cardY = 44;
        int cardH = 58;
        drawCyberBox(8, cardY, SCREEN_WIDTH - 16, cardH, 5, COLOR_CYBER_CYAN, 0x0822);
        
        M5.Lcd.setTextDatum(TL_DATUM);
        M5.Lcd.setTextColor(COLOR_CYBER_CYAN, 0x0822);
        M5.Lcd.drawString("PAIRING STATUS:", 16, cardY + 6, 2);

        M5.Lcd.setTextDatum(TR_DATUM);
        if (!bleEnabled) {
            M5.Lcd.setTextColor(COLOR_CYBER_RED, 0x0822);
            M5.Lcd.drawString("[ DISABLED ]", SCREEN_WIDTH - 16, cardY + 6, 2);
        } else if (bleConnected) {
            M5.Lcd.setTextColor(COLOR_CYBER_GREEN, 0x0822);
            M5.Lcd.drawString("[ SYNCED / CONNECTED ]", SCREEN_WIDTH - 16, cardY + 6, 2);
        } else {
            M5.Lcd.setTextColor(COLOR_CYBER_YELLOW, 0x0822);
            M5.Lcd.drawString("[ PAIRING / ADVERTISING ]", SCREEN_WIDTH - 16, cardY + 6, 2);
        }

        M5.Lcd.setTextDatum(TL_DATUM);
        M5.Lcd.setTextColor(COLOR_CYBER_WHITE, 0x0822);
        char macLine[40];
        snprintf(macLine, sizeof(macLine), "DEVICE: %s (%s)", BLE_DEVICE_NAME, macStr);
        M5.Lcd.drawString(macLine, 16, cardY + 26, 1);

        char pwrLine[40];
        snprintf(pwrLine, sizeof(pwrLine), "BATTERY: %d%% (%.2fV)", batPct, batVolt);
        M5.Lcd.setTextColor(COLOR_CYBER_GRAY, 0x0822);
        M5.Lcd.drawString(pwrLine, 16, cardY + 40, 1);

        // Option 1: Bluetooth ON / OFF Toggle Button
        int btn1Y = 110;
        int btnH = 34;
        uint16_t b1Col = bleEnabled ? COLOR_CYBER_CYAN : COLOR_CYBER_RED;
        drawCyberBox(8, btn1Y, SCREEN_WIDTH - 16, btnH, 6, b1Col, 0x0822);
        M5.Lcd.setTextDatum(MC_DATUM);
        M5.Lcd.setTextColor(b1Col, 0x0822);
        char b1Txt[40];
        snprintf(b1Txt, sizeof(b1Txt), "[1] BLUETOOTH RADIO: %s", bleEnabled ? ">> ON <<" : ">> OFF <<");
        M5.Lcd.drawString(b1Txt, SCREEN_WIDTH / 2, btn1Y + btnH / 2, 2);

        // Option 2: Turn Off / Power Down Button
        int btn2Y = 150;
        drawCyberBox(8, btn2Y, SCREEN_WIDTH - 16, btnH, 6, COLOR_CYBER_ORANGE, 0x0822);
        M5.Lcd.setTextColor(COLOR_CYBER_ORANGE, 0x0822);
        M5.Lcd.drawString("[2] POWER DOWN SYSTEM (TURN OFF)", SCREEN_WIDTH / 2, btn2Y + btnH / 2, 2);

        // Option 3: Return to Trackpad Button
        int btn3Y = 192;
        int btn3H = 38;
        drawCyberBox(8, btn3Y, SCREEN_WIDTH - 16, btn3H, 6, COLOR_CYBER_GREEN, 0x0000);
        M5.Lcd.setTextColor(COLOR_CYBER_GREEN, 0x0000);
        M5.Lcd.drawString("< RETURN TO TRACKPAD >", SCREEN_WIDTH / 2, btn3Y + btn3H / 2, 2);
    }
};
