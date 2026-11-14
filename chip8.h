#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  unsigned char Memory[4096];
  unsigned short PC;            // its the program counter
  unsigned short Registers[16]; // thats v0-vf
  unsigned short Iregister;
  unsigned short stack[16];
  unsigned char sp; // its stack position
  unsigned char DelayTimer;
  unsigned char SoundTimer;
  unsigned char Display[2048]; // as it is 64*32
  unsigned char Keypad[16];
} Chip8;

Chip8 chip8;