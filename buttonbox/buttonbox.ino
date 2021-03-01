
struct input
{
  int state;
  int previousState;
};
int RXLED = 17;
const int numInputs = 18;
input inputStates[numInputs];

void setup() {
  // put your setup code here, to run once:
  
  // TX LED is set as an output behind the scenes

  Serial.begin(9600); //This pipes to the serial monitor
  Serial.println("Initialize Serial Monitor");

  // row 1, 2, 3
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);

  for(int i = 3; i<21; i=i+1)
    pinMode(i, INPUT);
  for(int i = 0; i<18; i=i+1)
    inputStates[i] = {0,0};
    
  digitalWrite(1, LOW);
  digitalWrite(1, HIGH);
  digitalWrite(2, LOW);
  //pinMode(RXLED, OUTPUT);  // Set RX LED as an output
}

void loop() 
{
  static long previousStatus = 0;
  long inputStatus = 0;
  for(int c = 0; c<4 ; c=c+1)
  {
    inputStates[c].state = digitalRead(c+3);
    inputStatus = inputStatus + (inputStates[c].state << c);
  }

  if(previousStatus != inputStatus) Serial.println(inputStatus, HEX);
 //   digitalWrite(RXLED, HIGH);
  //else
   // digitalWrite(RXLED, LOW);
    //Serial.println(inputStatus, HEX);

  for(int c = 0; c<numInputs ; c=c+1)
    inputStates[c].previousState = inputStates[c].state;
  previousStatus = inputStatus;
}
