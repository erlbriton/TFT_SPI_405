/*
 * images.h
 *
 *  Created on: 27 мар. 2022 г.
 *      Author: erlbriton
 */

#ifndef INC_IMAGES_H_
#define INC_IMAGES_H_

#include "usart_dma_rx.h"

#define tempset1_plc   378,   (108+hh), 15,  35
#define tempset2_plc   360,   (108+hh), 15,  35
#define tempset3_plc   342,   (108+hh), 15,  35

#define temp_off1_plc  300, (40+hh), 30, 70
#define temp_off2_plc  335, (40+hh), 30, 70
#define temp_off3_plc  370, (40+hh), 30, 70


void count_one_min(uint8_t);
void count_dec_min(uint8_t);
void count_one_h(uint8_t);
void count_dec_h(uint8_t);
void count_temp1(uint8_t);
void count_temp2(uint8_t);
void count_temp3(uint8_t);
void count_tempset1(uint8_t);
void count_tempset2(uint8_t);
void count_tempset3(uint8_t);
void count_set(uint8_t);
void count_cool(uint8_t);
void count_fire(uint8_t);
void count_fire_90(uint8_t);
void count_fire_180(uint8_t);
void count_tmp_plt1(uint8_t);
void count_tmp_plt2(uint8_t);
void check_off(void);
void turn_off(void);
void draw_turn_off(void);
void count_mode_2(uint8_t);
void call_uart(void);//Функция при срабатывании прерывания UART
//void turn_off(void);
void check_images(void);
void count_temp1_off(uint8_t);
void count_watch(uint8_t);
void test_off(void);

extern uint8_t flag_off;
extern uint8_t flag_uart;

extern uint8_t new_temp1;//Новая тепература единицы (5-й член нового буфера)
extern uint8_t new_temp2;//Новая тепература десятки (6-й член нового буфера)
extern uint8_t new_temp3;//Новая тепература сотни (7-й член нового буфера)

extern uint8_t new_one_min;
extern uint8_t new_dec_min; //Десятки минут новые (2-й член нового буфера)
extern uint8_t new_one_h; //Единицы часов новые (3-й член нового буфера)
extern  uint8_t new_dec_h; //Десятки часов новые (4-й член нового буфера)
//---------------------Температура новая--------------------------------------------
extern uint8_t new_temp1; //Новая тепература единицы (5-й член нового буфера)
extern uint8_t new_temp2; //Новая тепература десятки (6-й член нового буфера)
extern uint8_t new_temp3; //Новая тепература сотни (7-й член нового буфера)
//------------Температра уставка новая----------------------------------------------
extern uint8_t new_tempset1; //Новая уставка температуры единицы (8-й член нового буфера)
extern uint8_t new_tempset2 ; //Новая уставка температуры десятки (9-й член нового буфера)
extern uint8_t new_tempset3; //Новая уставка температуры сотни (10-й член нового буфера)
//------------------Режим приготовления новый---------------------------------------
extern uint8_t new_set; //(11-й член нового буфера)
//------------------Точки часов новые------------------------------------------------
extern uint8_t new_watch; //12-й член нового буфера
//------------------Огонь новый-----------------------------------------------------
extern uint8_t new_fire; //Пламя внизу (13-й член нового буфера)
extern uint8_t new_fire_90; //Пламя сбоку (14-й член нового буфера)
extern uint8_t new_fire_180; //Пламя вверху (15-й член нового буфера)
//---------------------Тампература платы новая--------------------------------------
extern uint8_t new_tmp_plt1; //Новая температура платы единицы(16-й член нового буфера)
extern uint8_t new_tmp_plt2; //Новая температура платы десятки(17-й член нового буфера)
extern uint8_t new_mode_2; //2-й режим кнопки новый(18-й член нового буфера)
//void memcpy(uint8_t, uint8_t, uint8_t);
extern volatile uint8_t is_cooler_mode;

#endif /* INC_IMAGES_H_ */
