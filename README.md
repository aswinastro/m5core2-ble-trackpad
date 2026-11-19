# M5Stack Core2 Cyberpunk BLE Trackpad & Mouse Firmware

A high-performance, ultra-responsive Bluetooth Low Energy (BLE) HID trackpad and mouse firmware for the **M5Stack Core2** (ESP32), featuring a rich **Cyberpunk / Sci-Fi HUD interface**.

---

## ⚡ Features & Gestures

- **Full-Screen Multi-Touch Trackpad Surface**:
  - Full-screen capacitive trackpad navigation utilizing the FT6336U touch controller (entire 320x240 LCD area).
  - Kinetic acceleration curve with anti-jitter deadband filtering for silky-smooth cursor movement.
  - Zero drawing lag and no tracking traces on display for optimal 120Hz polling performance.

- **Laptop Trackpad Gestures**:
  - **1-Finger Drag**: Smooth cursor navigation with kinetic velocity curve.
  - **1-Finger Tap**: Left Mouse Click (`bleMouse.click(MOUSE_LEFT)`) with 20ms haptic feedback.
  - **Long-Press Select & Drag**: Press and hold in place for 400ms to lock Left Click down (confirmed with a solid 40ms haptic pulse), then drag to highlight text, move windows, or reposition items. Automatically releases on finger lift with haptic feedback.
  - **Double-Tap & Drag**: Tap then quickly tap-and-hold to drag with Left Click held down. Automatically releases on finger lift with haptic pulse.
  - **2-Finger Scroll**: Natural two-finger vertical and horizontal scrolling (`bleMouse.move(0, 0, wheel, hWheel)`).
  - **2-Finger Tap**: Right Mouse Click (`bleMouse.click(MOUSE_RIGHT)`) with distinct 30ms haptic confirmation.
  - **Anti-Phantom Multi-Touch Filter**: Intelligent cooldown prevents false single-finger clicks or cursor jumps when lifting two fingers.

- **Physical Touch Button Mapping** (Core2 Front Bezel):
  - **Button A (Left)**: Physical Left Mouse Click (with haptic feedback vibration).
  - **Button C (Right)**: Physical Right Mouse Click (with haptic feedback vibration).
  - **Button B (Center)**:
    - **Long Press (> 750ms)**: Enters the **System Settings Matrix** with an animated charging progress bar.
    - **Short Tap**: Middle Mouse Click.
- **System Settings Screen**:
  - **Pairing Status**: Live connection status, device Bluetooth MAC address, battery voltage, and percentage.
  - **Bluetooth Radio Toggle**: Instant toggle between `ON` and `OFF` (enables/disables advertising and disconnects host).
  - **Power Down System**: Safely shuts down the Core2 via the AXP192 power management unit (PMU).
  - **Back / Return**: Tap `< RETURN TO TRACKPAD >` or touch Button B to resume mouse mode.
- **Cyberpunk Sci-Fi Aesthetic**:
  - Sci-Fi dual-layer glowing title: `// CORE 2 // TRACKPAD` in the center.
  - Dotted cyber grid, chamfered HUD panels, and corner brackets.
  - Top HUD bar displaying live BLE status (`SYNCED`, `PAIRING...`, `OFF`), battery meter with charge indicator, and sensitivity multiplier.
  - Bottom HUD dock reflecting physical button presses in real time.

---

## 🛠️ Hardware Requirements

- **M5Stack Core2** (ESP32-D0WDQ6-V3 with 320x240 ILI9342C LCD, FT6336U capacitive touch, AXP192 PMU, and haptic vibration motor).
- USB-C cable for flashing and charging.
- Any Bluetooth-enabled host device (Windows 10/11, macOS, Linux, Android, iOS, iPadOS).

---

## 🚀 Getting Started

### Option 1: PlatformIO in VS Code (Recommended)

1. Open **VS Code** with the **PlatformIO IDE** extension installed.
2. Select **File > Open Folder...** and choose this project directory:
   ```
   C:\Users\Aswin\.gemini\antigravity\scratch\m5core2-ble-trackpad
   ```
3. Connect your M5Stack Core2 via USB-C.
4. Click the **PlatformIO Upload** checkmark/arrow button at the bottom status bar, or run:
   ```bash
   pio run --target upload
   ```
   *(PlatformIO automatically downloads `M5Core2`, `ESP32 BLE Mouse`, and `NimBLE-Arduino` dependencies).*

---

### Option 2: Arduino IDE 2.x

1. In Arduino IDE, ensure the **ESP32** board package (by Espressif Systems) is installed:
   - Go to **Tools > Board > Boards Manager...** and search for `esp32`.
2. Install the required libraries via **Sketch > Include Library > Manage Libraries...**:
   - `M5Core2` by M5Stack
   - `ESP32 BLE Mouse` by T-vK
   - `NimBLE-Arduino` by h2zero
3. Select board: **Tools > Board > esp32 > M5Stack-Core2**.
4. Set **Partition Scheme**: `Huge APP (3MB No OTA/1MB SPIFFS)`.
5. Open `m5core2-ble-trackpad.ino` and click **Upload**.

---

## 📱 How to Pair & Use

1. **Power On**: Turn on the M5Stack Core2.
2. **Observe HUD**: The top status bar will flash `[ BLE: PAIRING... ]` in yellow.
3. **On Your Computer / Device**:
   - Open **Bluetooth Settings** -> **Add Device**.
   - Look for **`Core2-Trackpad`**.
   - Click to Pair / Connect.
4. **Connected!**:
   - The top status bar turns green/cyan: `[ BLE: SYNCED ]`.
   - Slide your finger on the Core2 screen to move your mouse pointer.
   - Tap **Button A** for Left Click.
   - Tap **Button C** for Right Click.
   - Tap **Button B** for Middle Click.
   - **Press and hold Button B** for ~1 second to enter the Cyberpunk Settings menu.

---

## ⚙️ Configuration & Customization

All configuration parameters are consolidated in [Config.h](file:///C:/Users/Aswin/.gemini/antigravity/scratch/m5core2-ble-trackpad/include/Config.h):

| Setting | Default | Description |
| :--- | :--- | :--- |
| `BLE_DEVICE_NAME` | `"Core2-Trackpad"` | Advertised Bluetooth device name |
| `DEFAULT_BRIGHTNESS_PCT` | `70` | Screen brightness percentage (0-100%) via AXP192 PMU |
| `DEFAULT_SENSITIVITY` | `2.2f` | Cursor tracking speed multiplier |
| `ACCEL_FACTOR` | `1.6f` | Kinetic acceleration multiplier for fast swipes |
| `DEAD_ZONE_PX` | `1` | Anti-jitter deadband threshold in pixels |
| `TAP_MAX_DURATION_MS` | `250` | Maximum touch duration (ms) for tap detection |
| `TAP_MAX_DISTANCE_PX` | `8` | Maximum movement radius (px) permitted during tap |
| `TAP_TO_DRAG_TIMEOUT_MS` | `300` | Double-tap interval (ms) to trigger Left Click drag |
| `LONG_PRESS_DRAG_MS` | `400` | Hold duration (ms) to latch Left Click for select & drag |
| `SCROLL_SENSITIVITY` | `0.22f` | Two-finger scroll rate multiplier |
| `SCROLL_INVERT` | `false` | Traditional vs natural two-finger scroll inversion |
| `TWO_FINGER_TAP_MS` | `250` | Two-finger tap duration (ms) for Right Click |
| `MULTITOUCH_COOLDOWN_MS` | `120` | Cooldown to filter residual single-finger touch on 2-finger release |
| `BTN_HOLD_SETTINGS_MS`| `750` | Long-press duration (ms) on Button B to open Settings |

---

## 📜 Version & Feature Changelog

### v2.6.1 - Display Brightness Calibration
- **Set Default Brightness to 70%**: Calibrated the AXP192 LCD backlight voltage (`3030 mV`) for optimal visual clarity, cyberpunk contrast, and battery longevity.

### v2.6.0 - Long-Press Select & Drag
- **Added Long-Press Select & Drag**: Press and hold finger in place on the trackpad for 400ms to latch Left Click down with a solid 40ms haptic buzz, then drag to select text or drag windows. Automatically unlatches on release with haptic confirmation.
- **Enhanced Anti-Jitter Hold**: Suppresses micro-jitter while holding still for long press to prevent premature cursor displacement.

### v2.5.0 - Laptop Trackpad Gestures & Full-Screen Touch
- **Added 2-Finger Scrolling**: Smooth vertical & horizontal scrolling using natural two-finger swipe gestures.
- **Added 2-Finger Tap for Right-Click**: Tap with two fingers anywhere on the trackpad to trigger a Right Click with a 30ms haptic buzz.
- **Added Double-Tap & Drag (Tap to Drag)**: Tap once, tap again and hold to drag windows, select text, or move files with Left Click held down. Automatically releases on finger lift with haptic feedback.
- **Added Anti-Phantom Cooldown**: Filters out residual single-finger touch artifacts when lifting two fingers to prevent accidental clicks or cursor jumps.
- **Expanded Full-Screen Touch Area**: Active trackpad area expanded to cover the entire 320x240 LCD display (`y: 0..240`).
- **Removed Tracking Trace Printing**: Removed reticle drawing during cursor movements for zero drawing lag and clean UI.
- **Added Tap-to-Click**: Quick single finger tap triggers Left Click with a 20ms haptic vibration pulse.

### v2.4.0 - Initial Release
- Core BLE HID mouse implementation on ESP32 Core2.
- Cyberpunk HUD with telemetry (battery %, voltage, BLE status, sensitivity).
- System settings screen with Bluetooth toggle and safe PMU power off.
- Physical touch button mapping for Button A (Left Click), Button B (Center/Settings), and Button C (Right Click).

---

## 📂 Project Structure

```
m5core2-ble-trackpad/
├── platformio.ini              # PlatformIO build configuration & library dependencies
├── m5core2-ble-trackpad.ino    # Arduino IDE direct sketch
├── Config.h / CyberUI.h        # Root headers for Arduino IDE
├── include/
│   ├── Config.h                # Hardware pins, tuning, and Cyberpunk color palette
│   └── CyberUI.h               # Vector HUD graphics, sci-fi fonts & widgets
├── src/
│   └── main.cpp                # Main firmware logic, BLE HID engine, touch handlers
└── README.md                   # Documentation & flashing guide
```
