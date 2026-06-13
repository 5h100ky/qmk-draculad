# DracuLad RP2040

A port of the [DracuLad](https://github.com/MangoIV/dracuLad) split keyboard firmware for the **SparkFun Pro Micro RP2040**, with [Vial](https://get.vial.today/) support, dual rotary encoders, and an optional [Pimoroni Trackball (PIM447)](https://shop.pimoroni.com/products/trackball-breakout) on the right half.

---

## Overview

The original DracuLad was designed around the ATmega32U4 Pro Micro. This port adapts the firmware to the **RP2040** microcontroller (same Pro Micro footprint) using the Vial-QMK fork.

| Item | Original | This Port |
|------|----------|-----------|
| MCU | ATmega32U4 | **RP2040** (Pro Micro footprint) |
| WS2812 driver | `bitbang` | `vendor` (RP2040 PIO) |
| Split serial driver | `bitbang` | `vendor` (RP2040 PIO, half-duplex) |
| Encoder pins | AVR port labels (`B2`, `B4` …) | RP2040 GPIO numbers (`GP21`, `GP8` …) |
| Trackball | — | Pimoroni PIM447 (right half, I2C) |
| Keymap editor | — | Vial (real-time, no reflash needed) |

---

## Hardware

| Component | Specification |
|-----------|---------------|
| MCU | SparkFun Pro Micro RP2040 (or compatible) |
| Split connection | TRRS cable — half-duplex single-wire serial |
| RGB LEDs | WS2812B × 10 (5 per side) |
| Encoders | EC11 rotary encoder × 2 (one per half) |
| Display | SSD1306 OLED 128×64 (I2C, one per half) |
| Trackball | Pimoroni PIM447 (right half only, I2C) — optional |

**Important:** Always plug USB into the **left half**. The left half is the master (`MASTER_LEFT`).

---

## Pin Mapping

### Common (both halves use the same PCB)

| Function | GPIO |
|----------|------|
| Split serial (TRRS data) | `GP1` |
| WS2812B data | `GP0` |
| I2C SDA — OLED + Trackball | `GP2` |
| I2C SCL — OLED + Trackball | `GP3` |
| Matrix rows | `GP4`, `GP5`, `GP6`, `GP7` |
| Matrix columns | `GP29`, `GP28`, `GP27`, `GP26`, `GP22` |

### Encoder

| Side | Pin A | Pin B |
|------|-------|-------|
| Left | `GP21` | `GP23` |
| Right | `GP8` | `GP9` |

> The left encoder A/B pins are swapped relative to the ATmega32U4 original to correct the rotation direction on the RP2040.

### Trackball (right half)

| Signal | GPIO |
|--------|------|
| I2C SDA | `GP2` |
| I2C SCL | `GP3` |
| I2C address | `0x0A` |
| I2C clock | 400 kHz |

The trackball PCB is physically mounted at 45° on the DracuLad. The firmware applies a coordinate rotation (`new_x = x − y`, `new_y = x + y`) so movement aligns with the keyboard axes.

---

## Default Keymap

Five layers, editable at runtime via Vial.

| Layer | Name | Description |
|-------|------|-------------|
| 0 | `BASE` | QWERTY with home-row mod-taps |
| 1 | `NUM` | Numbers, arrow keys, mouse buttons |
| 2 | `SYMB` | Symbols and function keys |
| 3 | `MUS` | Mouse layer (left-hand mouse buttons) |
| 4 | `ADJ` | RGB / boot controls |

### Encoder actions

| Encoder | Action |
|---------|--------|
| Left | Volume up / down |
| Right | Volume up / down |

### OLED

| Half | Rotation | Content |
|------|----------|---------|
| Left (master) | 270° | Layer name, WPM, Caps Lock |
| Right (slave) | 90° | DracuLad logo |

---

## File Structure

```
qmk-draculad/
├── keyboard.json            # Matrix, encoders, RGB, split config
├── config.h                 # I2C, trackball, OLED, handedness defines
├── rules.mk                 # SERIAL_DRIVER, POINTING_DEVICE_DRIVER
└── keymaps/
    └── vial/
        ├── config.h         # VIAL_KEYBOARD_UID, layer count
        ├── keymap.c         # Keymap, OLED rendering, encoder & trackball logic
        ├── rules.mk         # VIAL_ENABLE = yes
        └── vial.json        # Vial layout descriptor
```

---

## Building

### Requirements

- [Vial-QMK](https://get.vial.today/docs/porting-to-vial.html) build environment
- `arm-none-eabi-gcc` toolchain

### Compile

```bash
# From vial-qmk root, after placing this folder under keyboards/
qmk compile -kb 5h100ky/dracula/qmk_draculad -km vial
```

> QMK requires folder names to use underscores. The build copies files into `qmk_draculad` (underscore) automatically via GitHub Actions.

### Flash

Put the Pro Micro RP2040 into bootloader mode by **double-tapping the reset button** (or hold BOOT while plugging in). A drive named `RPI-RP2` will appear.

```bash
cp .build/5h100ky_dracula_qmk_draculad_vial.uf2 /Volumes/RPI-RP2
```

Flash both halves separately with the **same** `.uf2` file. Handedness is set by `MASTER_LEFT` in `config.h` — no `EE_HANDS` or separate left/right binaries needed.

### GitHub Actions (auto-build)

Every push to `main` triggers a build. Download the artifact `draculad-rp2040-vial` from the Actions tab.

---

## Key Implementation Notes

### Encoder debounce

The left encoder's EC11 detent generates equal CW and CCW quadrature pulses at rest, causing the volume to oscillate without net change. A 250 ms opposite-direction debounce is applied in `encoder_update_user` to suppress the spring-back pulse.

### Trackball 45° correction

```c
// In pointing_device_task_user:
int16_t nx = x - y;
int16_t ny = x + y;
```

This rotates the sensor coordinate frame 45° clockwise to match the physical keyboard orientation.

### Split encoder indices

Both encoders report as global index `0` in `encoder_update_user` under this Vial-QMK version. The left encoder direction is corrected by swapping `pin_a`/`pin_b` in `keyboard.json` rather than in software.

---

## Troubleshooting

| Symptom | Cause / Fix |
|---------|-------------|
| Only one half works | USB must be in the **left** half; check TRRS cable seating |
| Encoder volume oscillates (up then down) | Encoder spring-back bounce — 250 ms debounce is applied; ensure encoder pins are well soldered |
| Trackball moves diagonally | 45° rotation correction in `pointing_device_task_user` already handles this; check I2C solder joints if trackball is unresponsive |
| OLED shows nothing | Verify I2C solder on `GP2`/`GP3`; check `OLED_DISPLAY_128X64` define |
| Build fails: folder name error | QMK forbids hyphens in keyboard paths — use `qmk_draculad` (underscore) |
| RGB does not light up | WS2812B data pin is `GP0`; driver must be `vendor` in `keyboard.json` |

---

## Credits

- Original DracuLad design and firmware: [MangoIV](https://github.com/MangoIV)
- QMK Firmware: [qmk/qmk_firmware](https://github.com/qmk/qmk_firmware)
- Vial: [vial-kb/vial-qmk](https://github.com/vial-kb/vial-qmk)
- Pimoroni Trackball QMK driver: QMK community contributors

---

## License

This firmware is based on [QMK Firmware](https://github.com/qmk/qmk_firmware) and is distributed under the **GNU General Public License v2 (or later)**. See [LICENSE](LICENSE) for the full text.

> The original DracuLad hardware design by MangoIV does not carry an explicit license. This RP2040 firmware port is shared for personal, non-commercial use by the keyboard community, in the same open spirit as the original project.
