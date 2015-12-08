#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWaveformSine   sine1;          //xy=383,308
AudioSynthWaveformSine   sine2;          //xy=383,368
AudioFilterStateVariable filter1;        //xy=619,308
AudioFilterStateVariable filter2;        //xy=619,378
AudioFilterStateVariable filter3;        //xy=819,308
AudioFilterStateVariable filter4;        //xy=820,390
AudioOutputI2S           i2s1;           //xy=996,345
AudioConnection          patchCord1(sine1, 0, filter1, 0);
AudioConnection          patchCord2(sine2, 0, filter2, 0);
AudioConnection          patchCord3(filter1, 0, filter3, 0);
AudioConnection          patchCord4(filter2, 0, filter4, 0);
AudioConnection          patchCord5(filter3, 0, i2s1, 0);
AudioConnection          patchCord6(filter4, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=452,593
// GUItool: end automatically generated code




void setup() {
  // put your setup code here, to run once:
  AudioMemory(18);
  
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  sgtl5000_1.unmuteHeadphone();
  sine1.amplitude(1);
  sine1.frequency(199);
  
  sine2.amplitude(1);
  sine2.frequency(201);

  filter1.frequency(600);  
  filter2.frequency(600);  
  filter3.frequency(600);  
  filter4.frequency(600);  

}

void loop() {
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