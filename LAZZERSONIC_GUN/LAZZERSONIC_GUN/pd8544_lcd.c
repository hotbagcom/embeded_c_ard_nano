/*
 * pd8544_lcd.c
 *
 * Created: 18.09.2023 12:00:28
 *  Author: tnc
 */ 
// Vcc   ---------- +3.3V			|--------------
// SCK   -- 330 -- EXT OSC			|   right side
// SDin  -- 330 -- Data in			|
// D/!C  -- 330 -- Data Command		|   Front 
//!(S)CE -- 330 -- Enable			|   Side
// GND   --- Gnd					|
// Vout  ------|1uF(-------|GND		|   left side
//!RST   -- 330 -- Reset			|--------------

#include "pd8544_lcd.h"





