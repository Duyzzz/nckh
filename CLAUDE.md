# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a UWB (Ultra-Wideband) ranging project using the DW1000/DWM1000 module with ESP32 WROVER boards. The project uses a forked version of the arduino-dw1000 library (`DW1000/`).

**Primary Objective:** Create a TWR (Two-Way Ranging) version that can embed custom data (sensor data) into ranging messages. Target payload sizes: 4, 10, 20, or 50 bytes.

## Hardware Configuration

- **Board:** ESP32 WROVER
- **Pin definitions:** Use `exampleForEsp32Board/` as reference for correct ESP32 pin mappings
  - SPI: SCK=18, MISO=19, MOSI=23
  - UWB Module: RST=27, IRQ=34, SS varies (4 or 21 depending on board variant)
  - I2C (for display): SDA=4, SCL=5

## Architecture

### DW1000 Library Structure (`DW1000/src/`)

| File | Purpose |
|------|---------|
| `DW1000.h/.cpp` | Low-level chip communication, SPI, configuration |
| `DW1000Ranging.h/.cpp` | TWR protocol implementation (main focus for modifications) |
| `DW1000Device.h/.cpp` | Remote device representation with timestamps |
| `DW1000Time.h/.cpp` | 64-bit timestamp handling for precise timing |
| `DW1000Mac.h/.cpp` | MAC frame encoding/decoding |
| `DW1000Constants.h` | Register addresses and constants |

### TWR Protocol Message Flow

The ranging protocol uses these message types (defined in `DW1000Ranging.h`):
```
BLINK (4) -> RANGING_INIT (5) -> POLL (0) -> POLL_ACK (1) -> RANGE (2) -> RANGE_REPORT (3)
```

**Tag initiates:** BLINK, POLL, RANGE
**Anchor responds:** RANGING_INIT, POLL_ACK, RANGE_REPORT

### Data Buffer

The `data[]` buffer is 90 bytes (`LEN_DATA`). Message payloads start after MAC headers:
- Short MAC frame: payload starts at `data[SHORT_MAC_LEN]`
- Long MAC frame: payload starts at `data[LONG_MAC_LEN]`

Current message structures in `DW1000Ranging.cpp`:
- **POLL:** `[MAC][type][numDevices][addr+replyTime per device (4 bytes each)]`
- **RANGE:** `[MAC][type][numDevices][addr+timestamps per device (17 bytes each)]`
- **RANGE_REPORT:** `[MAC][type][range(4)][rxpower(4)]`

### Embedding Custom Data

Candidate messages for embedding custom payload data:
1. **POLL/POLL_ACK** - Early in exchange, bidirectional
2. **RANGE** - Already carries timestamp data, could append
3. **RANGE_REPORT** - Final message, has room after existing 8 bytes of data

Key functions to modify for custom data:
- `transmitPoll()`, `transmitPollAck()` (lines ~796-851)
- `transmitRange()` (lines ~853-903)
- `transmitRangeReport()` (lines ~906-918)
- Message parsing in `loop()` around lines 520-680

## Development

This is an Arduino/PlatformIO project. No build system is configured in the repo - use Arduino IDE or PlatformIO with ESP32 support.

Example sketches:
- `exampleForEsp32Board/anchor.ino` - Anchor node (responds to tags)
- `exampleForEsp32Board/tag.ino` - Tag node with OLED display
