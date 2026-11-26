"""Generate the binary HEX replay example used by UltraSonicHMI."""

import math
import random
import struct
from pathlib import Path

CHANNELS = 12
SAMPLES = 4096
HEADER_BYTES = 258
CHANNEL_GAP_BYTES = 2
MV_PER_COUNT = 0.04882887


def generate():
    noise = random.Random(2026)
    frame = bytearray(HEADER_BYTES)
    for ch in range(CHANNELS):
        if ch:
            frame.extend(bytes(CHANNEL_GAP_BYTES))
        pulses = (
            (670 + ch * 21, 115, 1140 - ch * 33),
            (1810 + ch * 28, 170, 630 - ch * 19),
            (3040 - ch * 16, 105, 350 + ch * 9),
        )
        frequency = 0.023 + ch * 0.0008
        for index in range(SAMPLES):
            value = noise.uniform(-16, 16)
            for center, width, amplitude in pulses:
                delta = index - center
                value += amplitude * math.exp(-0.5 * (delta / width) ** 2) * math.cos(
                    2 * math.pi * frequency * delta
                )
            sample = round(value / MV_PER_COUNT)
            frame.extend(struct.pack("<h", sample))
    return frame


if __name__ == "__main__":
    Path(__file__).with_name("ultrasonic-demo.hex").write_bytes(generate())
