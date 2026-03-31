# UWB Air Time Calculation

## Table of Contents
- [Frame Structure](#frame-structure)
- [Air Time Formula](#air-time-formula)
- [Mode Examples](#mode-examples)
- [Mode Comparison](#mode-comparison)

---

## Frame Structure

A UWB frame consists of several components:

```
┌──────────────────────────────────────────────────────────────────┐
│                         UWB FRAME                                 │
├─────────────┬───────┬───────┬────────────────────────────────────┤
│  PREAMBLE   │  SFD  │  PHR  │              DATA                  │
│  (Sync)     │       │       │  (MAC Header + Payload + FCS)      │
└─────────────┴───────┴───────┴────────────────────────────────────┘
     SHR (Synchronization Header)        PHR + Data
```

### 1. Preamble (SYNC)

- **Purpose**: Receiver synchronization, channel estimation, timing acquisition
- **Structure**: Repeated preamble symbols
- **Length options**: 64, 128, 256, 512, 1024, 1536, 2048, 4096 symbols
- **Longer preamble** = Better sensitivity, longer range, but more air time

| PRF | Symbol Period |
|-----|---------------|
| 16 MHz | 993.59 ns |
| 64 MHz | 1017.63 ns |

**Example**: 1024 symbols × 993.59 ns = **1017 µs**

### 2. SFD (Start Frame Delimiter)

- **Purpose**: Marks the end of preamble and start of data
- **Function**: Receiver uses SFD to find exact frame timing

| Data Rate | SFD Length | Duration (16 MHz PRF) |
|-----------|------------|----------------------|
| 110 kbps | 64 symbols | 64 µs |
| 850 kbps | 8 symbols | 8 µs |
| 6.8 Mbps | 8 symbols | 8 µs |

### 3. PHR (PHY Header)

- **Purpose**: Contains frame length and control info
- **Content**: 19 bits (frame length, ranging bit, reserved bits)

| Data Rate | PHR Duration |
|-----------|--------------|
| 110 kbps | 172.3 µs |
| 850 kbps | 21.5 µs |
| 6.8 Mbps | 21.5 µs |

### 4. Data (PSDU - PHY Service Data Unit)

- **Content**: MAC header + Payload + FCS (2 bytes Frame Check Sequence)
- **MAC header**: 9 bytes (short) or 21 bytes (long)
- **Duration**: Depends on data rate and frame length

```
Data Time = (Frame Length in bytes × 8 bits) / Data Rate
```

---

## Air Time Formula

```
Air Time = T_preamble + T_SFD + T_PHR + T_data
```

Where:
```
T_preamble = N_symbols × T_symbol
T_SFD      = SFD_symbols × T_symbol
T_PHR      = (fixed based on data rate)
T_data     = (frame_bytes × 8) / data_rate
```

### Fixed Overhead per Frame

| Mode | Preamble | SFD | PHR | **Total Overhead** |
|------|----------|-----|-----|-------------------|
| 110 kbps | 1017 µs | 64 µs | 172 µs | **1253 µs** |
| 6.8 Mbps | 127 µs | 8 µs | 22 µs | **157 µs** |

---

## Mode Examples

### 110 kbps Mode (Long Range)

Configuration: 1024 preamble, 16 MHz PRF, 64-symbol SFD

| Component | Calculation | Duration |
|-----------|-------------|----------|
| Preamble | 1024 × 993.59 ns | 1017 µs |
| SFD | 64 × 993.59 ns | 64 µs |
| PHR | (fixed) | 172 µs |
| Data (20 bytes) | 20 × 8 / 110 kbps | 1454 µs |
| **Total** | | **2707 µs** |

### 6.8 Mbps Mode (Fast)

Configuration: 128 preamble, 16 MHz PRF, 8-symbol SFD

| Component | Calculation | Duration |
|-----------|-------------|----------|
| Preamble | 128 × 993.59 ns | 127 µs |
| SFD | 8 × 993.59 ns | 8 µs |
| PHR | (fixed) | 22 µs |
| Data (20 bytes) | 20 × 8 / 6800 kbps | 24 µs |
| **Total** | | **181 µs** |

---

## Mode Comparison

### Air Time for TWR Messages

| Frame | Size | 110 kbps | 6.8 Mbps |
|-------|------|----------|----------|
| POLL | ~20 bytes | 2707 µs | 181 µs |
| POLL_ACK | ~10 bytes | 1980 µs | 169 µs |
| RANGE | ~35 bytes | 3800 µs | 198 µs |
| RANGE_REPORT | ~23 bytes | 2925 µs | 184 µs |
| **Total TWR** | | **~11.4 ms** | **~732 µs** |

### Trade-offs

| Aspect | 110 kbps | 6.8 Mbps |
|--------|----------|----------|
| Range | ~300m | ~30m |
| Air Time | Long | Short |
| Multipath Resistance | Better | Worse |
| Power Consumption | Higher (longer TX) | Lower |
| Max Ranging Rate | ~10 Hz | ~100+ Hz |
| Noise Immunity | Better | Worse |

### Overhead Ratio

The fixed overhead (preamble + SFD + PHR) as percentage of total frame time:

**110 kbps mode (20-byte frame):**
```
Overhead = 1253 µs
Data     = 1454 µs
Ratio    = 1253 / 2707 = 46%
```

**6.8 Mbps mode (20-byte frame):**
```
Overhead = 157 µs
Data     = 24 µs
Ratio    = 157 / 181 = 87%
```

At higher data rates, the fixed overhead dominates - adding more payload costs very little extra air time.

---

## Configuration in Code

Edit `DW1000/src/DW1000RangingConfig.h`:

```cpp
// For 6.8 Mbps mode (fast, short range)
#define USING_6_8M_MODE

// For 110 kbps mode (default, long range)
// (leave USING_6_8M_MODE commented out)
```

---

## References

- DW1000 User Manual (Section 9: Frame Format)
- DW1000 Datasheet (Section 4: RF Characteristics)
- IEEE 802.15.4-2011 UWB PHY Specification
