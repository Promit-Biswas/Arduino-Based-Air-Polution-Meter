#include <LiquidCrystal.h>
#include <MQ2.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>


#define DHTPIN 8    
#define DHTTYPE DHT11
#define buzz 13

const int m2 = A2; 
const int mq5 = A5; 
const int mq135 = A1;



const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
DHT dht(DHTPIN, DHTTYPE);
MQ2 mq2(m2);

int gas, co2lvl;

byte smiley[8] = {
  0b00000,
  0b01010,
  0b01010,
  0b00000,
  0b00000,
  0b10001,
  0b01110,
  0b00000
};

byte sad[8] = {
  0b00000,
  0b01010,
  0b01010,
  0b00000,
  0b00000,
  0b01110,
  0b10001,
  0b00000
};

void setup() {
  pinMode(buzz, OUTPUT);
  pinMode(mq135, INPUT); 
  pinMode(mq5, INPUT); 
  pinMode(m2, INPUT); 

  Serial.begin(9600);
  dht.begin();
  mq2.begin();
  lcd.begin(16, 2);                       
  lcd.setCursor (0,0);
  lcd.print("Pollution Meter");
  delay(4000);
  lcd.clear();
  lcd.print("Warming coil");

  for(int i =0;i<=100;i++)
  {
    lcd.setCursor(12,0);
    lcd.print(" ");
    lcd.print(i);
    lcd.print("%");
    delay(700);
  }
  lcd.createChar(0, smiley);
  lcd.createChar(1, sad);
}




void display_lcd(double mq_value, int mid, int max) {

  if((mq_value >=0)&&(mq_value <= mid))
  {
    lcd.print("  Good ");
    lcd.write(byte(0));
    digitalWrite(buzz,LOW);
  }

  else if((mq_value >= mid)&&(mq_value <= max))
  {
    digitalWrite(buzz,HIGH);
    delay(300);
    digitalWrite(buzz, LOW);
    lcd.print("  Bad ");
    lcd.write(byte(1));
    
  }
  else if(mq_value > max)
  {
    lcd.print(" Danger!");
    digitalWrite(buzz,HIGH);
  }
  delay(3000);
  lcd.clear();
}



void loop() {
  
  double butane_value = mq2.readLPG();
  double co_value = mq2.readCO();
  double co2_value = mq2.readSmoke();
  double hum_value = dht.readHumidity();
  double temp_value = dht.readTemperature();
  double aqi_value = analogRead(mq135);
  double methane_value = analogRead(mq5);



  lcd.setCursor (0,0);
  lcd.print("Air Qlty Index:");
  lcd.setCursor(0,1);
  lcd.print(aqi_value);
  lcd.print(" ppm");

  display_lcd(aqi_value, 150, 200);
  


  lcd.setCursor (0,0);
  lcd.print("CO Level:");
  lcd.setCursor(0,1);
  lcd.print(co_value);
  lcd.print(" ppm");

  co_value = co_value - 60;
  
  display_lcd(co_value, 100, 300);



  lcd.setCursor (0,0);
  lcd.print("CO2 Level:");
  lcd.setCursor(0,1);
  lcd.print(co2_value);
  lcd.print(" ppm");
  
  display_lcd(co2_value, 1000, 1500);



  lcd.setCursor (0,0);
  lcd.print("Butane Level:");
  lcd.setCursor(0,1);
  lcd.print(butane_value);
  lcd.print(" ppm");

  display_lcd(butane_value, 800, 1500);



  lcd.setCursor (0,0);
  lcd.print("Methane Level:");
  lcd.setCursor(0,1);
  lcd.print(methane_value);
  lcd.print(" ppm");

  display_lcd(methane_value, 1000, 1500);


  

  lcd.setCursor (0,0);
  lcd.print("Temperature:");
  lcd.setCursor(0,1);
  lcd.print(temp_value);
  lcd.print(" C");
  delay(3000);
  lcd.clear();

  lcd.setCursor (0,0);
  lcd.print("Humidity Level:");
  lcd.setCursor(0,1);
  lcd.print(hum_value);
  lcd.print(" %");
  delay(3000);
  lcd.clear();
  
}                        
