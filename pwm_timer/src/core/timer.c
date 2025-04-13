#include "core/timer.h"
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/rcc.h>

// 96_000_000
#define PRESCALER (96)
#define ARR_VALUE (1000)

// freq = system_freq / ((prescalar -1 ) +(arr -1))

void timer_setup(void)
{
    //Enabling Peripheral for TIM2
    /*
        write a 1 on register 0x4002_3840 bit 0 enabling TIM2
    */
    rcc_periph_clock_enable(RCC_TIM2); //Table 9: Alternate function mapping PA5 

    // High level timer configuration
    /*
        TIM2 BaseAddr: 0x4000_0000
        Setting the CKD, CMS, DIR of the TIM2 control register1
            CKD: Clock Division
            CMS: Center-Aligned mode selection
            DIR: Direction
    */
    timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);

    // Setup PWM mode
    /*
        TIM2 BaseAddr + Capture/compare mode register: 0x4000_0018
        Write a 0x6 to OC1M (6:4): PWM mode 1-in upcounting, channel 1 is active as long as TIMx_CNT<TIMx_CCR1 else inactive. 
    */
    timer_set_oc_mode(TIM2, TIM_OC1, TIM_OCM_PWM1);

    // Enable PWM output
    /*
      TIM2 Base + Control Register: 
        0x4000_0000: write a 1 to bit 0
            enabling counter

      TIM2 Base + TIMx_CCER:
        0x4000_0020: write 1 on CC1E Bit 0
            On- OC1 signal is output on the corresponding output pin
    */
    timer_enable_counter(TIM2);
    timer_enable_oc_output(TIM2, TIM_OC1);

    // Setup frequency and resolution
    /*
        TIM2 Base + TIMx_PSC = write counter clock frequency CK_CNT
            0x4000_0028 = Clock frequency -1
        TIM2 Base + TIMx_ARR = write Period
            0x4000_002c = period
    */
    timer_set_prescaler(TIM2, PRESCALER - 1);
    timer_set_period(TIM2, ARR_VALUE - 1);
}

void timer_pwm_set_duty_cycle(float duty_cycle)
{
    //Duty cycle  = (ccr/arr) * 100
    // ccr = (duty cycle /100) * arr)
    const float raw_value = (float)ARR_VALUE * (duty_cycle / 100.0f);
    /*
        Base TIM2 + TIMx_CCR1: capture/compare register 1
            0x4000_0034 = duty_cycle
    */
    timer_set_oc_value(TIM2,  TIM_OC1, (uint32_t)raw_value);
}