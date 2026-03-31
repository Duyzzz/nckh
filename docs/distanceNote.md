# UWB Range Capabilities

## Quick Reference

| Mode | Typical Range (LOS) | Indoor Range | Best For |
|------|---------------------|--------------|----------|
| 110 kbps | 200-300 m | 30-50 m | Long range, outdoor |
| 6.8 Mbps | 30-50 m | 10-20 m | Fast update, indoor |

---

## 110 kbps Mode (Default - Long Range)

### Configuration
- Preamble: 1024 symbols
- Data rate: 110 kbps
- SFD: 64 symbols

### Range Performance

| Environment | Expected Range |
|-------------|----------------|
| Open field (LOS) | 200-300 m |
| Indoor open space | 30-50 m |
| Indoor with walls | 15-30 m |
| Heavy obstruction | 5-15 m |

### Why Long Range?

1. **Long preamble (1024 symbols)**
   - More time for receiver synchronization
   - Better channel estimation
   - Receiver sensitivity: **-121 dBm**

2. **Low data rate (110 kbps)**
   - More energy per bit
   - Better noise immunity
   - Can decode weaker signals

3. **Long SFD (64 symbols)**
   - More reliable frame detection

### Trade-offs
- Air time: ~2.7 ms per frame
- Max ranging rate: ~10 Hz
- Higher power consumption

---

## 6.8 Mbps Mode (Fast)

### Configuration
- Preamble: 128 symbols
- Data rate: 6.8 Mbps
- SFD: 8 symbols

### Range Performance

| Environment | Expected Range |
|-------------|----------------|
| Open field (LOS) | 30-50 m |
| Indoor open space | 10-20 m |
| Indoor with walls | 5-10 m |
| Heavy obstruction | 2-5 m |

### Why Shorter Range?

1. **Short preamble (128 symbols)**
   - Less sync time
   - Receiver sensitivity: **-105 dBm**

2. **High data rate (6.8 Mbps)**
   - Less energy per bit
   - More susceptible to noise

3. **Short SFD (8 symbols)**
   - Less robust frame detection

### Trade-offs
- Air time: ~180 µs per frame
- Max ranging rate: ~100+ Hz
- Lower power consumption

---

## Sensitivity Comparison

```
110 kbps:  -121 dBm  ████████████████████████  (better)
6.8 Mbps:  -105 dBm  ████████████████          (16 dB worse)
```

### What 16 dB Difference Means

Using the inverse square law for RF propagation:

```
Range ratio = 10^(dB_difference / 20)
            = 10^(16 / 20)
            = 10^0.8
            ≈ 6.3x
```

So 6.8 Mbps has roughly **1/6th the range** of 110 kbps mode.

| If 110 kbps achieves | 6.8 Mbps achieves |
|----------------------|-------------------|
| 300 m | ~50 m |
| 100 m | ~16 m |
| 50 m | ~8 m |
| 30 m | ~5 m |

---

## Factors Affecting Range

### 1. Line of Sight (LOS) vs Non-Line of Sight (NLOS)

| Condition | Impact |
|-----------|--------|
| Clear LOS | Maximum range |
| Glass/wood | -3 to -6 dB |
| Drywall | -3 to -5 dB |
| Concrete | -10 to -20 dB |
| Metal | Blocked |
| Human body | -3 to -6 dB |

### 2. Antenna

| Antenna Type | Gain | Range Impact |
|--------------|------|--------------|
| Chip antenna | 0 dBi | Baseline |
| PCB antenna | 2-3 dBi | +25-40% |
| External dipole | 3-5 dBi | +40-80% |

### 3. TX Power

DW1000 allows TX power adjustment:
- Higher power = longer range
- Must comply with regulations (FCC/ETSI)
- Typical max: +14 dBm (smart TX power)

### 4. Channel

| Channel | Frequency | Notes |
|---------|-----------|-------|
| 1 | 3.5 GHz | Better penetration |
| 2 | 4.0 GHz | Common |
| 3 | 4.5 GHz | Common |
| 4 | 4.0 GHz | Wider bandwidth |
| 5 | 6.5 GHz | Default, good balance |
| 7 | 6.5 GHz | Wider bandwidth |

Lower frequencies (Ch 1-3) penetrate obstacles better.

---

## Recommendations

### Use 110 kbps when:
- Range > 30 meters needed
- Outdoor applications
- Through-wall ranging required
- Reliability more important than speed
- Low update rate acceptable (~10 Hz)

### Use 6.8 Mbps when:
- Range < 30 meters sufficient
- Indoor line-of-sight
- High update rate needed (>50 Hz)
- Lower power consumption important
- Fast response time required

### For Your Setup (~1.8m range)
Either mode works fine. 6.8 Mbps is recommended for:
- Faster ranging updates
- Lower air time
- Less spectrum usage
- More capacity for multiple tags

---

## Testing Your Range

To find actual range limits:

1. Start close (1-2 m)
2. Verify stable ranging
3. Move apart gradually
4. Note distance when:
   - Range variance increases
   - Packet loss begins
   - Ranging fails completely

Monitor RX power level:
- Good: > -70 dBm
- OK: -70 to -85 dBm
- Weak: -85 to -95 dBm
- Limit: < -95 dBm (may fail)
