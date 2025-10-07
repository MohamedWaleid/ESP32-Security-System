#include <WiFi.h>
#include <Keypad.h>
#include <LiquidCrystal.h>
#include <ESP_Mail_Client.h>

// ====== LCD (4-bit) ======
#define RS_PIN 19
#define EN_PIN 23
#define LCD_D4 18
#define LCD_D5 5
#define LCD_D6 4
#define LCD_D7 2
LiquidCrystal lcd(RS_PIN, EN_PIN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

// ====== Keypad ======
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {13, 12, 14, 27};
byte colPins[COLS] = {26, 25, 33, 32};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// ====== WiFi (REPLACE LOCALLY BEFORE UPLOAD) ======
const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* wifiPassword = "REPLACE_WITH_YOUR_WIFI_PASSWORD";

// ====== Gmail SMTP (REPLACE LOCALLY BEFORE UPLOAD) ======
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465
#define EMAIL_SENDER "REPLACE_WITH_YOUR_EMAIL@gmail.com"
#define EMAIL_PASSWORD "REPLACE_WITH_YOUR_APP_PASSWORD"
#define EMAIL_RECIPIENT "REPLACE_WITH_RECIPIENT_EMAIL@gmail.com"

// ====== Buzzer ======
int buzzerPin = 15;

// ====== Password logic ======
String correctPassword = "1234";
String enteredPassword = "";
int wrongAttempts = 0;
const int maxAttempts = 3;
bool lockActive = false;
int lockTime = 60; // lock for 60 seconds

// ====== SMTP session ======
SMTPSession smtp;

// ====== Task handle ======
TaskHandle_t EmailTaskHandle = NULL;

// ====== Functions ======
void sendEmailTask(void * parameter){
  SMTP_Message message;
  message.sender.name = F("ESP32 Alert");
  message.sender.email = EMAIL_SENDER;
  message.subject = F("Wrong Password Alert");
  message.addRecipient(F("Owner"), EMAIL_RECIPIENT);
  String body = "Warning: 3 wrong password attempts detected on your ESP32 device.\r\n";
  body += "Device IP: " + WiFi.localIP().toString();
  message.text.content = body.c_str();

  ESP_Mail_Session session;
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = EMAIL_SENDER;
  session.login.password = EMAIL_PASSWORD;
  session.login.user_domain = "";

  Serial.println("Connecting to SMTP server...");
  if (!smtp.connect(&session)) {
    Serial.println("Connection to SMTP server failed!");
    vTaskDelete(NULL);
  }
  if (!MailClient.sendMail(&smtp, &message)) {
    Serial.print("Error sending Email: ");
    Serial.println(smtp.errorReason());
  } else {
    Serial.println("✅ Email sent successfully!");
  }
  smtp.closeSession();
  vTaskDelete(NULL); // finish task
}

void successTone() {
  digitalWrite(buzzerPin, HIGH); delay(150);
  digitalWrite(buzzerPin, LOW);  delay(80);
  digitalWrite(buzzerPin, HIGH); delay(120);
  digitalWrite(buzzerPin, LOW);
}

void failTone() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(buzzerPin, HIGH); delay(120);
    digitalWrite(buzzerPin, LOW);  delay(80);
  }
}

// ====== Smooth Lock Countdown ======
void lockCountdown() {
  lockActive = true;
  // Write clean header + spaces to clear previous chars
  lcd.setCursor(0,0);
  lcd.print("SYSTEM LOCK    "); // ensure trailing spaces
  lcd.setCursor(0,1);
  lcd.print("Time left:     s"); // spacing for clearing

  for (int t = lockTime; t >= 0; t--) {
    // update only the number area for smoothness
    lcd.setCursor(11,1); // column 11 (0-indexed) is after "Time left: "
    if (t < 10) lcd.print(' '); // keep alignment
    lcd.print(t);
    lcd.print(" "); // clear leftover digit
    Serial.print("Lock remaining: "); Serial.println(t);
    delay(1000);
  }

  lockActive = false;
  lcd.clear();
  lcd.print("Enter Password:");
  wrongAttempts = 0;
  enteredPassword = "";
}

void checkPassword() {
  if (enteredPassword == correctPassword) {
    lcd.clear();
    lcd.print("Welcome");
    Serial.println("Password correct! Welcome.");
    successTone();
    wrongAttempts = 0;
    delay(2000);
    lcd.clear();
    lcd.print("Enter Password:");
  } else {
    wrongAttempts++;
    lcd.clear();
    lcd.print("Wrong Password");
    Serial.print("Wrong password! Attempt ");
    Serial.println(wrongAttempts);
    failTone();
    delay(1000);

    if (wrongAttempts >= maxAttempts) {
      Serial.println("Max wrong attempts reached. Starting lock & email...");
      // start non-blocking email task
      xTaskCreatePinnedToCore(
        sendEmailTask,
        "EmailTask",
        8192,
        NULL,
        1,
        &EmailTaskHandle,
        1
      );
      lockCountdown(); // lock user for lockTime
    }
    lcd.clear();
    lcd.print("Enter Password:");
  }
  enteredPassword = "";
}

// ====== Setup & Loop ======
void setup() {
  Serial.begin(115200);
  lcd.begin(16,2);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  lcd.setCursor(0,0);
  lcd.print("Connecting WiFi");
  Serial.print("Connecting to WiFi: "); Serial.println(ssid);

  WiFi.begin(ssid, wifiPassword);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    lcd.print(".");
  }

  lcd.clear();
  lcd.print("Enter Password:");
  Serial.println("\nWiFi connected!");
  Serial.print("IP Address: "); Serial.println(WiFi.localIP());
}

void loop() {
  if(lockActive) return; // ignore keypad during lock

  char key = keypad.getKey();
  if(key) {
    digitalWrite(buzzerPin, HIGH); delay(50); digitalWrite(buzzerPin, LOW);
    Serial.print("Key pressed: "); Serial.println(key);

    if(key == '#') checkPassword();
    else if(key == '*') {
      enteredPassword = "";
      lcd.setCursor(0,1);
      lcd.print("Cleared!        ");
      Serial.println("Input cleared by user.");
      delay(800);
      lcd.setCursor(0,1);
      lcd.print("                ");
    } else {
      enteredPassword += key;
      lcd.setCursor(max(0,(int)enteredPassword.length()-1),1);
      lcd.print("*");
    }
  }
}
