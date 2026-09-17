#include <Arduino.h>
volatile uint32_t* direction_buffor = (volatile uint32_t*)0x3FF44020;
volatile uint32_t* voltage = (volatile uint32_t*)0x3FF44004;
void setup() {
    *direction_buffor = *direction_buffor | (1<<2); // maska bitowa z 1 na danym bicie oraz lub aby włączyć na sz bit oraz nie zerować innych bitów
}

void loop() {
    *voltage = *voltage | (1<<2);
    delay(500);
    *voltage = *voltage & ~(1<<2); //negacja naszej maski bitowaj aby wszędzie poza naszym bitem było 1 i wykonujemy and
    delay(500);
}