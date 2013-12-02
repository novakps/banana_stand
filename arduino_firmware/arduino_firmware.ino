
/*
 * Circuit: Common-anode RGB LED wired like so:
 * Red cathode: digital pin 9
 * Green cathode: digital pin 5
 * blue cathode: digital pin 6
 * anode: +5V
 *
 */

#define NL 13
#include "rgb.h"

/*
 * defaults tp pulse between white and black with 1 second period
 */
RGB color = {
  102, 0, 102};

RGB backgroundColor = {
  0, 0, 0};

float pulseStep = 0.001;

// output pins for the LEDs:
const int redPin = 9;
const int greenPin = 5;
const int bluePin = 6;

const int POLARITY_CORRECTION = 255;

// commands
const char SET_COLOR = '#';
const char SET_BACKGROUND_COLOR = 'b';
const char SET_RATE= 'p';

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

float in = 4.712;
float out;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  inputString.reserve(8);
  Serial.println("ready");
  // make the pins outputs:
  pinMode(redPin, OUTPUT); 
  pinMode(greenPin, OUTPUT); 
  pinMode(bluePin, OUTPUT); 
}

void loop() {
  if (stringComplete) {
    Serial.println(inputString);
    parseCommand(inputString);
    inputString = "";
    stringComplete = false;
  }
  pulseLed();
}

void pulseLed() {
  in = in + pulseStep;
  if (in > 10.995)
    in = 4.712;
  out = sin(in) * 0.5 + 0.5;
  RGB currentColor = interpolateRgbs(color, backgroundColor, out);
  analogWrite(redPin, POLARITY_CORRECTION - currentColor.r);
  analogWrite(greenPin, POLARITY_CORRECTION - currentColor.g);
  analogWrite(bluePin, POLARITY_CORRECTION - currentColor.b);
}

RGB interpolateRgbs (RGB rgb, RGB rgb2, float ratio){
  int r = rgb.r;
  int g = rgb.g;
  int b = rgb.b;
  r += ((rgb2.r) - r) * ratio;
  g += ((rgb2.g) - g) * ratio;
  b += ((rgb2.b) - b) * ratio;
  return(RGB){
    r, g, b    };
}

void parseCommand(String inputString) {
  switch(inputString.charAt(0)) {
  case SET_COLOR:
    color = parseColors(inputString.substring(1));
    break;
  case SET_BACKGROUND_COLOR:
    backgroundColor = parseColors(inputString.substring(1));
    break;
  case SET_RATE:
    pulseStep = parseRate(inputString.substring(1));
    break;
  default:
    Serial.println("Invalid command");
  } 
}

RGB parseColors(String text) {
  int red = hexToDec(text.substring(0,2));
  int green = hexToDec(text.substring(2,4));
  int blue = hexToDec(text.substring(4,6));
  return (RGB){
    red, green, blue    };
}

float parseRate(String text) {
  //todo constrain to reasonable rates
  text.trim();
  Serial.println(text);
  Serial.println(text.length());
  char buffer[text.length()];
  text.toCharArray(buffer, text.length());
  return atof(buffer) / 1000.0;  
}

unsigned int hexToDec(String hexString) {
  unsigned int decValue = 0;
  int nextInt;
  for (int i = 0; i < hexString.length(); i++) {
    nextInt = int(hexString.charAt(i));
    if (nextInt >= 48 && nextInt <= 57) nextInt = map(nextInt, 48, 57, 0, 9);
    if (nextInt >= 65 && nextInt <= 70) nextInt = map(nextInt, 65, 70, 10, 15);
    if (nextInt >= 97 && nextInt <= 102) nextInt = map(nextInt, 97, 102, 10, 15);
    nextInt = constrain(nextInt, 0, 15);
    decValue = (decValue * 16) + nextInt;
  }
  return decValue;
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char ch = (char)Serial.read(); 
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (ch == '\n') {
      stringComplete = true;
    } 
    else {
      // add it to the inputString:
      inputString += ch;
    }
  }
}















