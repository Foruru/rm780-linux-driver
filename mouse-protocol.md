# Mouse USB HID protocol

This documentation explain how to communicate with mouse (REAL EL RM-780 Gaming computer mouse) over USB HID protocol.

## Packets

Communication is based on packets. All 3 packets must be sent in order:
1. Packet I
2. Packet II
3. Packet III

Mouse does not send any data.

### Packet I

Byte length: 59
Report ID: 0x04

| Offset | Size | Field           | Description                                             |
|--------|------|-----------------|---------------------------------------------------------|
| 0      | 1 B  | Report ID       | USB HID report ID                                       |
| 1      | 1 B  | Reserved        | Always 0x00                                             |
| 2      | 1 B  | DPI count       | Number of active DPI levels (e.g. 1–6)                  |
| 3      | 1 B  | Critical        | Must be 0x20, otherwise device stops responding         |
| 4      | 1 B  | -               | Unknown                                                 |
| 5      | 6 B  | DPI levels      | One byte per DPI level                                  |
| 11     | 10 B | -               | Unknown                                                 |
| 21     | 2 B  | Effect config   | Define mouse`s light effect, colors etc                 |
| 23     | 21 B | Mouse colors    | Three bytes per color in RGB and is left aligned        |
| 44     | 6 B  | DPI colors      | One byte per color                                      |
| 50     | 2 B  | -               | Unknown                                                 |
| 52     | 1 B  | Vendor specific | 0x20 normally; 0x22 when lighting effect is set to None |
| 53     | 6 B  | Reserved        | Always 0x00                                             |

### Packet II

Byte length: 9
Report ID: 0x08

| Offset | Size | Field       | Description              |
|--------|------|-------------|--------------------------|
| 0      | 1 B  | Report ID   | USB HID report ID        |
| 1      | 1 B  | DPI switch  | Ability to change DPI    |
| 2      | 1 B  | USB polling | Polling rate             |
| 3      | 2 B  | -           | -                        |
| 5      | 1 B  | Initial DPI | Changes DPI upon request |
| 6      | 3 B  | -           | -                        |

### Packet III

Byte length: 1145
Report ID: 0x06

| Offset | Size   | Field              | Description                       |
|--------|--------|--------------------|-----------------------------------|
| 0      | 1 B    | Report ID          | USB HID report ID                 |
| 1      | 1024 B | Macros             | 128 bytes per macro, max 8 macros |
| 1025   | 40 B   | Profile 1 bindings | 10 bindings, 4 bytes each         |
| 1053   | 40 B   | Profile 2 bindings | 10 bindings, 4 bytes each         |
| 1091   | 40 B   | Profile 3 bindings | 10 bindings, 4 bytes each         |

## DPI

### DPI levels

| DPI  | Hex  |
|------|------|
| 500  | 0x01 |
| 750  | 0x02 |
| 1000 | 0x03 |
| 1250 | 0x04 |
| 1500 | 0x05 |
| 1750 | 0x06 |
| 2000 | 0x07 |
| 2500 | 0x08 |
| 3000 | 0x09 |
| 3500 | 0x0a |
| 4000 | 0x0b |

### DPI state

DPI can be disabled by adding `0x80` to its hex value.

### DPI colors

| Color  | Hex  |
|--------|------|
| Red    | 0x01 |
| Green  | 0x02 |
| Blue   | 0x03 |
| Cyan   | 0x04 |
| Yellow | 0x05 |
| Pink   | 0x06 |

### DPI switch

Specified DPI switch ability. On - 0x01, Off - 0x00.

### Initial DPI

Set active DPI upon request.

Additional:
- DPI indexes starts form 1.
- Inactive slots are ignored regardless of their value.

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
- Direction: Forward - 8, Back - 0.
- Random color: On - 8, Off - 0.

### Effect

'R' is Random color.

| Effect             | Hex  | Colors | Speed | Brightness | Direction |
|--------------------|------|--------|-------|------------|-----------|
| None               | 0x00 |        |       |            |           |
| Colorful Streaming | 0x10 |        | +     |            | +         |
| Steady             | 0x20 | 1      |       | +          |           |
| Breathing          | 0x30 | 7      | +     |            |           |
| Tail               | 0x40 |        | +     |            |           |
| Neon               | 0x50 |        | +     |            |           |
| Colorful Steady    | 0x60 | 7      |       |            |           |
| Flicker            | 0x70 | 2      |       |            |           |
| Response           | 0x80 | 7 / R  | +     |            |           |
| Streaming          | 0x90 |        | +     |            | +         |
| Wave               | 0xa0 |        | +     |            |           |
| Trailing           | 0xb0 |        | +     |            |           |

### Brightness

| %   | Hex  |
|-----|------|
| 5   | 0x01 |
| 15  | 0x02 |
| 25  | 0x03 |
| 35  | 0x04 |
| 45  | 0x05 |
| 55  | 0x06 |
| 65  | 0x07 |
| 75  | 0x08 |
| 85  | 0x09 |
| 100 | 0x09 |TODO

## USB polling

| Hz   | Hex  |
|------|------|
| 125  | 0x01 |
| 250  | 0x02 |
| 500  | 0x03 |
| 1000 | 0x04 |

## Bindings

Bindings is a sequence of 10 mouse keys (however mouse has only 7 physical keys), which can be redefined.

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
                                 |    reserved  |
                                 |    || ||     |
                                11 f0 00 00     | - regular key bind
                                |  ||           |
                                |  key          |
                                |               |
                                group           |
```

### Mouse buttons

| Button        | Hex  |
|---------------|------|
| Left Button   | 0x01 |
| Right Button  | 0x02 |
| Middle Button | 0x03 |
| Back          | 0x04 |
| Forward       | 0x05 |
| DPI +         | 0x06 |
| DPI -         | 0x07 |

### Key groups

| Group           | Hex  | Functions       |
|-----------------|------|-----------------|
| Regular         | 0x10 | Regular key     |
| Fire key        | 0x20 | Fire key        |
| Three click     | 0x30 | Three click     |
| DPI             | 0x40 | DPI             |
| Disable         | 0x50 | Disable         |
| Key combination | 0x60 | Key combination |
| Media           | 0x70 | Media           |
| Macro           | 0x90 | Macro           |

### Keys

| Key | Hex  | Key | Hex  | Key       | Hex  | Key | Hex  | Key     | Hex  | Key     | Hex  | Key           | Hex  |
|-----|------|-----|------|-----------|------|-----|------|---------|------|---------|------|---------------|------|
| A   | 0x04 | 1   | 0x1e | Enter     | 0x28 | F1  | 0x3a | NumLock | 0x53 | L-Ctrl  | 0xe0 | Left Button   | 0xf0 |
| B   | 0x05 | 2   | 0x1f | Esc       | 0x29 | F2  | 0x3b | Num /   | 0x54 | L-Shift | 0xe1 | Right Button  | 0xf1 |
| C   | 0x06 | 3   | 0x20 | Backspace | 0x2a | F3  | 0x3c | Num *   | 0x55 | L-Alt   | 0xe2 | Middle Button | 0xf2 |
| D   | 0x07 | 4   | 0x21 | Tab       | 0x2b | F4  | 0x3d | Num -   | 0x56 | L-Win   | 0xe3 | Back          | 0xf3 |
| E   | 0x08 | 5   | 0x22 | Space     | 0x2c | F5  | 0x3e | Num +   | 0x57 | R-Ctrl  | 0xe4 | Forward       | 0xf4 |
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

Regular keys: Left Button, Right Button, Middle Button, Forward, Back.

### Fire key

```
    mouse button
    |
    |    interval
    |    ||
.. 23 04 05 01 ..
   |  ||    ||
   |  ||    loop
   |  button
   |
   group
```

- 1st parameter is Intervals(5-255)
- 2nd parameter is Loop(1-255)

### Three click

Acts like a regular key with the key value 0x01.

### DPI

| Key      | Hex  |
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

### Macros

```
   cycle times          key            key
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

- One macro consists of 128 bytes.
- First 2 bytes specifie cycle.
- In one time can loaded 8 macros, which size depend on delay.
- ~31 key calls with delay bigger than or equal 100 ms can be.

#### Cycle

Specified cycle times is value in range 1-65535 that can be configure earlier.

| Description                  | Hex  |
|------------------------------|------|
| Cycle until the key released | 0x14 |
| Cycle until any key pressed  | 0x12 |
| Specified cycle times        | 0x11 |

#### Key Up/Down

When the key is pressed, 0x80 is added to the first delay.

#### Delay

If the delay is less than 100 ms, it is stored directly as a value in milliseconds.
If the delay is equal to or greater than 100 ms, the hundreds are separated and stored in the second part.
