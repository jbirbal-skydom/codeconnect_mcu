#include "stm32f4xx.h"  // Device-specific header file from CMSIS

void delay(volatile uint32_t delay_count) {
    while (delay_count--) {
        __asm("NOP");  // No operation, just waste cycles
    }
}

int main(void) {
    // Enable GPIOA Clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // Set PA5 as output
    GPIOA->MODER &= ~(3U << (5 * 2)); // Clear mode bits for PA5
    GPIOA->MODER |= (1U << (5 * 2)); // Set PA5 as output (01)

    while (1) {
        // Toggle PA5
        GPIOA->ODR ^= (1U << 5); // Toggle PA5

        // Delay
        delay(1000000);
    }
}
