
/*
 *Circuit: Common-anode RGB LED wired like so:
 * Red cathode: digital pin 3
 * Green cathode: digital pin 5
 * blue cathode: digital pin 6
 * anode: +5V
 */
 
 #define NL 13
 
 // pins for the LEDs:
const int redPin = 9;
const int greenPin = 5;
const int bluePin = 6;

const char SET_COLOR = '#';

char cmd;
char buffer[7];
char color_value[3];
int characters_received;
int red;
int green;
int blue;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  Serial.println("usage: ");
  // make the pins outputs:
  pinMode(redPin, OUTPUT); 
  pinMode(greenPin, OUTPUT); 
  pinMode(bluePin, OUTPUT); 
}

void loop() {
 if (Serial.available()) {
   char ch = char(Serial.read());
   Serial.print("command: ");
   Serial.println(ch);
   
   switch (ch) {
     case SET_COLOR:
       Serial.print("set color: ");
         characters_received = Serial.readBytesUntil(NL, buffer, 7);
         if(characters_received==7) {
           Serial.println(buffer);
           // todo extract method
           color_value[0] = buffer[0];
           color_value[1] = buffer[1];
           color_value[2] = 0; //null-terminate
           red = atoi(color_value);
           Serial.print("red: ");
           Serial.println(red);
           Serial.print("green: ");
           Serial.println(green);
           Serial.print("blue: ");
           Serial.println(blue); 
         } else {
          Serial.print("Invalid color value:"); 
          Serial.println(color_value);
         }
      
       break;
     default:
       Serial.println("invalid command");
   }
 } 
} 

  
  
  
 
