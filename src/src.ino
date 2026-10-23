#include <Wire.h>

// ============================================================================
// Hardware Configuration
// ============================================================================

constexpr uint8_t DPAD_I2C_ADDRESS = 0x20;
constexpr uint8_t DPAD_BUTTON_REGISTER = 0x00;

// Active-low button values discovered experimentally.
constexpr uint8_t BUTTON_STATE_NONE   = 0xFF;
constexpr uint8_t BUTTON_STATE_UP     = 0xFE;
constexpr uint8_t BUTTON_STATE_DOWN   = 0xFD;
constexpr uint8_t BUTTON_STATE_RIGHT  = 0xFB;
constexpr uint8_t BUTTON_STATE_LEFT   = 0xF7;
constexpr uint8_t BUTTON_STATE_CENTER = 0xEF;

// ============================================================================
// Staff Menu Definitions
// ============================================================================

enum class ColorMode : uint8_t
{
    Off = 0,
    White,
    Rainbow,
    Red,
    Orange,
    Yellow,
    Green,
    Blue,
    Indigo,
    Violet,

    Count
};

enum class EffectMode : uint8_t
{
    Off = 0,
    Solid,
    Slow,
    Fast,
    Special,

    Count
};

// ============================================================================
// Application State
// ============================================================================

struct StaffState
{
    ColorMode color = ColorMode::Off;
    EffectMode effect = EffectMode::Off;
};

StaffState g_staffState;

// ============================================================================
// Helper Functions
// ============================================================================

const char* colorToString(ColorMode color)
{
    switch (color)
    {
        case ColorMode::Off:     return "Off";
        case ColorMode::White:   return "White";
        case ColorMode::Rainbow: return "Rainbow";
        case ColorMode::Red:     return "Red";
        case ColorMode::Orange:  return "Orange";
        case ColorMode::Yellow:  return "Yellow";
        case ColorMode::Green:   return "Green";
        case ColorMode::Blue:    return "Blue";
        case ColorMode::Indigo:  return "Indigo";
        case ColorMode::Violet:  return "Violet";
        default:                 return "Unknown";
    }
}

const char* effectToString(EffectMode effect)
{
    switch (effect)
    {
        case EffectMode::Off:     return "Off";
        case EffectMode::Solid:   return "Solid";
        case EffectMode::Slow:    return "Slow";
        case EffectMode::Fast:    return "Fast";
        case EffectMode::Special: return "Special";
        default:                  return "Unknown";
    }
}

void printCurrentSelection()
{
    Serial.print("Color: ");
    Serial.print(colorToString(g_staffState.color));

    Serial.print(" | Effect: ");
    Serial.println(effectToString(g_staffState.effect));
}

// ============================================================================
// D-Pad Access
// ============================================================================

uint8_t readDPadState()
{
    Wire.beginTransmission(DPAD_I2C_ADDRESS);
    Wire.write(DPAD_BUTTON_REGISTER);

    if (Wire.endTransmission(false) != 0)
    {
        return BUTTON_STATE_NONE;
    }

    Wire.requestFrom(DPAD_I2C_ADDRESS, static_cast<uint8_t>(1));

    if (!Wire.available())
    {
        return BUTTON_STATE_NONE;
    }

    return Wire.read();
}

// ============================================================================
// Event Handling
// ============================================================================

void onLeftPressed()
{
    uint8_t value = static_cast<uint8_t>(g_staffState.color);

    if (value > 0)
    {
        --value;
        g_staffState.color = static_cast<ColorMode>(value);
        printCurrentSelection();
    }
}

void onRightPressed()
{
    uint8_t value = static_cast<uint8_t>(g_staffState.color);

    uint8_t maximum =
        static_cast<uint8_t>(ColorMode::Count) - 1;

    if (value < maximum)
    {
        ++value;
        g_staffState.color = static_cast<ColorMode>(value);
        printCurrentSelection();
    }
}

void onDownPressed()
{
    uint8_t value = static_cast<uint8_t>(g_staffState.effect);

    if (value > 0)
    {
        --value;
        g_staffState.effect = static_cast<EffectMode>(value);
        printCurrentSelection();
    }
}

void onUpPressed()
{
    uint8_t value = static_cast<uint8_t>(g_staffState.effect);

    uint8_t maximum =
        static_cast<uint8_t>(EffectMode::Count) - 1;

    if (value < maximum)
    {
        ++value;
        g_staffState.effect = static_cast<EffectMode>(value);
        printCurrentSelection();
    }
}

void onCenterPressed()
{
    Serial.println("Center button pressed (unassigned)");
}

// ============================================================================
// Input Processing
// ============================================================================

void processInput()
{
    static uint8_t previousButtonState = BUTTON_STATE_NONE;

    uint8_t currentButtonState = readDPadState();

    if (currentButtonState == previousButtonState)
    {
        return;
    }

    previousButtonState = currentButtonState;

    switch (currentButtonState)
    {
        case BUTTON_STATE_UP:
            onUpPressed();
            break;

        case BUTTON_STATE_DOWN:
            onDownPressed();
            break;

        case BUTTON_STATE_LEFT:
            onLeftPressed();
            break;

        case BUTTON_STATE_RIGHT:
            onRightPressed();
            break;

        case BUTTON_STATE_CENTER:
            onCenterPressed();
            break;

        default:
            break;
    }
}

// ============================================================================
// Effect Engine Stub
// ============================================================================

void updateEffects()
{
    // Empty for now.
    //
    // Eventually this function will:
    //   - Update animation timers
    //   - Compute NeoPixel colors
    //   - Push frames to the LEDs
    //
    // It should remain completely independent
    // from how button inputs are processed.
}

// ============================================================================
// Arduino Entry Points
// ============================================================================

void setup()
{
    Serial.begin(115200);

    while (!Serial)
    {
        delay(10);
    }

    Wire.begin();

    Serial.println();
    Serial.println("Wizard Staff");
    Serial.println("-----------------");

    printCurrentSelection();
}

void loop()
{
    processInput();
    updateEffects();
}