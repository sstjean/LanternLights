#include "Constants.h"
#include "AmbientLight.h"
#include "TwoStripManager.h"
#include "StripBrightnessManager.h"

#include "Effect.h"
#include "FireEffect.h"
#include "ChristmasEffect.h"


// The set of available effects:
FireEffect _fireEffect;
//ChristmasEffect _christmasEffect;

// The current effect:
Effect *_currentEffect;

// Strip and brightness managers:
StripManager *_stripManager = new TwoStripManager();
AmbientLight *_ambientLight = new AmbientLight(PIN_PHOTO_RESISTOR, TWILIGHT_LOW, TWILIGHT_HIGH);
StripBrightnessManager *_stripBrightnessManager = new StripBrightnessManager(_stripManager, _ambientLight);

//Debug
unsigned long loop_count;

void setup()
{
	Serial.begin(57600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Starting...");

	// Hardcoded effect selection (to be replaced with some kind of manual selection)
	_currentEffect = &_fireEffect;
	//_currentEffect = &_christmasEffect;
  
	_stripManager->Init();
	_stripBrightnessManager->Init();
  Serial.println("...Finished setup()");
}

void loop()
{
    _stripBrightnessManager->Update();
 	  _currentEffect->Update();
  	_stripManager->ApplyEffect(_currentEffect);

   Serial.print("Loop #");
   Serial.println(loop_count++);
  
}
