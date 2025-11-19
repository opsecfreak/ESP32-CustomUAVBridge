/*
 * dronebridge_config.h
 * Centralized compile-time defines and documented defaults for the XIAO ESP32-C3
 * telemetry-only build. This header is intentionally small and only contains
 * macros used by multiple source files to keep conditional compilation tidy.
 */
#ifndef DRONEBRIDGE_CONFIG_H
#define DRONEBRIDGE_CONFIG_H

/* Board selection: set by sdkconfig (CONFIG_DB_OFFICIAL_BOARD_1_X_C3)
 * Use these macros in code to branch for board-specific behavior. */
#if defined(CONFIG_DB_OFFICIAL_BOARD_1_X_C3)
#define DB_BOARD_XIAO_ESP32C3 1
#else
#define DB_BOARD_XIAO_ESP32C3 0
#endif

/* Build profile: telemetry-only builds are expected to be small and disable
 * heavyweight features (video, BLE, SD card logging, etc.). Use this macro
 * in code to hide/strip optional functionality. */
#ifndef DB_TELEMETRY_ONLY_BUILD
#define DB_TELEMETRY_ONLY_BUILD 1
#endif

/* UART defaults for the Seeed XIAO ESP32-C3 (4MB):
 * RX -> GPIO4
 * TX -> GPIO5
 * RTS -> GPIO6
 * CTS -> GPIO7
 * These values are also set as defaults in db_parameters and persisted to NVS
 * during boot for the C3 official build. */
#if DB_BOARD_XIAO_ESP32C3
#define DB_DEFAULT_UART_TX_GPIO 5
#define DB_DEFAULT_UART_RX_GPIO 4
#define DB_DEFAULT_UART_RTS_GPIO 6
#define DB_DEFAULT_UART_CTS_GPIO 7
#else
/* Fallback (generic) - keep previous behavior */
#define DB_DEFAULT_UART_TX_GPIO 1
#define DB_DEFAULT_UART_RX_GPIO 3
#define DB_DEFAULT_UART_RTS_GPIO 0
#define DB_DEFAULT_UART_CTS_GPIO 0
#endif

/* UART default baud for telemetry-only builds */
#define DB_DEFAULT_UART_BAUD 57600

/* Wi‑Fi defaults tuned for AP long-range (LR) telemetry:
 * - Default AP channel (can be changed in UI but forced at boot for C3 builds)
 * - Default max TX power (dBm)
 * - Default bandwidth for AP: use HT20 to keep channel narrow and stable
 */
#define DB_DEFAULT_AP_CHANNEL 6
#define DB_DEFAULT_AP_MAX_TX_POWER_DBM 20
#define DB_DEFAULT_AP_BANDWIDTH WIFI_BW_HT20

/* Server-Sent Events interval for streaming telemetry (ms). Keep conservative
 * to balance update rate vs network load on low-power links. */
#define DB_SSE_STATS_INTERVAL_MS 500

#endif // DRONEBRIDGE_CONFIG_H
