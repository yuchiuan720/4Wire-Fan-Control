#include <TFT.h>
#include <SPI.h>
#define fan_tho 10
#define fan_rpm 3
#define VR A9
#define cs A0
#define dc A1
#define rst A2
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
TFT TFTscreen = TFT(cs, dc, rst);
char vrPrintout[5];
char rpmPrintout[5];

volatile byte half_revolutions;
unsigned int rpm;
unsigned long old_time;
unsigned int height;//128
unsigned int width;//160
void rpm_fun()
{
  half_revolutions++;
}
void setup() {
  // put your setup code here, to run once:
  pinMode(fan_tho, OUTPUT);
  pinMode(fan_rpm, INPUT_PULLUP);
  pinMode(VR, INPUT);
  TFTscreen.begin();
  Serial.begin(9600);
  //while(!Serial);
  attachInterrupt(0, rpm_fun, RISING);
  half_revolutions = 0;
  rpm = 0;
  old_time = 0;

  TFTscreen.background(0, 0, 0);
  TFTscreen.setTextColor(GREEN, BLACK);
  TFTscreen.stroke(255, 255, 255);
  //TFTscreen.noStroke();
  TFTscreen.setTextSize(1);
  TFTscreen.text("ANALOG READ:", 0, 0);
  TFTscreen.text("RPM:", 0, 60);
  TFTscreen.setTextSize(5);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned int vr = analogRead(VR);
  String sensorVal = String(vr);
  sensorVal.toCharArray(vrPrintout, 5);
  analogWrite(fan_tho, vr/4);
  
  TFTscreen.stroke(255, 0, 0);
  TFTscreen.text(vrPrintout, 0, 20);
  if(half_revolutions >= 20)
  {
    rpm = 30*1000/(millis()-old_time)*half_revolutions;
    old_time = millis();
    half_revolutions = 0;
    Serial.println(rpm, DEC);
    sensorVal = String(rpm);
    sensorVal.toCharArray(rpmPrintout, 5);
    TFTscreen.text(rpmPrintout, 0, 80);
  }
  
  delay(250);
  TFTscreen.stroke(0, 0, 0);
  TFTscreen.text(vrPrintout, 0, 20);
  TFTscreen.text(rpmPrintout, 0, 80);
}
