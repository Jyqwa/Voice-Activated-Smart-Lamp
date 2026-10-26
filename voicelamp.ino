#include "DFRobot_DF2301Q.h"

#define Led 13
#define greenLed 12
#define redLed 11
#define blueLed 10
#define brightnessPin 9
#define sensor A2

unsigned long startTime = 0;      // Stores the time when the pin turned ON
const unsigned long duration = 60000; // How long to stay ON (2 seconds)
bool pinIsActive = false;
int currentBrightness = 255;


//I2C communication
DFRobot_DF2301Q_I2C asr;



void setup() {
  Serial.begin(115200);

  pinMode(Led, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
  pinMode(brightnessPin, OUTPUT);
  pinMode(sensor, INPUT);
  setColor(0,0,0);



  // Init the sensor
  while (!(asr.begin())) {
    Serial.println("Communication with device failed, please check connection");
    delay(3000);
  }
  Serial.println("Begin ok!");

  /**
   * @brief Set voice volume
   * @param voc - Volume value(1~7)
   */
  asr.setVolume(4);

  /**
     @brief Set mute mode
     @param mode - Mute mode; set value 1: mute, 0: unmute
  */
  asr.setMuteMode(0);

  /**
     @brief Set wake-up duration
     @param wakeTime - Wake-up duration (0-255)
  */
  asr.setWakeTime(20);

  /**
     @brief Get wake-up duration
     @return The currently-set wake-up period
  */
  uint8_t wakeTime = 0;
  wakeTime = asr.getWakeTime();
  Serial.print("wakeTime = ");
  Serial.println(wakeTime);

  // asr.playByCMDID(1);   // Wake-up command

  /**
     @brief Play the corresponding reply audio according to the ID
     @param CMDID - command word ID
  */
  //asr.playByCMDID(23);  // Command word ID
}

void loop() {
  
  bool value = digitalRead(sensor);
  if(value == 1){
    digitalWrite(brightnessPin, HIGH);
    startTime = millis();
    pinIsActive = true;
  }
  else if(value == 0 && (millis() - startTime >= duration)){
    digitalWrite(brightnessPin, LOW);
    pinIsActive = false;
  }
  
  
  

  /**
     @brief Get the ID corresponding to the command word 
     @return Return the obtained command word ID, returning 0 means no valid ID is obtained
  */
  uint8_t CMDID = asr.getCMDID();
  switch (CMDID) {
    case 103:                                                  //If the command is “Turn on the light”
      setColor(0,0,0);
      
                                     //Turn on the LED
      Serial.println("received'Turn on the light',command flag'103'");  //Serial transmits "received"Turn on the light",command flag"103
      break;

    case 104:                                                  //If the command is “Turn off the light”
      setColor(255,255,255);                               //Turn off the LED
      Serial.println("received'Turn off the light',command flag'104'");  //The serial transmits "received"Turn off the light",command flag"104""
      break;

                                                               //the basic structure will be the case number found on te wiki, then the commands for the LED
    case 105:
      currentBrightness = currentBrightness + 100;
      analogWrite(brightnessPin, currentBrightness);
    case 106:
      currentBrightness = currentBrightness - 100;
      analogWrite(brightnessPin, currentBrightness);
    case 107:
    currentBrightness = 255;
      digitalWrite(brightnessPin, 255);
    case 108:
      currentBrightness = 64;
      analogWrite(brightnessPin, 10);
    case 116:                                                  //set to red
      setColor(0,255,255);                                       //set the color to red, will need to set up the color pins
      break;
    case 117:
      setColor(0, 165, 255);
      break;
    case 118:
      setColor(0,0,255);
      break;
    case 119:
      setColor(255,0,255);
      break;
    case 120:
      setColor(255,0,0);
      break;
    case 121:
      setColor(255, 255, 0);
      break;
    case 122:
      setColor(0, 255, 0);
      break;
    case 123:
      setColor(0,0,0);
      break;
    
    default:
      if (CMDID != 0) {
        Serial.print("CMDID = ");  //Printing command ID
        Serial.println(CMDID);
      }
  }
  
}

void setColor(int red, int green, int blue) {
  #ifdef COMMON_ANODE
    // Invert the values if using a Common Anode LED
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif

  analogWrite(redLed, red);
  analogWrite(greenLed, green);
  analogWrite(blueLed, blue);
}