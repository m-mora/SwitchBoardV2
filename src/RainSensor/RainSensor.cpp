/******************************************************************************
 * Project: Klic Smart Irrigation System
 * 
 * Description:
 *    [Briefly describe what this file does, e.g., "Handles input/output 
 *    operations for user interaction in the application."]
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
Rain Sensor digital output has to be calibrated
TBD Define procedure with team to calibrate sensor

A rain drop sensor is basically a board on which
When there is no rain drop on board. Resistance is high so we get high voltage according to V=IR.
When rain drop present it reduces the resistance because water is a conductor of electricity and
the presence of water connects nickel lines in parallel so reduced resistance and the reduced voltage
drop across it.

*/
void ReadRainSensor (
  void
  )
{
  return ;
}


/*
Rain Sensor digital output has to be calibrated
TBD Define procedure with team to calibrate sensor

*/
void ReadMoisture (
  void
  )
{
  return ;
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
