/*
 * images.c
 *
 *  Created on: Nov 26, 2021
 *      Author: erlbriton
 */
//25, 65535
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "images.h"
#include "Init.h"
#include "usart_dma_rx.h"
//#include "iwdg.h"

#define one_min_plc     397,  (53+hh),   20,  45
#define dec_min_plc     375,  (53+hh),   20,  45
#define one_h_plc       340,  (53+hh),   20,  45
#define dec_h_plc       318,  (53+hh),   20,  45
#define temp1_plc       378,  (8+hh),    15,  35
#define temp2_plc       360,  (8+hh),    15,  35
#define temp3_plc       342,  (8+hh),    15,  35
#define off_plc           2,  (2+hh),   468, 148
#define fire_off_plc      2,  (124+hh),  138, 23//Откорректировать!
#define fire_off90_plc  125,  (2+hh),    23, 136
#define fire_off180_plc   2,  (2+hh),   138,  23
#define animal            8,  (32+hh),  120,  80
#define word            170,  (8+hh),   120,  30

uint8_t rxBuffer[RX_BUFFER_SIZE];  // Буфер для приема данных по DMA
//uint8_t buff_new[PACKET_SIZE];     // Буфер для обработки пакета

extern UART_HandleTypeDef huart1;  // Переменная для USART1
extern DMA_HandleTypeDef hdma_usart1_rx;  // DMA для USART1

void USART1_DMA_Init(void) {
    // Инициализация DMA для USART1 (если DMA не инициализировано ранее в MX_DMA_Init)
    HAL_UART_Receive_DMA(&huart1, rxBuffer, RX_BUFFER_SIZE);
    // Включаем прерывание по IDLE, чтобы отслеживать завершение пакета
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
}
uint8_t buff_old[20]; //Буфер экрана старые значения
uint8_t buff_new[20]; //Буфер экрана новый

//------------------Точки часов старые--------------------------------------------------------
static uint8_t old_watch = 0; //12-й член старого буфера
//------------------Огонь старый-------------------------------------------------------------
static uint8_t old_fire = 0; //Нижний огонь старый (13-й член старого буфера)
static uint8_t old_fire_90 = 0; //Боковой огонь старый (14-й член старого буфера)
static uint8_t old_fire_180 = 0; //Верхний огонь старый (15-й член старого буфера)
//--------------------------------------------------------------------------------------------
static uint8_t old_mode_2 = 0; //2-й режим кнопки старый(18-й член нового буфера)

//-------------Время новое----------------------------------------------------------
uint8_t new_one_min = 0; //Единицы минут новые (1-й член нового буфера)
uint8_t new_dec_min = 0; //Десятки минут новые (2-й член нового буфера)
 uint8_t new_one_h = 0; //Единицы часов новые (3-й член нового буфера)
 uint8_t new_dec_h = 0; //Десятки часов новые (4-й член нового буфера)
//---------------------Температура новая--------------------------------------------
uint8_t new_temp1 = 0; //Новая тепература единицы (5-й член нового буфера)
uint8_t new_temp2 = 0; //Новая тепература десятки (6-й член нового буфера)
uint8_t new_temp3 = 0; //Новая тепература сотни (7-й член нового буфера)
//------------Температра уставка новая----------------------------------------------
 uint8_t new_tempset1 = 0; //Новая уставка температуры единицы (8-й член нового буфера)
 uint8_t new_tempset2 = 0; //Новая уставка температуры десятки (9-й член нового буфера)
 uint8_t new_tempset3 = 0; //Новая уставка температуры сотни (10-й член нового буфера)
//------------------Режим приготовления новый---------------------------------------
uint8_t new_set = 0; //(11-й член нового буфера)
//------------------Точки часов новые------------------------------------------------
 uint8_t new_watch = 0; //12-й член нового буфера
//------------------Огонь новый-----------------------------------------------------
 uint8_t new_fire = 0; //Пламя внизу (13-й член нового буфера)
 uint8_t new_fire_90 = 0; //Пламя сбоку (14-й член нового буфера)
 uint8_t new_fire_180 = 0; //Пламя вверху (15-й член нового буфера)
//---------------------Тампература платы новая--------------------------------------
 uint8_t new_tmp_plt1 = 0; //Новая температура платы единицы(16-й член нового буфера)
 uint8_t new_tmp_plt2 = 0; //Новая температура платы десятки(17-й член нового буфера)
 uint8_t new_mode_2 = 0; //2-й режим кнопки новый(18-й член нового буфера)

 volatile static uint8_t var_off = 0; //Флаг режима Off
 volatile static uint8_t dots_img = 0; //Флаг включения часов
 volatile static uint8_t cool_img = 0; //Флаг включения кулера
 volatile static uint8_t pass_startTim2 = 0;//Флаг старта TIM2
 extern volatile uint8_t is_cooler_mode = 0;


//-----------------  Kонстантные массивы указателей  -----------------------------------
const uint16_t *ptr_animals[11] = {0, cow, tea, termo, lamb, hen, pig, rabbit,
fish, cap, bear }; //Животные
const uint16_t *ptr_words[11] = {0, pred, sushca, temperature, grill, zapec,
		conv_txt, intensiv, standart, culinar, svet }; //Слова
const uint16_t *ptr_digitwhite[10] = { zero, one, two, three, four, five, six,
		seven, eight, nine }; //Белые цифры
const uint16_t *ptr_digitwhite_big[10] = {zero_big, one_big, two_big,
		three_big, four_big, five_big, six_big, seven_big, eight_big, nine_big }; //Белые большие цифры
const uint16_t *ptr_digitgreen[10] = {zero_green, one_green, two_green,
		three_green, four_green, five_green, six_green, seven_green,
		eight_green, nine_green }; //Зеленые цифры
const uint16_t *ptr_fire[2] = {fire_off, fire }; //Огонь снизу
const uint16_t *ptr_fire_90[2] = {fire_off_90, fire_90 }; //Огонь сбоку
const uint16_t *ptr_fire_180[2] = {fire_off, fire_180 }; //Огонь сверху
const uint16_t *ptr_cooler[2] = {cooler_0, cooler_180}; //Кулер

uint8_t size_of = 20; //Количество членов массива принимаемых данных

void screen_first(void) //Начальный экран
{
	fillScreen(0x0000);
	fillRect(0, (0 + hh), 465, 2, ILI9488_CYAN); //горизонт
	fillRect(0, (148 + hh), 465, 2, ILI9488_CYAN); //горизонт
	fillRect(0, (2 + hh), 2, 148, ILI9488_CYAN); //вертикаль
	fillRect(463, (2 + hh), 2, 148, ILI9488_CYAN); //вертикаль
	fillRect(149, (0 + hh), 2, 150, ILI9488_CYAN); //вертикаль

	drawImage(dots, 365, (63 + hh), 5, 25); //Точки между часами и минутами
	drawImage(termometr, 300, (6 + hh), 14, 35);
	drawImage(grad_C, 402, (8 + hh), 8, 8); //
	drawImage(grad_C, 402, (108 + hh), 8, 8); //
	drawImage(regulator, 290, (107 + hh), 35, 35);
	drawImage(barbecue, 430, (6 + hh), 30, 140);
	drawImage(cooler_0, 200, (70 + hh), 50, 52);

	drawImage(ptr_digitwhite[new_one_min], one_min_plc); //Изменяем единицы минут
	drawImage(ptr_digitwhite[new_dec_min], dec_min_plc); //Изменяем десятки минут
	drawImage(ptr_digitwhite[new_one_h], one_h_plc);    //Изменяем единицы часов
	drawImage(ptr_digitwhite[new_dec_h], dec_h_plc);    //Изменяем десятки часов
	drawImage(ptr_digitgreen[new_temp1], temp1_plc); //Текущая температура единицы
	drawImage(ptr_digitgreen[new_temp2], temp2_plc); //Текущая температура десятки
	drawImage(ptr_digitgreen[new_temp3], temp3_plc); //Текущая температура сотни
	drawImage(ptr_digitgreen[new_tempset1], tempset1_plc); //Установленная температура единицы
	drawImage(ptr_digitgreen[new_tempset2], tempset2_plc); //Установленная температура десятки
	drawImage(ptr_digitgreen[new_tempset3], tempset3_plc); //Установленная температура сотни
}
//-------------------------------------------------------------------------------------
void cooler_start(){
	HAL_TIM_Base_Start_IT(&htim5); //Включаем кулер
}
void cooler_stop(){
	HAL_TIM_Base_Stop_IT(&htim5); //Выключаем кулер
}
//---------------------- Callbacks of TIMs -----------------------------------------
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
//-------------Проверка включения режима "Off" в течении 10 сек-------------------------------
	if (htim->Instance == TIM2){
		turn_off();    //Включаем повара
	}
//------------------------Мигаем точками в часах-------------------------------------
	if (htim->Instance == TIM4){

		dots_img = (1 - dots_img);
		if (dots_img == 1)
			drawImage(dots, 365, (63 + hh), 5, 25);    //Мигаем
		if (dots_img == 0)
			drawImage(dots_off, 365, (63 + hh), 5, 25);//точками в часах
	}
//------------------------ Включаем вентилятор ---------------------------------------
	if (htim->Instance == TIM5) {
		is_cooler_mode = 1;
		HAL_TIM_Base_Stop_IT(&htim5);
	}
}
//---------- Включение режима off через 10 сек выдержки --------------------------------
void turn_off(){
	if (new_set == 0 && var_off == 0){//Если через 10 сек режим "Off" не был выключен(проверяем для исключения ненужных переходов в этот режим)
		HAL_TIM_Base_Stop_IT(&htim2);//Останавливаем таймер
		HAL_TIM_Base_Stop_IT(&htim4);//Останавливаем точки в часах
		drawImage(off, 2, (2 + hh), 468, 148);//Рисуем повара
		fillRect(0, (0 + hh), 465, 2, ILI9488_CYAN);//горизонт
		fillRect(0, (148 + hh), 465, 2, ILI9488_CYAN);//горизонт
		fillRect(0, (2 + hh), 2, 148, ILI9488_CYAN);//вертикаль
		fillRect(463, (2 + hh), 2, 148, ILI9488_CYAN);//вертикаль
		drawImage(grad_w, 405, (44 + hh), 16, 16);//Рисуем значек градусов
		drawImage(termometr_big, 250, (35 + hh), 34, 80);
		HAL_TIM_Base_Stop_IT(&htim5);//Отключаем кулер
		var_off = 1;    //Устанавливаем флаг "Был включен режим "Off"
		//pass_startTim2 = 1;//Повар нарисован
	} else if (new_set == 0 && var_off == 1){
		draw_turn_off(); //и включаем режим "Off"(проверяем только текущую температуру)
	}
}
void draw_turn_off(){ //Показываем только текущую температуру
	drawImage(ptr_digitwhite_big[new_temp1], temp_off3_plc);
	drawImage(ptr_digitwhite_big[new_temp2], temp_off2_plc);
	drawImage(ptr_digitwhite_big[new_temp3], temp_off1_plc);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
}
/*-------------------------------------- Изменение изображений ------------------------------------*/
void check_images() {
	if (new_set > 10){
	        return; // Игнорируем некорректный пакет.
	    }
	if (new_set != 0){      //Если новый 11 член массива массива не равен "0"
		pass_startTim2 = 0;
		if (var_off == 1){ //Если быстро крутили и повар не стерся
			screen_first(); //Рисуем экран режимов
			var_off = 0; //обнуляем флаг режима Off   cooler_0
		}
//------------------------ Картинка и слово ----------------------------------------------------------
		drawImage(ptr_animals[new_set], animal); //Изменяем картинку режима приготовления
		drawImage(ptr_words[new_set], word); //Изменяем слово режима приготовления
//------------------------------ Часы ----------------------------------------------------------------
		drawImage(ptr_digitwhite[new_one_min], one_min_plc); //Изменяем единицы минут
		drawImage(ptr_digitwhite[new_dec_min], dec_min_plc); //Изменяем десятки минут
		drawImage(ptr_digitwhite[new_one_h], one_h_plc); //Изменяем единицы часов
		drawImage(ptr_digitwhite[new_dec_h], dec_h_plc); //Изменяем десятки часов

//--------------------------- Текущая температура ----------------------------------------------------

		drawImage(ptr_digitgreen[new_temp1], temp1_plc); //Изменяем единицы текущей температуры
		drawImage(ptr_digitgreen[new_temp2], temp2_plc); //Изменяем десятки текущей температуры
		drawImage(ptr_digitgreen[new_temp3], temp3_plc); //Изменяем десятки текущей температуры
		drawImage(ptr_digitgreen[new_tempset1], tempset1_plc); //Изменяем единицы установленной температуры
		drawImage(ptr_digitgreen[new_tempset2], tempset2_plc); //Изменяем десятки установленной температуры
		drawImage(ptr_digitgreen[new_tempset3], tempset3_plc); //Изменяем десятки установленной температуры

//---------------- Включаем точки в часах -----------------------------------------------------
		if (new_watch != old_watch) {
			count_watch(new_watch); //Включаем точки на часах
		}
//---------------Включение пламени-------------------------------------------------------------
		drawImage(ptr_fire[new_fire], fire_off_plc); //Включаем нижнее пламя
		drawImage(ptr_fire_90[new_fire_90], fire_off90_plc); //Включаем боковое пламя
		drawImage(ptr_fire_180[new_fire_180], fire_off180_plc); //Включаем верхнее пламя

//--------------------------Второй режим кнопки---------------------------------------------------
		if (new_mode_2 != old_mode_2) {
			count_mode_2(new_mode_2);
		}
//---------------Включение или выключение вентилятора------------------------------------------------------
		if (new_set == 1 || new_set == 2 || new_set == 6 || new_set == 7 || new_set == 9){
			cooler_start();
		} else {
			cooler_stop();
		}
//---------------Рисование новых данных закончено, приравниваем старые данные к новым----------------------------
		old_watch = new_watch;
		old_fire = new_fire;
		old_fire_90 = new_fire_90;
		old_fire_180 = new_fire_180;
		old_mode_2 = new_mode_2;
	}
//---------------------------------------------------------------------------------------------

	else if (new_set == 0 && var_off == 0 && pass_startTim2 == 0){ //Если режим Off включился только что(var_off == 0)
		HAL_TIM_Base_Start_IT(&htim2); //Задержка включения режима Off
		pass_startTim2 = 1;
	} else if (new_set == 0 && var_off == 1 && pass_startTim2 == 1){//Если режим Off уже был включен
		//pass_startTim2 = 0;
		draw_turn_off(); //Включаем картинку режима Off
	}
	//HAL_GPIO_WritePin(GPIOA, Line_Pin, GPIO_PIN_SET); //Контроль рисования картинки
	//HAL_IWDG_Refresh(&hiwdg); //Обнуляем watchdog
	HAL_NVIC_EnableIRQ(USART1_IRQn);
	SET_BIT(RCC->CSR, RCC_CSR_RMVF);//Сбросим флаги всех прерываний от RCC
	//HAL_IWDG_Refresh(&hiwdg); //Обнуляем watchdog
}
//************************** если слишком быстро крутили и повар не стерся ***************************************************
void test_off() {
	if (var_off == 1){ //если был режим Off
		screen_first(); //рисуем заново весь экран
		var_off = 0; //обнуляем флаг режима Off
	}
}

void count_watch(uint8_t new_watch) //Включаем точки на часах
{
	if (new_watch == 1)
		HAL_TIM_Base_Start_IT(&htim4); //Если пришла команда "пуск режима" - включаем мигание
	if (new_watch == 0) //Если пришла команда "стоп режима" -
			{
		HAL_TIM_Base_Stop_IT(&htim4); //отключаем мигание
		drawImage(dots, 365, (63 + hh), 5, 25); //включаем точки
	}
}

void count_mode_2(uint8_t new_mode_2) {
	if (new_mode_2 == 1) {
		fillRect(312, (50 + hh), 110, 3, ILI9488_RED); //                   *
		fillRect(312, (53 + hh), 3, 47, ILI9488_RED); //Рисуем красный      *
		fillRect(312, (100 + hh), 110, 3, ILI9488_RED); //прямоугольник вокруг*
		fillRect(419, (53 + hh), 3, 47, ILI9488_RED); //часов              *
	}
	if (new_mode_2 == 0) {
		fillRect(312, (50 + hh), 110, 3, ILI9488_BLACK); //                   *
		fillRect(312, (53 + hh), 3, 47, ILI9488_BLACK); //Стираем красный     *
		fillRect(312, (100 + hh), 110, 3, ILI9488_BLACK); //прямоугольник вокруг*
		fillRect(419, (53 + hh), 3, 47, ILI9488_BLACK); //часов              *
	}
}

