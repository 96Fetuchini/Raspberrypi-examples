#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
 
#define CORE_FLAG 1
// Core 1 interrupt Handler
void swapThread(int ms){
    printf("swapping core\n");
    bool val = multicore_fifo_rvalid();
    if(val){//non empty
        multicore_fifo_pop_blocking();
        sleep_ms(ms);
    }
    else{//empty
        multicore_fifo_push_blocking(CORE_FLAG);
        sleep_ms(ms);
    }
}

// Core 1 Main Code
void core1_entry() {
    while (1) {
        printf("hello from core 1\n");
        swapThread(100);
    }
}

// Core 0 Main Code
int main(void){
    stdio_init_all();

    multicore_launch_core1(core1_entry); // Start core 1 - Do this before any interrupt configuration

    while (!stdio_usb_connected());
    while (!stdio_usb_init());

    // Primary Core 0 Loop
    while (1) { 
        printf("hello from core 0\n");
        swapThread(100);
    }
}