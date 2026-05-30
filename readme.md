# 🤖 EVE - Expressive Desktop Robot

A tiny desktop robot powered by ESP32-C3, MPU6050, and OLED display that can express emotions through animated eyes.

<p align="center">
  <img src="images.jpeg" width="300">
</p>

---

## 🎯 About EVE

EVE is the official mascot of the Robotics Community at Universitas Atma Jaya Yogyakarta (UAJY).

This little robot was created as a graduation gift for our fellow robotics members who are moving on to the next chapter of their lives.

While EVE may only have a small OLED screen and a few electronic components, it carries something much larger: the spirit of curiosity, creativity, teamwork, and friendship that defines our community.

Through its animated expressions and playful reactions, EVE symbolizes the journey we shared together—from our first lines of code, failed prototypes, and competition preparations, to the successful projects and memories that shaped us as engineers.

As our graduates step into the professional world, we hope EVE remains a small companion and a reminder that they will always be part of the Robotics UAJY family.

**Once a roboticist, always a roboticist. 🤖❤️**

---

## ✨ Features

- 👀 Smooth animated eyes using FluxGarage RoboEyes
- 😴 Idle behavior with blinking and happy expressions
- 📳 Shake detection using MPU6050
- 😵 Dizzy reaction when shaken
- 😠 Angry expression
- 😑 Flat-eye expression
- 😢 Sad expression
- 🔋 Battery powered using LiPo battery
- ⚡ Portable and standalone operation

---

## 🎭 Emotion Flow

The robot reacts to physical interaction through a simple emotional state machine.

```text
          ┌────────────┐
          │    Idle    │
          │ 😊 😄 😉    │
          └─────┬──────┘
                │
                │ Shake detected
                ▼
          ┌────────────┐
          │  Flicker   │
          │ ⚡⚡⚡⚡⚡    │
          └─────┬──────┘
                │
                ▼
          ┌────────────┐
          │   Dizzy    │
          │ 😵😵😵      │
          └─────┬──────┘
                │
                ▼
          ┌────────────┐
          │   Angry    │
          │ 😠😠😠      │
          └─────┬──────┘
                │
                ▼
          ┌────────────┐
          │ Flat Eyes  │
          │  -    -    │
          └─────┬──────┘
                │
                ▼
          ┌────────────┐
          │    Sad     │
          │ 😢😢😢      │
          └─────┬──────┘
                │
                ▼
          ┌────────────┐
          │    Idle    │
          └────────────┘
```

---

## 🛠 Hardware

| Component           | Description              |
| ------------------- | ------------------------ |
| ESP32-C3 Super Mini | Main Controller          |
| MPU6050             | Motion & Shake Detection |
| OLED SH1106 128x64  | Face Display             |
| LiPo Battery 3.7V   | Power Source             |
| TP4056              | Battery Charging Module  |
| Slide Switch        | Power Control            |
| 3D Printed Body     | Robot Enclosure          |

---

## 🔌 Wiring

### OLED

| OLED | ESP32-C3 |
| ---- | -------- |
| VCC  | 3V3      |
| GND  | GND      |
| SDA  | GPIO8    |
| SCL  | GPIO9    |

### MPU6050

| MPU6050 | ESP32-C3 |
| ------- | -------- |
| VCC     | 3V3      |
| GND     | GND      |
| SDA     | GPIO8    |
| SCL     | GPIO9    |

### Power

```text
LiPo Battery
      │
      ▼
   TP4056
      │
      ▼
 Power Switch
      │
      ▼
 ESP32-C3 5V
```

---

## 📚 Libraries

- FluxGarage RoboEyes
- Adafruit GFX
- Adafruit SH110X
- Adafruit MPU6050
- Adafruit Unified Sensor

---
