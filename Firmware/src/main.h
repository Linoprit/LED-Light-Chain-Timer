#include <ch32v00x.h>
#include <debug.h>


#undef USE_UART_1MIN
#define USE_UART_60MIN

// Definitions to allow easy adjustment
#define BLINKY_GPIO_PORT GPIOD
#define BLINKY_GPIO_PIN GPIO_Pin_2
#define LED_FAIRY_PORT GPIOC
#ifdef J4M6
#define LED_FAIRY_PIN GPIO_Pin_1
#else
#define LED_FAIRY_PIN GPIO_Pin_0
#endif
#define BLINKY_ON 1
#define BLINKY_OFF 0
// #define BLINK_LENGTH_MS 100

// Error codes
#define OK 0
#define ERR_OUT_OF_RANGE 1

int main(void);
void parse_USART(void);

void NMI_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

void GPIOConfig()
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    // GPIO_InitTypeDef GPIO_InitStructureLedFairy = {0};

    RCC_APB2PeriphClockCmd(
        RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;   // Apply the settings on all pins under the selected port
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // Internal pull-up -> Contributes to power saving
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    // ---------
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

#ifdef F4P6
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(LED_FAIRY_PORT, &GPIO_InitStructure);
    GPIO_WriteBit(LED_FAIRY_PORT, GPIO_Pin_6, BLINKY_OFF);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_Init(LED_FAIRY_PORT, &GPIO_InitStructure);
    GPIO_WriteBit(LED_FAIRY_PORT, GPIO_Pin_5, BLINKY_OFF);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(LED_FAIRY_PORT, &GPIO_InitStructure);
    GPIO_WriteBit(LED_FAIRY_PORT, GPIO_Pin_4, BLINKY_OFF);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(LED_FAIRY_PORT, &GPIO_InitStructure);
    GPIO_WriteBit(LED_FAIRY_PORT, GPIO_Pin_3, BLINKY_OFF);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(LED_FAIRY_PORT, &GPIO_InitStructure);
    GPIO_WriteBit(LED_FAIRY_PORT, GPIO_Pin_2, BLINKY_OFF);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(LED_FAIRY_PORT, &GPIO_InitStructure);
    GPIO_WriteBit(LED_FAIRY_PORT, GPIO_Pin_1, BLINKY_OFF);

    GPIO_InitStructure.GPIO_Pin = BLINKY_GPIO_PIN;
    GPIO_Init(BLINKY_GPIO_PORT, &GPIO_InitStructure);
    GPIO_WriteBit(BLINKY_GPIO_PORT, BLINKY_GPIO_PIN, BLINKY_OFF);
#endif

    // ---------
    GPIO_InitStructure.GPIO_Pin = LED_FAIRY_PIN;
    GPIO_Init(LED_FAIRY_PORT, &GPIO_InitStructure);
    GPIO_WriteBit(LED_FAIRY_PORT, LED_FAIRY_PIN, BLINKY_OFF);

    // remap UTX pin to PD6 on J4M6
#if (defined(USE_UART_1MIN) || defined(USE_UART_60MIN)) && defined J4M6
    RCC_APB2PeriphClockCmd(
        RCC_APB2Periph_GPIOD | RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_PartialRemap2_USART1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
#endif
}

#ifdef F4P6
void DoInitSequence()
{
    uint32_t bits = 1;
    for (int i = 0; i < 7; i++)
    {
        GPIO_WriteBit(LED_FAIRY_PORT, bits, BLINKY_ON);
        bits = bits << 1;
        Delay_Ms(500);
    }

    GPIO_WriteBit(BLINKY_GPIO_PORT, BLINKY_GPIO_PIN, BLINKY_ON);
    Delay_Ms(500);

    bits = 1;
    for (int i = 0; i < 7; i++)
    {
        GPIO_WriteBit(LED_FAIRY_PORT, bits, BLINKY_OFF);
        bits = bits << 1;
        Delay_Ms(500);
    }

    GPIO_WriteBit(BLINKY_GPIO_PORT, BLINKY_GPIO_PIN, BLINKY_OFF);
    Delay_Ms(500);
}
#endif
