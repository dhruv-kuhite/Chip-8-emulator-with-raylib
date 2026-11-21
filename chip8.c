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
  // for (int i=0; i<Rom_Size; i++) {

  //   printf("%d : %d\n",i,buffer[i]);

  // }

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

void DrawwOnScreen() {

 char DrawX = 0, DrawY = 0;


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

  case 0xa:
    chip8.Iregister = op_code & 0b0000111111111111;
    break;

  case 0x6:
    regIndex = op_code & 0b0000111111111111;
    regIndex = regIndex >> 8;
    chip8.Registers[0] = (char)(op_code & 0b0000000011111111);
    break;

  case 0xd:

    DrawwOnScreen();

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
    WaitTime(1);

    EndDrawing();
  }

  return 0;
}