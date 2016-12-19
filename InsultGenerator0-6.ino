//Insult Generator
//A terrible project of Jon Hanford 
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif


// Configure the pin connected OLED reset/RST.
// this isn't used, or present, but the thing borks if not defined. 
#define OLED_RESET   4

// Configure orientation of the display.
// 0 = none, 1 = 90 degrees clockwise, 2 = 180 degrees, 3 = 270 degrees CW
#define ROTATION     0

// define a buotton
#define goButton  10
#define intensityKnob A0

// Data pin for RGB lamp
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      1

// default texts for display
#define splash "Press for Pain"
#define error "Something went wrong"



// Create OLED and RGB global.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_SSD1306 display(OLED_RESET);

// define insults
// these are getting loaded into ROM because this is a lot
const char insult_1[] PROGMEM = "You Are Not Smart";
const char insult_2[] PROGMEM = "You Don't Smell Good";
const char insult_3[] PROGMEM = "People Don't Really Like You";
const char insult_4[] PROGMEM = "Your professional work is mediocre at best";
const char insult_5[] PROGMEM = "Your cooking is bland";
const char insult_6[] PROGMEM = "Your mother is so fat her doctor reccomended she limit her caloric intake";
const char insult_7[] PROGMEM = "You aren't bright enough to properly interpret this";
const char insult_8[] PROGMEM = "I wish I weren't being held by you";
const char insult_9[] PROGMEM = "Ugh.";
const char insult_10[] PROGMEM = "Please just stop.";
const char insult_11[] PROGMEM = "You aren't bright enough to properly interpret this.";
const char insult_12[] PROGMEM = "You are impressively dumb";
const char insult_13[] PROGMEM = "Barf factor five.";
const char insult_14[] PROGMEM = "Wow. You turned up the intensity on the machine that insults you. You're a fucking genius.";
const char insult_15[] PROGMEM = "You know, I think they have medication for that now.";
const char insult_16[] PROGMEM = "Perhaps it's best if you came with a warning label.";
const char insult_17[] PROGMEM = "You are a compelling argument against darwin's work.";
const char insult_18[] PROGMEM = "Get bent.";
const char insult_19[] PROGMEM = "lvl 2 insult 19. It's not a mistake. I just was too lazy to bother.";
const char insult_20[] PROGMEM = "Isn't there a one-way space mission you should be on?";
const char insult_21[] PROGMEM = "Facepalm. You again?";
const char insult_22[] PROGMEM = "Holy shit, what's that smell?";
const char insult_23[] PROGMEM = "I'm a robot and you're a waste of MY time.";
const char insult_24[] PROGMEM = "If that genie granted four wishes you'd be dead.";
const char insult_25[] PROGMEM = "For Real? THAT's your face?";
const char insult_26[] PROGMEM = "You're not totally worthless, you have value as a cautionary tale";
const char insult_27[] PROGMEM = "If you don't have anything nice to say - whatever, you suck.";
const char insult_28[] PROGMEM = "Cat Pee Smells Like You";
const char insult_29[] PROGMEM = "You're an emotional cripple, your soul is made of dog shit, and everything about you is ugly. ";
const char insult_30[] PROGMEM = "I can't beleive we're still having this conversation";
const char insult_31[] PROGMEM = "Storytellers will sing songs about your astounding half-wittedness for eons to come.";
const char insult_32[] PROGMEM = "Seriously why the fuck do you keep pressing this button?";
const char insult_33[] PROGMEM = "Fuck you, jackass.";
const char insult_34[] PROGMEM = "You're so creepy a search warrant will be issued for your crawl space.";
const char insult_35[] PROGMEM = "You're pheremones smell like taco farts.";
const char insult_36[] PROGMEM = "You're the patient zero of suck.";
const char insult_37[] PROGMEM = "I hope cancer cures you.";
const char insult_38[] PROGMEM = "Other people's parents blame you for their divorce.";
const char insult_39[] PROGMEM = "What's black and white and red all over? I don't know,  I'm distracted by my undying hatred of you.";
const char insult_40[] PROGMEM = "I hope you die on my birthday";


// define default variables for algorithm
int goButtonState = 1;
int lastGoButtonState = 1;
bool needToPaint = false;
int insultIndex = 0;
int indexMin = 1;
int indexMax = 5;
int rawIntensity = 0;
int niceIntensity = 1;
String insultText = error;
int redVal = 0;
int greenVal = 0; 
int blueVal = 0;
int insultTextLength = 160;
const char* const insult_table[] PROGMEM = {insult_1,  insult_2,  insult_3,  insult_4, insult_5, insult_6, insult_7, insult_8, insult_9, insult_10, insult_11, insult_12, insult_13, insult_14, insult_15, insult_16, insult_17, insult_18, insult_19, insult_20, insult_21, insult_22, insult_23, insult_24, insult_25, insult_26, insult_27, insult_28, insult_29, insult_30, insult_31, insult_32, insult_33, insult_34, insult_35, insult_36, insult_37, insult_38, insult_39, insult_40};
char buffer[168];

void setup()   {
  pixels.begin(); // This initializes the NeoPixel library.
  // Set Insult delivery button as defined above with 10k pullup
  pinMode(goButton, INPUT_PULLUP);
  // Setup the OLED display with internal high voltage generation
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // Clear the display.
  display.clearDisplay();
  display.display();
  
  // Set rotation.
  display.setRotation(ROTATION);

  // Set text size and color and initialize
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.println(splash);
  display.display();
}
void displayLoop(){
  // check if the display needs to be updated and run the loop if so
  if ( needToPaint == true ){
    paintInsults();
  }
}
void lightLoop(){
  // this really should have been "case" but eh. who cares
  // check the intensity level, set RGB value appropriate
  if (niceIntensity == 0){
    redVal = 0;
    greenVal = 0;
    blueVal = 0;
  }
    if (niceIntensity == 1){
    redVal = 0;
    greenVal = 64;
    blueVal = 0;
  }
    if (niceIntensity == 2){
    redVal = 28;
    greenVal = 64;
    blueVal = 0;
  }
  if (niceIntensity == 3){
    redVal = 64;
    greenVal = 28;
    blueVal = 0;
  }
    if (niceIntensity == 4){
    redVal = 64;
    greenVal = 0;
    blueVal = 0;
  }
  // lol left the FOR loop from the adafruit code even tho we usin one
    for(int i=0;i<NUMPIXELS;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(greenVal,redVal,blueVal)); // takes the RGB (actually GRB) color from above

    pixels.show(); // This sends the updated pixel color to the hardware.
}
}

void loop() {
   goButtonState = digitalRead(goButton);
   // read knob and derive 1-4 intensity factor 
   rawIntensity = analogRead(intensityKnob);
   if (rawIntensity >= 767){
    niceIntensity = 4;
   }
   else if (rawIntensity >= 511){
    niceIntensity = 3;
   }
   else if (rawIntensity >= 256){
    niceIntensity = 2;
   } else {
    niceIntensity = 1;
   }
   // if the state of the button is differnet than last go
   if ( goButtonState != lastGoButtonState ){
    // and the state of the button is pressed
    if ( goButtonState == LOW ){
      //pick numbers from the range approps for the intensity level
      if (niceIntensity == 4){
        indexMin = 30;
        indexMax = 40;
      }
      else if (niceIntensity == 3){
        indexMin = 20;
        indexMax = 30;
      }
      else if (niceIntensity == 2){
        indexMin = 10;
        indexMax = 20;
      }
            
      else if (niceIntensity == 1){
        indexMin = 1;
        indexMax = 10;
      }
      else {
        indexMin = 1;
        indexMax = 40;
      }
      // note we need to write the display
      needToPaint = true;
      // generate a random number within the range
      insultIndex = random(indexMin, indexMax);

      
    }
    // remember the button state for next go
    lastGoButtonState = goButtonState;
   }
   //make nice the light
   lightLoop();
   //check if display needs doing and do if it does, what?
   displayLoop();
}

void paintInsults(){
  // we check one more time to make sure the screen needs repaint. just in case. enter the loop if so.
  if (needToPaint == true){
    // okay so, this does some stuf I don't fully understand with regard to casting
    // what it DOES do is copy to the BUFFER the string from the array of strings defined in progmem.
    // insultIndex here is a int variable, so define that as anything 1-40 and you get the string above
    // this is way easier than explicity defineing case statements for each dingus insult.  
  strcpy_P(buffer, (char*)pgm_read_word(&(insult_table[insultIndex])));
    // copy the buffer to an insultText subvariable to determine length. buffer has set length. 
  insultText = buffer;
    // read that length
  insultTextLength = insultText.length();
    // bigger text if under 40 chars
  if (insultTextLength >= 40){
    display.setTextSize(1);
  } else {
    display.setTextSize(2);
  }
  display.clearDisplay();
  display.setCursor(0,0);
  display.println(buffer);
  //display.println(niceIntensity); //uncomment to debug intensity level
  //shit son word wrap would have been nice but that's haaaaaard. 
  display.display();
  needToPaint = false;
  }
}

