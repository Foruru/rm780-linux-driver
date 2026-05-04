# Mouse Protocol

## Table of Contents

1. [First packet](#first-packet)
   1. [DPI Config](#dpi-config)
      1. [State](#state)
      2. [DPI Values](#dpi-values)
      3. [DPI Colors](#dpi-colors)
  2. [Effect Config](#effect-config)
     1. [Effect](#effect)
     2. [Brightness](#brightness)
     3. [Direction](#direction)
     4. [Random color](#random-color)
2. [Second packet](#second-packet)
   1. [USB polling](#usb-polling)
3. [Third packet](#third-packet)
   1. [Bindings](#bindings)
      1. [Mouse buttons](#mouse-buttons)
      2. [Key groups](#key-groups)
      3. [Keys](#keys)
      4. [Regular key](#regular-key)
      5. [Fire key](#fire-key)
      6. [Three click](#three-click)
      7. [DPI](#dpi)
      8. [Disable](#disable)
      9. [Key combination](#key-combination)
         1. [Mod keys](#mod-keys)
     10. [Media](#media)
     11. [Macro](#macro)
         1. [Cycle](#cycle)
         2. [Key Up/Down](#key-updown)
         3. [Delay](#delay)

# First packet

| Offset | Size     | Value                           | Notes                                                                    |
|--------|----------|---------------------------------|--------------------------------------------------------------------------|
| 0      | 1  byte  | ReportID                        |                                                                          |
| 1      | 1  byte  | 0x00                            |                                                                          |
| 2      | 1  byte  | DPI count                       | Number of active DPI entries.                                            |
| 3      | 1  byte  | 0x20                            | The mouse works only with this specific value.                           |
| 4      | 1  byte  | -                               |                                                                          |
| 5      | 6  bytes | [DPI Values](#dpi-values)       | One byte per DPI value.                                                  |
| 11     | 10 bytes | -                               |                                                                          |
| 21     | 2  bytes | [Effect Config](#effect-config) | Effect, speed (or brightness), direction (or random color), color count. |
| 23     | 21 bytes | Colors                          | Three bytes per color. Colors are left-aligned.                          |
| 44     | 6  bytes | [DPI Colors](#dpi-colors)       | One byte per DPI.                                                        |
| 50     | 2  bytes | -                               |                                                                          |
| 52     | 1  byte  | 0x20                            | Replaced with 0x22 when the effect is None.                              |
| 53     | 2  bytes | -                               |                                                                          |
| 55     | 5  bytes | -                               |                                                                          |

## DPI Config

```
   state
   |
.. 01 ..
    |
    dpi
```

### State

| State | Value |
|-------|-------|
| ON    | 8     |
| OFF   | 0     |

### DPI Values

| DPI  | value |
|------|-------|
| 500  | 1     |
| 750  | 2     |
| 1000 | 3     |
| 1250 | 4     |
| 1500 | 5     |
| 1750 | 6     |
| 2000 | 7     |
| 2500 | 8     |
| 3000 | 9     |
| 3500 | a     |
| 4000 | b     |

### DPI Colors

| Color  | Hex  |
|--------|------|
| Red    | 0x01 |
| Green  | 0x02 |
| Blue   | 0x03 |
| Cyan   | 0x04 |
| Yellow | 0x05 |
| Pink   | 0x06 |

## Effect Config

```
   effect
   |
   |    direction
   |       or
   |  random color
   |  |
.. 00 00 ..
    |  |
    |  color count
    |
    speed
     or
  brightness
```

- 'or' mean that value depend on current effect.
- Speed is value from 1 to 3.
- On None effect substituted 0x20 on 0x22 at the end.

### Effect

'R' is Random color.

| Effect             | Value | Colors | Speed | Brightness | Direction |
|--------------------|-------|--------|-------|------------|-----------|
| None               | 0     |        |       |            |           |
| Colorful Streaming | 1     |        | +     |            | +         |
| Steady             | 2     | 1      |       | +          |           |
| Breathing          | 3     | 7      | +     |            |           |
| Tail               | 4     |        | +     |            |           |
| Neon               | 5     |        | +     |            |           |
| Colorful Steady    | 6     | 7      |       |            |           |
| Flicker            | 7     | 2      |       |            |           |
| Response           | 8     | 7 / R  | +     |            |           |
| Streaming          | 9     |        | +     |            | +         |
| Wave               | a     |        | +     |            |           |
| Trailing           | b     |        | +     |            |           |

### Brightness

| %   | Value |
|-----|-------|
| 5   | 1     |
| 15  | 2     |
| 25  | 3     |
| 35  | 4     |
| 45  | 5     |
| 55  | 6     |
| 65  | 7     |
| 75  | 8     |
| 85  | 9     |
| 100 | 9     |

### Direction

| Direction | Value |
|-----------|-------|
| Forward   | 8     |
| Back      | 0     |

### Random color

| Random color | Value |
|--------------|-------|
| ON           | 8     |
| OFF          | 0     |

# Second packet

| Offset | Size    | Value                       | Notes                  |
|--------|---------|-----------------------------|------------------------|
| 0      | 1 byte  | ReportID                    |                        |
| 1      | 1 byte  | DPI swtiching               | Ability to change DPI  |
| 2      | 1 byte  | [USB polling](#usb-polling) | Polling rate           |
| 3      | 2 bytes | -                           |                        |
| 5      | 1 byte  | Initial DPI                 | Change DPI on response |
| 6      | 3 bytes | -                           |                        |

## USB polling

| Hz   | Hex  |
|------|------|
| 125  | 0x01 |
| 250  | 0x02 |
| 500  | 0x03 |
| 1000 | 0x04 |

# Third packet

| Offset | Size       | Value                 | Notes                          |
|--------|------------|-----------------------|--------------------------------|
| 0      | 1    byte  | ReportID              |                                |
| 1      | 1024 bytes | Macros storage        | Store actions of active macros |
| 1025   | 28   bytes | [Bindings](#bindings) | 4 bytes per bind.              |
| 1053   | 92   bytes | -                     |                                |

## Bindings

```
                                                                           DPI -
                                                               DPI +       |
                                                   forward     |           |
                                       back        |           |           |
                           middle      |           |           |           |
               right       |           |           |           |           |
   left        |           |           |           |           |           |
   |           |           |           |           |           |           |
.. 11 f0 00 00 12 f1 00 00 13 f2 00 00 14 f3 00 00 15 f4 00 00 46 20 00 00 47 40 00 00 ..

      
                                 mouse button   |
                                 |              |
                                 |    unused    |
                                 |    || ||     |
                                11 f0 00 00     | - regular key binding
                                |  ||           |
                                |  key          |
                                |               |
                                group           |
                                 regular key    |
```

### Mouse buttons

| Button        | Value |
|---------------|-------|
| Left Button   | 1     |
| Right Button  | 2     |
| Middle Button | 3     |
| Forward       | 4     |
| Back          | 5     |
| DPI +         | 6     |
| DPI -         | 7     |

### Key groups

| Group           | Value | Functions                           |
|-----------------|-------|-------------------------------------|
| Regular         | 1     | [Regular key](#regular-key)         |
| Fire key        | 2     | [Fire key](#fire-key)               |
| Three click     | 3     | [Three click](#three-click)         |
| Dpi             | 4     | [DPI](#dpi)                         |
| Disable         | 5     | [Disable](#disable)                 |
| Key combination | 6     | [Key combination](#key-combination) |
| Media           | 7     | [Media](#media)                     |
| Macro           | 9     | [Macro](#macro)                     |

### Keys

| Key | Hex  | Key | Hex  | Key       | Hex  | Key | Hex  | Key     | Hex  | Key     | Hex  | Key           | Hex  |
|-----|------|-----|------|-----------|------|-----|------|---------|------|---------|------|---------------|------|
| A   | 0x04 | 1   | 0x1e | Enter     | 0x28 | F1  | 0x3a | NumLock | 0x53 | L-Ctrl  | 0xe0 | Left Button   | 0xf0 |
| B   | 0x05 | 2   | 0x1f | Esc       | 0x29 | F2  | 0x3b | Num /   | 0x54 | L-Shift | 0xe1 | Right Button  | 0xf1 |
| C   | 0x06 | 3   | 0x20 | Backspace | 0x2a | F3  | 0x3c | Num *   | 0x55 | L-Alt   | 0xe2 | Middle Button | 0xf2 |
| D   | 0x07 | 4   | 0x21 | Tab       | 0x2b | F4  | 0x3d | Num -   | 0x56 | L-Win   | 0xe3 | Forward       | 0xf3 |
| E   | 0x08 | 5   | 0x22 | Space     | 0x2c | F5  | 0x3e | Num +   | 0x57 | R-Ctrl  | 0xe4 | Back          | 0xf4 |
| F   | 0x09 | 6   | 0x23 | -         | 0x2d | F6  | 0x3f | Num 1   | 0x59 | R-Shift | 0xe5 |               |      |
| G   | 0x0a | 7   | 0x24 | =         | 0x2e | F7  | 0x40 | Num 2   | 0x5a | R-Alt   | 0xe6 |               |      |
| H   | 0x0b | 8   | 0x25 | [         | 0x2f | F8  | 0x41 | Num 3   | 0x5b | R-Win   | 0xe7 |               |      |
| I   | 0x0c | 9   | 0x26 | ]         | 0x30 | F9  | 0x42 | Num 4   | 0x5c |         |      |               |      |
| J   | 0x0d | 0   | 0x27 | \         | 0x31 | F10 | 0x43 | Num 5   | 0x5d |         |      |               |      |
| K   | 0x0e |     |      | ;         | 0x33 | F11 | 0x44 | Num 6   | 0x5e |         |      |               |      |
| L   | 0x0f |     |      | '         | 0x34 | F12 | 0x45 | Num 7   | 0x5f |         |      |               |      |
| M   | 0x10 |     |      | `         | 0x35 |     |      | Num 8   | 0x60 |         |      |               |      |
| N   | 0x11 |     |      | ,         | 0x36 |     |      | Num 9   | 0x61 |         |      |               |      |
| O   | 0x12 |     |      | .         | 0x37 |     |      | Num 0   | 0x62 |         |      |               |      |
| P   | 0x13 |     |      | /         | 0x38 |     |      | Num .   | 0x63 |         |      |               |      |
| Q   | 0x14 |     |      | Caps Lock | 0x39 |     |      |         |      |         |      |               |      |
| R   | 0x15 |     |      |           |      |     |      |         |      |         |      |               |      |
| S   | 0x16 |     |      |           |      |     |      |         |      |         |      |               |      |
| T   | 0x17 |     |      |           |      |     |      |         |      |         |      |               |      |
| U   | 0x18 |     |      |           |      |     |      |         |      |         |      |               |      |
| V   | 0x19 |     |      |           |      |     |      |         |      |         |      |               |      |
| W   | 0x1a |     |      |           |      |     |      |         |      |         |      |               |      |
| X   | 0x1b |     |      |           |      |     |      |         |      |         |      |               |      |
| Y   | 0x1c |     |      |           |      |     |      |         |      |         |      |               |      |
| Z   | 0x1d |     |      |           |      |     |      |         |      |         |      |               |      |

### Regular key

| Key     | Hex  |
|---------|------|
| Left    | 0xf0 |
| Right   | 0xf1 |
| Middle  | 0xf2 |
| Back    | 0xf3 |
| Forward | 0xf4 |

### Fire key

```
    mouse button
    |
    |    parameters
    |    || ||
.. 23 04 05 01 ..
   |  ||
   |  button
   |
   group
```

- 1st parameter is Intervals(5-255)
- 2nd parameter is Loop(1-255)

### Three click

Acts like a regular key with the value 0x01.

### DPI

| key      | hex  |
|----------|------|
| DPI loop | 0x00 |
| DPI +    | 0x20 |
| DPI -    | 0x40 |
| DPI 500  | 0x81 |
| DPI 750  | 0x82 |
| DPI 1000 | 0x83 |
| DPI 1250 | 0x84 |
| DPI 1500 | 0x85 |
| DPI 1750 | 0x86 |
| DPI 2000 | 0x87 |
| DPI 2500 | 0x88 |
| DPI 3000 | 0x89 |
| DPI 3500 | 0x8a |
| DPI 4000 | 0x8b |

### Disable

Acts like a regular key with the value 0x01.

### Key combination

```
     mouse button
     |
     |    buttons
     |    || ||
 .. 63 00 04 05 ..
    |  ||
    |  mod key/s
    |
    group
```

#### Mod keys

A key combination is the sum of the keys in the specified combination.

| Key   | Hex  |
|-------|------|
| Ctrl  | 0x01 |
| Shift | 0x02 |
| Alt   | 0x04 |
| Win   | 0x08 |

### Media

| Key          | Hex            |
|--------------|----------------|
| Media player | 0x00           |
| Play/pause   | 0x08           |
| Next         | 0x01           |
| Previous     | 0x02           |
| Stop         | 0x04           |
| Mute         | 0x10           |
| Volume up    | 0x40           |
| Volume down  | 0x80           |
| Email        | 0x00 0x10      |
| Calculator   | 0x00 0x20      |
| Explorer     | 0x00 0x02      |
| Home page    | 0x00 0x00 0x02 |

### Macro

```
n   cycle times          key            key
   || ||                ||             ||
.. 00 00 ..       .. 01 00 .. or .. 1b 04 00 03 ..
                     ||             ||    ||
                     delay          delay ||
                                     (ms) ||
                                          delay(0.1s)

   mouse button
   |
.. 93 11 00 00 ..
   |  ||
   |  cycle
   |
   group
```

- One macro consists of 127 bytes.
- In one time can loaded 8 macros.
- 31 full size key calls can be.

#### Cycle

Specified cycle times is value in range 1-65535 that can be configure earlier.

| Description                  | Hex  |
|------------------------------|------|
| Cycle until the key released | 0x14 |
| Cycle until any key pressed  | 0x12 |
| Specified cycle times        | 0x11 |

#### Key Up/Down

Used when delay isn't specified.

| Key  | Hex  |
|------|------|
| Up   | 0x01 |
| Down | 0x81 |

#### Delay

If the delay is less than 100, it is stored directly as a value in milliseconds.
If the delay is greater than or equal to 100, the hundreds are separated and stored in the second part.
