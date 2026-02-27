/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_exti.h"
#include "stm32f4xx_ll_cortex.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_dma.h"
#include "stm32f4xx_ll_spi.h"
#include "stm32f4xx_ll_gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define RCC_IN_Pin LL_GPIO_PIN_0
#define RCC_IN_GPIO_Port GPIOH
#define RCC_OUT_Pin LL_GPIO_PIN_1
#define RCC_OUT_GPIO_Port GPIOH
#define SCK_LCD_Pin LL_GPIO_PIN_5
#define SCK_LCD_GPIO_Port GPIOA
#define MOSI_LCD_Pin LL_GPIO_PIN_7
#define MOSI_LCD_GPIO_Port GPIOA
#define DC_LSD_Pin LL_GPIO_PIN_4
#define DC_LSD_GPIO_Port GPIOC
#define RST_Pin LL_GPIO_PIN_0
#define RST_GPIO_Port GPIOB
#define CS_LCD_Pin LL_GPIO_PIN_1
#define CS_LCD_GPIO_Port GPIOB
#define pwm_Pin LL_GPIO_PIN_6
#define pwm_GPIO_Port GPIOC
#define TX_Pin LL_GPIO_PIN_9
#define TX_GPIO_Port GPIOA
#define RX_Pin LL_GPIO_PIN_10
#define RX_GPIO_Port GPIOA
#define SWDIO_Pin LL_GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin LL_GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define line_Pin LL_GPIO_PIN_9
#define line_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
extern const uint16_t zero[];//------------------------------------------------------//
extern const uint16_t one[];                                                         //
extern const uint16_t two[];                                                         //
extern const uint16_t three[];                                                       //
extern const uint16_t four[];//---------------Цифры часов(белые)---------------------//
extern const uint16_t five[];                                                        //
extern const uint16_t six[];                                                         //
extern const uint16_t seven[];                                                       //
extern const uint16_t eight[];                                                       //
extern const uint16_t nine[];//------------------------------------------------------//

extern const uint16_t zero_big[];//--------------------------------------------------//
extern const uint16_t one_big[];                                                     //
extern const uint16_t two_big[];                                                     //
extern const uint16_t three_big[];                                                   //
extern const uint16_t four_big[];                                                    //
extern const uint16_t five_big[];//Большие белые цифры для вывода--------------------//
extern const uint16_t six_big[];//температуры в режиме "Off"-------------------------//
extern const uint16_t seven_big[];                                                   //
extern const uint16_t eight_big[];                                                   //
extern const uint16_t nine_big[];                                                    //

extern const uint16_t zero_green[];//------------------------------------------------//
extern const uint16_t one_green[];                                                   //
extern const uint16_t two_green[];                                                   //
extern const uint16_t three_green[];                                                 //
extern const uint16_t four_green[];                                                  //
extern const uint16_t five_green[];//---------Цифры температуры(зеленые)-------------//
extern const uint16_t six_green[];                                                   //
extern const uint16_t seven_green[];                                                 //
extern const uint16_t eight_green[];                                                 //
extern const uint16_t nine_green[];//------------------------------------------------//
extern const uint16_t grad_C[];
extern const uint16_t cooler_0[];
extern const uint16_t cooler_180[];
extern const uint16_t dots[];
extern const uint16_t pig[];
extern const uint16_t termometr[];
extern const uint16_t termometr_big[];
extern const uint16_t regulator[];
extern const uint16_t barbecue[];
extern const uint16_t fire[];
extern const uint16_t fire_180[];
extern const uint16_t fire_90[];
extern const uint16_t set[];
extern const uint16_t hen[];
extern const uint16_t lamb[];
extern const uint16_t rabbit[];
extern const uint16_t cow[];
extern const uint16_t fish[];
extern const uint16_t bear[];
extern const uint16_t cap[];
extern const uint16_t scale[];
extern const uint16_t tea[];
extern const uint16_t termo[];//Режим измерения температуры платы
extern const uint16_t off[];//Все выключено
extern const uint16_t fire_off[];//Стираем нижнее или верхнее пламя
extern const uint16_t fire_off_90[];////Стираем боковое пламя
extern const uint16_t tmp_off[];
extern const uint16_t dots_off[];//Стираем точки в часах
extern const uint16_t grad_w[];
//Слова
extern const uint16_t pred[];
extern const uint16_t conv_txt[];
extern const uint16_t intensiv[];
extern const uint16_t zapec[];
extern const uint16_t culinar[];
extern const uint16_t svet[];
extern const uint16_t temperature[];
extern const uint16_t standart[];
extern const uint16_t sushca[];
extern const uint16_t grill[];
extern const uint16_t Pred_mini[];

extern uint8_t size_of;
extern uint8_t buff_old[];//Буфер экрана старый
extern uint8_t buff_new[];//Буфер экрана новый
extern uint8_t hh;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
