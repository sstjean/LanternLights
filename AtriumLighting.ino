#include "Constants.h"
#include "AmbientLight.h"
#include "TwoStripManager.h"
#include "StripBrightnessManager.h"

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>

#include "Effect.h"
#include "FireEffect.h"
#include "ChristmasEffect.h"


// The set of available effects:
FireEffect _fireEffect;
ChristmasEffect _christmasEffect;

// The current effect:
Effect *_currentEffect;

// Strip and brightness managers:
StripManager *_stripManager = new TwoStripManager();
AmbientLight *_ambientLight = new AmbientLight(PIN_PHOTO_RESISTOR, TWILIGHT_LOW, TWILIGHT_HIGH);
StripBrightnessManager *_stripBrightnessManager = new StripBrightnessManager(_stripManager, _ambientLight);

bool _currentlyOn = false;
bool _needToTurnOff = false;
bool _needToTurnOn = false;

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

Adafruit_GFX_Button buttons[4];


void setup()
{
	Serial.begin(9600);
	Serial.println("Starting...");

	// Hardcoded effect selection (to be replaced with some kind of manual selection)
	_currentEffect = &_fireEffect;
	//_currentEffect = &_christmasEffect;


  tft.reset();
  tft.begin(0x9341);
  tft.setRotation(2);
  tft.fillScreen(BLACK);
  
  // create buttons
  buttons[0].initButton(&tft, BUTTON_X, BUTTON_Y, BUTTON_W_LG, BUTTON_H_LG, ILI9341_WHITE, ILI9341_DARKGREEN, ILI9341_WHITE, "On", BUTTON_TEXTSIZE_LG); 
  buttons[0].drawButton();
  buttons[1].initButton(&tft, BUTTON_X, BUTTON_Y + BUTTON_H_LG + BUTTON_SPACING_Y, BUTTON_W_LG, BUTTON_H_LG, ILI9341_WHITE, ILI9341_LIGHTGREY, ILI9341_WHITE,"Off", BUTTON_TEXTSIZE_LG);
  buttons[1].drawButton();

  buttons[2].initButton(&tft, BUTTON_X, BUTTON_Y + BUTTON_H_LG * 2 + BUTTON_SPACING_Y * 2, BUTTON_W_LG, BUTTON_H_LG, ILI9341_WHITE, ILI9341_ORANGE, ILI9341_WHITE,"Candle", BUTTON_TEXTSIZE_LG);
  buttons[2].drawButton();
  buttons[3].initButton(&tft, BUTTON_X, BUTTON_Y + BUTTON_H_LG * 3 + BUTTON_SPACING_Y * 3, BUTTON_W_LG, BUTTON_H_LG, ILI9341_WHITE, ILI9341_RED, ILI9341_WHITE,"XMas", BUTTON_TEXTSIZE_LG);
  buttons[3].drawButton();
  
	_stripManager->Init();
	_stripBrightnessManager->Init();
}

void loop()
{
/************************
 *    DEBUG
 */

 _currentlyOn = true;
          _needToTurnOff = false;
          _needToTurnOn = true;
          Serial.println("Turning on");

 //  END DEBUG


 
  ///////////////////////////////////////
  //
  //    Update Lights
  //
  ///////////////////////////////////////
  if (_needToTurnOff)
  {
    Serial.println("Turned off");
    _stripManager->TurnOff();
    _needToTurnOff = false;
  }

  if (_needToTurnOn)
  {
    Serial.println("Turned on");
    _stripBrightnessManager->Update();
    _stripManager->TurnOn();
    Serial.print("Current brightness: "); Serial.println(_stripBrightnessManager->GetBrightness());
    _needToTurnOn = false;
  }
  
  if (!_currentlyOn)
  {
    //The controller is set to Off
    return;
  }
 
	if (!_stripBrightnessManager->Update())
	{
		// The strip is OFF.
		return;
	}

	_currentEffect->Update();

	_stripManager->ApplyEffect(_currentEffect);

}
