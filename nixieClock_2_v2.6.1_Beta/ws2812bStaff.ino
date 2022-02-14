//========RainbowWheel========//
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

//========  LedBribthness  ========//
void ws2812Bright() {
  if (btnR.isClick()) {
    if (++wsBrightCounter > 4) wsBrightCounter = 0;
  }
  switch (wsBrightCounter) {
    case 0: strip.setBrightness(50); break;      // Ночная подсветка, для изменения яркости введите число 0 - 255 в скобках
    case 1: strip.setBrightness(100); break;
    case 2: strip.setBrightness(150); break;     // Дневная подсветка, для изменения яркости введите число 0 - 255 в скобках
    case 3: strip.setBrightness(200); break;
    case 4: strip.setBrightness(250); break;
  }
}

//========  SetAll  ========//
void setAll(byte r, byte g, byte b) {
  if (pulseFlag == false) {
    ws2812Bright();
    strip.setPixelColor(0, strip.Color(r, g, b));
    strip.setPixelColor(1, strip.Color(r, g, b));
    strip.setPixelColor(2, strip.Color(r, g, b));
    strip.setPixelColor(3, strip.Color(r, g, b));
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
    case 1: rainbowCycle(40); break;
    case 2: rainbowFade(40); break;
    case 3: setAll(250, 0, 0); break;
    case 4: setAll(250, 40, 0); break;
    case 5: setAll(250, 250, 0); break;
    case 6: setAll(0, 250, 0); break;
    case 7: setAll(0, 250, 250); break;
    case 8: setAll(0, 0, 250); break;
    case 9: setAll(100, 0, 200); break;
    case 10: setAll(250, 0, 250); break;
    case 11: setAll(250, 250, 250); break;
    case 12: break;
    case 13: break;
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
