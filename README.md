# ESP32 Chair Remote Control

This project replaces a wired recliner remote with a wireless one using two ESP32-C3 Mini boards, one OLED, and relays.

## 🔧 Hardware Overview

### Transmitter (Remote)
- ESP32-C3 Mini
- 3.7V 2000mAh LiPo + BMS + MP1495
- SSD1306 0.96" OLED (I2C)
- 2x buttons (UP/DOWN)
- Optional: battery voltage divider, CHRG indicator

### Receiver
- ESP32-C3 Mini
- Powered from 12V via MP1495
- 2x Relays (for UP and DOWN)
- Power LED

## 📡 Communication
Uses ESP-NOW for fast, peer-to-peer control.

## 📷 Display (Remote)
- Battery voltage
- Charging status
- Current command (UP/DOWN/IDLE)

## 📦 Required Libraries
- Adafruit GFX
- Adafruit SSD1306
- ESP-NOW (built into ESP32 Arduino core)

## 📘 Setup
- Flash `transmitter.ino` to the remote ESP32
- Flash `receiver.ino` to the chair-side ESP32
- Update `receiverMAC[]` in transmitter with your receiver's MAC
