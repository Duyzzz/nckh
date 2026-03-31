# UWB Two-Way Ranging (TWR)

## Table of Contents
- [Message Flow](#message-flow)
- [Message Details](#message-details)
- [Distance Calculation](#distance-calculation)

---

## Message Flow

```
    TAG                                    ANCHOR
     |                                        |
     |  -------- POLL (broadcast) -------->   |  T1: timePollSent
     |                                        |  T2: timePollReceived
     |                                        |
     |  <------- POLL_ACK -----------------   |  T3: timePollAckSent
     |  T4: timePollAckReceived               |
     |                                        |
     |  -------- RANGE (broadcast) ------->   |  T5: timeRangeSent
     |                                        |  T6: timeRangeReceived
     |                                        |
     |  <------- RANGE_REPORT -------------   |  (contains computed distance)
     |                                        |
```

---

## Message Details

### 1. POLL (Tag → Anchor, broadcast)

`DW1000Ranging.cpp:796-841`
- Sent by TAG to initiate ranging
- Contains list of all anchors and their assigned reply delay times
- **Custom data appended here** (Tag → Anchor)
- TAG records timePollSent (T1)

### 2. POLL_ACK (Anchor → Tag)

`DW1000Ranging.cpp:843-851`
- Anchor responds after receiving POLL
- Anchor records timePollReceived (T2) and timePollAckSent (T3)
- TAG records timePollAckReceived (T4)

### 3. RANGE (Tag → Anchor, broadcast)

`DW1000Ranging.cpp:853-917`
- Contains timestamps: T1, T4, T5 embedded in message
- TAG records timeRangeSent (T5)
- Anchor records timeRangeReceived (T6)

### 4. RANGE_REPORT (Anchor → Tag)

`DW1000Ranging.cpp:977-1004`
- Anchor computes distance and sends result back to TAG
- **Custom data appended here** (Anchor → Tag)
- Contains: range (4 bytes) + rxPower (4 bytes) + customData (0-50 bytes)

---

## Distance Calculation

`DW1000Ranging.cpp:1020-1045` - `computeRangeAsymmetric()`:

### Asymmetric Two-Way Ranging Formula

```
round1 = T4 - T1  (timePollAckReceived - timePollSent)
reply1 = T3 - T2  (timePollAckSent - timePollReceived)
round2 = T6 - T3  (timeRangeReceived - timePollAckSent)
reply2 = T5 - T4  (timeRangeSent - timePollAckReceived)

TOF = (round1 × round2 - reply1 × reply2) / (round1 + round2 + reply1 + reply2)

distance = TOF × speed_of_light
```

This asymmetric formula cancels out clock drift between devices, making it more accurate than simple TWR.

### DW1000 Timestamp Resolution

- Each timestamp tick = **15.65 picoseconds**
- Distance per tick = 0.00469 meters ≈ **4.69 mm**
- Speed of light = 299,702,547 m/s

### Why Asymmetric TWR?

Simple TWR assumes both devices have identical clocks:
```
TOF = (round_trip_time - processing_time) / 2
```

But clock drift causes errors. Asymmetric TWR uses two round trips to cancel clock errors:
- First round: Tag → Anchor → Tag
- Second round: Anchor → Tag → Anchor

The formula mathematically eliminates clock frequency differences.
