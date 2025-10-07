# System Overview

This document describes the ESP32 Security System (keypad + LCD + email alert).

## Summary
The ESP32 acts as a local access control device. Users enter a PIN on the 4x4 keypad. The device shows feedback on the 16x2 LCD and sounds a buzzer. After 3 failed attempts the system locks for a configurable time and sends an email alert to the owner (uses Gmail SMTP via ESP Mail Client).

## Components
- ESP32 DevKit v1
- 4x4 Keypad
- 16x2 LCD (4-bit mode)
- Buzzer
- Wi-Fi network
- Gmail account with App Password

## Wiring / Diagrams
Place wiring images in this same `docs/` folder with names:
- `wiring_diagram.png`  — main wiring/circuit image
- `flowchart.png`       — system flowchart

You can embed them here like:
```markdown
![Wiring Diagram](wiring_diagram.png)
![Flowchart](flowchart.png)
