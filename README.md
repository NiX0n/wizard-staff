# Wizard Staff

A programmable fantasy-themed wizard staff focused on lighting effects, spell simulation, and future gesture- and voice-controlled interactions.

The initial goal is to build a reliable, convention-ready prop for Halloween using a small number of components and a simple user interface. Future revisions will expand the staff into a more sophisticated interactive platform capable of gesture recognition, voice-triggered spell casting, sound effects, and custom animations.

---

## Project Goals

### Phase 0.1 - Input Validation

Validate the basic hardware platform:

- SparkFun RedBoard Artemis Nano
- SparkFun Qwiic Directional Pad
- USB power bank
- Serial debug output

The D-pad is used to navigate an invisible two-dimensional state table consisting of:

- Color selection (horizontal axis)
- Effect selection (vertical axis)

Changes are reported via USB serial output.

### Phase 0.2 - Lighting Effects

Integrate NeoPixels into the staff orb.

Features:

- Solid colors
- Rainbow mode
- Slow pulse
- Fast pulse
- Color-specific special effects

Examples:

| Color | Special Effect |
|---------|---------|
| White | Magical energy pulse |
| Red | Fire flicker |
| Blue | Water shimmer |
| Green | Arcane poison cloud |
| Rainbow | Rainbow chase |

### Phase 1.0 - Convention Ready

Produce a stable, portable prop suitable for costume use.

Requirements:

- Powered from USB battery pack
- Self-contained electronics compartment
- Hot-glued orb assembly
- Simple and reliable controls
- Non-blocking firmware architecture

### Future Development

Potential future enhancements:

#### Gesture Recognition

Detect staff motion using an accelerometer or IMU.

Examples:

- Circle motion
- Forward thrust
- Shake
- Swipe

#### Voice Activation

A gesture acts as a "wake" command:

```text
User performs gesture
       ↓
Staff enters listening mode
       ↓
User speaks spell
       ↓
Effect activates
```

Example spells:

- Lumos
- Lumos Maxima
- Nox
- Incendio
- Aguamenti

#### Audio Effects

Future additions may include:

- Crackling fire
- Magical hum
- Lightning discharge
- Water sounds
- Spell charge-up effects

---

# Hardware

## Controller

- SparkFun RedBoard Artemis Nano

## Input

- SparkFun Qwiic Directional Pad
- SparkFun Flexible Qwiic Cable (500 mm)

## Lighting

- Adafruit NeoPixel LED Dots
- Quantity: 2 pixels used

## Electronics

- 330Ω resistor (data line protection)
- Electrolytic capacitor (recommended across LED supply rails)

## Power

- USB power bank

## Mechanical

- Wooden staff
- Translucent white ornament globe
- Cotton stuffing for light diffusion
- Hot glue for final assembly

---

# User Interface

The D-pad navigates an invisible state table.

## Horizontal Axis (Left / Right)

```text
0  Off
1  White
2  Rainbow
3  Red
4  Orange
5  Yellow
6  Green
7  Blue
8  Indigo
9  Violet
```

No wraparound is used.

At either end of the list:

```text
Left on Off     -> Off
Right on Violet -> Violet
```

---

## Vertical Axis (Up / Down)

```text
0  Off
1  Solid
2  Slow
3  Fast
4  Special
```

No wraparound is used.

---

## Center Button

Reserved.

Behavior intentionally left undefined until practical experience identifies a useful purpose.

Potential uses:

- Brightness control
- Favorite spell
- Quick preset recall
- Diagnostic mode

---

# Electrical Design

## D-Pad Register Mapping

Experimentally determined:

```text
Register 0x00

Idle    = 0xFF

Up      = 0xFE
Down    = 0xFD
Right   = 0xFB
Left    = 0xF7
Center  = 0xEF
```

Equivalent bitfield representation:

```text
Bit 0 = Up
Bit 1 = Down
Bit 2 = Right
Bit 3 = Left
Bit 4 = Center

Active Low
```

```text
None    11111111
Up      11111110
Down    11111101
Right   11111011
Left    11110111
Center  11101111
```

Input processing is based on edge detection rather than button state polling to avoid repeated events while a button is held.

---

# NeoPixel Wiring

## Power

Current design powers NeoPixels from the Artemis 3.3V rail due to the Artemis Nano's available headers and low LED count.

```text
Artemis 3V3 -----> NeoPixel V+
Artemis GND -----> NeoPixel GND
```

## Data

```text
Artemis GPIO
      |
     330Ω
      |
      +----> DIN
```

A series resistor is placed on the data line near the controller.

## Capacitor

A capacitor is connected across the power rails:

```text
3V3 ----+-------> NeoPixels
         |
        +|
      1000uF
        -|
         |
GND -----+-------> NeoPixels
```

The capacitor resides in the electronics compartment rather than inside the orb for easier serviceability.

---

# NeoPixel Directionality

NeoPixels are directional devices.

Data must flow:

```text
DIN ----> Pixel ----> DOUT
```

Current understanding of the LED strand:

```text
Female Connector = DIN
Male Connector   = DOUT
```

Final verification should be performed during bring-up before permanent assembly.

---

# Physical Layout

## Orb Assembly

Contents:

```text
Translucent Globe
 ├─ NeoPixel #1
 ├─ NeoPixel #2
 └─ Cotton Diffuser
```

After testing is complete, the globe will be permanently secured using hot glue.

## Staff

A center bore is drilled only part-way into the staff.

The wiring path is:

```text
Orb
 │
 │
Center Bore
 │
 │
Electronics Compartment
 │
 ├─ Artemis Nano
 ├─ Capacitor
 ├─ Data Resistor
 └─ USB Connection
```

The electronics compartment is expected to be located approximately near the normal hand position on the staff.

This allows future maintenance without disturbing the orb assembly.

---

# Software Architecture

The firmware is designed around a non-blocking state machine.

```text
loop()
 ├─ processInput()
 ├─ updateEffectState()
 └─ renderEffects()
```

## Design Principles

- No delay()-based animation logic
- Fast main loop
- Edge-triggered input handling
- Animation timing based on millis()
- Input processing separated from effects rendering

This architecture is intended to scale to future additions including:

- IMU input
- Voice recognition
- Audio playback
- Additional lighting effects

without significant redesign.

---

# Repository Status

## Complete

- Hardware selection
- D-pad bring-up
- I²C discovery
- Button register mapping
- User interface design
- Firmware architecture planning

## In Progress

- NeoPixel integration
- Effect engine implementation
- Staff construction

## Future

- IMU integration
- Voice recognition
- Audio effects
- Advanced spell engine

---

# License

[MIT](LICENCE)
