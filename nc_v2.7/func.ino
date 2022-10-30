void burnIndicators() {
  for (byte k = 0; k < BURN_LOOPS; k++) {
    for (byte d = 0; d < 10; d++) {
      for (byte i = 0; i < INDI_COUNT; i++) {
        indiDigits[i]--;
        if (indiDigits[i] < 0) indiDigits[i] = 9;
      }
      delay(BURN_TIME);
    }
  }
}

//========  ClockLoop ========//
void clockLoop() {
  if (dotTimer.isReady()) {
    calculateTime();                                // каждые 500 мс пересчёт и отправка времени
  }
  if (newTimeFlag && curMode == 0) flipTick();      // перелистывание цифр
  if (DOT_CHANGE) dotBrightTick();                  // плавное мигание точки
  if (BACKL_TYPE == 0 ) backlBrightTick();          // плавное мигание подсветки ламп
  if (GLITCH_ALLOWED && mode == 0) glitchTick();    // глюки
  buttonsTick();                                    // кнопки
  settingsTick();                                   // настройки
}
