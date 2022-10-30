void loop() {
  if (BACKL_TYPE == 1) {
    clockLoop();
    switchFx();
  }
  if (BACKL_TYPE == 0) {
    clockLoop();
  }
}
