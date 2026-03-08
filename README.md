# Arwing

My first ambitious project, built while I was learning to code — no AI assistance was used. The Arwing is an autonomous boat controller built on a **Teensy 4.1** microcontroller, integrating:

- **GPS navigation** (GT-U7 module, TinyGPS++)
- **Iridium satellite communication** (RockBLOCK 9603) for remote messaging
- **Bluetooth telemetry & command interface** (HM10 BLE module)
- **9-axis IMU** (Adafruit BNO055) for orientation sensing
- **Dual brushless ESC motor control** with variable throttle modes (crawl, cruise, burn)
- **Servo-driven rudder steering** with autopilot course corrections
- **Flysky iBus remote control** for manual override
- **Temperature monitoring** (2x MCP9808 sensors)
- **Current draw & battery voltage monitoring**
- **SD card data logging** with timestamped CSV output
- **Audio feedback** via speaker for system status tones
- **Autopilot guidance system** with waypoint navigation, bearing calculations, and multiple mission modes (move, recon, stop)

There are definitely some things I would do differently today versus what I used back then, and I would definitely use Claude Code. The boat was intended for an ocean voyage. The hardware was successfully wired up and functioning, and did see a shakedown cruise on a lake before the project was put on pause — building a sea-worthy boat would have been too expensive and time-consuming while I was studying electrical engineering.

I plan on migrating away from Arduino and rebuilding the entire codebase on my own microcontroller using STM32CubeIDE.
