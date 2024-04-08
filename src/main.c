#include <stdbool.h>
#include <stm8s.h>
//#include <stdio.h>
#include "main.h"
#include "milis.h"
//#include "delay.h"
//#include "uart1.h"
#include "daughterboard.h"
void init(void) {
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // taktovani MCU na 16MHz

    init_milis();

    GPIO_Init(PWM_R_PORT, PWM_R_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(PWM_G_PORT, PWM_G_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(PWM_B_PORT, PWM_B_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);

    GPIO_Init(GPIOG, GPIO_PIN_6, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(GPIOG, GPIO_PIN_7, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(GPIOG, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_SLOW);

    GPIO_Init(S1_PORT, S1_PIN, GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(S2_PORT, S2_PIN, GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(S3_PORT, S3_PIN, GPIO_MODE_IN_PU_NO_IT);

    TIM2_TimeBaseInit(TIM2_PRESCALER_16, 10000 - 1);

    TIM2_OC1Init(TIM2_OCMODE_PWM1,        // inicializujmee kanaál 1 (TIM2_CH1)
                 TIM2_OUTPUTSTATE_ENABLE, // Režím PWM1
                 0,                     // Výstup povolen (TIMer ovládá pin)
                 TIM2_OCPOLARITY_HIGH); // výchozí hodnota šířky pulzu
    TIM2_OC2Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, 0,
                 TIM2_OCPOLARITY_HIGH);
    TIM2_OC3Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, 0,
                 TIM2_OCPOLARITY_HIGH);

    TIM2_OC1PreloadConfig(ENABLE);
    TIM2_OC2PreloadConfig(ENABLE);
    TIM2_OC3PreloadConfig(ENABLE);

    TIM2_Cmd(ENABLE); // povolí TIM2
}

int main(void) {
    uint8_t mod = 1;
    uint32_t time = 0;
    int8_t r = 0, g = 0, b = 0;
    int8_t * color_pointer;
    uint8_t s_mem = 0;
    init();

    while (1) {

        if (milis() - time > 10) {
            time = milis();

            if (PUSH(S1)) {
                if (s_mem == 0) {      // provedu akci
                    mod += 1;
                    if (mod > 3) {
                        mod = 1;
                    }
                }
                s_mem = 1;
            } else
                s_mem = 0;
            
            
            
            
            if (mod == 1) {          // Zvyšování a snižování složky R
                GPIO_WriteHigh(GPIOG, GPIO_PIN_6);
                GPIO_WriteLow(GPIOG, GPIO_PIN_7);
                if (PUSH(S2)) {
                    r += 10;
                    if (r > 100)
                        r = 100;
                    TIM2_SetCompare1(r);
                }
                if (PUSH(S3)) {
                    if (r >= 10)
                        r -= 10;
                    TIM2_SetCompare1(r);
                }
            }
            
            
            if (mod == 2) {
                GPIO_WriteHigh(GPIOG, GPIO_PIN_4);
                GPIO_WriteLow(GPIOG, GPIO_PIN_6);
                if (PUSH(S2)) {
                    g += 10;
                    if (g > 100)
                        g = 100;
                    TIM2_SetCompare2(g);
                }
                if (PUSH(S3)) {
                    if (g >= 10)
                        g -= 10;
                    TIM2_SetCompare2(g);
                }
            }
           
           
            if (mod == 3) {
                GPIO_WriteHigh(GPIOG, GPIO_PIN_7);
                GPIO_WriteLow(GPIOG, GPIO_PIN_4);
                if (PUSH(S2)) {    
                    b += 10;
                    if (b > 100)
                        b = 1000;
                    TIM2_SetCompare3(b);
                }
                if (PUSH(S3)) {
                    if (b >= 10)
                        b -= 10;
                    TIM2_SetCompare3(b);
                }
            }
        }

    }
}

/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"
