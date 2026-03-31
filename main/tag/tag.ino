/*
 * UWB Tag with Custom Data
 *
 * Sends custom data to anchors in POLL messages
 * Receives custom data from anchors in RANGE_REPORT messages
 */

#include <SPI.h>
#include "DW1000Ranging.h"

#define TAG_ADDR "7D:00:22:EA:82:60:3B:9B"

#define SPI_SCK 18
#define SPI_MISO 19
#define SPI_MOSI 23

const uint8_t PIN_RST = 27; // reset pin
const uint8_t PIN_IRQ = 34; // irq pin
const uint8_t PIN_SS = 21;  // spi select pin (21 for Pro variant)

// Custom data to send to anchors (e.g., sensor data)
byte tagData[4] = {0x01, 0x02, 0x03, 0x04};

// Timing
uint32_t lastRangeTime = 0;
uint32_t rangeCount = 0;
uint32_t lastStatsTime = 0;

void setup()
{
    Serial.begin(115200);
    delay(1000);

    // Init SPI
    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
    DW1000Ranging.initCommunication(PIN_RST, PIN_SS, PIN_IRQ);

    // Antenna delay calibration - decrease to reduce measured distance
    DW1000.setAntennaDelay(16500);

    // Airtime calculation mode: false = 110 kbps (default), true = 6.8 Mbps
    DW1000Ranging.use6M8Mode(true);  // Uncomment for 6.8 Mbps mode

    // Attach handlers
    DW1000Ranging.attachNewRange(newRange);
    DW1000Ranging.attachNewDevice(newDevice);
    DW1000Ranging.attachInactiveDevice(inactiveDevice);
    DW1000Ranging.attachCustomDataReceived(onCustomDataReceived);

    // Enable range filter to smooth readings
    DW1000Ranging.useRangeFilter(true);

    // Start as tag
    DW1000Ranging.startAsTag(TAG_ADDR, DW1000.MODE_LONGDATA_RANGE_LOWPOWER);

    Serial.println("Tag ready with custom data support");
}

void loop()
{
    // Update tag data (example: could be sensor readings)
    // Here we simulate sensor data with a counter
    static uint32_t lastUpdate = 0;
    if (millis() - lastUpdate > 500) {
        tagData[0]++;
        // Could read real sensors here:
        // tagData[0] = readTemperature();
        // tagData[1] = readHumidity();
        // etc.
        lastUpdate = millis();
    }

    // Set custom data to be sent in POLL
    DW1000Ranging.setCustomData(tagData, sizeof(tagData));

    DW1000Ranging.loop();
}

void newRange()
{
    uint32_t now = millis();
    uint32_t cycleTime = now - lastRangeTime;
    lastRangeTime = now;
    rangeCount++;

    Serial.print("from: ");
    Serial.print(DW1000Ranging.getDistantDevice()->getShortAddress(), HEX);
    Serial.print("\t Range: ");
    Serial.print(DW1000Ranging.getDistantDevice()->getRange());
    Serial.print(" m");
    Serial.print("\t RX power: ");
    Serial.print(DW1000Ranging.getDistantDevice()->getRXPower());
    Serial.print(" dBm");
    Serial.print("\t Cycle: ");
    Serial.print(cycleTime);
    Serial.print(" ms");
    Serial.print("\t POLL: ");
    Serial.print(DW1000Ranging.getPollAirTimeUs());
    Serial.println(" us");

    // Print stats every 5 seconds
    if (now - lastStatsTime >= 5000) {
        float rate = rangeCount * 1000.0 / (now - lastStatsTime);
        Serial.print(">>> Ranging rate: ");
        Serial.print(rate, 1);
        Serial.println(" Hz");
        rangeCount = 0;
        lastStatsTime = now;
    }
}

void newDevice(DW1000Device *device)
{
    Serial.print("ranging init; 1 device added ! -> ");
    Serial.print(" short:");
    Serial.println(device->getShortAddress(), HEX);
}

void inactiveDevice(DW1000Device *device)
{
    Serial.print("delete inactive device: ");
    Serial.println(device->getShortAddress(), HEX);
}

void onCustomDataReceived()
{
    byte* data = DW1000Ranging.getReceivedCustomData();
    uint8_t len = DW1000Ranging.getReceivedCustomDataLen();

    Serial.print("Custom data from anchor (");
    Serial.print(len);
    Serial.print(" bytes): ");
    for (uint8_t i = 0; i < len; i++) {
        if (data[i] < 0x10) Serial.print("0");
        Serial.print(data[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
}
