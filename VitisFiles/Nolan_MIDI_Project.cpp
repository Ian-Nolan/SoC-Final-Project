/*****************************************************************//**
 * @file main_sampler_test.cpp
 *
 * @brief Basic test of nexys4 ddr mmio cores
 *
 * @author p chu
 * @version v1.0: initial release
 *********************************************************************/

// #define _DEBUG
#include "chu_init.h"
#include "gpio_cores.h"
#include "sseg_core.h"

/**
 * check individual led
 * @param led_p pointer to led instance
 * @param n number of led
 */
void led_check(GpoCore *led_p, int n) {
   int i;

   for (i = 0; i < n; i++) {
      led_p->write(1, i);
      sleep_ms(100);
      led_p->write(0, i);
      sleep_ms(100);
   }
}

/**
 * leds flash according to switch positions.
 * @param led_p pointer to led instance
 * @param sw_p pointer to switch instance
 */
void sw_check(GpoCore *led_p, GpiCore *sw_p) {
   int i, s;

   s = sw_p->read();
   for (i = 0; i < 30; i++) {
      led_p->write(s);
      sleep_ms(50);
      led_p->write(0);
      sleep_ms(50);
   }
}

/**
 * uart transmits test line.
 * @note uart instance is declared as global variable in chu_io_basic.h
 */
void uart_check() {
   static int loop = 0;

   uart.disp("test #");
   uart.disp(loop);
   uart.disp("\n\r");
   loop++;
}

/**
 * core test
 * @param led_p pointer to led instance
 * @param sw_p pointer to switch instance
 */
void show_test_id(int n, GpoCore *led_p) {
   int i, ptn;

   ptn = n; //1 << n;
   for (i = 0; i < 20; i++) {
      led_p->write(ptn);
      sleep_ms(30);
      led_p->write(0);
      sleep_ms(30);
   }
}

/**
 * Test pattern in 7-segment LEDs
 * @param sseg_p pointer to 7-seg LED instance
 */

void sseg_check(SsegCore *sseg_p) {
   int i, n;
   uint8_t dp;

   //turn off led
   for (i = 0; i < 8; i++) {
      sseg_p->write_1ptn(0xff, i);
   }
   //turn off all decimal points
   sseg_p->set_dp(0x00);

   // display 0x0 to 0xf in 4 epochs
   // upper 4  digits mirror the lower 4
   for (n = 0; n < 4; n++) {
      for (i = 0; i < 4; i++) {
         sseg_p->write_1ptn(sseg_p->h2s(i + n * 4), 3 - i);
         sseg_p->write_1ptn(sseg_p->h2s(i + n * 4), 7 - i);
         sleep_ms(300);
      } // for i
   }  // for n
      // shift a decimal point 4 times
   for (i = 0; i < 4; i++) {
      bit_set(dp, 3 - i);
      sseg_p->set_dp(1 << (3 - i));
      sleep_ms(300);
   }
   //turn off led
   for (i = 0; i < 8; i++) {
      sseg_p->write_1ptn(0xff, i);
   }
   //turn off all decimal points
   sseg_p->set_dp(0x00);

}

/**
 * Test pattern in 7-segment LEDs
 * @param sseg_p pointer to 7-seg LED instance
 */

void sseg_test(SsegCore *sseg_p) {
   int i, n;
   uint8_t dp;

   //turn off led
   for (i = 0; i < 8; i++) {
      sseg_p->write_1ptn(0xff, i);
   }
   //turn off all decimal points
   sseg_p->set_dp(0x00);

   for (n = 0; n < 2; n++) {
      for (i = 0; i < 8; i++) {
         sseg_p->write_1ptn(sseg_p->h2s(i + n * 8), 7 - i);
         sleep_ms(50);
      } // for i
   }  // for n
   



   //turn off led
   for (i = 0; i < 8; i++) {
      sseg_p->write_1ptn(0xff, i);
   }
   //turn off all decimal points
   sseg_p->set_dp(0x00);

   
   sleep_ms(500);
   sseg_p->write_1ptn(sseg_p->h2s(0xa), 7);
   sleep_ms(50);
   sseg_p->write_1ptn(sseg_p->h2s(0xb), 6);
   sleep_ms(50);
   sseg_p->write_1ptn(sseg_p->h2s(0xc), 5);
   sleep_ms(50);
   sseg_p->write_1ptn(sseg_p->h2s(0xd), 4);
   sleep_ms(50);
   sseg_p->write_1ptn(sseg_p->h2s(0xe), 3);
   sleep_ms(50);
   sseg_p->write_1ptn(sseg_p->h2s(0xf), 2);
   sleep_ms(50);
   sseg_p->write_1ptn(sseg_p->h2s(0x6), 1);
   sleep_ms(50);
   sseg_p->write_1ptn(sseg_p->h2s(0xff), 0);
   sleep_ms(1000);


   //turn off led
   for (i = 0; i < 8; i++) {
      sseg_p->write_1ptn(0xff, i);
   }

}



void sseg_test(SsegCore *sseg_p, unsigned int num) {
   //turn off all decimal points
   sseg_p->set_dp(0x00);
   
   if ((num >= 0x0a) && (num <= 0x0f)) {
      sseg_p->write_1ptn(sseg_p->h2s(num), (num - ((2*(num - 9)) + 1)));
   } else if (num == 0x06) {
      sseg_p->write_1ptn(sseg_p->h2s(0x06), 1);
   } else {
      sseg_p->write_1ptn(sseg_p->h2s(num), 0);
   }

   sleep_ms(100);

}

void sseg_add_key(SsegCore *sseg_p, uint32_t num) {   
   if ((num >= 0x0a) && (num <= 0x0f)) {
      sseg_p->write_1ptn(sseg_p->h2s(num), (num - ((2*(num - 9)) + 1)));
   } else if (num == 0x06) {
      sseg_p->write_1ptn(sseg_p->h2s(0x06), 1);
   } else {
      sseg_p->write_1ptn(sseg_p->h2s(num), 0);
   }
}

void sseg_remove_key(SsegCore *sseg_p, uint32_t num) {   
   if ((num >= 0x0a) && (num <= 0x0f)) {
      sseg_p->write_1ptn(sseg_p->h2s(0xff), (num - ((2*(num - 9)) + 1)));
   } else if (num == 0x06) {
      sseg_p->write_1ptn(sseg_p->h2s(0xff), 1);
   } else {
      sseg_p->write_1ptn(sseg_p->h2s(0xff), 0);
   }
}

void sseg_clear(SsegCore *sseg_p) {  
   //turn off led
   for (int i = 0; i < 8; i++) {
      sseg_p->write_1ptn(0xff, i);
   }
   //turn off all decimal points
   sseg_p->set_dp(0x00);

}

void sseg_update_keys(SsegCore *sseg_p, uint32_t keysArray[], uint32_t arraySize) {  
   for (int i = 0; i < arraySize; i++) {
      uint32_t num = keysArray[i];

      if ((num >= 0x0a) && (num <= 0x0f)) {
         sseg_p->write_1ptn(sseg_p->h2s(num), (num - ((2*(num - 9)) + 1)));
      } else if (num == 0x06) {
         sseg_p->write_1ptn(sseg_p->h2s(0xff), 1);
      } else {
         sseg_p->write_1ptn(sseg_p->h2s(num), 0);
      }      
   } 
}

void sseg_disp_error(SsegCore *sseg_p, uint32_t keysArray[], uint32_t arraySize) {  
   for (int i = 0; i < arraySize; i++) {
      uint32_t num = keysArray[i];
      sseg_p->write_1ptn(sseg_p->h2s(num), 7-i); 
   } 
   sseg_p->set_dp(0b10101010);
}

void uart_keys(uint32_t keysArray[], uint32_t arraySize) {
   uart.disp("keys:     ");
   for (int i = 0; i < arraySize; i++) {
      int num = keysArray[i];

      uart.disp(num, 16);
      uart.disp("  ");
   }
   uart.disp("\n\r");
}

uint32_t read_midi(GpiCore *midi_p) {
   return midi_p->read();
}

uint32_t read_switches(GpiCore *sw_p) {
   return sw_p->read();
}

uint32_t read_address_three(GpiCore *midi_p) {
   return midi_p->read();
}


GpoCore led(get_slot_addr(BRIDGE_BASE, S2_LED));
GpiCore sw(get_slot_addr(BRIDGE_BASE, S3_SW));
SsegCore sseg(get_slot_addr(BRIDGE_BASE, S8_SSEG));

GpiCore midi(get_slot_addr(BRIDGE_BASE, S4_USER));

int main() {
   //uint8_t id, ;

   //timer_check(&led);
   sleep_ms(1000);
   sseg_clear(&sseg);
   sleep_ms(1000);

   enum KeyMap {
      C0 = 12,
      D0 = 14,
      E0 = 16,
      F0 = 17,
      G0 = 19,
      A0 = 21,
      B0 = 23,
      C1 = 24,
      D1 = 26,
      E1 = 28,
      F1 = 29,
      G1 = 31,
      A1 = 33,
      B1 = 35,
      C2 = 36,
      D2 = 38,
      E2 = 40,
      F2 = 41,
      G2 = 43,
      A2 = 45,
      B2 = 47,
      C3 = 48,
      D3 = 50,
      E3 = 52,
      F3 = 53,
      G3 = 55,
      A3 = 57,
      B3 = 59,
      C4 = 60,
      D4 = 62,
      E4 = 64,
      F4 = 65,
      G4 = 67,
      A4 = 69,
      B4 = 71,
      C5 = 72
   };

   bool errorDetected = 0;
   //uint8_t switches_input = 0x00;
   uint32_t history_input = 0;
   uint32_t current_input = 0x00000000;
   uint8_t current_input_letter = 0x00;
   const uint32_t keysArrayLength = 8;
   uint32_t keysArray[keysArrayLength] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

   while (1) {
      uart_check();
      errorDetected = false;
      
      current_input = read_midi(&midi);
      //history_input = read_address_three(&midi);
      //switches_input = read_switches(&sw);
      
      uart.disp("current_input = ");
      uart.disp("\n\r");
      uart.disp(current_input, 16, 32);
      uart.disp("\n\r");
      uart.disp(current_input, 2, 32);
      uart.disp("\n\r\n\r");
      
      uart.disp("history_input = ");
      uart.disp("\n\r");
      uart.disp(history_input, 16, 32);
      uart.disp("\n\r");
      uart.disp(history_input, 2, 32);
      uart.disp("\n\r\n\r");

      if (current_input == 0) {
         errorDetected = true;
         for (int i = 0; i < keysArrayLength; i++) {
            keysArray[i] = 0x02;
         }

      } else {
         errorDetected = false;

         current_input_letter = current_input & 0xFF;
         uart.disp("current_input_letter = ");
         uart.disp(current_input_letter, 2);
         uart.disp("\n\r");

         uart.disp("current_input >> 8 = ");
         uart.disp(((current_input >> 8) & 1), 2, 3);
         uart.disp("\n\r");
         if ((current_input >> 8) & 1) {       // Check key Press/release
            switch (current_input_letter) {
               case A0:
               case A1:
               case A2:
               case A3:
               case A4:
                  keysArray[0] = 0x0A;
                  break;
               case B0:
               case B1:
               case B2:
               case B3:
               case B4:
                  keysArray[1] = 0x0B;
                  break;
               case C0:
               case C1:
               case C2:
               case C3:
               case C4:
               case C5:
                  keysArray[2] = 0x0C;
                  break;
               case D0:
               case D1:
               case D2:
               case D3:
               case D4:
                  keysArray[3] = 0x0D;
                  break;
               case E0:
               case E1:
               case E2:
               case E3:
               case E4:
                  keysArray[4] = 0x0E;
                  break;
               case F0:
               case F1:
               case F2:
               case F3:
               case F4:
                  keysArray[5] = 0x0F;
                  break;
               case G0:
               case G1:
               case G2:
               case G3:
               case G4:
                  keysArray[6] = 0x06;
                  break;
               default:
                  errorDetected = true;
                  keysArray[7] = 0x05;
                  break;
            }
         } else {
            switch (current_input_letter) {
               case A0:
               case A1:
               case A2:
               case A3:
               case A4:
                  keysArray[0] = 0xFF;
                  break;
               case B0:
               case B1:
               case B2:
               case B3:
               case B4:
                  keysArray[1] = 0xFF;
                  break;
               case C0:
               case C1:
               case C2:
               case C3:
               case C4:
               case C5:
                  keysArray[2] = 0xFF;
                  break;
               case D0:
               case D1:
               case D2:
               case D3:
               case D4:
                  keysArray[3] = 0xFF;
                  break;
               case E0:
               case E1:
               case E2:
               case E3:
               case E4:
                  keysArray[4] = 0xFF;
                  break;
               case F0:
               case F1:
               case F2:
               case F3:
               case F4:
                  keysArray[5] = 0xFF;
                  break;
               case G0:
               case G1:
               case G2:
               case G3:
               case G4:
                  keysArray[6] = 0xFF;
                  break;
               default:
                  errorDetected = true;
                  keysArray[7] = 0x07;
                  break;
            }        
         }

         //sleep_ms(10);
      }

      if (!errorDetected) {
         sseg_update_keys(&sseg, keysArray, keysArrayLength);
         uart_keys(keysArray, keysArrayLength);
      } else {
         sseg_disp_error(&sseg, keysArray, keysArrayLength);
         //uart_keys(keysArray, keysArrayLength);
      }

      //sleep_ms(50);

   } //while
} //main

/* switch testing

      
      // uart.disp("switches_input = ");
      // uart.disp("\n\r");
      // uart.disp(switches_input, 16, 8);
      // uart.disp("\n\r");
      // uart.disp(switches_input, 2, 8);
      // uart.disp("\n\r");


if (switches_input & 1) {
         current_input_letter = switches_input - 1;
            switch (current_input_letter) {
               case A0:
               case A1:
               case A2:
               case A3:
               case A4:
                  keysArray[0] = 0x0A;
                  break;
               case B0:
               case B1:
               case B2:
               case B3:
               case B4:
                  keysArray[1] = 0x0B;
                  break;
               case C0:
               case C1:
               case C2:
               case C3:
               case C4:
               case C5:
                  keysArray[2] = 0x0C;
                  break;
               case D0:
               case D1:
               case D2:
               case D3:
               case D4:
                  keysArray[3] = 0x0D;
                  break;
               case E0:
               case E1:
               case E2:
               case E3:
               case E4:
                  keysArray[4] = 0x0E;
                  break;
               case F0:
               case F1:
               case F2:
               case F3:
               case F4:
                  keysArray[5] = 0x0F;
                  break;
               case G0:
               case G1:
               case G2:
               case G3:
               case G4:
                  keysArray[6] = 0x06;
                  break;
               default:
                  errorDetected = true;
                  keysArray[7] = 0x07;
                  break;
            }

      } else 
      */


/* Errors
         if ((current_input & 0x7E000000) != 0) {
            errorDetected = true;
            keysArray[7] = (current_input >> 30) & 1;
            keysArray[6] = (current_input >> 29) & 1;
            keysArray[5] = (current_input >> 28) & 1;
            keysArray[4] = (current_input >> 27) & 1;
            keysArray[3] = (current_input >> 26) & 1;
            keysArray[2] = (current_input >> 25) & 1;
         }

         if ((current_input & 0x40000000) == 1) {
            errorDetected = true;
            keysArray[1] = 0x00;
         }
         
         if ((current_input & 0x20000000) == 1) {
            errorDetected = true;
            keysArray[2] = 0x01;
         }
         
         if ((current_input & 0x10000000) == 1) {
            errorDetected = true;
            keysArray[3] = 0x03;
         }
         
         if ((current_input & 0x08000000) == 1) {
            errorDetected = true;
            keysArray[4] = 0x05;
         }
         
         if ((current_input & 0x04000000) == 1) {
            errorDetected = true;
            keysArray[5] = 0x05;
         }
         
         if ((current_input & 0x02000000) == 1) {
            errorDetected = true;
            keysArray[6] = 0x05;
         }
         
         if ((current_input & 0x7E000000) != 0) {
            errorDetected = true;
            keysArray[3] = 0x04;
         }
         */
