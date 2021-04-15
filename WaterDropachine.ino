
/* Water Drop Machine V1
*
* Sketch that control the waterDropMachin created to perform water drop photography.
* The system is based on arduino and a solenoid valve that drops water at a regular time interval.
* The flash and the camero trigger are automatically triggered at predefined interval.
* By Antranik Zekian, 2021 - part of the code freely inspired from the code "Water Drop Controller V4" by Gareth Bellamy
*
*/


//Constant declaration
const int DELAY_CAM_FLASH = 100; // delay between the camera triggering and the flash triggering
const int CAMERA_BUTTON_HOLD_TIME = 100; // delay between the camera triggering and the flash triggering

//Arduino pin declaration
const int camPIN = 3;        //Set camPIN (camera) to pin 3
const int flashPIN = 4;        //Set camPIN (camera) to pin 3
const int solPIN = 8;        //Set solPIN (soleniod valve) to pin 8

int inputs[5] = {0, 0, 0, 0, 0}; //{1st drop size, 2nd drop size, valve delay, camera delay, iterations}




void displayValues()
{

  int i =0;
      
  while (i<5)
  {
    Serial.println("Display value in memory:");
    Serial.print(inputs[i]);
    Serial.print("|");  
    i++;
  }
  
  Serial.print("|");  

}

/*
Function that launch the water dropping and capture actions
DropOneSize - 1st drop size input variable (milliseconds)
DropTwoSize - 2nd drop size input variable (milliseconds)
SolDelay - Set delay between drops (milliseconds)
CamDelay - Set delay between drops and camera activation (milliseconds)
*/

void launchCapture(int DropOneSize,int DropTwoSize,int SolDelay,int CamDelay)
{
  
  digitalWrite (solPIN, HIGH);            //Opens solenoid valve
  delay (DropOneSize);                    //Valve stays open for time value DropOneSize -> define 1st drop size
  digitalWrite (solPIN, LOW);             //Closes solenoid valve
  
  delay (SolDelay);                       //Time delay between drops
  digitalWrite (solPIN, HIGH);            //Opens solenoid valve
  delay (DropTwoSize);                    //Valve stays open for time value DropTwoSize - 2nd drop size
  digitalWrite (solPIN, LOW);             //Closes solenoid valve
  
  delay (CamDelay);                       //Time delay for camera activation
  
  digitalWrite (camPIN, HIGH);            //Sets camPIN to high - fires camera
  delay (DELAY_CAM_FLASH);                //Holds camPIN high for 100 milliseconds before firing the flash
  digitalWrite (flashPIN, HIGH);          //Sets flashcamPIN to high - trigger flash
  delay (CAMERA_BUTTON_HOLD_TIME);        //Holds camPIN high for 100 milliseconds to ensure signal
  digitalWrite (flashPIN, LOW);             //Sets flashPIN to low
  digitalWrite (camPIN, LOW);               //Sets camPIN to low
  
}



void setup() {

  //Initial Configuration of Arduino setup
  
   Serial.begin(9600);                //Start the Serial Monitor
   pinMode (camPIN, OUTPUT);          //Set camPIN to an output
   pinMode (flashPIN, OUTPUT);        //Set flashPIN  to an output
   pinMode (solPIN, OUTPUT);          //Set solPIN to an output

}


void loop() {
   
//Here is where the time delays will be entered into the serial monitor

   Serial.println("Enter the settings - Format: ");
   Serial.println("1st drop size (ms) | 2nd drop size (ms) in milliseconds | Valve Delay (ms) | Camera Delay (ms) | Iteration");               //Print text asking for 1st drop size
   
   if (Serial.available() > 0) {

    char buff[32];
    int returned = Serial.readBytesUntil('\n', buff, 32);
    buff[31] = 0;    
    char *values = strtok(buff, "|");
    
    // Check data typed and convert them into int
    if (returned == 0) {
    
      Serial.println("ERROR: invalid data received");
    
    } else {
    
      Serial.println("INFO: valid data received");
      
      int i = 0;
      
      while (values != NULL) {
        
        int val = atoi(values);
        Serial.println(val);
        inputs[i] = val;
        i++;
        values = strtok(NULL, "|");
      
      }
      
      // check if a valid number of inputs was found
      if (i == 5) {
        
        displayValues();

        int Iteration = inputs[4];
        int j=0;
        
        while (j < Iteration )
        {
          Serial.print("INFO: iteration #");
          Serial.print(j);
          launchCapture(inputs[0],inputs[1],inputs[2],inputs[3]);
          j++;
          Serial.println("   COMPLETED");
        }
        
      }else{
        
        Serial.println("ERROR: number of values received incorrect");
        
      }
      
    }

   }
   
   Serial.println("INFO: end of loop()");
}
