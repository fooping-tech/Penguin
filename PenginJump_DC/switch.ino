
// スイッチの初期化処理
void switch_Setup() {
 pinMode(switch_PIN, INPUT_PULLUP);
}

// スイッチの読み込み
boolean switch_Read() {
  return (digitalRead(switch_PIN) == LOW);
}
