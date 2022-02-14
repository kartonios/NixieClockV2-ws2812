void flipTick() {
  if (FLIP_EFFECT == 0) {
    sendTime(hrs, mins);
    newTimeFlag = false;
  }
  else if (FLIP_EFFECT == 1) {
    if (!flipInit) {
      flipInit = true;
      // запоминаем, какие цифры поменялись и будем менять их яркость
      for (byte i = 0; i < 4; i++) {
        if (indiDigits[i] != newTime[i]) flipIndics[i] = true;
        else flipIndics[i] = false;
      }
    }
    if (flipTimer.isReady()) {
      if (!indiBrightDirection) {
        indiBrightCounter--;            // уменьшаем яркость
        if (indiBrightCounter <= 0) {   // если яроксть меньше нуля
          indiBrightDirection = true;   // меняем направление изменения
          indiBrightCounter = 0;        // обнуляем яркость
          sendTime(hrs, mins);          // меняем цифры
        }
      } else {
        indiBrightCounter++;                        // увеличиваем яркость
        if (indiBrightCounter >= indiMaxBright) {   // достигли предела
          indiBrightDirection = false;              // меняем направление
          indiBrightCounter = indiMaxBright;        // устанавливаем максимум
          // выходим из цикла изменения
          flipInit = false;
          newTimeFlag = false;
        }
      }
      for (byte i = 0; i < 4; i++)
        if (flipIndics[i]) indiDimm[i] = indiBrightCounter;   // применяем яркость
    }
  }
  else if (FLIP_EFFECT == 2) {
    if (!flipInit) {
      flipInit = true;
      // запоминаем, какие цифры поменялись и будем менять их
      for (byte i = 0; i < 4; i++) {
        if (indiDigits[i] != newTime[i]) flipIndics[i] = true;
        else flipIndics[i] = false;
      }
    }

    if (flipTimer.isReady()) {
      byte flipCounter = 0;
      for (byte i = 0; i < 4; i++) {
        if (flipIndics[i]) {
          indiDigits[i]--;
          if (indiDigits[i] < 0) indiDigits[i] = 9;
          if (indiDigits[i] == newTime[i]) flipIndics[i] = false;
        } else {
          flipCounter++;        // счётчик цифр, которые не надо менять
        }
      }
      if (flipCounter == 4) {   // если ни одну из 4 цифр менять не нужно
        // выходим из цикла изменения
        flipInit = false;
        newTimeFlag = false;
      }
    }

    //byte cathodeMask[] = {1, 0, 2, 9, 3, 8, 4, 7, 5, 6};  // порядок катодов in14
  }
  else if (FLIP_EFFECT == 3) {
    if (!flipInit) {
      flipInit = true;
      // запоминаем, какие цифры поменялись и будем менять их
      for (byte i = 0; i < 4; i++) {
        if (indiDigits[i] != newTime[i]) {
          flipIndics[i] = true;
          for (byte c = 0; c < 10; c++) {
            if (cathodeMask[c] == indiDigits[i]) startCathode[i] = c;
            if (cathodeMask[c] == newTime[i]) endCathode[i] = c;
          }
        }
        else flipIndics[i] = false;
      }
    }

    if (flipTimer.isReady()) {
      byte flipCounter = 0;
      for (byte i = 0; i < 4; i++) {
        if (flipIndics[i]) {
          if (startCathode[i] > endCathode[i]) {
            startCathode[i]--;
            indiDigits[i] = cathodeMask[startCathode[i]];
          } else if (startCathode[i] < endCathode[i]) {
            startCathode[i]++;
            indiDigits[i] = cathodeMask[startCathode[i]];
          } else {
            flipIndics[i] = false;
          }
        } else {
          flipCounter++;
        }
      }
      if (flipCounter == 4) {   // если ни одну из 4 цифр менять не нужно
        // выходим из цикла изменения
        flipInit = false;
        newTimeFlag = false;
      }
    }
  }
  // --- train --- //
  else if (FLIP_EFFECT == 4) {
    if (!flipInit) {
      flipInit = true;
      currentLamp = 0;
      trainLeaving = true;
      flipTimer.reset();
    }
    if (flipTimer.isReady()) {
      if (trainLeaving) {
        for (byte i = 3; i > currentLamp; i--) {
          indiDigits[i] = indiDigits[i - 1];
        }
        anodeStates[currentLamp] = 0;
        currentLamp++;
        if (currentLamp >= 4) {
          trainLeaving = false; //coming
          currentLamp = 0;
          //sendTime(hrs, mins);
        }
      }
      else { //trainLeaving == false
        for (byte i = currentLamp; i > 0; i--) {
          indiDigits[i] = indiDigits[i - 1];
        }
        indiDigits[0] = newTime[3 - currentLamp];
        anodeStates[currentLamp] = 1;
        currentLamp++;
        if (currentLamp >= 4) {
          flipInit = false;
          newTimeFlag = false;
        }
      }
    }
  }

  // --- elastic band --- //
  else if (FLIP_EFFECT == 5) {
    if (!flipInit) {
      flipInit = true;
      flipEffectStages = 0;
      flipTimer.reset();
    }
    if (flipTimer.isReady()) {
      switch (flipEffectStages++) {
        case 1:
          anodeStates[3] = 0; break;
        case 2:
          anodeStates[2] = 0;
          indiDigits[3] = indiDigits[2];
          anodeStates[3] = 1; break;
        case 3:
          anodeStates[3] = 0; break;
        case 4:
          anodeStates[1] = 0;
          indiDigits[2] = indiDigits[1];
          anodeStates[2] = 1; break;
        case 5:
          anodeStates[2] = 0;
          indiDigits[3] = indiDigits[1];
          anodeStates[3] = 1; break;
        case 6:
          anodeStates[3] = 0; break;
        case 7:
          anodeStates[0] = 0;
          indiDigits[1] = indiDigits[0];
          anodeStates[1] = 1; break;
        case 8:
          anodeStates[1] = 0;
          indiDigits[2] = indiDigits[0];
          anodeStates[2] = 1; break;
        case 9:
          anodeStates[2] = 0;
          indiDigits[3] = indiDigits[0];
          anodeStates[3] = 1; break;
        case 10:
          anodeStates[3] = 0;
          //sendTime(hrs,mins);
          break;
        case 11:
          indiDigits[0] = newTime[3];
          anodeStates[0] = 1; break;
        case 12:
          anodeStates[0] = 0;
          indiDigits[1] = newTime[3];
          anodeStates[1] = 1; break;
        case 13:
          anodeStates[1] = 0;
          indiDigits[2] = newTime[3];
          anodeStates[2] = 1; break;
        case 14:
          anodeStates[2] = 0;
          indiDigits[3] = newTime[3];
          anodeStates[3] = 1; break;
        case 15:
          indiDigits[0] = newTime[2];
          anodeStates[0] = 1; break;
        case 16:
          anodeStates[0] = 0;
          indiDigits[1] = newTime[2];
          anodeStates[1] = 1; break;
        case 17:
          anodeStates[1] = 0;
          indiDigits[2] = newTime[2];
          anodeStates[2] = 1; break;
        case 18:
          indiDigits[0] = newTime[1];
          anodeStates[0] = 1; break;
        case 19:
          anodeStates[0] = 0;
          indiDigits[1] = newTime[1];
          anodeStates[1] = 1; break;
        case 20:
          indiDigits[0] = newTime[0];
          anodeStates[0] = 1; break;
        case 21:
          flipInit = false;
          newTimeFlag = false;
      }
    }
  }
}


// =======================  Эффекты подсветки  =======================//


//========  PulseFx  ========//
void pulse(byte r, byte g, byte b, byte maxBrightness) {
  wsFxTimer.setInterval(pulsePeriod);
  if (wsFxTimer.isReady()) {
    pulseLedBrigthness += pulseStep;
    strip.setPixelColor(0, strip.Color(r, g, b));
    strip.setPixelColor(1, strip.Color(r, g, b));
    strip.setPixelColor(2, strip.Color(r, g, b));
    strip.setPixelColor(3, strip.Color(r, g, b));
    strip.setBrightness(pulseLedBrigthness);
    strip.show();
    if (pulseLedBrigthness == 0 || pulseLedBrigthness == maxBrightness) pulseStep = -pulseStep;
  }
}

//========  RainbowCycleFx  ========//
void rainbowCycle(uint16_t wait) {
  wsFxTimer.setInterval(wait);
  if (wsFxTimer.isReady()) {
    strip.setPixelColor(0, Wheel(((0 * 256 / 30) + rainbowWheel) & 255));
    strip.setPixelColor(1, Wheel(((1 * 256 / 30) + rainbowWheel) & 255));
    strip.setPixelColor(2, Wheel(((2 * 256 / 30) + rainbowWheel) & 255));
    strip.setPixelColor(3, Wheel(((3 * 256 / 30) + rainbowWheel) & 255));
    strip.show();
    rainbowWheel++;
  }
}


//========  RainbowFadeFx ========//
void rainbowFade(uint16_t wait) {
  wsFxTimer.setInterval(wait);
  if (wsFxTimer.isReady()) {
    strip.setPixelColor(0, Wheel(rainbowWheel & 255));
    strip.setPixelColor(1, Wheel(rainbowWheel & 255));
    strip.setPixelColor(2, Wheel(rainbowWheel & 255));
    strip.setPixelColor(3, Wheel(rainbowWheel & 255));
    strip.show();
    rainbowWheel++;
  }
}


//=============================  В разработке  =============================//


//========  PinPongFx ========//
void pinPong(uint16_t wait, byte r, byte g, byte b) {
  int i = 3;
  uint32_t timer;
  bool flag = true;
  bool buttonState = false;
  strip.clear();
  while (!buttonState) {
    ws2812Bright();
    while ( 3 >= i && i > 0 && flag == true && !buttonState) {
      btnL.tick();
      ws2812Bright();
      if (btnL.isClick()) {
        buttonState = true;
        wsFxCounter++;
        break;
      }
      clockLoop();
      if (millis() - timer > wait) {
        timer = millis();
        strip.clear();
        strip.setPixelColor(i, strip.Color(r, g, b));
        strip.show();
        i -= 1;
      }
    }
    flag = !flag;
    while ( 0 <= i && i < 3 && flag == false && !buttonState) {
      ws2812Bright();
      btnL.tick();
      ws2812Bright();
      if (btnL.isClick()) {
        buttonState = true;
        wsFxCounter++;
        break;
      }
      clockLoop();
      if (millis() - timer > wait) {
        timer = millis();
        strip.clear();
        strip.setPixelColor(i, strip.Color(r, g, b));
        strip.show();
        i += 1;
      }
    }
  }
}

//========FillUpFx========//
void fillUp(uint16_t wait, byte r, byte g, byte b) {
  strip.clear();
  uint32_t timer;
  byte j = 0 ;
  while (j < 5) {
    ws2812Bright();
    int8_t i = 4;
    while (i > j - 1 ) {
      btnL.tick();
      ws2812Bright();
      if (btnL.isClick()) {
        wsFxCounter++;
        break;
      }
      clockLoop();
      if (millis() - timer > wait) {
        timer = millis();
        strip.setPixelColor(i + 1, strip.Color(0, 0, 0));
        strip.setPixelColor(i, strip.Color(r, g, b));
        strip.show();
        i--;
      }
    }
    j++;
  }
}
