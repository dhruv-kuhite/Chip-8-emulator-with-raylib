#include "chip8.h"
#include <raylib.h>
#include <stdio.h>
#include <string.h>

// check /docs folder for information whatever i am making is writen in docs
// folder

void intializeChip8() {

  memset(&chip8, 0, sizeof(chip8));

  chip8.PC = 0x200; // we are doing because we can only use 4096-512 =3584 space
                    // to load in memory . 0 - 511 are reserved for
                    // interpreter and system use in orignal system
                    // 0x200 mean 512
}

int Check_RomSize_setMemory() {
  chip8_rom = fopen(Rom_Location, "rb");

  unsigned char buffer[chip8_memory_avail_space];
  memset(buffer, 0, chip8_memory_avail_space);

  while (1) {
    fread(&buffer[Rom_Size], 1, 1, chip8_rom);

    if (feof(chip8_rom) > 0) {
      break;
    }
    Rom_Size++;
  }

  if (Rom_Size > chip8_memory_avail_space) {
    printf(
        "Rom Size is greater than available memory :(\nGet proper roms....\n");
    return -1;
  }

  // Setting memory hereeeeeeeeeeeeeeeeeeeeeeeeeeee

  memcpy(&chip8.Memory[chip8.PC], buffer, Rom_Size);

  return 0;
}

void getOPCODE() {
  op_code = 0;
  op_code = chip8.Memory[chip8.PC];
  op_code = op_code << 8;

  chip8.PC++;

  op_code |= chip8.Memory[chip8.PC];

  chip8.PC++;
}

void ClearEveryThing() {

  for (int i = 0; i < display_height; i++) {
    memset(chip8.Display[i], 0, display_width);
  }
}

void WhereToDraw() {

  unsigned short DrawXreg = 0, DrawYreg = 0, Rows = 0;

  DrawXreg = op_code & 0b0000111100000000;
  DrawXreg = DrawXreg >> 8;

  DrawYreg = op_code & 0b0000000011110000;
  DrawYreg = DrawYreg >> 4;

  unsigned char DrawXvalue = chip8.Registers[DrawXreg],
                DrawYvalue = chip8.Registers[DrawYreg];

  Rows = op_code & 0b0000000000001111;

  for (int i = 0; i < Rows; i++) {

    unsigned char spriteData = chip8.Memory[chip8.Iregister];

    unsigned char window = 0b10000000;
    // here inner loop 8 times because one byte is 8 bits in chip 8
    // every sprite width is 8 like this example 0000000
    for (int j = 0; j < 8; j++) {

      // here 7-i because we need to shift one to index of binary 1
      chip8.Display[DrawYvalue + i][DrawXvalue + j] =
          (spriteData >> (7 - j) & 1);
    }
    chip8.Iregister++;
  }
  chip8.Iregister--;
}

void NowDecode_Execute() {

  switch (op_code) {
  case 0x00e0:
    ClearEveryThing();
    return;
    break;
  }

  char checkOpCode = op_code >> 12;
  int regIndex = 0;

  switch (checkOpCode) {

    // set Iregister
  case 0xa:
    chip8.Iregister = op_code & 0b0000111111111111;

    break;

    // load in registers
  case 0x6:
    regIndex = op_code & 0b0000111100000000;
    regIndex = regIndex >> 8;
    chip8.Registers[regIndex] = (op_code & 0b0000000011111111);
    break;

    // draw on screen
  case 0xd:

    WhereToDraw();

    break;

    // add to a particular register
  case 0x7:

    regIndex = op_code & 0b0000111100000000;
    regIndex = regIndex >> 8;
    chip8.Registers[regIndex] += (op_code & 0b0000000011111111);
    break;

    // jump instruction set pc

  case 0x1:
    chip8.PC = op_code & 0b0000111111111111;

    break;
  }
}

void DrawEveryThing() {

  for (int i = 0; i < display_height; i++) {

    for (int j = 0; j < display_width; j++) {

      if (chip8.Display[i][j] == 0) {
        DrawRectangle(j * 10, i * 10, block_width, block_height, WHITE);
      } else {
        DrawRectangle(j * 10, i * 10, block_width, block_height, PINK);
      }
    }
  }
}

int main() {
  SetTargetFPS(60);

  InitWindow(window_width, window_height, "CHIP8 emulator");

  intializeChip8();
  Check_RomSize_setMemory();

  int size = chip8.PC + Rom_Size;

  while (chip8.PC < size) {
    BeginDrawing();
    if (WindowShouldClose()) {
      break;
    }

    getOPCODE();
    NowDecode_Execute();
    DrawEveryThing();

    EndDrawing();
  }

  return 0;
}