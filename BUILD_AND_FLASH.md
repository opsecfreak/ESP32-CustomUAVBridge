BUILD & FLASH (Seeed XIAO ESP32-C3) — ESP-IDF 5.x

Overview
--------
This document explains how to prepare your Windows machine, build the trimmed DroneBridge firmware for the Seeed XIAO ESP32-C3 (4MB flash), and flash it.

Prerequisites
-------------
- Windows 10/11
- Python 3.10+ (matching ESP-IDF requirements)
- Git
- ESP-IDF 5.x (follow Espressif official "Get Started" Windows guide)
  - The ESP-IDF installer provides PowerShell export scripts that place `idf.py` on your PATH for the session.
  - Recommended: use ESP-IDF v5.x to match this codebase.

Quick outline
-------------
1. Install ESP-IDF & open an ESP-IDF PowerShell environment.
2. Clone this repo and copy the telemetry sdkconfig.
3. Run `idf.py fullclean` then `idf.py build`.
4. If binary fits 4MB, flash with `idf.py -p <PORT> flash monitor`.
5. If too large, run `idf.py size-components`, paste results here and I will help trim components.

Detailed steps (Windows PowerShell)
----------------------------------
1) Prepare ESP-IDF environment
- Install prerequisites per Espressif: Python 3.10+, Git, and the ESP-IDF tools.
- Follow the official guide for Windows: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/windows-setup.html
- After installation, open the "ESP-IDF PowerShell" shortcut (this ensures `idf.py` is available). Alternatively, in a PowerShell session run the provided export script from the ESP-IDF installation to set PATH and environment variables:

# Example (replace path if needed)
& "C:\\Espressif\\tools\\idf\\export.ps1"

2) Clone repository and prepare build

# In a PowerShell session with ESP-IDF environment active
cd C:\Users\Admin\Documents\1_ESP32_Dronebridge_Custom
# If you haven't already cloned:
# git clone https://github.com/opsecfreak/ESP32-CustomUAVBridge.git
cd ESP32-CustomUAVBridge

# Use the trimmed telemetry sdkconfig as a starting point (optional but recommended):
copy .\sdkconfig_c3_telemetry .\sdkconfig

# Inspect or adjust in menuconfig
idf.py menuconfig
# Make sure target is esp32c3 and CONFIG_DB_OFFICIAL_BOARD_1_X_C3 is set if you want the XIAO defaults.

3) Build

idf.py fullclean
idf.py build

- If build completes successfully, the firmware images will be in `build/` and `build/your_project.bin` (actual artifact names depend on partition and component).
- Check binary size using:

idf.py size-components

4) Flash

# Replace COM3 by the serial port of your XIAO C3
idf.py -p COM3 flash monitor

# If you need to erase flash first:
idf.py -p COM3 erase-flash; idf.py -p COM3 flash monitor

5) Troubleshooting & size optimization
- If `idf.py` isn't recognized in PowerShell, make sure you started the "ESP-IDF PowerShell" that the installer created, or ran the export script.
- If build fails with compilation errors, save the `idf.py build` output and paste it into the issue or here — I'll analyze and propose precise fixes.
- If the image is bigger than 4MB:
  - Run `idf.py size-components` and paste the top 10 largest components here.
  - Typical reductions: disable Bluetooth, remove mbedTLS features you don't need, reduce logging level, disable SSL/TLS, remove large drivers/components.

Hardware specifics & verification steps
--------------------------------------
- UART pins (XIAO C3 build): TX = GPIO5, RX = GPIO4, RTS = GPIO6, CTS = GPIO7.
  - The firmware will try hardware flow control; if `uart_driver_install` fails it falls back to no HW flow control.
- Verify at runtime by enabling serial monitor and checking logs for lines like "UART driver installed" or "flow control fallback".
- Web UI: open `http://<ap-ip>/` (default AP ip in code is 192.168.2.1) and check the telemetry dashboard.

Packaging a release (zip)
-------------------------
- The repo includes `create_release_zip.ps1`. After a successful build you can create a release zip with:

# PowerShell (in repo root)
.\create_release_zip.ps1

- Alternatively, zip the `build/` directory and `frontend/` assets together for distribution.

What I attempted here
---------------------
- I attempted to run `idf.py fullclean; idf.py build` in your workspace but `idf.py` is not available in the current terminal/session (PowerShell returned "idf.py : The term 'idf.py' is not recognized"). That means you need to open the ESP-IDF PowerShell environment or run the export script so `idf.py` is on PATH.

Next steps I can do for you
--------------------------
- If you run the build locally and paste the full output (or `size-components`) I will:
  - Diagnose and fix any remaining compilation issues for C3.
  - Propose precise Kconfig and minor code removals to reduce image size to fit 4MB.
  - Iterate until the image fits and produce a final `BUILD_AND_FLASH.md` and `release.zip` instructions.

If you want, I can also generate an aggressive `sdkconfig` that disables many optional components and may likely fit in 4MB in one pass — but this commonly requires iterative builds to tune dependencies and resolve missing symbols. If you prefer that, say "Please generate aggressive sdkconfig" and I'll produce it next.