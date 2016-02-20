#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWaveformSine   sine4;          //xy=262,521
AudioSynthWaveformSine   sine1;          //xy=266,301
AudioSynthWaveformSine   sine2;          //xy=266,361
AudioSynthWaveformSine   sine3;          //xy=273,443
AudioOutputI2SQuad       i2s_quad1;      //xy=882,354
AudioConnection          patchCord1(sine4, 0, i2s_quad1, 3);
AudioConnection          patchCord2(sine1, 0, i2s_quad1, 0);
AudioConnection          patchCord3(sine2, 0, i2s_quad1, 1);
AudioConnection          patchCord4(sine3, 0, i2s_quad1, 2);
AudioControlSGTL5000     sgtl5000_1;     //xy=651,514
AudioControlSGTL5000     sgtl5000_2;     //xy=675,568
// GUItool: end automatically generated code


void setup() {
  // put your setup code here, to run once:
  AudioMemory(35);
  
  sgtl5000_1.setAddress(LOW);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);

  sgtl5000_2.setAddress(HIGH);
  sgtl5000_2.enable();
  sgtl5000_2.volume(0.5);

  sine1.amplitude(1);
  sine1.frequency(50);
  
  sine2.amplitude(1);
  sine2.frequency(80);

  sine3.amplitude(1);
  sine3.frequency(192);
  
  sine4.amplitude(1);
  sine4.frequency(385);



}

void loop() {
	while(1);
  // put your main code here, to run repeatedly:
  sine1.frequency(199);
  sine2.frequency(201);	
	delay(20);
  sine1.frequency(399);
  sine2.frequency(401);	
	delay(20);
  sine1.frequency(799);
  sine2.frequency(801);	
	delay(20);
}