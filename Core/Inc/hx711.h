/*
 * hx711.h
 *
 *  Created on: Jan 4, 2024
 *      Author: admin
 */

#ifndef INC_HX711_H_
#define INC_HX711_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include "stdint.h"
#include "stdio.h"

//####################################################################################################################

typedef struct
{
  GPIO_TypeDef  *clk_gpio;
  GPIO_TypeDef  *dat_gpio;
  uint16_t      clk_pin;
  uint16_t      dat_pin;
  int32_t       offset;
  float         coef;
  uint8_t       lock;

}hx711_t;

//####################################################################################################################

void        hx711_init(hx711_t *hx711, GPIO_TypeDef *clk_gpio, uint16_t clk_pin, GPIO_TypeDef *dat_gpio, uint16_t dat_pin);
int32_t     hx711_value(hx711_t *hx711);
int32_t     hx711_value_ave(hx711_t *hx711, uint16_t sample);

void        hx711_coef_set(hx711_t *hx711, float coef);
float       hx711_coef_get(hx711_t *hx711);
void        hx711_calibration(hx711_t *hx711, int32_t value_noload, int32_t value_load, float scale);
void        hx711_tare(hx711_t *hx711, uint16_t sample);
float       hx711_weight(hx711_t *hx711, uint16_t sample);
void        hx711_power_down(hx711_t *hx711);
void        hx711_power_up(hx711_t *hx711);

//####################################################################################################################

#ifdef __cplusplus
}
#endif

#endif
