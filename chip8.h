#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define display_height 32
#define  display_width 64

typedef struct {
  unsigned char Memory[4096];
  unsigned short PC;            // its the program counter
  unsigned char Registers[16]; // thats v0-vf
  unsigned short Iregister;
  unsigned short stack[16];
  unsigned char sp; // its stack position
  unsigned char DelayTimer;
  unsigned char SoundTimer;
  unsigned char Display[display_height][display_width]; // as it is 64*32
  unsigned char Keypad[16];
} Chip8;

unsigned int chip8_memory_avail_space =
    4096 - 512; // we are doing because we can only use 4096-512 =3584
                // space to load in memory . 0 - 511 are reserved for
                // interpreter and system use in orignal system
                // 0x200 is 512

Chip8 chip8;

FILE *chip8_rom;
char Rom_Location[200] = "Roms/2-ibm-logo.ch8";
unsigned short Rom_Size = 0;

// setting opcode is imp
unsigned short op_code;

// raylib windos  size

const int window_width = 640;
const int window_height = 320;

const int block_width = window_width / 64;
const int block_height = window_width / 32;
