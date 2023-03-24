#pragma once
/*
	defs.h - just some commmon defs to remove previous magic numbers
*/
enum Keys : unsigned char 
{ 
	KEYS_BS = 0x08, 
	KEYS_RET = 0x0D, 
	KEYS_ESC = 0x1B 
};

enum Colour : WORD{
	BLACK,
	DARKBLUE,
	DARKGREEN,
	DARKRED = 4,
	DARKPURPLE,
	LIGHTGREY = 7,
	GREY,
	BLUE,
	GREEN,
	LIGHTBLUE,
	RED,
	PINK,
	YELLOW,
	WHITE,
	COLOUR_MAX
};

enum Chars: CHAR{
	CHAR_NULL,
	CHAR_RIGHTARROW = (CHAR)62,
	CHAR_ALTBLOCK = (CHAR)176,
	CHAR_BLOCK = (CHAR)219,
	CHAR_HYPHEN = (CHAR)240
};
