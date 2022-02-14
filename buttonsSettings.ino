void settingsTick() {
  if (curMode == 1) {
    if (blinkTimer.isReady()) {
      sendTime(changeHrs, changeMins);
      lampState = !lampState;
      if (lampState) {
        anodeStates[0] = 1;
        anodeStates[1] = 1;
        anodeStates[2] = 1;
        anodeStates[3] = 1;
      } else {
        if (!currentDigit) {
          anodeStates[0] = 0;
          anodeStates[1] = 0;
        } else {
          anodeStates[2] = 0;
          anodeStates[3] = 0;
        }
      }
    }
  }
}

bool buttonsTick() {
  btnSet.tick();
  btnL.tick();
  btnR.tick();
  if (curMode == 1 && wsFxMode == 0) {
    if (btnR.isClick()) {
      if (!currentDigit) {
        changeHrs++;
        if (changeHrs > 23) changeHrs = 0;
      } else {
        changeMins++;
        if (changeMins > 59) {
          changeMins = 0;
          changeHrs++;
          if (changeHrs > 23) changeHrs = 0;
        }
      }
      sendTime(changeHrs, changeMins);
    }
    if (btnL.isClick()) {
      if (!currentDigit) {
        changeHrs--;
        if (changeHrs < 0) changeHrs = 23;
      } else {
        changeMins--;
        if (changeMins < 0) {
          changeMins = 59;
          changeHrs--;
          if (changeHrs < 0) changeHrs = 23;
        }
      }
      sendTime(changeHrs, changeMins);
    }
  }
  else if (curMode == 0 && wsFxMode == 0) {
    wsFxModeBlink.reset();
    while (wsFxMode == 0 && wsFxFlag == 0) {
      setAll(0, 0, 250);
      strip.show();
      if (wsFxModeBlink.isReady()) {
        setAll(0, 0, 0);
        wsFxFlag = !wsFxFlag;
      }
    }
    // =======================  Эффекты цифр  =======================
    if (btnR.isClick()) {
      if (++FLIP_EFFECT >= FLIP_EFFECT_NUM) FLIP_EFFECT = 0;
      EEPROM.put(0, FLIP_EFFECT);
      flipTimer.setInterval(FLIP_SPEED[FLIP_EFFECT]);
      for (byte i = 0; i < 4; i++) {
        indiDimm[i] = indiMaxBright;
        anodeStates[i] = 1;
      }
      // показать эффект
      newTimeFlag = true;
      for (byte i = 0; i < 4; i++) indiDigits[i] = FLIP_EFFECT;
    }

    // =======================  Эффекты подсветки  =======================
    if (btnL.isClick()) {
      if (BACKL_TYPE == 0) {
        if (++BACKL_MODE >= 3) BACKL_MODE = 0;
        EEPROM.put(1, BACKL_MODE);
        if (BACKL_MODE == 1) {
          setPWM(BACKL, backlMaxBright);
        } else if (BACKL_MODE == 2) {
          digitalWrite(BACKL, 0);
        }
      }
    }
  }

  // =======================  WS2812B  ======================= //
  else if (curMode == 0 && wsFxMode == 1) {
    wsFxModeBlink.reset();
    while (wsFxMode == 1 && wsFxFlag == 1) {
      setAll(0, 250, 0);
      strip.show();
      if (wsFxModeBlink.isReady()) {
        setAll(0, 0, 0);
        wsFxFlag = !wsFxFlag;
      }
    }
    if (btnL.isClick()) {
      wsFxCounter++;
      rainbowWheel = 0;
      pulseLedBrigthness = 0;
      pulseStep = 1;
      //if (wsFxCounter == 0 ) wsFxCounter++;
      //else if (wsFxCounter == CASE_COUNT || changeSetAll == true) wsFxCounter++;
    }
    if (btnSet.isClick()) {
      pulseLedBrigthness = 0;
      pulseStep = 1;
      pulseFlag = !pulseFlag;
    }
  }
  // =======================  ГЛЮКИ  ========================= //
  if (btnL.isHolded() && wsFxMode == 0) {
    GLITCH_ALLOWED = !GLITCH_ALLOWED;
    EEPROM.put(2, GLITCH_ALLOWED);
  }

  if (btnR.isHolded()) {
    wsFxMode = !wsFxMode;
  }
  if (btnSet.isHolded() && wsFxMode == 0) {
    anodeStates[0] = 1;
    anodeStates[1] = 1;
    anodeStates[2] = 1;
    anodeStates[3] = 1;
    currentDigit = false;
    if (++curMode >= 2) curMode = 0;
    switch (curMode) {
      case 0:
        hrs = changeHrs;
        mins = changeMins;
        secs = 0;
        rtc.adjust(DateTime(2019, 12, 05, hrs, mins, 0));
        changeBright();
        break;
      case 1:
        changeHrs = hrs;
        changeMins = mins;
        break;
    }
  }
  if (btnSet.isClick() && wsFxMode == 0) {
    if (curMode == 1) currentDigit = !currentDigit; // настройка времени
  }
}
