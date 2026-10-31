#include <Wire.h>
#include <Adafruit_NeoPixel.h>

// ============================================================================
// Hardware Configuration
// ============================================================================

//
// Inputs
//
constexpr uint8_t DPAD_I2C_ADDRESS = 0x20;
constexpr uint8_t DPAD_BUTTON_REGISTER = 0x00;

// Active-low button values
constexpr uint8_t BUTTON_STATE_NONE   = 0xFF;
constexpr uint8_t BUTTON_STATE_UP     = 0xFE;
constexpr uint8_t BUTTON_STATE_DOWN   = 0xFD;
constexpr uint8_t BUTTON_STATE_RIGHT  = 0xFB;
constexpr uint8_t BUTTON_STATE_LEFT   = 0xF7;
constexpr uint8_t BUTTON_STATE_CENTER = 0xEF;

// Active-high 
constexpr uint8_t BUTTON_UP_MASK     = 0x01;
constexpr uint8_t BUTTON_DOWN_MASK   = 0x02;
constexpr uint8_t BUTTON_RIGHT_MASK  = 0x04;
constexpr uint8_t BUTTON_LEFT_MASK   = 0x08;
constexpr uint8_t BUTTON_CENTER_MASK = 0x10;

//
// Outputs
//
constexpr uint8_t N_NEOPIXEL = 40;
constexpr uint8_t NEOPIXEL_PIN = A16;
constexpr neoPixelType NEOPIXEL_TYPE = NEO_GRB + NEO_KHZ800;
//constexpr uint8_t NEOPIXEL_TYPE = NEO_GRB + NEO_KHZ800;
//constexpr uint8_t NEOPIXEL_TYPE = 0;

Adafruit_NeoPixel strip(N_NEOPIXEL, NEOPIXEL_PIN, NEOPIXEL_TYPE);


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
    static uint8_t previousState = 0;

    uint8_t currentState = ~readDPadState();

    uint8_t changed = currentState ^ previousState;
    uint8_t pressed = changed & currentState;

    previousState = currentState;

    if (pressed & BUTTON_UP_MASK)
    {
        onUpPressed();
    }

    if (pressed & BUTTON_DOWN_MASK)
    {
        onDownPressed();
    }

    if (pressed & BUTTON_LEFT_MASK)
    {
        onLeftPressed();
    }

    if (pressed & BUTTON_RIGHT_MASK)
    {
        onRightPressed();
    }

    if (pressed & BUTTON_CENTER_MASK)
    {
        onCenterPressed();
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

    // initialize strip
    strip.begin();
    //strip.show(); // Initialize all pixels to 'off'
    //strip.setBrightness(255);
    
    //strip.rainbow();
    //strip.setPixelColor(0, 255, 0, 0);
    //strip.setPixelColor(1, 0, 0, 255);
    strip.clear();

    strip.show();
    Serial.println("LEDs Engaged!");
}

void loop()
{
    processInput();
    updateEffects();
}