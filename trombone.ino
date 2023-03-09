#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWaveform       waveform2;      //xy=71,339
AudioSynthWaveform       waveform1;      //xy=187,254
AudioSynthWaveformModulated waveformMod1;   //xy=268,351
AudioMixer4              mixer1;         //xy=435,285
AudioAmplifier           amp1;           //xy=612,292
AudioSynthWaveformDc     dc1;            //xy=614,381
AudioFilterStateVariable filter1;        //xy=765,300
AudioOutputI2S           i2s1;           //xy=945,306
AudioConnection          patchCord1(waveform2, 0, waveformMod1, 0);
AudioConnection          patchCord2(waveform1, 0, mixer1, 0);
AudioConnection          patchCord3(waveformMod1, 0, mixer1, 1);
AudioConnection          patchCord4(mixer1, amp1);
AudioConnection          patchCord5(amp1, 0, filter1, 0);
AudioConnection          patchCord6(dc1, 0, filter1, 1);
AudioConnection          patchCord7(filter1, 0, i2s1, 0);
AudioConnection          patchCord8(filter1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=100,63
// GUItool: end automatically generated code



void setup() {
  Serial.begin(9600);
  AudioMemory(8);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.75);
  pinMode(1, OUTPUT);
  pinMode(0, INPUT);
  delay(1000);
  
  mixer1.gain(0, 0.75);
  mixer1.gain(1, 0.25);
  
  waveform1.begin(1, 200, WAVEFORM_BANDLIMIT_SQUARE);
  waveform2.begin(1, 20, WAVEFORM_SINE);
  waveformMod1.begin(1, 200, WAVEFORM_BANDLIMIT_SQUARE);
  waveformMod1.frequencyModulation(0);

//  filter1.frequency(440);
//  filter1.octaveControl(3);

  amp1.gain(1.0);
}

void loop() {

  int knob = analogRead(A1);
  float vol = knob / 1023.0;
  sgtl5000_1.volume(vol);

  float joystickY = (analogRead(A0) - 512) / 512.0;
  float joystickX = analogRead(A2) / 1023.0;
  amp1.gain(abs(joystickY));
  waveform2.frequency(joystickX / 5);

  delay(10);
  // Clears the trigPin
  digitalWrite(1, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(1, HIGH);
  delayMicroseconds(10);
  digitalWrite(1, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  long duration = pulseIn(0, HIGH);
  // Calculating the distance
  float distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.println(distance);

  float minDist = 5;
  float maxDist = 50;

  float distRatio = (distance - minDist) / (maxDist - minDist);
  distRatio = min(distRatio, 1.0);
  distRatio = max(distRatio, 0.0);

  float minFreq = 300;
  float maxFreq = minFreq * pow(2, 2.5); //2.5 octaves

  float freq = distRatio * (maxFreq - minFreq) + minFreq;
  waveform1.frequency(freq);
  waveformMod1.frequency(freq);

}
