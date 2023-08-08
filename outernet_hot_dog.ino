#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 5
#define EXP_DECAY .5
#define EXP_DECAY_NEW .6
#define ANALOG_THRESH 500
const float DECAY_CONST = pow(1.06, -1.0 / 20.0);

// last one is the end of the row

struct LINE {
  int startn;
  int endn;
};

const LINE xmap[] = {
  { 1, 11 },
  { 22, 12 },
  { 23, 34 },
  { 35, 45 },
  { 46, 57 },

};
//  The overall fire brightness
//  (this can affect both color levels and power consumption)
int brightness = 255;

struct RGB {
  byte r;
  byte g;
  byte b;
};

//  The flame color array (the first is the default):
RGB flameColors[] = {
  { 226, 15, 30},    // Red flame
  { 226, 121, 35},  // Orange flame
  { 158, 8, 148},   // Purple flame
  { 74, 150, 12},   // Green flame
};

Adafruit_NeoPixel strip = Adafruit_NeoPixel(max( xmap[sizeof(xmap) / sizeof(LINE) - 1].startn, xmap[sizeof(xmap) / sizeof(LINE) - 1].endn), PIN, NEO_GRB + NEO_KHZ800);



void setup() {
    pinMode(5, OUTPUT);
  Serial.begin(9600);
  Serial.println("HI");
  Serial.println(max( xmap[sizeof(xmap) / sizeof(LINE) - 1].startn, xmap[sizeof(xmap) / sizeof(LINE) - 1].endn));

  strip.begin();
  strip.setBrightness(brightness);
  strip.show(); // Initialize all pixels to 'off'

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(13, OUTPUT);
  pinMode(3, OUTPUT);
  // initialize the pushbutton pin as an input:
  // for (int i = A0; i <= A7; i++) {
  // pinMode(i, INPUT);
  // }
}

long buttons[18];

int taser = 0;

void loop() {



  if (analogRead(A0) > ANALOG_THRESH || analogRead(A4) > ANALOG_THRESH) taser = 25;
  taser--;
  if (taser > 0) {
    digitalWrite(13, 1);
  } else digitalWrite(13, 0);


  if (analogRead(A1) > ANALOG_THRESH) buttons[0] = millis() - 1;
  if (analogRead(A2) > ANALOG_THRESH) buttons[1] = millis() - 1;
  if (analogRead(A3) > ANALOG_THRESH) buttons[2] = millis() - 1;


  Serial.print(analogRead(A0)); Serial.print(" ");
  Serial.print(analogRead(A1)); Serial.print(" ");
  Serial.print(analogRead(A2)); Serial.print(" ");
  Serial.print(analogRead(A3)); Serial.print(" ");
  Serial.print(analogRead(A4)); Serial.println(" ");





 // setLineColor(0, pow(DECAY_CONST, millis() - buttons[0]) );
//  setLineColor(1, pow(DECAY_CONST, millis() - buttons[0]) );
 // setLineColor(2, pow(DECAY_CONST, millis() - buttons[1]) );
//  setLineColor(3, pow(DECAY_CONST, millis() - buttons[2]) );
 // setLineColor(4, pow(DECAY_CONST, millis() - buttons[2]) );


  long maxx = 0;
  int maxxi = -1;
  for (int i = 0; i <= 2; i++) {
    if (buttons[i] > maxx){ 
      maxx = buttons[i];
    maxxi = i;
    }
    Serial.println(buttons[i]);
  }
 // maxxi = 1;
 Serial.println(maxx);
  Serial.println(maxxi);
  if (maxx + 2000 > millis()) {

    if (maxxi == 1) digitalWrite(3,1);
    
    else if (maxxi == 2) digitalWrite(3,1);
    else digitalWrite(3, 0);
  } else digitalWrite(3,0);


  strip.show();


  delay(20);
}

void setLineColor(int x, float intst) {

  int numinline = abs(xmap[x].endn - xmap[x].startn);

  int intensity = min(intst * numinline, numinline);


  //  Flicker, based on our initial RGB values
  for (int i = 0; i <= intensity; i++) {
    RGB color = flameColors[i * 4 / numinline];
    int flicker = random(0, 30);
    int r1 = color.r - flicker;
    int g1 = color.g - flicker;
    int b1 = color.b - flicker;
    if (g1 < 0) g1 = 0;
    if (r1 < 0) r1 = 0;
    if (b1 < 0) b1 = 0;
    setThingColor(x, i, r1, g1, b1);
  }
  for (int i = intensity + 1; i <= numinline; i++) {
    setThingColor(x, i, 0, 0, 0);

  }
}

void setThingColor(int x, int y, int r1, int g1, int b1) {

  if (xmap[x].startn > xmap[x].endn) {
    if (xmap[x].endn + y > xmap[x].startn) return;
    strip.setPixelColor(xmap[x].endn + y, r1, g1, b1);
  } else {
    if (xmap[x].startn + y > xmap[x].endn) return;
    //    Serial.print(xmap[x].startn);
    //   Serial.print(" ");
    //  Serial.print(y);
    // Serial.print(" ");
    //Serial.print(r1);
    //Serial.print(" "); Serial.print(g1); Serial.print(" "); Serial.println(b1);
    strip.setPixelColor(xmap[x].startn + y, r1, g1, b1);
  }
}
