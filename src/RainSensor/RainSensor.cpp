/******************************************************************************
 * Project: Klic Smart Irrigation System
 * 
 * Description:
 *    Module to read rain sensors potentiometer based (analog and digital inputs) 
 *    
 * 
 * License:
 *    This project is open for use, modification, and distribution for personal
 *    or educational purposes only. Commercial use is strictly prohibited 
 *    without explicit permission from the author
 * 
 * Usage:
 *    Rainsensor module with output digital and analog input formats
 *    a voltage comparator is widely use
 * 
 * TODO:
 *    
 ******************************************************************************/

#include <Arduino.h>

using namespace std;

typedef enum MOISTURE_STATES {
  dry = 0,
  wet,
  soaked
};

//
// To be a class...
//
typedef struct {
  unsigned char DigitalSensorPin;
  unsigned char AnalogSensorPin;
} RAIN_SENSOR_PARAM;


class RainSensor {
  public:
    RainSensor (uint8_t  DigitalInPin, uint8_t AnalogInPin) : DigitalInPin (DigitalInPin),  AnalogInPin (AnalogInPin) {

    }

  uint16_t MoisturePct (void);

  private:

    const uint8_t  DigitalInPin;
    const uint8_t  AnalogInPin;

    void InitAdc (void);
    uint16_t ReadRainSensor (void);
};


void RainSensor::InitAdc (void) {

  
  pinMode(AnalogInPin, INPUT);

  //
  // 10 bits ADC counter 0 <= counts < 1023
  //
  analogReadResolution(10);
  
  //
  // Max Input voltage
  //
  analogSetAttenuation(ADC_11db);
}

/*
Rain Sensor digital output has to be calibrated

Add moisture 
Turn potentiometer to desidered treshold

*/
void EnterCalibratioMode (

  )
{
  return ;
}


/*
Rain Sensor digital output has to be calibrated
TBD Define procedure with team to calibrate sensor

*/
void ExitCalibratioMode (
  void
  )
{
  return ;
}

/*

When rain drop present it reduces the resistance because water is a conductor of electricity and
the presence of water connects nickel lines in parallel so reduced resistance and the reduced voltage
drop across it.

*/
uint16_t RainSensor::ReadRainSensor (
  void
  )
{
  return analogRead (AnalogInPin);
}


uint16_t RainSensor::MoisturePct (void) {
  //
  // Adc resolution to be adjustable
  //
  return (ReadRainSensor () * 100 )/ ((1 << 10) - 1)
}

void DetermineMoistureLevels (
  void
  )
{


}


void SamplerFilter (
  void
  )
{

}
