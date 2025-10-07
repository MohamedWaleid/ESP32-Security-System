# 🔐 ESP32 Security System with Keypad & Email Alert

This project is a smart **security system** built using an **ESP32**, **4x4 keypad**, **LCD**, and **buzzer**.  
It sends an **email alert** after 3 wrong password attempts using the `ESP_Mail_Client` library.

---

## 🚀 Features
- Password entry via 4x4 keypad  
- LCD feedback for user input  
- Buzzer sound for correct and wrong inputs  
- Lock system for 60 seconds after 3 failed attempts  
- Automatic email alert via Gmail SMTP  

---

## 🧠 How It Works
1. User enters a password on the keypad.  
2. If correct → access granted + success tone.  
3. If wrong → attempts counted.  
4. After 3 wrong tries:
   - The system locks for 60 seconds.  
   - Sends an email notification to the owner.  

---

## 🧰 Hardware Used
- ESP32 DevKit v1  
- 4x4 Keypad  
- 16x2 LCD (4-bit mode)  
- Buzzer  
- Internet (WiFi)  

---

## ⚙️ Configuration
Before uploading, replace:
```cpp
const char* ssid = "YOUR_WIFI_NAME";
const char* wifiPassword = "YOUR_WIFI_PASSWORD";
#define EMAIL_SENDER "YOUR_GMAIL@gmail.com"
#define EMAIL_PASSWORD "YOUR_APP_PASSWORD"
#define EMAIL_RECIPIENT "RECIPIENT_EMAIL@gmail.com"
