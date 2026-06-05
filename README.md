# DracuLad RP2040

> A port of the [DracuLad](https://github.com/qmk/qmk_firmware/tree/master/keyboards/draculad) keyboard firmware for the **SparkFun Pro Micro RP2040** (and compatible boards), with Vial support, dual rotary encoders, and optional Pimoroni Trackball on the right half.

---

## Overview

This firmware is based on the original DracuLad keyboard by [MangoIV](https://github.com/MangoIV), ported from ATmega32U4 to the **RP2040 microcontroller** (Pro Micro footprint). It targets the [Vial-QMK](https://get.vial.today/) fork for real-time keymap editing.

**Key differences from the original:**
- MCU: ATmega32U4 → **Pro Micro RP2040**
- WS2812 driver: `bitbang` → `vendor` (RP2040 PIO)
- Split serial driver: `bitbang` → `vendor` (RP2040 PIO, half-duplex)
- Encoder pin naming: AVR-style (`B2`, `B4`) → RP2040-style (`GP10`, `GP12`, etc.)
- Added Pimoroni Trackball support on the right half via I2C

---

## Hardware

| Component | Detail |
|-----------|--------|
| MCU | SparkFun Pro Micro RP2040 (or compatible) |
| Split connection | TRRS cable (half-duplex, single wire) |
| RGB | WS2812 LEDs × 10 (5 per side) |
| Encoders | 2× rotary encoders (one per side) |
| Display | OLED (I2C) |
| Trackball | Pimoroni Trackball (right half, I2C) — optional |

---

## Pin Mapping

| Function | Pin |
|----------|-----|
| Split serial (TRRS) | `GP1` |
| WS2812 data | `GP7` |
| I2C SDA (OLED + Trackball) | `GP2` |
| I2C SCL (OLED + Trackball) | `GP3` |
| Encoder L A/B | `GP10` / `GP14` |
| Encoder R A/B | `GP12` / `GP13` |

> ⚠️ **Note:** Verify all pin assignments against your actual PCB schematic before flashing. These values are based on a common Pro Micro RP2040 pinout and may differ depending on your specific board revision.

---

## File Structure

```
keyboards/your_keyboard/
├── keyboard.json       # Matrix, encoder, RGB, split config
├── config.h            # Split, I2C, trackball defines
├── rules.mk            # SERIAL_DRIVER, POINTING_DEVICE_DRIVER
└── keymaps/
    └── vial/
        ├── config.h    # VIAL_KEYBOARD_UID, unlock combo
        ├── keymap.c    # Default keymap
        ├── rules.mk    # VIAL_ENABLE = yes
        └── vial.json   # Vial layout definition
```

---

## Building

### Prerequisites

- [Vial-QMK](https://get.vial.today/docs/porting-to-vial.html) build environment set up
- `arm-none-eabi-gcc` toolchain

### Compile

```bash
make your_keyboard_path:vial
```

### Flash

Put the Pro Micro RP2040 into bootloader mode (double-tap reset or hold BOOT while plugging in), then copy the generated `.uf2` file to the mounted RPI-RP2 drive:

```bash
cp .build/your_keyboard_path_vial.uf2 /Volumes/RPI-RP2
```

Or use QMK Toolbox.

### Set Handedness (first flash only)

Flash each half separately using EE_HANDS:

```bash
# Left half
qmk flash -kb your_keyboard -km vial -bl uf2-split-left

# Right half
qmk flash -kb your_keyboard -km vial -bl uf2-split-right
```

---

## Key Configuration

### `keyboard.json` highlights

```json
"ws2812": { "pin": "GP7", "driver": "vendor" },
"split": {
    "enabled": true,
    "serial": { "pin": "GP1" }
}
```

### `rules.mk`

```makefile
SERIAL_DRIVER = vendor
POINTING_DEVICE_DRIVER = pimoroni_trackball
POINTING_DEVICE_ENABLE = yes
```

### `config.h`

```c
#define EE_HANDS
#define SPLIT_POINTING_ENABLE
#define SOFT_SERIAL_PIN GP1
#define POINTING_DEVICE_RIGHT
#define I2C_DRIVER I2CD1
#define I2C1_SDA_PIN GP2
#define I2C1_SCL_PIN GP3
```

---

## Troubleshooting

| Error | Fix |
|-------|-----|
| `'B2' undeclared` | Replace AVR-style pins (`B2`) with RP2040-style (`GP10`) in `keyboard.json` |
| `Please use vendor WS2812 driver` | Add `"driver": "vendor"` to `ws2812` in `keyboard.json` |
| `palWaitLineTimeout` undeclared | Set `SERIAL_DRIVER = vendor` in `rules.mk` |
| `SERIAL_USART_TX_PIN redefined` | Remove `SERIAL_USART_TX_PIN`; use `SOFT_SERIAL_PIN` instead |
| `SPLIT_POINTING_ENABLE not defined` | Add `#define SPLIT_POINTING_ENABLE` to `config.h` |

---

## Credits

- Original DracuLad keyboard design & firmware: [MangoIV](https://github.com/MangoIV)
- QMK Firmware: [qmk/qmk_firmware](https://github.com/qmk/qmk_firmware)
- Vial: [vial-kb/vial-qmk](https://github.com/vial-kb/vial-qmk)
- Pimoroni Trackball QMK driver: QMK community

---

## License

This project is based on QMK Firmware and follows the same license terms.

```
Copyright (C) 2024 contributors

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://www.gnu.org/licenses/>.
```

The original DracuLad keyboard design is by MangoIV and is used with respect to its original licensing terms.
