#include "core/system.h"
#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/vector.h>
#include <libopencm3/stm32/rcc.h>

/*
    0xffffffffffffffff  #milli seconds
    /1000               #seconds
    /60                 #minutes
    /60                 #hours
    /24                 #days
    /365                #years
*/
static volatile uint64_t ticks = 0; //private to this module

void sys_tick_handler(void)
{
    //There's an issue here
    ticks++; //2 instruction since its a 32 bit system

}

static void rcc_setup(void)
{
    rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_3V3_96MHZ]);
}

static void systick_setup(void)
{
    systick_set_frequency(SYSTICK_FREQ, CPU_FREQ);
    systick_counter_enable();
    systick_interrupt_enable();
}

uint64_t system_get_ticks(void)
{
    return ticks;
}

void system_setup(void)
{
    rcc_setup();
    systick_setup();
}