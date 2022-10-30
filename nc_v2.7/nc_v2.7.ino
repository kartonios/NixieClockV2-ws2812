/*
  Скетч к проекту "Часы на ГРИ версия 2"
  Страница проекта (схемы, описания): https://alexgyver.ru/nixieclock_v2/
  Исходники на GitHub: https://github.com/AlexGyver/NixieClock_v2
  Нравится, как написан код? Поддержи автора! https://alexgyver.ru/support_alex/
  Автор: AlexGyver Technologies, 2018
  https://AlexGyver.ru/

  Modificated by Kartonios (совместно с Moonlight)
  Модификация на GitHub: https://github.com/kartonios/NixieClockV2-ws2812
  Актуальная версия: 2.7 
  Связаться со мной: https://www.instagram.com/kartonios/
  
*/

/*  В версии на 6 ламп by kartonios: 
 *   PWM - 11 пин, вместо 9
 *   WS2812B - 9 пин, вместо 11
 * 
*/




/*
  Управление:
  - Настройка времени:
    - Левая кнопка - выбор, остальные "больше" и "меньше"
    - Удержать "выбор" - настройка времени
    - Клик по "выбору" - смена настройки часы/минуты
    - Клик "больше" и "меньше" - изменение времени
    - Удержать "выбор" - возврат к режиму часов

  - Управление эффектами В РЕЖИМЕ ЧАСОВ:
    - Удержание центральной кнопки включает и выключает "глюки"
    - Клик по центральной кнопке переключает режимы подсветки ламп
      - Дыхание
      - Постоянное свечение
      - Отключена
    - Клик по правой кнопке переключает режимы перелистывания цифр
      - Без эффекта
      - Плавное угасание
      - Перемотка по порядку числа
      - Перемотка по катодам
      - Поезд
      - Резинка

  - Управление ws2812: 
    - Удерждание правой кнопки включает и выключает режим настройки подсветки 
      - Клик по правой кнопке меняет яркость 
      - Клик по левой кнопке меняет режим подсветки, на данный момент доступно 11 режимов подсветки 
      - Клик по кнопке "set" включает и выключает режим пульсации
        
*/
/*
  Версия 2.0 (совместно с Pavel Smelov):
  - Поправлены косяки с подсветкой
  - Добавлены эффекты переключения "поезд" и "резинка"
  - Оптимизирован код

  Версия 2.1:
  - Исправлена ошибка с ходом времени

  Версия 2.2:
  - Сброс секунд при установке времени
  
  Версия 2.3:
  - Добавлены расширенные настройки яркости
  - Исправлены зависания при нулевых значениях некоторых настроек

  Версия 2.4:
  - Продолжаем исправлять баги с нулевыми настройками

  Версия 2.6:
  - Демонстрация эффекта при выборе

  Версия 2.6.1 Beta:
  - Добавлена подсветка на адресных светодиодах ws2812
  - На выбор доступно 11 режимов подсветки 
  - Добавлена возможность семны типа подсветки
  - Синхронизация подсветки и счета времени

  Версия 2.7:
  - Добавлены настройки для 6 ламп
  - Фикс подсветки на ws2812b

*/

// ************************** НАСТРОЙКИ **************************
#define BOARD_TYPE 3
// тип платы часов:
// 0 - IN-12 turned (индикаторы стоят правильно)
// 1 - IN-12 (индикаторы перевёрнуты)
// 2 - IN-14 (обычная и neon dot)
// 3 - другие индикаторы
// 4 - мои часы
#define INDI_COUNT 6
// кол-во ламп:
// 4 - 4 лампы
// 6 - 6 ламп
#define BACKL_TYPE 1
// тип подсветки
// 0 - светодиоды 3мм
// 1 - адресные светодиоды ws2812
#define DOT_CHANGE 0
// режим точки:
// 0 - Выключить точку (OFF)
// 1 - Включить точку (ON)

#define DUTY 220        // скважность ШИМ. От скважности зависит напряжение! у меня 175 вольт при значении 180 и 145 вольт при 120

// ======================= ЭФФЕКТЫ =======================
// эффекты перелистывания часов
byte FLIP_EFFECT = 0;
// Выбранный активен при первом запуске и меняется кнопками. Запоминается в память
// 0 - нет эффекта
// 1 - плавное угасание и появление (рекомендуемая скорость: 100-150)
// 2 - перемотка по порядку числа (рекомендуемая скорость: 50-80)
// 3 - перемотка по порядку катодов в лампе (рекомендуемая скорость: 30-50)
// 4 - поезд (рекомендуемая скорость: 50-170)
// 5 - резинка (рекомендуемая скорость: 50-150)

// =======================  WS2812B  =======================
#define CASE_COUNT       13       // Кол-во эффектов
#define LED_COUNT        6        // Кол-во светодиодов
#define PULSE_PERIOD     20       // Коэффициент пульсации подстветки (рекомендуемый период: 50-100)

// =======================  ЯРКОСТЬ  =======================
#define NIGHT_LIGHT      1       // менять яркость от времени суток (1 вкл, 0 выкл)
#define NIGHT_START      23      // час перехода на ночную подсветку (BRIGHT_N)
#define NIGHT_END        7       // час перехода на дневную подсветку (BRIGHT)

#define INDI_BRIGHT      23      // яркость цифр дневная (1 - 24) !на 24 могут быть фантомные цифры!
#define INDI_BRIGHT_N    4       // яркость ночная (1 - 24)

#define DOT_BRIGHT       35      // яркость точки дневная (1 - 255)
#define DOT_BRIGHT_N     15      // яркость точки ночная (1 - 255)

#define BACKL_BRIGHT     250     // макс. яркость подсветки ламп дневная (0 - 255) 
#define BACKL_BRIGHT_N   50      // макс. яркость подсветки ламп ночная (0 - 255, 0 - подсветка выключена)
#define BACKL_MIN_BRIGHT 20      // мин. яркость подсветки ламп в режиме дыхание (0 - 255)
#define BACKL_PAUSE      400     // пазуа "темноты" между вспышками подсветки ламп в режиме дыхание, мс

// =======================  ГЛЮКИ  =======================
#define GLITCH_MIN       30      // минимальное время между глюками, с
#define GLITCH_MAX       120     // максимальное время между глюками, с

// ======================  МИГАНИЕ  =======================
#define DOT_TIME         500     // время мигания точки, мс
#define DOT_TIMER        20      // шаг яркости точки, мс

#define BACKL_STEP       2       // шаг мигания подсветки
#define BACKL_TIME       5000    // период подсветки, мс

// ==================  АНТИОТРАВЛЕНИЕ  ====================
#define BURN_TIME        10      // период обхода индикаторов в режиме очистки, мс
#define BURN_LOOPS       3       // количество циклов очистки за каждый период
#define BURN_PERIOD      15      // период антиотравления, минут


// ***********************  ДЛЯ РАЗРАБОТЧИКОВ  ***********************
byte BACKL_MODE = 0;                          // Выбранный активен при запуске и меняется кнопками
byte FLIP_SPEED[] = {0, 130, 50, 40, 70, 70}; // скорость эффектов, мс (количество не меняй)
byte FLIP_EFFECT_NUM = sizeof(FLIP_SPEED);    // количество эффектов
boolean GLITCH_ALLOWED = 1;                   // 1 - включить, 0 - выключить глюки. Управляется кнопкой

// ---------  БУДИЛЬНИК  ---------
#define ALM_TIMEOUT     30      // таймаут будильника
#define FREQ            900     // частота писка будильника

// пины

#define KEY0     5      // часы
#define KEY1     4      // часы 
#define KEY2     3      // минуты
#define KEY3     2      // минуты   
#define KEY4     1      // секунды 
#define KEY5     0      // секунды 
#define BTN1     7      // кнопка 1
#define BTN2     8      // кнопка 2
#define BTN3     6     // кнопка 3
#define GEN      9      // генератор
#define BACKL    11     // подсветка
#define DOT 12    // точка

//
//#define PIEZO 2   // пищалка
//#define KEY0 3    // часы
//#define KEY1 4    // часы 
//#define KEY2 5    // минуты
//#define KEY3 6    // минуты
//#define BTN1 7    // кнопка 1
//#define BTN2 8    // кнопка 2
//#define GEN 9     // генератор
//#define DOT 10    // точка
//#define BACKL 11  // подсветка
//#define BTN3 12   // кнопка 3
//

// дешифратор
#define DECODER0 A0
#define DECODER1 A1
#define DECODER2 A2
#define DECODER3 A3

// распиновка ламп
#if (BOARD_TYPE == 0 && INDI_COUNT == 4)
const byte digitMask[] = {7, 3, 6, 4, 1, 9, 8, 0, 5, 2};   // маска дешифратора платы in12_turned (цифры нормальные)
const byte opts[] = {KEY0, KEY1, KEY2, KEY3};              // порядок индикаторов слева направо
const byte cathodeMask[] = {1, 6, 2, 7, 5, 0, 4, 9, 8, 3}; // порядок катодов in12

#elif (BOARD_TYPE == 0 && INDI_COUNT == 6)
const byte digitMask[] = {7, 3, 6, 4, 1, 9, 8, 0, 5, 2};   // маска дешифратора платы in12_turned (цифры нормальные)
const byte opts[] = {KEY0, KEY1, KEY2, KEY3, KEY4, KEY5};  // порядок индикаторов слева направо
const byte cathodeMask[] = {1, 6, 2, 7, 5, 0, 4, 9, 8, 3}; // порядок катодов in12

#elif (BOARD_TYPE == 1 && INDI_COUNT == 4)
const byte digitMask[] = {2, 8, 1, 9, 6, 4, 3, 5, 0, 7};   // маска дешифратора платы in12 (цифры вверх ногами)
const byte opts[] = {KEY3, KEY2, KEY1, KEY0};              // порядок индикаторов справа налево (для IN-12 turned) и ин-14
const byte cathodeMask[] = {1, 6, 2, 7, 5, 0, 4, 9, 8, 3}; // порядок катодов in12

#elif (BOARD_TYPE == 1 && INDI_COUNT == 6)
const byte digitMask[] = {2, 8, 1, 9, 6, 4, 3, 5, 0, 7};   // маска дешифратора платы in12 (цифры вверх ногами)
const byte opts[] = {KEY5, KEY4, KEY3, KEY2, KEY1, KEY0};  // порядок индикаторов справа налево (для IN-12 turned) и ин-14
const byte cathodeMask[] = {1, 6, 2, 7, 5, 0, 4, 9, 8, 3}; // порядок катодов in12

#elif (BOARD_TYPE == 2 && INDI_COUNT == 4)
const byte digitMask[] = {9, 8, 0, 5, 4, 7, 3, 6, 2, 1};   // маска дешифратора платы in14
const byte opts[] = {KEY3, KEY2, KEY1, KEY0};              // порядок индикаторов справа налево (для IN-12 turned) и ин-14
const byte cathodeMask[] = {9, 0, 2, 1, 3, 8, 4, 7, 5, 6}; // порядок катодов in14

#elif (BOARD_TYPE == 2 && INDI_COUNT == 6)
const byte digitMask[] = {9, 8, 0, 5, 4, 7, 3, 6, 2, 1};   // маска дешифратора платы in14
const byte opts[] = {KEY5, KEY4, KEY3, KEY2, KEY1, KEY0};  // порядок индикаторов справа налево (для IN-12 turned) и ин-14
const byte cathodeMask[] = {9, 0, 2, 1, 3, 8, 4, 7, 5, 6}; // порядок катодов in14

#elif (BOARD_TYPE == 3 && INDI_COUNT == 4)
const byte digitMask[] = {9, 8, 2, 3, 7, 6, 4, 5, 1, 0};   // тут вводим свой порядок пинов
const byte opts[] = {KEY0, KEY1, KEY2, KEY3};              // свой порядок индикаторов
const byte cathodeMask[] = {9, 0, 2, 1, 3, 8, 4, 7, 5, 6}; // и свой порядок катодов

#elif (BOARD_TYPE == 3 && INDI_COUNT == 6)
const byte digitMask[] = {9, 8, 2, 3, 7, 6, 4, 5, 1, 0};   // тут вводим свой порядок пинов
const byte opts[] = {KEY0, KEY1, KEY2, KEY3, KEY4, KEY5};  // свой порядок индикаторов
const byte cathodeMask[] = {9, 0, 2, 1, 3, 8, 4, 7, 5, 6}; // и свой порядок катодов

#elif (BOARD_TYPE == 4 && INDI_COUNT == 4)
const byte digitMask[] = {5, 6, 7, 3, 2, 4, 8, 9, 0, 1};   // маска дешифратора платы in12_turned (цифры нормальные)
const byte opts[] = {KEY0, KEY1, KEY2, KEY3};              // порядок индикаторов слева направо
const byte cathodeMask[] = {1, 6, 2, 7, 5, 0, 4, 9, 8, 3}; // порядок катодов in12


#endif




/*
  ард ног ном
  А0  7   4
  А1  6   2
  А2  4   8
  А3  3   1
*/
