/**
 * @file main.c
 * @author A.M. Smith (amsmith@arrowair.com)
 * @brief Blinking LED Example
 *
 */

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_cortex.h"

/**
 * @brief System Clock Configuration
 *
 */
void SysTick_Handler(void) {
    HAL_IncTick();
}

/**
 * @brief Blinking LED Loop
 *
 * @return 0 if the program exits normally
 */
int main(void)
{
    HAL_Init();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    for (;;) {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        HAL_Delay(1000);
    }

    return 0;
}
