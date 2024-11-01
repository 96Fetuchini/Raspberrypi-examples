#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/irq.h"
#include "hardware/adc.h"
 
// Core 1 interrupt Handler
void swapThread(int ms){
    uint16_t raw ;
    bool val = multicore_fifo_rvalid();
    if(val){//non empty
        raw = multicore_fifo_pop_blocking();
        sleep_ms(ms);
    }
    else{//empty
        uint16_t raw = adc_read();
        multicore_fifo_push_blocking(raw);
        sleep_ms(ms);
    }
}

// Core 1 Main Code
void core1_entry() {
    while (1) {
        swap_Thread(100);
    }
}

// Core 0 Main Code
int main(void){
    stdio_init_all();

    multicore_launch_core1(core1_entry); // Start core 1 - Do this before any interrupt configuration

    // Configure the ADC
    adc_init();
    adc_set_temp_sensor_enabled(true); // Enable on board temp sensor
    adc_select_input(4);
    
    // Primary Core 0 Loop
    while (1) { 
        swap_Thread(100);
    }
}