//========  RainbowWheel  ========//
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

//========  SetColor  ========//
uint32_t setColor(byte switchColor) {
  switch (switchColor) {
    case 0: return strip.Color(250, 0, 0); break;       // Красный
    case 1: return strip.Color(250, 40, 0); break;      // Оранжевый
    case 2: return strip.Color(250, 250, 0); break;     // Желтый
    case 3: return strip.Color(0, 250, 0); break;       // Зеленый
    case 4: return strip.Color(0, 250, 250); break;     // Голубой
    case 5: return strip.Color(0, 0, 250); break;       // Синий
    case 6: return strip.Color(100, 0, 200); break;     // Фиолетовый
    case 7: return strip.Color(250, 0, 250); break;     // Розовый
  }
}
//========  LedBribthness  ========//
void ws2812Bright() {
  if (btnR.isClick()) {
    if (++wsBrightCounter > 4) wsBrightCounter = 0;
  }
  switch (wsBrightCounter) {
    case 0: strip.setBrightness(10); break;      // Ночная подсветка, для изменения яркости введите число 0 - 255 в скобках
    case 1: strip.setBrightness(50); break;
    case 2: strip.setBrightness(100); break;     // Дневная подсветка, для изменения яркости введите число 0 - 255 в скобках
    case 3: strip.setBrightness(125); break;
    case 4: strip.setBrightness(150); break;
  }
}

//========  SetAll  ========//
void setAll(byte r, byte g, byte b) {
  if (pulseFlag == false && wsFxTimer.isReady()) {
    ws2812Bright();
    if (INDI_COUNT == 4) {
      strip.setPixelColor(0, strip.Color(r, g, b));
      strip.setPixelColor(1, strip.Color(r, g, b));
      strip.setPixelColor(2, strip.Color(r, g, b));
      strip.setPixelColor(3, strip.Color(r, g, b));
    }
    else if (INDI_COUNT == 6) {
      strip.setPixelColor(0, strip.Color(r, g, b));
      strip.setPixelColor(1, strip.Color(r, g, b));
      strip.setPixelColor(2, strip.Color(r, g, b));
      strip.setPixelColor(3, strip.Color(r, g, b));
      strip.setPixelColor(4, strip.Color(r, g, b));
      strip.setPixelColor(5, strip.Color(r, g, b));
    }
    strip.show();
  }
  if (pulseFlag == true) pulse(r, g, b, wsMaxBright);
}

//========  SwitchFx  ========//
void switchFx() {
  ws2812Bright();
  if (wsFxCounter > CASE_COUNT) wsFxCounter = 0;
  switch (wsFxCounter) {
    case 0:
      strip.clear();
      strip.show();
      break;
    case 1: rainbowCycle(40); break;              // Радуга, сменяется по горизонтали
    case 2: rainbowFade(40); break;               // Радуга, сменяется по веритикали
    case 3: setAll(250, 0, 0); break;             // Красный
    case 4: setAll(250, 40, 0); break;            // Оранжевый
    case 5: setAll(250, 250, 0); break;           // Желтый
    case 6: setAll(0, 250, 0); break;             // Зеленый
    case 7: setAll(0, 250, 250); break;           // Голубой
    case 8: setAll(0, 0, 250); break;             // Синий
    case 9: setAll(100, 0, 200); break;           // Фиолетовый
    case 10: setAll(250, 0, 250); break;          // Розовый
    case 11: setAll(250, 250, 250); break;        // Белый
    case 12: rainbowPulse(wsMaxBright); break;    // Пульсация со сменой цветов по порядку
    case 13: randomPulse(wsMaxBright); break;     // Пульсация с рандомной сменой цветов
    case 15: break;
    case 16: break;
    case 17: break;
    case 18: break;
    case 19: break;
    case 20: break;

    default:
      //Serial.println("switchFx Erorr!");
      strip.clear();
      strip.show();
      break;
  }
}
