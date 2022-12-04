#include "mbed.h"
#include "stm32f413h_discovery_lcd.h"
#include "stm32f413h_discovery_ts.h"
#include "vector"

InterruptIn gore(p5);
InterruptIn dolje(p6);
InterruptIn lijevo(p7);
InterruptIn desno(p8);
InterruptIn sredina(p9);

bool zazuti = false;
bool g = false, d = false, li = false, de = false, s = false, pocetak = true;
bool startaj = false;
bool nextlvl = true, res = false;
bool ucitavanje = false;
bool jeLiPocelo = false;

int vrijeme1 = 0, vrijeme2 = 0;

//Funkcije namijenjene tasterima za kretanje zmije
void funGore() {
  if (jeLiPocelo) {
    g = true;
    pocetak = false;
    d = false;
    li = false;
    de = false;
  }
}

void funDolje() {
  if (jeLiPocelo) {
    d = true;
    pocetak = false;
    g = false;
    li = false;
    de = false;
  }
}

void funLijevo() {
  if (jeLiPocelo) {
    li = true;
    pocetak = false;
    g = false;
    d = false;
    de = false;
  }
}

void funDesno() {
  if (jeLiPocelo) {
    de = true;
    pocetak = false;
    g = false;
    d = false;
    li = false;
  }
}

//Funkcije namijenjene tasterima za biranje opcija na ekranu
void funSredina() { zazuti = true; }

void funGore2() {
  nextlvl = true;
  res = false;
}

void funDolje2() {
  nextlvl = false;
  res = true;
}

//Funkcija koja se aktivira na kraju levela 1
void level1Complete() {
  startaj = false;
  
  //Izgled ekrana
  BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
  BSP_LCD_FillRect(00, 00, 240, 240);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_DrawRect(00, 00, 240, 240);

  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_FillRect(05, 05, 230, 230);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_DrawRect(05, 05, 230, 230);

  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(20, 220, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(40, 220, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(60, 220, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(80, 220, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(100, 220, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(120, 220, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(140, 220, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_FillCircle(142, 215, 4);
  BSP_LCD_FillCircle(142, 225, 4);
  BSP_LCD_SetTextColor(LCD_COLOR_RED);
  BSP_LCD_FillRect(150, 218, 15, 3);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(220, 140, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(220, 120, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(220, 100, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(220, 80, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(220, 60, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(220, 40, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(220, 20, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(200, 20, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(180, 20, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(160, 20, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(140, 20, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_FillCircle(138, 24, 4);
  BSP_LCD_FillCircle(138, 16, 4);
  BSP_LCD_SetTextColor(LCD_COLOR_RED);
  BSP_LCD_FillRect(114, 20, 15, 3);

  char completed[20];
  snprintf(completed, 200, "LEVEL 1 COMPLETED");
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_GREEN);
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_DisplayStringAt(10, 80, (uint8_t *)completed, LEFT_MODE);
  char stanje[20];
  
  //Ispis vremena i bodova
  if (vrijeme2 <= 9) {
    if (vrijeme1 <= 9)
      snprintf(stanje, 200, "Time: %d%d:%d%d, Score: %d", 0, vrijeme2, 0,
               vrijeme1, 50);
    else
      snprintf(stanje, 200, "Time: %d%d:%d, Score: %d", 0, vrijeme2, vrijeme1,
               50);
  } else {
    if (vrijeme1 <= 9)
      snprintf(stanje, 200, "Time: %d:%d%d, Score: %d", vrijeme2, 0, vrijeme1,
               50);
    else
      snprintf(stanje, 200, "Time: %d%d:%d, Score: %d", 0, vrijeme2, vrijeme1,
               50);
  }
  
  //Ispis opcija Next Level i Restart
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_GREEN);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(25, 100, (uint8_t *)stanje, LEFT_MODE);
  char nextlevel[20];
  snprintf(nextlevel, 200, "NEXT LEVEL");
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_ORANGE);
  BSP_LCD_SetFont(&Font20);
  BSP_LCD_DisplayStringAt(35, 115, (uint8_t *)nextlevel, LEFT_MODE);
  char restart[100];
  snprintf(restart, 200, "RESTART", 0, 0, 0, 0, 100);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetFont(&Font20);
  BSP_LCD_DisplayStringAt(55, 140, (uint8_t *)restart, LEFT_MODE);

  zazuti = false;
  ucitavanje = false;
  gore.rise(&funGore2);
  dolje.rise(&funDolje2);
  while (1) {

    if (nextlvl) {
      char nextlevel[20];
      snprintf(nextlevel, 200, "NEXT LEVEL");
      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
      BSP_LCD_SetBackColor(LCD_COLOR_ORANGE);
      BSP_LCD_SetFont(&Font20);
      BSP_LCD_DisplayStringAt(35, 115, (uint8_t *)nextlevel, LEFT_MODE);
      char restart[100];
      snprintf(restart, 200, "RESTART", 0, 0, 0, 0, 100);
      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
      BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
      BSP_LCD_SetFont(&Font20);
      BSP_LCD_DisplayStringAt(55, 140, (uint8_t *)restart, LEFT_MODE);
    }

    if (res) {
      char nextlevel[20];
      snprintf(nextlevel, 200, "NEXT LEVEL");
      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
      BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
      BSP_LCD_SetFont(&Font20);
      BSP_LCD_DisplayStringAt(35, 115, (uint8_t *)nextlevel, LEFT_MODE);
      char restart[100];
      snprintf(restart, 200, "RESTART", 0, 0, 0, 0, 100);
      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
      BSP_LCD_SetBackColor(LCD_COLOR_ORANGE);
      BSP_LCD_SetFont(&Font20);
      BSP_LCD_DisplayStringAt(55, 140, (uint8_t *)restart, LEFT_MODE);
    }

    if (zazuti == true && nextlvl == true) {
      zazuti = true;
      char nextlevel[20];
      snprintf(nextlevel, 200, "NEXT LEVEL");
      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
      BSP_LCD_SetBackColor(LCD_COLOR_YELLOW);
      BSP_LCD_SetFont(&Font20);
      BSP_LCD_DisplayStringAt(35, 115, (uint8_t *)nextlevel, LEFT_MODE);
      ucitavanje = true;
      wait(1);
      break;
    } else if (zazuti == true && res == true) {
      char start[20];
      snprintf(start, 200, "RESTART");
      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
      BSP_LCD_SetBackColor(LCD_COLOR_YELLOW);
      BSP_LCD_SetFont(&Font20);
      BSP_LCD_DisplayStringAt(55, 140, (uint8_t *)start, LEFT_MODE);
      ucitavanje = true;
      wait(1);
      break;
    }
    wait(0.5);
  }

  int brojac = 0;
  int x1 = 95, y1, x2, y2;
  int brojac_vremena = 0;
  gore.rise(&funGore);
  dolje.rise(&funDolje);
  
  //Loading ekran
  while (1) {
    if (ucitavanje) {
      BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
      BSP_LCD_FillRect(00, 00, 240, 240);
      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
      BSP_LCD_DrawRect(00, 00, 240, 240);

      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
      BSP_LCD_FillRect(05, 05, 230, 230);
      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
      BSP_LCD_DrawRect(05, 05, 230, 230);

      char loading[20];
      snprintf(loading, 200, "Loading");
      BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
      BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
      BSP_LCD_SetFont(&Font20);
      BSP_LCD_DisplayStringAt(70, 82, (uint8_t *)loading, LEFT_MODE);

      ucitavanje = false;
    }

    if (brojac != 0) {
      BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
      BSP_LCD_FillCircle(x1, y1, 4);
    }
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    x2 = x1 + 8;
    y2 = 110;
    BSP_LCD_FillCircle(x2, y2, 4);
    x1 = x2;
    y1 = y2;
    brojac++;
    if (brojac == 7) {
      startaj = true;
      break;
    }
    wait(0.5);
  }
}

//Funkcija koja se aktivira na kraju levela 2
void level2Complete() {
  startaj = false;
  
  //Izgled ekrana
  BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
  BSP_LCD_FillRect(00, 00, 240, 240);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_DrawRect(00, 00, 240, 240);

  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_FillRect(05, 05, 230, 230);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_DrawRect(05, 05, 230, 230);

  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(20, 220, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(40, 220, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(60, 220, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(80, 220, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(100, 220, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(120, 220, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(140, 220, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_FillCircle(142, 215, 4);
  BSP_LCD_FillCircle(142, 225, 4);
  BSP_LCD_SetTextColor(LCD_COLOR_RED);
  BSP_LCD_FillRect(150, 218, 15, 3);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(220, 140, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(220, 120, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(220, 100, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(220, 80, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(220, 60, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(220, 40, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(220, 20, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(200, 20, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(180, 20, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(160, 20, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(140, 20, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_FillCircle(138, 24, 4);
  BSP_LCD_FillCircle(138, 16, 4);
  BSP_LCD_SetTextColor(LCD_COLOR_RED);
  BSP_LCD_FillRect(114, 20, 15, 3);

  char completed[20];
  snprintf(completed, 200, "LEVEL 2 COMPLETED");
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_GREEN);
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_DisplayStringAt(10, 80, (uint8_t *)completed, LEFT_MODE);
  char stanje[20];
  
  //Ispis vremena i bodova
  if (vrijeme2 <= 9) {
    if (vrijeme1 <= 9)
      snprintf(stanje, 200, "Time: %d%d:%d%d, Score: %d", 0, vrijeme2, 0,
               vrijeme1, 75);
    else
      snprintf(stanje, 200, "Time: %d%d:%d, Score: %d", 0, vrijeme2, vrijeme1,
               75);
  } else {
    if (vrijeme1 <= 9)
      snprintf(stanje, 200, "Time: %d:%d%d, Score: %d", vrijeme2, 0, vrijeme1,
               75);
    else
      snprintf(stanje, 200, "Time: %d%d:%d, Score: %d", 0, vrijeme2, vrijeme1,
               75);
  }
  
  //Ispis opcija Next Level i Restart
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_GREEN);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(25, 100, (uint8_t *)stanje, LEFT_MODE);
  char nextlevel[20];
  snprintf(nextlevel, 200, "NEXT LEVEL");
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_ORANGE);
  BSP_LCD_SetFont(&Font20);
  BSP_LCD_DisplayStringAt(35, 115, (uint8_t *)nextlevel, LEFT_MODE);
  char restart[100];
  snprintf(restart, 200, "RESTART", 0, 0, 0, 0, 100);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetFont(&Font20);
  BSP_LCD_DisplayStringAt(55, 140, (uint8_t *)restart, LEFT_MODE);

  zazuti = false;
  ucitavanje = false;
  gore.rise(&funGore2);
  dolje.rise(&funDolje2);
  nextlvl = true, res = false;
  while (1) {

    if (nextlvl) {
      char nextlevel[20];
      snprintf(nextlevel, 200, "NEXT LEVEL");
      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
      BSP_LCD_SetBackColor(LCD_COLOR_ORANGE);
      BSP_LCD_SetFont(&Font20);
      BSP_LCD_DisplayStringAt(35, 115, (uint8_t *)nextlevel, LEFT_MODE);
      char restart[100];
      snprintf(restart, 200, "RESTART", 0, 0, 0, 0, 100);
      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
      BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
      BSP_LCD_SetFont(&Font20);
      BSP_LCD_DisplayStringAt(55, 140, (uint8_t *)restart, LEFT_MODE);
    }

    if (res) {
      char nextlevel[20];
      snprintf(nextlevel, 200, "NEXT LEVEL");
      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
      BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
      BSP_LCD_SetFont(&Font20);
      BSP_LCD_DisplayStringAt(35, 115, (uint8_t *)nextlevel, LEFT_MODE);
      char restart[100];
      snprintf(restart, 200, "RESTART", 0, 0, 0, 0, 100);
      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
      BSP_LCD_SetBackColor(LCD_COLOR_ORANGE);
      BSP_LCD_SetFont(&Font20);
      BSP_LCD_DisplayStringAt(55, 140, (uint8_t *)restart, LEFT_MODE);
    }

    if (zazuti == true && nextlvl == true) {
      zazuti = true;
      char nextlevel[20];
      snprintf(nextlevel, 200, "NEXT LEVEL");
      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
      BSP_LCD_SetBackColor(LCD_COLOR_YELLOW);
      BSP_LCD_SetFont(&Font20);
      BSP_LCD_DisplayStringAt(35, 115, (uint8_t *)nextlevel, LEFT_MODE);
      ucitavanje = true;
      wait(1);
      break;
    } else if (zazuti == true && res == true) {
      char start[20];
      snprintf(start, 200, "RESTART");
      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
      BSP_LCD_SetBackColor(LCD_COLOR_YELLOW);
      BSP_LCD_SetFont(&Font20);
      BSP_LCD_DisplayStringAt(55, 140, (uint8_t *)start, LEFT_MODE);
      ucitavanje = true;
      wait(1);
      break;
    }
    wait(0.5);
  }

  int brojac = 0;
  int x1 = 95, y1, x2, y2;
  int brojac_vremena = 0;
  gore.rise(&funGore);
  dolje.rise(&funDolje);
  
  //Loading ekran
  while (1) {
    if (ucitavanje) {
      BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
      BSP_LCD_FillRect(00, 00, 240, 240);
      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
      BSP_LCD_DrawRect(00, 00, 240, 240);

      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
      BSP_LCD_FillRect(05, 05, 230, 230);
      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
      BSP_LCD_DrawRect(05, 05, 230, 230);

      char loading[20];
      snprintf(loading, 200, "Loading");
      BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
      BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
      BSP_LCD_SetFont(&Font20);
      BSP_LCD_DisplayStringAt(70, 82, (uint8_t *)loading, LEFT_MODE);

      ucitavanje = false;
    }

    if (brojac != 0) {
      BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
      BSP_LCD_FillCircle(x1, y1, 4);
    }
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    x2 = x1 + 8;
    y2 = 110;
    BSP_LCD_FillCircle(x2, y2, 4);
    x1 = x2;
    y1 = y2;
    brojac++;
    if (brojac == 7) {
      startaj = true;
      break;
    }
    wait(0.5);
  }
}

//Funkcija koja se aktivira na kraju levela 1
void level3Complete() {
  startaj = false;
  
  //Izgled ekrana
  BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
  BSP_LCD_FillRect(00, 00, 240, 240);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_DrawRect(00, 00, 240, 240);

  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_FillRect(05, 05, 230, 230);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_DrawRect(05, 05, 230, 230);

  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(20, 220, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(40, 220, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(60, 220, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(80, 220, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(100, 220, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(120, 220, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(140, 220, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_FillCircle(142, 215, 4);
  BSP_LCD_FillCircle(142, 225, 4);
  BSP_LCD_SetTextColor(LCD_COLOR_RED);
  BSP_LCD_FillRect(150, 218, 15, 3);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(220, 140, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(220, 120, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(220, 100, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(220, 80, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(220, 60, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(220, 40, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(220, 20, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(200, 20, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(180, 20, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(160, 20, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(140, 20, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_FillCircle(138, 24, 4);
  BSP_LCD_FillCircle(138, 16, 4);
  BSP_LCD_SetTextColor(LCD_COLOR_RED);
  BSP_LCD_FillRect(114, 20, 15, 3);

  char completed[20];
  snprintf(completed, 200, "CONGRATULATIONS!");
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_GREEN);
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_DisplayStringAt(17, 80, (uint8_t *)completed, LEFT_MODE);
  char stanje[20];
  int score;
  if (vrijeme2 == 0) {
    score = 100;
  } else {
    score = 75 + 200 / vrijeme2;
  }
  if (score > 100) {
    score = 100;
  }
  
  //Ispis vremena i bodova
  if (vrijeme2 <= 9) {
    if (vrijeme1 <= 9)
      snprintf(stanje, 200, "Time: %d%d:%d%d, Score: %d", 0, vrijeme2, 0,
               vrijeme1, score);
    else
      snprintf(stanje, 200, "Time: %d%d:%d, Score: %d", 0, vrijeme2, vrijeme1,
               score);
  } else {
    if (vrijeme1 <= 9)
      snprintf(stanje, 200, "Time: %d:%d%d, Score: %d", vrijeme2, 0, vrijeme1,
               score);
    else
      snprintf(stanje, 200, "Time: %d%d:%d, Score: %d", 0, vrijeme2, vrijeme1,
               score);
  }
  
  //Ispis opcije New Game
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_GREEN);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(25, 100, (uint8_t *)stanje, LEFT_MODE);
  char restart[100];
  snprintf(restart, 200, "NEW GAME", 0, 0, 0, 0, 100);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetFont(&Font20);
  BSP_LCD_DisplayStringAt(50, 120, (uint8_t *)restart, LEFT_MODE);

  zazuti = false;
  ucitavanje = false;
  while (1) {
    if (zazuti) {
      zazuti = true;
      char start[20];
      snprintf(start, 200, "NEW GAME");
      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
      BSP_LCD_SetBackColor(LCD_COLOR_YELLOW);
      BSP_LCD_SetFont(&Font20);
      BSP_LCD_DisplayStringAt(50, 120, (uint8_t *)start, LEFT_MODE);
      ucitavanje = true;
      wait(1);
      break;
    }
    wait(0.5);
  }

  int brojac = 0;
  int x1 = 95, y1, x2, y2;
  int brojac_vremena = 0;
  
  //Loading ekran
  while (1) {
    if (ucitavanje) {
      BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
      BSP_LCD_FillRect(00, 00, 240, 240);
      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
      BSP_LCD_DrawRect(00, 00, 240, 240);

      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
      BSP_LCD_FillRect(05, 05, 230, 230);
      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
      BSP_LCD_DrawRect(05, 05, 230, 230);

      char loading[20];
      snprintf(loading, 200, "Loading");
      BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
      BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
      BSP_LCD_SetFont(&Font20);
      BSP_LCD_DisplayStringAt(70, 82, (uint8_t *)loading, LEFT_MODE);

      ucitavanje = false;
    }

    if (brojac != 0) {
      BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
      BSP_LCD_FillCircle(x1, y1, 4);
    }
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    x2 = x1 + 8;
    y2 = 110;
    BSP_LCD_FillCircle(x2, y2, 4);
    x1 = x2;
    y1 = y2;
    brojac++;
    if (brojac == 7) {
      startaj = true;
      break;
    }
    wait(0.5);
  }
}

//Funkcija koja se aktivira kada zmija ujede samu sebe
void fail() {
  startaj = false;
  
  //Izgled ekrana
  BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
  BSP_LCD_FillRect(00, 00, 240, 240);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_DrawRect(00, 00, 240, 240);

  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_FillRect(05, 05, 230, 230);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_DrawRect(05, 05, 230, 230);

  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(20, 220, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(40, 220, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(60, 220, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(80, 220, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(100, 220, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(120, 220, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(140, 220, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_FillCircle(142, 215, 4);
  BSP_LCD_FillCircle(142, 225, 4);
  BSP_LCD_SetTextColor(LCD_COLOR_RED);
  BSP_LCD_FillRect(150, 218, 15, 3);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(220, 140, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(220, 120, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(220, 100, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(220, 80, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(220, 60, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(220, 40, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(220, 20, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(200, 20, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(180, 20, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(160, 20, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillCircle(140, 20, 10);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_FillCircle(138, 24, 4);
  BSP_LCD_FillCircle(138, 16, 4);
  BSP_LCD_SetTextColor(LCD_COLOR_RED);
  BSP_LCD_FillRect(114, 20, 15, 3);
  
  //Ispis opcije Restart
  char restart[20];
  snprintf(restart, 200, "RESTART");
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetFont(&Font20);
  BSP_LCD_DisplayStringAt(60, 120, (uint8_t *)restart, LEFT_MODE);
  char fail[20];
  snprintf(fail, 200, "FAIL!");
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_RED);
  BSP_LCD_SetFont(&Font20);
  BSP_LCD_DisplayStringAt(70, 90, (uint8_t *)fail, LEFT_MODE);

  zazuti = false;
  ucitavanje = false;
  while (1) {
    if (zazuti) {
      zazuti = true;
      char start[20];
      snprintf(start, 200, "RESTART");
      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
      BSP_LCD_SetBackColor(LCD_COLOR_YELLOW);
      BSP_LCD_SetFont(&Font20);
      BSP_LCD_DisplayStringAt(60, 120, (uint8_t *)start, LEFT_MODE);
      ucitavanje = true;
      wait(1);
      break;
    }
    wait(0.5);
  }

  int brojac = 0;
  int x1 = 95, y1, x2, y2;
  int brojac_vremena = 0;
  
  //Loading ekran
  while (1) {
    if (ucitavanje) {
      BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
      BSP_LCD_FillRect(00, 00, 240, 240);
      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
      BSP_LCD_DrawRect(00, 00, 240, 240);

      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
      BSP_LCD_FillRect(05, 05, 230, 230);
      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
      BSP_LCD_DrawRect(05, 05, 230, 230);

      char loading[20];
      snprintf(loading, 200, "Loading");
      BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
      BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
      BSP_LCD_SetFont(&Font20);
      BSP_LCD_DisplayStringAt(70, 82, (uint8_t *)loading, LEFT_MODE);

      ucitavanje = false;
    }

    if (brojac != 0) {
      BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
      BSP_LCD_FillCircle(x1, y1, 4);
    }
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    x2 = x1 + 8;
    y2 = 110;
    BSP_LCD_FillCircle(x2, y2, 4);
    x1 = x2;
    y1 = y2;
    brojac++;
    if (brojac == 7) {
      startaj = true;
      break;
    }
    wait(0.5);
  }
}

//Main funkcija
int main() {
  gore.rise(&funGore);
  dolje.rise(&funDolje);
  lijevo.rise(&funLijevo);
  desno.rise(&funDesno);
  sredina.rise(&funSredina);
  while (1) {
    startaj = false;
    
    //Izgled početnog ekrana
    BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
    BSP_LCD_FillRect(00, 00, 240, 240);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_DrawRect(00, 00, 240, 240);

    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_FillRect(05, 05, 230, 230);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_DrawRect(05, 05, 230, 230);

    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_FillCircle(20, 220, 10);
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_FillCircle(40, 220, 10);
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_FillCircle(60, 220, 10);
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_FillCircle(60, 200, 10);
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_FillCircle(60, 180, 10);
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_FillCircle(80, 180, 10);
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_FillCircle(100, 180, 10);
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_FillCircle(100, 160, 10);
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_FillCircle(118, 150, 10);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_FillCircle(122, 154, 4);
    BSP_LCD_FillCircle(122, 146, 4);
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_FillRect(130, 149, 15, 3);
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_FillCircle(220, 140, 10);
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_FillCircle(220, 120, 10);
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_FillCircle(220, 100, 10);
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_FillCircle(220, 80, 10);
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_FillCircle(220, 60, 10);
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_FillCircle(220, 40, 10);
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_FillCircle(220, 20, 10);
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_FillCircle(200, 20, 10);
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_FillCircle(180, 20, 10);
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_FillCircle(160, 20, 10);
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_FillCircle(140, 20, 10);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_FillCircle(138, 24, 4);
    BSP_LCD_FillCircle(138, 16, 4);
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_FillRect(114, 20, 15, 3);

    char start[20];
    snprintf(start, 200, "START");
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
    BSP_LCD_SetFont(&Font20);
    BSP_LCD_DisplayStringAt(70, 82, (uint8_t *)start, LEFT_MODE);
    char snake[20];
    snprintf(snake, 200, "SNAKE GAME");
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_SetBackColor(LCD_COLOR_GREEN);
    BSP_LCD_SetFont(&Font20);
    BSP_LCD_DisplayStringAt(40, 60, (uint8_t *)snake, LEFT_MODE);

    ucitavanje = false;
    while (1) {
      if (zazuti) {
        zazuti = true;
        char start[20];
        snprintf(start, 200, "START");
        BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
        BSP_LCD_SetBackColor(LCD_COLOR_YELLOW);
        BSP_LCD_SetFont(&Font20);
        BSP_LCD_DisplayStringAt(70, 82, (uint8_t *)start, LEFT_MODE);
        ucitavanje = true;
        wait(0.1);
        break;
      }
      wait(0.5);
    }

    int brojac = 0;
    int x1 = 95, y1, x2, y2;
    int brojac_vremena = 0;
    
    //Loading ekran
    while (1) {
      if (ucitavanje) {
        BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
        BSP_LCD_FillRect(00, 00, 240, 240);
        BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
        BSP_LCD_DrawRect(00, 00, 240, 240);

        BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
        BSP_LCD_FillRect(05, 05, 230, 230);
        BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
        BSP_LCD_DrawRect(05, 05, 230, 230);

        char loading[20];
        snprintf(loading, 200, "Loading");
        BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
        BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
        BSP_LCD_SetFont(&Font20);
        BSP_LCD_DisplayStringAt(70, 82, (uint8_t *)loading, LEFT_MODE);

        ucitavanje = false;
      }

      if (brojac != 0) {
        BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
        BSP_LCD_FillCircle(x1, y1, 4);
      }
      BSP_LCD_SetTextColor(LCD_COLOR_RED);
      x2 = x1 + 8;
      y2 = 110;
      BSP_LCD_FillCircle(x2, y2, 4);
      x1 = x2;
      y1 = y2;
      brojac++;
      if (brojac == 7) {
        startaj = true;
        break;
      }
      wait(0.5);
    }
    
    //Ekran Zmija lovi miša
    if (startaj) {
      BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
      BSP_LCD_FillRect(00, 00, 240, 240);
      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
      BSP_LCD_DrawRect(00, 00, 240, 240);

      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
      BSP_LCD_FillRect(05, 05, 230, 230);
      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
      BSP_LCD_DrawRect(05, 05, 230, 230);

      int x1 = 10, y1 = 230, x2, y2, points = 0, velicina_zmije = 2;
      int x[] = {18,  26,  34,  42,  50,  58,  66,  74,  82,
                 90,  98,  106, 114, 122, 130, 138, 146, 154,
                 162, 170, 178, 186, 194, 202, 210, 218, 226};
      int y[] = {230, 222, 214, 206, 198, 190, 182, 174, 166,
                 158, 150, 142, 134, 126, 118, 110, 102, 94,
                 86,  78,  70,  62,  54,  46,  38,  30,  22};
      int randomx, randomy;
      randomx = rand() % 26;
      randomy = rand() % 26;
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
      BSP_LCD_FillCircle(x[randomx], y[randomy], 4);
      int temp = 1;
      double T = 0.5;
      std::vector<int> vX;
      std::vector<int> vY;
      int points2 = 0;

      while (1) {
        if (points == velicina_zmije) {
          randomx = rand() % 26;
          randomy = rand() % 26;
          BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
          BSP_LCD_FillCircle(x[randomx], y[randomy], 4);
          velicina_zmije++;
        }
        char score[20];
        if (points2 == 0) {
          snprintf(score, 100, "Score: %d", points2);
        } else if (temp == 2 || temp == 3) {
          snprintf(score, 100, "Score: %d", points2);
        } else {
          snprintf(score, 100, "Score: %d", points2 - 1);
        }
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
        BSP_LCD_SetFont(&Font8);
        BSP_LCD_DisplayStringAt(190, 10, (uint8_t *)score, LEFT_MODE);
        
        //Računanje vremena
        brojac_vremena++;
        if (brojac_vremena % 2 == 0 && T == 0.5 ||
            brojac_vremena % 4 == 0 && T == 0.25) {
          vrijeme1 = vrijeme1 + 1;
          if (vrijeme1 == 60) {
            vrijeme1 = 0;
            vrijeme2 += 1;
          }
          brojac_vremena = 0;
        }
        char vrijeme[20];
        if (vrijeme1 >= 0 && vrijeme1 < 10 && vrijeme2 >= 0 && vrijeme2 < 10)
          snprintf(vrijeme, 100, "0%d:0%d", vrijeme2, vrijeme1);
        if (vrijeme1 >= 10 && vrijeme2 >= 0 && vrijeme2 < 10)
          snprintf(vrijeme, 100, "0%d:%d", vrijeme2, vrijeme1);
        if (vrijeme1 >= 0 && vrijeme1 < 10 && vrijeme2 >= 0 && vrijeme2 >= 10)
          snprintf(vrijeme, 100, "%d:0%d", vrijeme2, vrijeme1);
        if (vrijeme1 >= 10 && vrijeme2 >= 10)
          snprintf(vrijeme, 100, "%d:%d", vrijeme2, vrijeme1);
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
        BSP_LCD_SetFont(&Font8);
        BSP_LCD_DisplayStringAt(205, 10, (uint8_t *)vrijeme, RIGHT_MODE);
        
        //Početno kretanje zmije
        if (pocetak) {
          jeLiPocelo = true;
          BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
          BSP_LCD_FillCircle(x1, y1, 4);
          BSP_LCD_SetTextColor(LCD_COLOR_RED);
          if (x2 == 226)
            x1 = 10;
          x2 = x1 + 8;
          y2 = 230;
          BSP_LCD_FillCircle(x2, y2, 4);
          x1 = x2;
          y1 = y2;
          vX.push_back(x2);
          vY.push_back(y2);
        }
        
        //Kretanje zmije desno
        if (de) {
          BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
          BSP_LCD_FillCircle(x1, y1, 4);
          BSP_LCD_SetTextColor(LCD_COLOR_RED);
          if (x2 == 226)
            x1 = 10;
          x2 = x1 + 8;
          y2 = y1;
          BSP_LCD_FillCircle(x2, y2, 4);
          x1 = x2;
          y1 = y2;
          vX.push_back(x2);
          vY.push_back(y2);
        }
        
        //Kretanje zmije lijevo
        if (li) {
          BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
          BSP_LCD_FillCircle(x1, y1, 4);
          BSP_LCD_SetTextColor(LCD_COLOR_RED);
          if (x2 == 18)
            x1 = 226;
          x2 = x1 - 8;
          y2 = y1;
          BSP_LCD_FillCircle(x2, y2, 4);
          x1 = x2;
          y1 = y2;
          vX.push_back(x2);
          vY.push_back(y2);
        }
        
        //Kretanje zmije dolje
        if (d) {
          BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
          BSP_LCD_FillCircle(x1, y1, 4);
          BSP_LCD_SetTextColor(LCD_COLOR_RED);
          if (y2 == 230)
            y1 = 22;
          x2 = x1;
          y2 = y1 + 8;
          BSP_LCD_FillCircle(x2, y2, 4);
          x1 = x2;
          y1 = y2;
          vX.push_back(x2);
          vY.push_back(y2);
        }
        
        //Kretanje zmije gore
        if (g) {
          BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
          BSP_LCD_FillCircle(x1, y1, 4);
          BSP_LCD_SetTextColor(LCD_COLOR_RED);
          if (y2 == 22)
            y1 = 238;
          x2 = x1;
          y2 = y1 - 8;
          BSP_LCD_FillCircle(x2, y2, 4);
          x1 = x2;
          y1 = y2;
          vX.push_back(x2);
          vY.push_back(y2);
        }
        
        //Zmija je pojela miša
        if (x2 == x[randomx] && y2 == y[randomy]) {
          if (points == 0) {
            points = points + 2;
            points2 = points2 + 2;
          } else {
            points++;
            points2++;
          }
        }
        
        //Rep zmije
        if (vX.size() > 1) {
          int temp1, temp2, temp5;
          if (vX.size() - 1 - points < 0) {
            temp5 = 0;
          } else {
            temp5 = vX.size() - 1 - points;
          }
          for (int i = vX.size() - 2; i >= temp5; i--) {
            if (i == -1) {
              break;
            }
            BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
            BSP_LCD_FillCircle(vX.at(i), vY.at(i), 4);
            temp1 = vX.at(i);
            temp2 = vY.at(i);
            
            //Zmija je ujela samu sebe
            if (x2 == vX.at(i) && y2 == vY.at(i)) {
              de = false;
              li = false;
              d = false;
              g = false;
              jeLiPocelo = false;
              fail();
              BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
              BSP_LCD_FillRect(00, 00, 240, 240);
              BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
              BSP_LCD_DrawRect(00, 00, 240, 240);

              BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
              BSP_LCD_FillRect(05, 05, 230, 230);
              BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
              BSP_LCD_DrawRect(05, 05, 230, 230);

              x1 = 10, y1 = 230, x2, y2, points = 0, velicina_zmije = 2;
              points2 = 0;
              randomx = rand() % 26;
              randomy = rand() % 26;
              BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
              BSP_LCD_FillCircle(x[randomx], y[randomy], 4);
              pocetak = true;
              vrijeme1 = 0;
              vrijeme2 = 0;
              temp = 1;
              T = 0.5;
              break;
            }
          }
          BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
          BSP_LCD_FillCircle(temp1, temp2, 4);
        }
        
        //Promjena brzine kretanja zmije na levelu 2
        if (temp == 2 && points == 11) {
          T = 0.25;
        }
        
        //Završetak levela 1
        if (points == 11 && temp == 1) {
          vX.clear();
          vY.clear();
          de = false;
          li = false;
          d = false;
          g = false;
          jeLiPocelo = false;
          level1Complete();
          if (nextlvl) {
            points = 5;
            velicina_zmije = 6;
            temp++;
          } else if (res) {
            points = 0;
            velicina_zmije = 2;
            temp = 1;
            vrijeme1 = 0;
            vrijeme2 = 0;
          }
          BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
          BSP_LCD_FillRect(00, 00, 240, 240);
          BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
          BSP_LCD_DrawRect(00, 00, 240, 240);

          BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
          BSP_LCD_FillRect(05, 05, 230, 230);
          BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
          BSP_LCD_DrawRect(05, 05, 230, 230);

          x1 = 10, y1 = 230, x2, y2;
          points2 = 0;
          randomx = rand() % 26;
          randomy = rand() % 26;
          BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
          BSP_LCD_FillCircle(x[randomx], y[randomy], 4);
          pocetak = true;
        }
        
        //Završetak levela 2
        if (temp == 2 && points == 21) {
          vX.clear();
          vY.clear();
          de = false;
          li = false;
          d = false;
          g = false;
          jeLiPocelo = false;
          level2Complete();
          if (nextlvl) {
            points = 10;
            velicina_zmije = 11;
            temp++;
          } else if (res) {
            points = 0;
            velicina_zmije = 2;
            temp = 1;
            vrijeme1 = 0;
            vrijeme2 = 0;
          }
          BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
          BSP_LCD_FillRect(00, 00, 240, 240);
          BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
          BSP_LCD_DrawRect(00, 00, 240, 240);

          BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
          BSP_LCD_FillRect(05, 05, 230, 230);
          BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
          BSP_LCD_DrawRect(05, 05, 230, 230);

          x1 = 10, y1 = 230, x2, y2;
          points2 = 0;
          randomx = rand() % 26;
          randomy = rand() % 26;
          BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
          BSP_LCD_FillCircle(x[randomx], y[randomy], 4);
          pocetak = true;
        }
        
        //Završetak levela 3
        if (temp == 3 && points == 31) {
          de = false;
          li = false;
          d = false;
          g = false;
          jeLiPocelo = false;
          level3Complete();
          BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
          BSP_LCD_FillRect(00, 00, 240, 240);
          BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
          BSP_LCD_DrawRect(00, 00, 240, 240);

          BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
          BSP_LCD_FillRect(05, 05, 230, 230);
          BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
          BSP_LCD_DrawRect(05, 05, 230, 230);

          x1 = 10, y1 = 230, x2, y2, points = 0, velicina_zmije = 2;
          points2 = 0;
          randomx = rand() % 26;
          randomy = rand() % 26;
          BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
          BSP_LCD_FillCircle(x[randomx], y[randomy], 4);
          pocetak = true;
          vrijeme1 = 0;
          vrijeme2 = 0;
          temp = 1;
          T = 0.5;
        }
        wait(T);
      }
    }
  }
}