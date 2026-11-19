#pragma once
#include <Arduino.h>

// =============================================================================
// M5STACK CORE2 BLE CYBERPUNK TRACKPAD CONFIGURATION
// =============================================================================

// BLE Identity
#define BLE_DEVICE_NAME        "Core2-Trackpad"
#define BLE_MANUFACTURER_NAME  "M5Stack CyberCorp"

// Screen Dimensions
#define SCREEN_WIDTH           320
#define SCREEN_HEIGHT          240

// UI Layout Coordinates
#define TOP_BAR_HEIGHT         28
#define BOTTOM_BAR_HEIGHT      32
#define TRACKPAD_Y_MIN         0
#define TRACKPAD_Y_MAX         SCREEN_HEIGHT

// Mouse Navigation Tuning
#define MOUSE_SENSITIVITY_MIN  1
#define MOUSE_SENSITIVITY_MAX  5
#define DEFAULT_SENSITIVITY    2.2f     // Linear tracking multiplier
#define ACCEL_THRESHOLD        12       // Pixel delta to trigger acceleration
#define ACCEL_FACTOR           1.6f     // Acceleration multiplier for rapid flicks
#define DEAD_ZONE_PX           1        // Ignores micro-jitter less than 1px

// Tap-to-Click Tuning
#define TAP_MAX_DURATION_MS    250      // Max press duration for tap detection (ms)
#define TAP_MIN_DURATION_MS    25       // Min duration to reject electrical noise (ms)
#define TAP_MAX_DISTANCE_PX    8        // Max movement radius allowed for a tap (px)

// Long Press Duration (Center Touch Button -> Settings)
#define BTN_HOLD_SETTINGS_MS   750      // Milliseconds of hold required

// Haptic Feedback
#define HAPTIC_MOTOR_PIN       AXP_LDO3 // Core2 internal vibration motor on AXP192 LDO3

// Cyberpunk 16-bit RGB565 Color Palette
#define COLOR_CYBER_BG         0x0842   // Deep space void / dark navy (#080812)
#define COLOR_CYBER_GRID       0x1107   // Faint circuit grid (#102038)
#define COLOR_CYBER_CYAN       0x07FF   // High-voltage Neon Cyan (#00F0FF)
#define COLOR_CYBER_MAGENTA    0xF81F   // Neon Hot Magenta / Pink (#FF00FF)
#define COLOR_CYBER_YELLOW     0xFFE0   // Warning Cyber Yellow (#FFE500)
#define COLOR_CYBER_GREEN      0x07E0   // Terminal Phosphor Green (#00FF66)
#define COLOR_CYBER_ORANGE     0xFD20   // Hazard Orange (#FF6600)
#define COLOR_CYBER_RED        0xF800   // Alert Red (#FF0033)
#define COLOR_CYBER_DARKCYAN   0x0273   // Dimmed Cyan for borders
#define COLOR_CYBER_DARKMAG    0x4008   // Dimmed Magenta
#define COLOR_CYBER_GRAY       0x5AEB   // Tech Metallic Gray (#586175)
#define COLOR_CYBER_WHITE      0xFFFF   // Bright Matrix White
