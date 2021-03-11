/*
 * RandomMelody.cpp
 *
 * Plays a melody from FLASH.
 *
 * More RTTTL songs can be found under http://www.picaxe.com/RTTTL-Ringtones-for-Tune-Command/
 *
 *  Copyright (C) 2019  Armin Joachimsmeyer
 *  armin.joachimsmeyer@gmail.com
 *
 *  This file is part of PlayRttl https://github.com/ArminJo/PlayRtttl.
 *
 *  PlayRttl is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/gpl.html>.
 *
 */

#include <Arduino.h>

#include <PlayRtttl.h>

#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny87__) || defined(__AVR_ATtiny167__)
#include "ATtinySerialOut.h"
#undef LED_BUILTIN
#define LED_BUILTIN PB1  // on my digispark board
/*
 * Only tone at pin1 and 4 are generated by hardware.
 * If generated by software, the millis() interrupt disturbs the tone generation on an 1 MHz ATtiny.
 */
const int TONE_PIN = 1; // use pin 1 (also used for internal LED :-( ) since pin 4 is connected to USB
#else
const int TONE_PIN = 11;
#endif

char StarWarsInRam[] =
        "StarWars:d=32,o=5,b=45,l=2,s=N:p,f#,f#,f#,8b.,8f#.6,e6,d#6,c#6,8b.6,16f#.6,e6,d#6,c#6,8b.6,16f#.6,e6,d#6,e6,8c#6";

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
#if defined(__AVR_ATmega32U4__) || defined(SERIAL_USB) || defined(SERIAL_PORT_USBVIRTUAL)  || defined(ARDUINO_attiny3217)
    delay(4000); // To be able to connect Serial monitor after reset or power up and before first printout
#endif
#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny87__) || defined(__AVR_ATtiny167__)
    MCUSR = 0;
#endif
    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_PLAY_RTTTL));

    /*
     * Play one melody
     */
    playRtttlBlocking(TONE_PIN, StarWarsInRam);
    delay(5000);
}

void loop() {
    /*
     * And all the other melodies, but use now the non blocking functions
     */
//    for (uint8_t i = 1; i < ARRAY_SIZE_MELODIES_SMALL; ++i) {
        for (uint8_t i = 1; i < ARRAY_SIZE_MELODIES_TINY; ++i) {
        const char* tSongPtr;
#if defined(__AVR__)
//        tSongPtr = (char*) pgm_read_word(&RTTTLMelodiesSmall[i]);
        tSongPtr = (char*) pgm_read_word(&RTTTLMelodiesTiny[i]);
#else
        tSongPtr = (char*) RTTTLMelodiesSmall[i];
#endif
        Serial.println(F("Play next melody"));
        startPlayRtttlPGM(TONE_PIN, tSongPtr);
        while (updatePlayRtttl()) {
            /*
             * your own code here...
             */
            delay(1);
        }
        delay(2000);
    }
    delay(20000);
}

