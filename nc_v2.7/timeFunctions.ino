// отправить на индикаторы

void sendTime(byte hours, byte minutes, byte seconds) {
  indiDigits[0] = (byte)hours / 10;
  indiDigits[1] = (byte)hours % 10;

  indiDigits[2] = (byte)minutes / 10;
  indiDigits[3] = (byte)minutes % 10;

  indiDigits[4] = (byte)seconds / 10;
  indiDigits[5] = (byte)seconds % 10;
}

void sendTime(byte hours, byte minutes) {
  indiDigits[0] = (byte)hours / 10;
  indiDigits[1] = (byte)hours % 10;

  indiDigits[2] = (byte)minutes / 10;
  indiDigits[3] = (byte)minutes % 10;

}

// для эффектов
void setNewTime() {
  newTime[0] = (byte)hrs / 10;
  newTime[1] = (byte)hrs % 10;

  newTime[2] = (byte)mins / 10;
  newTime[3] = (byte)mins % 10;

  if (INDI_COUNT == 6) {
    newTime[4] = (byte)secs / 10;
    newTime[5] = (byte)secs % 10;
  }
}
