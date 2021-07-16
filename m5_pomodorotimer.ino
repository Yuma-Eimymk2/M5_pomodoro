#include <M5Stack.h>

//ポモドーロ設定（集中２５分）
const int activeMM = 25;
const int activeSS = 0;

/*
const char* jpg_name[]={
  "/pomodoro1.jpg", "/pomodoro2.jpg"
  "/pomodoro3.jpg", "/pomodoro4.jpg"
  "/pomodoro5.jpg", "/pomodoro6.jpg"
};
*/
//ポモドーロ設定（休憩5分）
const int breakMM = 5;
const int breakSS = 0;

int target_time = 0;
int omm = 99;
int oss = 99;

int mm = activeMM;
int ss = activeSS;

bool hasStop = true;  //タイマーの状態を管理
bool hasEnd = false;  //タイマーが終わっているかの管理
bool active = true;  //

void setup(){
  M5.begin();
  M5.Speaker.begin();
  M5.Speaker.setVolume(1);
  M5.Lcd.fillScreen(TFT_BLACK);  //画面を黒くする
  M5.Lcd.setCursor(10, 10);  //カーソルの位置を設定
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);  //文字の色(テキストの色，テキストの背景色)
  M5.Lcd.setTextSize(3);  //文字の大きさ

  delay(3000);

  //M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextFont(7);
  M5.Lcd.setTextSize(2);

  target_time = millis() + 1000;
  resetActive();
}

//分数の表示
void drawMinutes(int mm){
  M5.Lcd.setTextColor(0x1903);
  M5.Lcd.setCursor(15,80);
  M5.Lcd.print("88");
  M5.Lcd.setTextColor(TFT_ORANGE);

  if(mm < 10){
    M5.Lcd.setCursor(15,80);
    M5.Lcd.print("0");
    M5.Lcd.print(mm);
  }
  else{
    M5.Lcd.setCursor(15,80);
    M5.Lcd.print(mm);
  } 
}

//秒数の表示
void drawSeconds(int ss){
  M5.Lcd.setTextColor(0x1903);
  M5.Lcd.setCursor(180,80);
  M5.Lcd.print("88");
  M5.Lcd.setTextColor(TFT_ORANGE);

  if(ss < 10){
    M5.Lcd.setCursor(180,80);
    M5.Lcd.print("0");
    M5.Lcd.print(ss);
  }
  else{
    M5.Lcd.setCursor(180,80);
    M5.Lcd.print(ss);
  }
}

// インジケーターの表示
void drawIndivator(int ss, bool hasStop) {
  if (ss % 2 && !hasStop) {
    M5.Lcd.setCursor( 150, 80);
    M5.Lcd.setTextColor(0x1903);
    M5.Lcd.print(":");
    M5.Lcd.setTextColor(TFT_ORANGE);
  } else {
    M5.Lcd.setCursor( 150, 80);
    M5.Lcd.print(":");
  }
}

//集中に切り替え
void resetActive(){
  M5.Lcd.clear();

  M5.Lcd.drawJpgFile(SD, "/pomodoro2.jpg");

  active = true;
  mm = activeMM;
  ss = activeSS;
  drawSeconds(ss);
  drawMinutes(mm);

  M5.update();
  
}

//休憩に切り替え
void resetBreak(){

  M5.Lcd.clear();

  M5.Lcd.drawJpgFile(SD, "/pomodoro3.jpg");

  active = false;
  mm = breakMM;
  ss = breakSS;
  drawSeconds(ss);
  drawMinutes(mm);

  M5.update();
}

void loop(){

  if(M5.BtnA.wasPressed()){  //ボタンA(集中)が押されたら
    resetActive();
    hasStop = true;
  }

  if(M5.BtnB.wasPressed()){
    resetBreak();
    hasStop = true;
  }

  if(M5.BtnC.wasPressed() && !hasStop){  //ボタンC（切り替え）が押される&&タイマーが動いている
    hasStop = true;
  }
  else if(M5.BtnC.wasPressed() && hasStop){  //ボタンC（切り替え）が押される&&タイマーが止まっている
    hasStop = false;
  }

  if(target_time < millis()){
    target_time = millis() + 1000;

    //時間停止していない場合
    if(!hasStop){
      ss--;  //秒を進める
      if(ss == -1){  //秒が０になった時（タイマー終了）
        if(mm == 0){  //分も０
          ss = 0;
          hasStop = true;
          hasEnd = true;
          //**ここにビープ音を入れたい**
          
        }
        else{  //タイマーが終了しないとき
          ss = 59;
          omm = mm;
          mm--;
        }
      }
    }

    //タイマーが終了したとき
    if(hasEnd){
      hasEnd = false;
      hasStop = false;
      M5.Speaker.beep();
      delay(2000);
      M5.Speaker.mute();
      if(active){
        resetBreak();
      }
      else{
        resetActive();
      }
    }

    // 分の更新
    if (omm != mm) {
      omm = mm;
      drawMinutes(mm);
    }

    // 秒の更新
    if (oss != ss) {
      oss = ss;
      drawIndivator(ss, hasStop);
      drawSeconds(ss);
    }

    M5.update();
    
  }  
}
