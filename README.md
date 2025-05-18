# Automated Poultry Feeding and Egg Incubation System (Smart Coop)

This project uses Arduino boards and wireless communication to build a smart system for monitoring and automating poultry care — including feeding and egg incubation under ideal conditions.

## Project Overview

The system includes:
- **Temperature sensors** for monitoring chicken coop conditions.
- **Humidity-temperature sensors** for egg incubation control.
- **Force sensors** for detecting when food trays need refilling.
- A **receiver module** (central station) to collect all data and trigger actions.

## Components

- Arduino Uno (4x)
- RFM22 Transceivers (4x)
- DS18B20 Temperature Sensor
- DHT11 Humidity-Temperature Sensor
- FSR Force Sensor
- LEDs for feedback/alerts

## Files

- `temp_sensor_Tx.ino` – Transmitter for coop temperature
- `humi-temp_sensor_Tx.ino` – Transmitter for incubation conditions
- `weight_sensor_Tx.ino` – Transmitter for food tray weight
- `reciever.ino` – Central receiver (controls logic)
- `Τεχνική_Αναφορά.pdf` – Technical report (in Greek)

## How It Works

- Each Arduino module sends measurements wirelessly using ALOHA protocol.
- The central receiver compares values to thresholds.
- If a value is out of range, an LED is triggered (e.g. low food, bad humidity).
- The system is designed for extensibility and low manual intervention.

## Communication

- Wireless protocol: ALOHA
- Range: ~500 meters
- Data transmission rate varies by sensor type

## Practical Use

Designed for use by poultry farmers to:
- Improve egg hatch rates
- Automate feeding cycles
- Monitor environmental conditions remotely

---



