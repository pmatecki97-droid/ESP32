#include <Arduino.h>
volatile uint32_t* output_voltage = (volatile uint32_t*)0x3FF44004;
volatile uint32_t* gpio2_output_enable = (volatile uint32_t*)0x3FF44020;
volatile uint32_t* io_gpio2_mode = (volatile uint32_t*)0x3FF49040;

volatile uint32_t* gpio5_in = (volatile uint32_t*)0x3FF4403C;
volatile uint32_t* io_gpio5_mode = (volatile uint32_t*)0x3FF4906C; //WYJAŚNIA TO STRONA 149 MANUALA (OPISUJE KTÓRY PIN W REJESTRZE MUX ZA CO ODPOWIADA)

uint8_t stable_state = 0; //JEST TO STAN GDY PRZYCISK NIE JEST WCISNIĘTY I V = 3,3V
uint8_t last_state = 0; //POPRZEDNI STAN
unsigned long last_change_time = 0; //CZAS OD OSTATNIEGO WCISNIĘCIA BLASZKI
const unsigned long debounce_delay = 50; //mils

uint8_t is_pressed() {
    if (*gpio5_in & (1<<5)) {
        return 0 ; //NIE JEST WCIŚNIĘTY
    }
    else {
        return 1; //JEST WCIŚNIĘTY
    }
}
void setup() {
    /* KONFIGRUACJA PIN 2 DIODA*/
    *io_gpio2_mode |= (2<<12);
    *gpio2_output_enable |= (1<<2);
    /* KONFIGURACJA PINU 5 ODCZYT*/
    *io_gpio5_mode |= (2<<12) | (1<<8) | (1<<9); // NAJPIERW USTAWIAMY MU TRYB NA FUNCTION 2 (GPIO), POTEM ZAŁĄCZAMY PULLUP RESISTOR, A NA KONIEC UMOŻLIWIAMY INPUT !!
}

void loop() {
    //SPRAWDZENIE CZY PRZYCISK JEST KLIKNIĘTY
    uint8_t current_state = is_pressed();
    //RESETUJEMY TUMER PO ZMIANIE STANU
    if (current_state!=last_state) {
        last_change_time = millis();
    }
    //SPRAWDZAM,Y CZY JEST STAN USTALONY (STABILNY)
    if ((millis()-last_change_time)>debounce_delay) {
        if (current_state != stable_state) { //STAN
            stable_state = current_state; //STAN AKTUALNY STAJE SIĘ STANEM STABILNYM
        }
        if (stable_state == 1) { //JEŚLI STAN STABILNY JEST RÓWNY 1 CZYLI PRZYTCISK JEST WCISNIĘTY
            *output_voltage |= (1<<2); //ZAPAL
        }
        else { //JEŚLI STAN STABILNY JEST RÓWNY 1 CZYLI PRZYTCISK NIE JEST WCISNIĘTY
            *output_voltage &= ~(1<<2); //ZGAŚ
        }
    }
    last_state = current_state; //PRZYPISZ STAN PRZED NASTEPNYM SPRAWDZENIEM
}