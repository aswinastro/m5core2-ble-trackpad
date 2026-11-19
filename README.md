# M5Stack Core2 Cyberpunk BLE Trackpad & Mouse Firmware

A high-performance, ultra-responsive Bluetooth Low Energy (BLE) HID trackpad and mouse firmware for the **M5Stack Core2** (ESP32), featuring a rich **Cyberpunk / Sci-Fi HUD interface**.

---

## ⚡ Features

- **Multi-Touch Trackpad Surface**:
  - Full-screen capacitive trackpad navigation utilizing the FT6336U touch controller (entire 320x240 screen area).
  - Tap-to-click gesture support with crisp haptic vibration motor confirmation.
  - Kinetic acceleration curve with anti-jitter deadband filtering for silky-smooth cursor movement.
  - Clean cyberpunk HUD optimized for zero drawing lag or trace artifacts.
- **Physical Touch Button Mapping** (Core2 Front Touch Panel):
  - **Button A (Left)**: Left Mouse Click (with haptic feedback vibration).
  - **Button C (Right)**: Right Mouse Click (with haptic feedback vibration).
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
| `DEFAULT_SENSITIVITY` | `2.2f` | Cursor tracking speed multiplier |
| `ACCEL_FACTOR` | `1.6f` | Kinetic acceleration multiplier for fast swipes |
| `BTN_HOLD_SETTINGS_MS`| `750` | Long-press duration (ms) to open Settings |
| `DEAD_ZONE_PX` | `1` | Anti-jitter deadband threshold in pixels |

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
