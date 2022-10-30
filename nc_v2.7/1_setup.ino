void setup() {
  //Serial.begin(9600);

  // включаем ws2812b
  strip.begin();

  // случайное зерно для генератора случайных чисел
  randomSeed(analogRead(6) + analogRead(7));

  // настройка пинов на выход
  pinMode(DECODER0, OUTPUT);
  pinMode(DECODER1, OUTPUT);
  pinMode(DECODER2, OUTPUT);
  pinMode(DECODER3, OUTPUT);
  pinMode(KEY0, OUTPUT);
  pinMode(KEY1, OUTPUT);
  pinMode(KEY2, OUTPUT);
  pinMode(KEY3, OUTPUT);
  if (INDI_COUNT == 6) {
    //pinMode(KEY4, OUTPUT);
    //pinMode(KEY5, OUTPUT);
  }

  pinMode(GEN, OUTPUT);
  pinMode(BACKL, OUTPUT);
  if (DOT_CHANGE)
    pinMode(DOT, OUTPUT);
  //pinMode(PIEZO, OUTPUT);

  // задаем частоту ШИМ на 9 и 10 выводах 31 кГц
  TCCR1B = TCCR1B & 0b11111000 | 1;    // ставим делитель 1

  // включаем ШИМ
  setPWM(9, DUTY);

  // перенастраиваем частоту ШИМ на пинах 3 и 11 на 7.8 кГц и разрешаем прерывания COMPA
  TCCR2B = (TCCR2B & B11111000) | 2;    // делитель 8
  TCCR2A |= (1 << WGM21);   // включить CTC режим для COMPA
  TIMSK2 |= (1 << OCIE2A);  // включить прерывания по совпадению COMPA

  // ---------- RTC -----------
  rtc.begin();
  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  DateTime now = rtc.now();
  secs = now.second();
  mins = now.minute();
  hrs = now.hour();

  // EEPROM
  if (EEPROM.read(1023) != 100) {   // первый запуск
    EEPROM.put(1023, 100);
    //EEPROM.put(0, FLIP_EFFECT);
    if (BACKL_TYPE == 0) {
      EEPROM.put(1, BACKL_MODE);
    }
    else if (BACKL_TYPE == 1 ) {
      EEPROM.put(1, wsFxCounter);
    }
    EEPROM.put(2, GLITCH_ALLOWED);
  }

  EEPROM.get(0, FLIP_EFFECT);
  if (BACKL_TYPE == 0) {
    EEPROM.get(1, BACKL_MODE);
  }
  else if (BACKL_TYPE == 1 ) {
    EEPROM.get(1, wsFxCounter);
  }
  EEPROM.get(2, GLITCH_ALLOWED);

  sendTime(hrs, mins, secs);  // отправить время на индикаторы
  changeBright();       // изменить яркость согласно времени суток

  // установить яркость на индикаторы
  for (byte i = 0; i < INDI_COUNT; i++)
    indiDimm[i] = indiMaxBright;

  // расчёт шага яркости точки
  dotBrightStep = ceil((float)dotMaxBright * 2 / DOT_TIME * DOT_TIMER);
  if (dotBrightStep == 0) dotBrightStep = 1;

  // дыхание подсветки
  if (backlMaxBright > 0 && BACKL_TYPE == 0)
    backlBrightTimer.setInterval((float)BACKL_STEP / backlMaxBright / 2 * BACKL_TIME);

  // стартовый период глюков
  glitchTimer.setInterval(random(GLITCH_MIN * 1000L, GLITCH_MAX * 1000L));
  indiBrightCounter = indiMaxBright;

  // скорость режима при запуске
  flipTimer.setInterval(FLIP_SPEED[FLIP_EFFECT]);
  //almTimer.stop();

  setAll(0, 250, 0);
      strip.show();
}
