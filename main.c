// Breadboard example app
//
// Read from multiple analog sensors and control an RGB LED

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "app_timer.h"
#include "nrf_delay.h"
#include "nrfx_saadc.h"

#include "microbit_v2.h"


#include "gpio.h"
#include "light.h"
#include "algorithm.h"
#include "i2c.h"

#include "nrf_twi_mngr.h"

// Analog inputs
// Breakout pins 1 and 2
// These are GPIO pin numbers that can be used in ADC configurations
// AIN1 is breakout pin 1. AIN2 is breakout pin 2.
#define ANALOG_TEMP_IN  NRF_SAADC_INPUT_AIN1
#define ANALOG_LIGHT_IN NRF_SAADC_INPUT_AIN0

// ADC channel configurations
// These are ADC channel numbers that can be used in ADC calls
#define ADC_TEMP_CHANNEL  0
#define ADC_LIGHT_CHANNEL 1

// Global variables
APP_TIMER_DEF(sample_timer);
NRF_TWI_MNGR_DEF(twi_mngr_instance, 1 ,0);
struct machine_State status;

// Function prototypes
static void adc_init(void);
static float adc_sample_blocking(uint8_t channel);


float light(float_t volts){
    return volts;
}

float_t temper(float_t volts){
    // first convert volts to resistance using voltage div
    //printf("Voltage: %f ", volts);
    float_t R2 = 10000;
    float_t V = 3.3;
    float_t R1 = (V * R2) / volts;
    R1 = R1 - R2;
    //printf("Resistance: %f \n", R1);

    // then, use equation to convert R into T
    float_t B = 3950.0;
    float_t rinf = R2 * exp(-B/298.15);
    //printf("rinf is %f \n", rinf);
    float_t T = B / log(R1 / rinf);
    //printf("Kelvin: %f \n", T);
    T = T - 273.15;
    return T;
}

static void sample_timer_callback(void* _unused) {
    // Do things periodically here
    // TODO

    // get button data & update status
    status.butA = gpio_read(14);
    status.butB = gpio_read(23);

    // get the temperature data & update status
    float volts = adc_sample_blocking(0);
    status.temp = temper(volts);

    // get the light data & update status
    volts = adc_sample_blocking(1);
    status.light = light(volts);

    //update time & update status
    if (status.second < 59) status.second = status.second + 1;
    else{
        status.second = 0;
        if (status.minute < 59) status.minute = status.minute + 1;
        else{
            status.minute = 0;
            if (status.hour < 23) status.hour = status.hour + 1;
            else status.hour = 0;
        }
    }

    // now, use the current input status to update output status
    // also prints out input states so that I know what's what
    algo(&status);
}

static void saadc_event_callback(nrfx_saadc_evt_t const* _unused) {
    // don't care about saadc events
    // ignore this function
}


static void adc_init(void) {
    // Initialize the SAADC
    nrfx_saadc_config_t saadc_config = {
            .resolution = NRF_SAADC_RESOLUTION_12BIT,
            .oversample = NRF_SAADC_OVERSAMPLE_DISABLED,
            .interrupt_priority = 4,
            .low_power_mode = false,
    };
    ret_code_t error_code = nrfx_saadc_init(&saadc_config, saadc_event_callback);
    APP_ERROR_CHECK(error_code);

    // Initialize temperature sensor channel
    nrf_saadc_channel_config_t temp_channel_config = NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(ANALOG_TEMP_IN);
    error_code = nrfx_saadc_channel_init(ADC_TEMP_CHANNEL, &temp_channel_config);
    APP_ERROR_CHECK(error_code);

    // Initialize light sensor channel
    nrf_saadc_channel_config_t light_channel_config = NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(ANALOG_LIGHT_IN);
    error_code = nrfx_saadc_channel_init(ADC_LIGHT_CHANNEL, &light_channel_config);
    APP_ERROR_CHECK(error_code);
}

static float adc_sample_blocking(uint8_t channel) {
    // read ADC counts (0-4095)
    // this function blocks until the sample is ready
    int16_t adc_counts = 0;
    ret_code_t error_code = nrfx_saadc_sample_convert(channel, &adc_counts);
    APP_ERROR_CHECK(error_code);

    // convert ADC counts to volts
    // 12-bit ADC with range from 0 to 3.6 Volts
    // TODO
    float_t volts = adc_counts / 1137.78;
    //printf("%f \n", volts);
    // return voltage measurement
    return volts;
}

static void state_init(void){
    status.hour = 12;
    status.minute = 59;
    status.second = 30;
    status.cur = Up;
    status.priority = 0;
}

static void twi_init(void){

    nrf_drv_twi_config_t i2c_config = NRF_DRV_TWI_DEFAULT_CONFIG;
    i2c_config.scl = EDGE_P19;//I2C_SCL;
    i2c_config.sda = EDGE_P20;//I2C_SDA;
    i2c_config.frequency = NRF_TWIM_FREQ_100K;
    nrf_twi_mngr_init(&twi_mngr_instance, &i2c_config);

    i2c_init(&twi_mngr_instance);
}

int main(void) {
    printf("Board started!\n");

    // initialize status (kinda)
    state_init();

    // initialize lights
    lights_init();
    light_turn_on(Green);

    // configure A & B Buttons
    gpio_config(14, GPIO_INPUT);
    gpio_config(23, GPIO_INPUT);

    // initialize ADC
    adc_init();

    // initialize I2C
    twi_init();

    // initialize app timers
    app_timer_init();
    app_timer_create(&sample_timer, APP_TIMER_MODE_REPEATED, sample_timer_callback);

    // start timer
    // change the rate to whatever you want
    app_timer_start(sample_timer, 32768, NULL);
    enableMotor(On);
    // loop forever
    while (1) {
        // these lines use the state variable to adjust the motor
        // they don't care about anything but motor adjustment
        // note that if the motor is in reverse, the lower number
        // is higher power (I'm guessing two's complement

        if (status.cur == Transing){
            if (status.dest == Up) setMotorSpeed(Right, Reverse, 0);
            if (status.dest == Down) setMotorSpeed(Right, Forward, 127);
        }
        else setMotorSpeed(Right, Reverse, 127);
    }
}