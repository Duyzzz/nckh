#!/bin/bash

# UWB Project - Flash Helper
# Source this file: source export.sh
# Then use: flash-both, flash-tag, flash-anchor

UWB_DIR="/home/rogo/Desktop/test/uwb"
FQBN="esp32:esp32:esp32wrover"
TAG_PORT="/dev/ttyUSB0"
ANCHOR_PORT="/dev/ttyUSB1"

flash-tag() {
    echo "Compiling tag..."
    arduino-cli compile --fqbn "$FQBN" --library "$UWB_DIR/DW1000" "$UWB_DIR/main/tag" || return 1
    echo "Flashing tag to $TAG_PORT..."
    arduino-cli upload --fqbn "$FQBN" --port "$TAG_PORT" "$UWB_DIR/main/tag"
}

flash-anchor() {
    echo "Compiling anchor..."
    arduino-cli compile --fqbn "$FQBN" --library "$UWB_DIR/DW1000" "$UWB_DIR/main/anchor" || return 1
    echo "Flashing anchor to $ANCHOR_PORT..."
    arduino-cli upload --fqbn "$FQBN" --port "$ANCHOR_PORT" "$UWB_DIR/main/anchor"
}

flash-both() {
    echo "=== Compiling both ==="
    arduino-cli compile --fqbn "$FQBN" --library "$UWB_DIR/DW1000" "$UWB_DIR/main/tag" || return 1
    arduino-cli compile --fqbn "$FQBN" --library "$UWB_DIR/DW1000" "$UWB_DIR/main/anchor" || return 1

    echo "=== Flashing tag to $TAG_PORT ==="
    arduino-cli upload --fqbn "$FQBN" --port "$TAG_PORT" "$UWB_DIR/main/tag" || return 1

    echo "=== Flashing anchor to $ANCHOR_PORT ==="
    arduino-cli upload --fqbn "$FQBN" --port "$ANCHOR_PORT" "$UWB_DIR/main/anchor" || return 1

    echo "=== Done! ==="
    echo "Tag:    $TAG_PORT (monitor: screen $TAG_PORT 115200)"
    echo "Anchor: $ANCHOR_PORT (monitor: screen $ANCHOR_PORT 115200)"
}

echo "UWB flash commands loaded: flash-both, flash-tag, flash-anchor"
