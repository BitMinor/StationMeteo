#include<LiquidCrystal.h> // import module pour lcd
#include <Wire.h>
#include <DS3231.h> // import module  pour horloge
#include "DHT.h"// import temp + humi
#include <Adafruit_BMP085.h> //press



#define A 6
#define DHTPIN 2   // Digital pin connected to the sDHT sensor temp + hum
#define DHTTYPE DHT11   // température et humidité


String infoSerial = "";

int screen = 1; // n° page lcd
int bgPin = 9; //pin rétroeclairage lcd

float t = 0; //valeur température
int h = 0; // valeur humidité

int prev_hour = 0;
int prev_min = 0;
int n_tab = 0;
String prevision = "";
long int tab_pression[A] {};
int z = 0;
long int moyenne_press = 0;
long int pression = 0;

int BtnHour = 10;
int state_BtnHour = false;
int BtnTemp = 11;
int state_BtnTemp = false;
int BtnPress = 12;
int state_BtnPress = false;
int BtnPrev = 13;
int state_BtnPrev = false;

LiquidCrystal lcd(3, 4, 5, 6, 7, 8); //décalaration pin lcd
RTClib myRTC; //clock
DHT dht(DHTPIN, DHTTYPE); // température + humumidité
Adafruit_BMP085 bmp; //pression

// création des caractères spéciaux pour l'afficheur lcd
byte temp[] = {
  B01110,
  B01010,
  B01010,
  B01110,
  B11111,
  B11111,
  B11111,
  B01110
};
byte hydro[] = {
  B00100,
  B01110,
  B01110,
  B11111,
  B11111,
  B11111,
  B01110,
  B00000
};
byte cel[] = {
  B00110,
  B01001,
  B01001,
  B00110,
  B00000,
  B00000,
  B00000,
  B00000
};
byte hours[] = {
  B00000,
  B01110,
  B10101,
  B10111,
  B10001,
  B01110,
  B00000,
  B00000
};
byte day[] = {
  B00000,
  B11111,
  B11111,
  B10001,
  B10101,
  B10001,
  B11111,
  B00000
};



void setup() {
  
  Serial.begin(9600);

  dht.begin();
  bmp.begin();
  if (!bmp.begin()) {
    Serial.println("[+] !Could not find a valid BMP085 sensor, check wiring! [+]");
    while (1) {}
  }

  pinMode(BtnHour, INPUT_PULLUP);
  pinMode(BtnTemp, INPUT_PULLUP);
  pinMode(BtnPress, INPUT_PULLUP);
  pinMode(BtnPrev, INPUT_PULLUP);
  
  lcd.createChar(0, temp);
  lcd.createChar(1, hydro);
  lcd.createChar(2, cel);
  lcd.createChar(3, hours);
  lcd.createChar(4, day);
  lcd.clear();

  Wire.begin();

  pinMode(bgPin, OUTPUT);
  digitalWrite(bgPin, HIGH);
  lcd.begin(16, 2); // setup lcd nbr digit
  lcd.setCursor(4, 0);
  lcd.print("init ..."); // setup lcd text default
  lcd.setCursor(2, 1);
  lcd.print("Home Station");
  init_prev_min();
  get_press();
  init_tab();


  delay(2000);
  lcd.clear();

}


void loop() {
  read_btn();
  while(Serial.available()){
    infoSerial = Serial.read();
    Serial.print("recived : ");
    Serial.println(infoSerial);
    if (infoSerial == "102"){
      digitalWrite(bgPin, LOW);
      Serial.print("[+] rétro-éclairage désactivé");
    }
    else if (infoSerial == "110"){
      digitalWrite(bgPin, HIGH);
      Serial.print("[+] rétro-éclairage activé");
    }
  }
  switch (screen) {

    case 1: {    //Design of home page 1
        lcd.clear();
        screen_1();
      } 
      break;
    case 2: { //Design of page 2
        lcd.clear();
        screen_2();
      }
      break;

    case 3: {   //Design of page 3
        lcd.clear();
        screen_3();
      }
      break;
    case 4: {   //Design of page 3
        lcd.clear();
        screen_4();
      }
      break;
  }
}
void read_btn(){ // lecture des boutons de séléctions
  state_BtnHour = digitalRead(BtnHour);
  state_BtnTemp = digitalRead(BtnTemp);
  state_BtnPress = digitalRead(BtnPress);
  state_BtnPrev = digitalRead(BtnPrev);
  
  if (state_BtnHour == LOW){
    screen = 1;
  }
  else if (state_BtnTemp == LOW){
    screen = 2;
  }
  else if (state_BtnPress == LOW){
    screen = 3;
  }
  else if (state_BtnPrev == LOW){
    screen = 4;
  }
  
}
void get_press() { // récupération de la valeur de la pression
  pression = bmp.readPressure();
}

void up_press() { // misse a jours de la pression chaque minutes et heures
  DateTime now = myRTC.now();
  if (now.minute() == prev_min + 10) {
    prev_min = now.minute();
    verif_prevision();
  }
  if (now.hour() != prev_hour) {
    prev_hour = now.hour();
    dif_pression();
  }
}
void verif_prevision() {
  tab_pression[z] = pression;
  z ++;
  if (z == 6) {
    z = 0;
  }
}

void dif_pression() { // Calcule de la prévision météo
  moyenne_press = (tab_pression[0] + tab_pression[1] + tab_pression[2] + tab_pression[3] + tab_pression[4] + tab_pression[5] - tab_pression[z]) / 5;
  Serial.print("[+] moyenne :");
  Serial.println(moyenne_press);
  Serial.print("[+] pression :");
  Serial.println(pression);
  
  if (moyenne_press == 0) {

      Serial.println("[+] !error : pression = 0 !");
      prevision = "X";

  }
  else if ((pression - 35) < moyenne_press && moyenne_press < (pression + 35)) {
    Serial.println("[+] Calme");
    prevision = "calme";
    
  }
  else if (moyenne_press < (pression + 35)) {
    Serial.println("[+] Beau temps");
    prevision = "beau temps";
    
  }
  else if (moyenne_press > (pression - 35) && pression <= 101500) {
    Serial.println("[+] Nuage/Pluie");
    prevision = "nuage/pluie";
    
  }
  else if (moyenne_press < (pression + 150) && pression <= 101500) {
    Serial.println("[+] Soleil à court terme");
    prevision = "soleil CT";
    
  }
  else if (moyenne_press > (pression - 150) && pression > 99500) {
    Serial.println("[+] Orage");
    prevision = "orage";
    
  }
  else if (moyenne_press < (pression + 150) && pression >= 101500) {
    Serial.println("[+] Soleil à long terme");
    prevision = "soleil LT";
    
  }
  else if (moyenne_press > (pression - 150) && pression <= 99500) {
    Serial.println("[+] Tempete");
    prevision = "tempête";
    
  }
  else {
    
    Serial.println("[+] non definie");
    prevision = "non définie";
  }

}
void init_prev_min() {
  DateTime now = myRTC.now();
  prev_min = now.minute();
}

void init_tab() { // initialisation tableau avec les valeurs de la pression
  for (int i = 1; i <= 6; i++) {
    tab_pression[i] = pression;
  }
}


void screen_1() { //Design page 1 : affichage Date et heure
  func_loop();
  DateTime now = myRTC.now();
  lcd.setCursor(3, 0);
  lcd.print(now.day(), DEC);
  lcd.print('/');
  lcd.print(now.month(), DEC);
  lcd.print('/');
  lcd.print(now.year(), DEC);
  lcd.setCursor(4, 1);
  lcd.print(now.hour()-1, DEC);
  lcd.print(':');
  lcd.print(now.minute(), DEC);
  lcd.print(':');
  lcd.print(now.second(), DEC);
  delay(1000);
}
void screen_2() { //Design page 2 : affichage température et humidité
  func_loop();
  
  h = dht.readHumidity() - 9;      // read humidity

  t = dht.readTemperature();     // read temperature

  if (isnan(h) || isnan(t)) {
    Serial.println(F("[+] ! Failed to read from DHT sensor !"));
    return;
  }
  float hic = dht.computeHeatIndex(t, h, false);

  lcd.setCursor(4, 0);
  lcd.write(byte(0));
  lcd.setCursor(5, 0);
  lcd.print(t);
  lcd.setCursor(9, 0);
  lcd.write(byte(2));
  lcd.setCursor(10, 0);
  lcd.print("C");
  lcd.setCursor(5, 1);
  lcd.write(byte(1));
  lcd.setCursor(6, 1);
  lcd.print(h);
  lcd.setCursor(8, 1);
  lcd.print("%");

  delay(1500);
}

void screen_3() { //Design page 3 : affichage pression et prévision
  func_loop();
  lcd.setCursor(0, 0);
  lcd.print("press:");
  lcd.setCursor(6, 0);
  lcd.print(pression / 100);
  lcd.setCursor(11, 0);
  lcd.print("HPa");
  lcd.setCursor(0, 1);
  lcd.print("prev:");
  lcd.setCursor(5, 1);
  lcd.print(prevision);

  delay(1000);
}
void screen_4() { //Design page 4 : moyenne de la pression atmosphérique
  func_loop();
  lcd.setCursor(0, 0);
  lcd.print("moyenne: ");
  lcd.print(moyenne_press);
  delay(1000);

}

void func_loop() {
  get_press();
  up_press();
  read_btn();
}
