#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

#define BUTTON_PIN 14 // Assuming button is connected to GPIO pin 14
#define LED_PIN 15    // Assuming LED is connected to GPIO pin 15

int main()
{
    stdio_init_all();    // initializes all the pins
    adc_init();          // init the adc module
    adc_gpio_init(26);   // set ADC0 pin to be adc input instead of GPIO
    adc_select_input(0); // select to read from ADC0

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 1);

    while (!stdio_usb_connected())
    {
        sleep_ms(100);
    }
    int num_samples;

    while (1)
    {
        // Check if button is pressed
        if (!gpio_get(BUTTON_PIN))
        {
            printf("Enter a number of samples between 1 and 100: ");
            scanf("%d", &num_samples);
        }

        for (int i = 0; i < num_samples; i++)
        {
            // Read voltage on ADC0
            uint16_t result = adc_read();
            float voltage = (result - 0.06) * 22 * (3.3f / (float)UINT16_MAX);
            printf("Sample %d - Voltage on ADC0: %.2f V\n", i + 1, voltage);

            // Check button state and control LED accordingly
            if (gpio_get(BUTTON_PIN)) // Button not pressed
            {
                gpio_put(LED_PIN, 0); // Turn off LED
            }
            else // Button pressed
            {
                gpio_put(LED_PIN, 1); // Turn on LED
            }

            sleep_ms(100); // Adjust this delay as needed for approximately 100Hz sampling rate
        }
    }
}