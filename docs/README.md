# 🧭 System Overview

This document describes the **ESP32 Security System** (keypad + LCD + email alert).

---

## 🧠 Summary
The **ESP32** acts as a local access control device.  
Users enter a PIN using the **4x4 keypad**, and feedback is displayed on the **16x2 LCD** while a **buzzer** provides audible responses.  
After 3 incorrect attempts, the system **locks for 60 seconds** and automatically **sends an email alert** to the owner using Gmail SMTP via the `ESP_Mail_Client` library.

---

## ⚙️ Components
- ESP32 DevKit v1  
- 4x4 Keypad  
- 16x2 LCD (4-bit mode)  
- Buzzer  
- Wi-Fi Network  
- Gmail Account with App Password  

---

## 🔌 Wiring / Diagrams

Below are the connection and system flow diagrams.  
Upload your circuit and logic visuals here later (recommended file names below 👇):

- `wiring_diagram.png` — main wiring/circuit diagram  
- `flowchart.png` — system flowchart  

### Wiring Diagram
![Wiring Diagram](wiring_diagram.png)

### Flowchart
![Flowchart](flowchart.png)

---

## 🧩 Notes
- All images in this folder are for **documentation purposes** only.  
- The system code is located in the main `.ino` file.  
- Future versions may include **Bluetooth control** or **cloud-based logging**.

---

> © 2025 Mohamed Waleid – ESP32 Security System Project
