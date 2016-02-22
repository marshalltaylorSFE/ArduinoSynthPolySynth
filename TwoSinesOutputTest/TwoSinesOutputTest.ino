#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWaveformSine   sine3;          //xy=176.88888549804688,208
AudioSynthWaveformSine   sine1;          //xy=177.88888549804688,112
AudioSynthWaveformSine   sine2;          //xy=177.88888549804688,160
AudioSynthWaveformSine   sine4;          //xy=177.88888549804688,258
AudioOutputI2SQuad       i2s_quad1;      //xy=454.888916015625,147
AudioConnection          patchCord1(sine3, 0, i2s_quad1, 2);
AudioConnection          patchCord2(sine1, 0, i2s_quad1, 0);
AudioConnection          patchCord3(sine2, 0, i2s_quad1, 1);
AudioConnection          patchCord4(sine4, 0, i2s_quad1, 3);
AudioControlSGTL5000     sgtl5000_1;     //xy=429.888916015625,226
AudioControlSGTL5000     sgtl5000_2;     //xy=429.888916015625,271
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

void loop()
{
	sine1.frequency(220);
	sine2.frequency(311.127);
	sine3.frequency(220);
	sine4.frequency(440);	
	delay(1000);
	sine1.frequency(311.127);
	sine2.frequency(220);
	sine3.frequency(440);
	sine4.frequency(220);	
	delay(1000);
//  // put your main code here, to run repeatedly:
//  sine1.frequency(199);
//  sine2.frequency(201);	
//	delay(20);
//  sine1.frequency(399);
//  sine2.frequency(401);	
//	delay(20);
//  sine1.frequency(799);
//  sine2.frequency(801);	
//	delay(20);
}