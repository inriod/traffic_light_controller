# 4-Way Traffic Light Controller

```text
 __________________   ________________________
| >>Ashot<<        | |      e-mail me    || -.\
|   >>Nahapetyan<< | | ashotnhp@gmail.com|| |_\\
|__________________|. |___________________|| -  |
B_,-.___________,-._|-B_,-._,-.____________|,-._)
--`-'-----------`-'-----`-'-`-'-------------`-'--
```

This project is a small traffic light controller system made using an **Arduino Mega 2560**. It simulates a 4-way intersection with North-South and East-West traffic lights, pedestrian buttons, and pedestrian crossing signals.

Project is written in Arduino IDE using `millis()` real-time timing instead of `delay()`.

It simulates a 4-way intersection with:
- North-South traffic axis
- East-West traffic axis
- Pedestrian crossing buttons
- Pedestrian LEDs
- Buzzer sounds

---
[Walkthrough + Live Preview](https://www.youtube.com/watch?v=-PDQRHgW4zY)

## How to Run

1. Open `traffic_light_controller.ino` in Arduino IDE
2. Select **Arduino Mega2560** from the board menu
3. Connect the Arduino using USB
4. Upload the code
5. Assemble the circuit using the hardware assembly diagram from the documentation
6. Power the board and test the traffic light system

---

## Features

- Vehicle traffic lights
- Pedestrian crossing system
- Pedestrian request buttons
- Buzzer for warnings and crossings
- Safe all-red transition state
- No `delay()` used
- Finite-State Machine (FSM)

---

## Components

| Component | Quantity |
|---|---|
| Arduino Mega 2560 | 1 |
| LEDs | Red x4 Yellow x2 Green x4 |
| Push buttons | 2 |
| Passive buzzer | 1 |
| Breadboard | 1 |
| Jumper wires | many |
| Resistors | as needed |

---

## Pinout

| Pin | Function |
|---|---|
| A1 | Buzzer |
| A2 | NS Button |
| A3 | EW Button |
| D2-D4 | NS Traffic LEDs |
| D5-D7 | EW Traffic LEDs |
| D8-D9 | NS Pedestrian LEDs |
| D10-D11 | EW Pedestrian LEDs |

---

## How It Works

The traffic lights switch between NS and EW directions.

If a pedestrian button is pressed, the request is saved and activated during the next safe state.

The whole system works using `millis()` timing.

---

## Useful Files
[Download the Code](./code/traffic_light_controller.ino)
