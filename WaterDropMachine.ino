
/* Water Drop Machine V1
 *
 * Sketch that control the waterDropMachine created to perform water drop photography.
 * The system is based on arduino and a solenoid valve that drops water at a regular time interval.
 * The flash and the camero trigger are automatically triggered at predefined interval.
 * By Antranik Zekian, 2021 - part of the code freely inspired from the code "Water Drop Controller V4" by Gareth Bellamy
 * Refactored by Pierre-Arnaud Galiana - 2023
 */

// Constants
const int CAMERA_BUTTON_HOLD_TIME = 100;

// Arduino pins
const int camPIN = 3; // pin for camera
const int solPIN = 8; // pin for soleniod valve

struct captureData
{
  int drop1;
  int dropDelay;
  int drop2;
  int camDelay;
};

struct iterationsData
{
  int numberOfIterations;
  int delayBetweenIterations;
};

int readInt(String message = "")
{
  Serial.println(message);
  while (Serial.available() == 0)
  {
  }
  return Serial.parseInt();
}

captureData readCaptureData()
{
  Serial.println("Enter the capture values in milliseconds:");
  int drop1 = readInt("- drop1:");
  int dropDelay = readInt("- dropDelay:");
  int drop2 = readInt("- drop2:");
  int camDelay = readInt("- camDelay:");
  return {
    drop1 : drop1,
    dropDelay : dropDelay,
    drop2 : drop2,
    camDelay : camDelay
  };
}

iterationsData readIterationsData()
{
  Serial.println("Enter the iterations values in milliseconds:");
  int numberOfIterations = readInt("- numberOfIterations:");
  int delayBetweenIterations = readInt("- delayBetweenIterations:");
  return {
    numberOfIterations : numberOfIterations,
    delayBetweenIterations : delayBetweenIterations
  };
}

void displayCaptureData(captureData captureData)
{
  Serial.println("Capture values: ");
  Serial.print("- drop1           : ");
  Serial.println(captureData.drop1);
  Serial.print("- dropDelay       : ");
  Serial.println(captureData.dropDelay);
  Serial.print("- drop2           : ");
  Serial.println(captureData.drop2);
  Serial.print("- camDelay        : ");
  Serial.println(captureData.camDelay);
}

void displayIterationsData(iterationsData iterationsData)
{
  Serial.println("Iterations values: ");
  Serial.print("- iterations      : ");
  Serial.println(iterationsData.numberOfIterations);
  Serial.print("- iterationsDelay : ");
  Serial.println(iterationsData.delayBetweenIterations);
}

void printIteration(int iteration, int numberOfIterations)
{
  Serial.print("INFO: iteration #");
  Serial.print(iteration);
  Serial.print("/");
  Serial.println(numberOfIterations);
}

void printIterationDelay(int delayBetweenIterations)
{
  Serial.print("INFO: waiting for ");
  Serial.print(delayBetweenIterations);
  Serial.println("ms before next iteration");
}

void launchCapture(captureData captureData)
{
  digitalWrite(solPIN, HIGH); // Opens solenoid valve
  delay(captureData.drop1);   // Valve stays open for time value drop1 -> define 1st drop size
  digitalWrite(solPIN, LOW);  // Closes solenoid valve

  delay(captureData.dropDelay); // Time delay between drops

  digitalWrite(solPIN, HIGH); // Opens solenoid valve
  delay(captureData.drop2);   // Valve stays open for time value drop2 - 2nd drop size
  digitalWrite(solPIN, LOW);  // Closes solenoid valve

  delay(captureData.camDelay); // Time delay for camera activation

  digitalWrite(camPIN, HIGH);     // Sets camPIN to high - fires camera
  delay(CAMERA_BUTTON_HOLD_TIME); // Holds camPIN high for set delay to ensure signal
  digitalWrite(camPIN, LOW);      // Sets camPIN to low
}

void setup()
{
  Serial.begin(9600);      // Start the Serial Monitor
  pinMode(camPIN, OUTPUT); // Set camPIN to an output
  pinMode(solPIN, OUTPUT); // Set solPIN to an output
}

void loop()
{
  captureData captureData = readCaptureData();
  iterationsData iterationsData = readIterationsData();

  displayCaptureData(captureData);
  displayIterationsData(iterationsData);

  int iteration = 1;
  while (iteration <= iterationsData.numberOfIterations)
  {
    printIteration(iteration, iterationsData.numberOfIterations);
    launchCapture(captureData);
    if (iteration < iterationsData.numberOfIterations)
    {
      printIterationDelay(iterationsData.delayBetweenIterations);
      delay(iterationsData.delayBetweenIterations);
    }
    else
    {
      Serial.println("INFO: last iteration completed");
    }
    iteration++;
  }

  Serial.println("INFO: end of loop()");
}
