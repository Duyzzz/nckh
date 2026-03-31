# ESP32 UWB Ranging Project

UWB (Ultra-Wideband) ranging project using the DW1000/DWM1000 module with ESP32 WROVER boards. Features bidirectional custom data embedding in ranging messages.

## Hardware

- **MCU:** ESP32 WROVER
- **UWB Module:** DW1000 / DWM1000

### Pin Configuration

| Function | Pin |
|----------|-----|
| SPI SCK  | 18  |
| SPI MISO | 19  |
| SPI MOSI | 23  |
| UWB RST  | 27  |
| UWB IRQ  | 34  |
| UWB SS   | 4 or 21 (board variant) |

## Quick Start

1. Clone this repository
2. Open with Arduino IDE or PlatformIO
3. Upload `main/anchor/anchor.ino` to anchor device
4. Upload `main/tag/tag.ino` to tag device

## Mode Configuration

Switch between 110 kbps (long range) and 6.8 Mbps (fast) modes:

```cpp
// In setup(), before starting:
DW1000Ranging.use6M8Mode(true);   // 6.8 Mbps - fast, short range
DW1000Ranging.use6M8Mode(false);  // 110 kbps - slow, long range (default)
```

| Mode | Range (LOS) | Update Rate | Best For |
|------|-------------|-------------|----------|
| 110 kbps | 200-300 m | ~10 Hz | Outdoor, long range |
| 6.8 Mbps | 30-50 m | ~100 Hz | Indoor, fast updates |

## Custom Data Embedding

Embed up to **50 bytes** of custom data (sensor readings, commands, etc.) in ranging messages.

### API

```cpp
// Set data to send (call in loop before DW1000Ranging.loop())
DW1000Ranging.setCustomData(data, length);

// Receive data (in callback)
byte* data = DW1000Ranging.getReceivedCustomData();
uint8_t len = DW1000Ranging.getReceivedCustomDataLen();

// Register callback
DW1000Ranging.attachCustomDataReceived(onCustomDataReceived);
```

### Data Flow

- **Tag -> Anchor:** Custom data is embedded in POLL messages
- **Anchor -> Tag:** Custom data is embedded in RANGE_REPORT messages

### Example: Tag Sending Sensor Data

```cpp
byte sensorData[4];

void loop() {
    // Read sensors
    sensorData[0] = readTemperature();
    sensorData[1] = readHumidity();
    sensorData[2] = readBatteryLevel();
    sensorData[3] = getStatus();

    // Attach to next ranging message
    DW1000Ranging.setCustomData(sensorData, sizeof(sensorData));

    DW1000Ranging.loop();
}
```

### Example: Anchor Receiving Data

```cpp
void setup() {
    // ... initialization ...
    DW1000Ranging.attachCustomDataReceived(onCustomDataReceived);
}

void onCustomDataReceived() {
    byte* data = DW1000Ranging.getReceivedCustomData();
    uint8_t len = DW1000Ranging.getReceivedCustomDataLen();

    Serial.print("Received ");
    Serial.print(len);
    Serial.print(" bytes: ");
    for (uint8_t i = 0; i < len; i++) {
        Serial.print(data[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
}
```

## Antenna Delay Calibration

Adjust antenna delay to calibrate distance measurements:

```cpp
DW1000.setAntennaDelay(16500);  // ~213 units ≈ 1 meter offset
```

- Increase value = decrease measured distance
- Calibrate with known reference distance

## Project Structure

```
├── main/                      # Main examples with custom data
│   ├── anchor/anchor.ino
│   └── tag/tag.ino
├── DW1000/                    # Forked DW1000 library
│   └── src/
│       ├── DW1000.h/.cpp           # Low-level chip communication
│       ├── DW1000Ranging.h/.cpp    # TWR protocol + custom data
│       ├── DW1000Device.h/.cpp     # Remote device representation
│       ├── DW1000Time.h/.cpp       # Timestamp handling
│       └── DW1000Mac.h/.cpp        # MAC frame encoding
└── docs/                      # Documentation
    ├── distanceNote.md        # Range capabilities
    ├── rangingCalculated.md   # TWR calculations
    └── airtime.md             # Air time analysis
```

## TWR Protocol

Two-Way Ranging message flow with custom data:

```
Tag                              Anchor
 │                                 │
 │──────── BLINK ─────────────────>│
 │<─────── RANGING_INIT ───────────│
 │──────── POLL [+custom data] ───>│  <- Tag sends data here
 │<─────── POLL_ACK ───────────────│
 │──────── RANGE ─────────────────>│
 │<─────── RANGE_REPORT [+data] ───│  <- Anchor sends data here
 │                                 │
```

## Documentation

See `docs/` for detailed information:
- [Range Capabilities](docs/distanceNote.md) - Mode comparison
- [Ranging Calculations](docs/rangingCalculated.md) - TWR math
- [Air Time Analysis](docs/airtime.md) - Packet timing

## License

See [DW1000/LICENSE.md](DW1000/LICENSE.md)
