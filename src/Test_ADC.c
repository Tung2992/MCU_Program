#include "system_S32K144.h"
#include "device_registers.h"
#include "ARM_ADC.h"
#include "ARM_GPIO.h"
#include "ARM_SYSTICK.h"
#include "My_Device.h"

#define RANGE_RESOLUTION    4095
#define PMW_CYCLE_TIME      255

extern ARM_DRIVER_GPIO Driver_GPIO;
static ARM_DRIVER_GPIO * GPIOdrv = &Driver_GPIO;

volatile uint32_t adcResult;
volatile uint32_t count;

void Init_Devices() {
    /* LED_RED */
    GPIOdrv->Setup(LED_RED_DEVICE, NULL);
    GPIOdrv->SetDirection(LED_RED_DEVICE, ARM_GPIO_OUTPUT);
    GPIOdrv->SetOutput(LED_RED_DEVICE, LED_OFF);

    /* LED_GREEN */
    GPIOdrv->Setup(LED_GREEN_DEVICE, NULL);
    GPIOdrv->SetDirection(LED_GREEN_DEVICE, ARM_GPIO_OUTPUT);
    GPIOdrv->SetOutput(LED_GREEN_DEVICE, LED_OFF);

    /* LED_BLUE */
    GPIOdrv->Setup(LED_BLUE_DEVICE, NULL);
    GPIOdrv->SetDirection(LED_BLUE_DEVICE, ARM_GPIO_OUTPUT);
    GPIOdrv->SetOutput(LED_BLUE_DEVICE, LED_OFF);

    /* POTENTIOMETER */
    adc_config_t adc_config = {
        .clockDivide = ADIV_1,
        .dmaEnable = false,
        .inputClock = ADICLK_ALTCLK_1,
        .resolution = ADC_MODE_12,
        .sampleTime = ADC_AVG_SAMPLE_4,
        .triggerType = ADTRG_SW,
        .voltageRef = ADC_REFSEL_DEF
    };
    ARM_ADC_Init(POTENTIOMETER_ADC, &adc_config);

    adc_channel_t adc_channel = {
        .controlChannel = 0,
        .inputChannel = POTENTIOMETER_INPUT_CHANNEL,
        .enableInterrupt = true
    };
    ARM_ADC_ConfigChannel(POTENTIOMETER_ADC, &adc_channel);

    /* Start initial ADC conversion */
    ARM_ADC_StartConversion(POTENTIOMETER_ADC, 0, POTENTIOMETER_INPUT_CHANNEL);
}

void ADC0_IRQHandler(void) {
    adcResult = ARM_ADC_GetConversionResult(POTENTIOMETER_ADC, 0) * PMW_CYCLE_TIME / RANGE_RESOLUTION;
    ARM_ADC_StartConversion(POTENTIOMETER_ADC, 0, POTENTIOMETER_INPUT_CHANNEL);
}

void SysTick_Handler(void) {
    count++;
}

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} Color;

// Define the rainbow colors (simplified for demonstration)
Color rainbow[] = {
    {255, 0, 0},   // Red
    {255, 127, 0}, // Orange
    {255, 255, 0}, // Yellow
    {0, 255, 0},   // Green
    {0, 0, 255},   // Blue
    {75, 0, 130},  // Indigo
    {148, 0, 211}  // Violet
};

void SetLEDOutput(int device, int state) {
    GPIOdrv->SetOutput(device, state);
}

void UpdateLED(int device, uint8_t intensity) {
    if (count <= intensity) {
        SetLEDOutput(device, LED_ON);
    } else {
        SetLEDOutput(device, LED_OFF);
    }
}

Color interpolateColor(Color c1, Color c2, float t) {
    Color result;
    result.red = c1.red + (c2.red - c1.red) * t;
    result.green = c1.green + (c2.green - c1.green) * t;
    result.blue = c1.blue + (c2.blue - c1.blue) * t;
    return result;
}

void PMW_Update() {
    int numColors = sizeof(rainbow) / sizeof(Color);
    float position = ((float)adcResult / RANGE_RESOLUTION) * (numColors - 1);
    int index = (int)position;
    float t = position - index;

    Color currentColor = interpolateColor(rainbow[index], rainbow[(index + 1) % numColors], t);

    // Update each LED based on the current color
    UpdateLED(LED_RED_DEVICE, currentColor.red * PMW_CYCLE_TIME / 255);
    UpdateLED(LED_GREEN_DEVICE, currentColor.green * PMW_CYCLE_TIME / 255);
    UpdateLED(LED_BLUE_DEVICE, currentColor.blue * PMW_CYCLE_TIME / 255);

    if (count >= PMW_CYCLE_TIME) {
        count = 0;
    }
}

int main() {
    Init_Devices();
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / 1000); // 1ms tick interval

    while (true) {
        PMW_Update();
    }

    return 1;
}
