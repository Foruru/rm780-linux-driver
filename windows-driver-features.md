# Windows Driver Features

## Table of Contents
- [Driver Features](#driver-features)
  - [Profiles](#profiles)
  - [Bindings](#bindings)
  - [DPI Settings](#dpi-settings)
  - [Lighting](#lighting)
  - [Mouse Parameters](#mouse-parameters)
  - [Macro Editor](#macro-editor)

# Driver Features

## Profiles

You can perform the following actions with profiles:
- Create
- Delete
- Modify
- Export
- Rename

## Bindings

### Available Buttons

The following mouse buttons can be configured:
1. Left Button
2. Right Button
3. Middle Button
4. Back
5. Forward
6. DPI +
7. DPI -

### Assignable Functions

The following functions can be assigned to the buttons above:
- Left Click
- Right Click
- Middle Click
- Back
- Forward
- Triple Click
- Fire Key
- Key Combination
- Macro
- Disable
- DPI (dropdown list)
- Multimedia (dropdown list)
- DPI Lock (dropdown list)

### Dropdown Values

| DPI        | Multimedia   | DPI Lock |
|------------|--------------|----------|
| DPI +      | Media Player | 500      |
| DPI -      | Play/Pause   | 750      |
| DPI Cycle  | Next Track   | 1000     |
|            | Previous     | 1250     |
|            | Stop         | 1500     |
|            | Mute         | 1750     |
|            | Volume Up    | 2000     |
|            | Volume Down  | 2500     |
|            | Email        | 3000     |
|            | Calculator   | 3500     |
|            | File Explorer| 4000     |
|            | Home Page    |          |

## DPI Settings

Supports up to six DPI levels, each defined by:
- **Enabled (ON):** Boolean value
- **DPI Value:** Predefined list of integer values
- **Color:** RGB value (3 bytes, each from 0 to 255)

## Lighting

Lighting consists of multiple effects, each with its own configurable parameters:

| Effect             | Color | Speed | Brightness | Direction |
|--------------------|-------|-------|------------|-----------|
| Colorful Streaming |       | +     |            | +         |
| Steady             | 1     |       | +          |           |
| Breathing          | 7     | +     |            |           |
| Tail               |       | +     |            |           |
| Neon               |       | +     |            |           |
| Colorful           | 7     |       |            |           |
| Flicker            | 2     |       |            |           |
| Response           | 7 / R | +     |            |           |
| Streaming          |       | +     |            | +         |
| Wave               |       | +     |            |           |
| Trailing           |       | +     |            |           |
| None               |       |       |            |           |

**Notes:**
- `R` = Random color
- Color can be disabled for supported effects

## Mouse Parameters

- **Sensitivity:** Range from 1 to 20
- **Enhance Pointer Precision:** On/Off
- **Scrolling Speed:** Range from 1 to 10
- **Scroll One Page:** On/Off
- **Double-Click Speed:** 200 to 830 ms (step: 70 ms)
- **Polling Rate:** 125 Hz, 250 Hz, 500 Hz, 1000 Hz

## Macro Editor

### Macro List
- Create new macro
- Delete macro

### Key List
- Modify key events
- Delete key events

### Recording Options
- Start recording
- Automatically insert delays
- Playback modes:
  - Cycle until key is released
  - Cycle until any key is released
  - Fixed cycle count

### Manual Event Insertion
- Key Down
- Key Up
- Delay
- Left Click
- Right Click
- Middle Click

**Notes:**
- Only keyboard keys can be recorded
- Only Latin alphabet is supported
- Delay range: 1 to 2,147,483,647 ms (32-bit integer limit)
- Fixed cycle range: 1 to 65,535 ms (32-bit integer limit)
