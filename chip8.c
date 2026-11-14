#include "chip8.h"

void intializeChip8() {

  memset(&chip8, 0, sizeof(chip8));

  chip8.PC = 512;
}

int main() {
    
    return 0; 

}