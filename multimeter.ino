#include <LiquidCrystal_I2C.h>
#include <Wire.h>

/////////////////////voltage meter///
int analogInput = 0; //A0 pin of arduino
float vout = 0.0;
float vin = 0.0;
float R1 = 100000.0; // resistance of R1 (100K)
float R2 = 10000.0; // resistance of R2 (10K)
int value = 0;

///////////////////Ohm meter///////////
int analogPin = 1; //A1 pin of arduino
int raw = 0;
int Vin = 5;
float Vout = 0;
float R1_1 = 10000; // 10K
float R2_2 = 0; // this is the resistor which need to calculate, means put any resistor which you need to calculate resistance 
float buffer = 0;


/////////////////Capicitance meter/////
int analogPin1 = A2; // A2 pin of arduino
int charge = 8;
int discharge = 9;
unsigned long start_timer=0;
unsigned long stop_timer=0;
unsigned long duration=0;

float voltage=3;
float answer=0.0;
int measure (void);
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

byte Right_arrow[] = { // it draw right arrow on the lcd screen
  B00000,
  B00100,
  B00010,
  B11111,
  B00010,
  B00100,
  B00000,
  B00000
};

byte Left_arrow[] = {  // it draw left arrow on the lcd screen
  B00000,
  B00100,
  B01000,
  B11111,
  B01000,
  B00100,
  B00000,
  B00000
};

void setup(){
   Serial.begin(9600);
   pinMode(analogInput, INPUT);
   pinMode(analogPin, INPUT);

   pinMode(discharge,INPUT);
   pinMode(charge,OUTPUT);
   digitalWrite(charge,HIGH);
   lcd.init();  // initialize the lcd 
   lcd.backlight(); // turn on the back light of lcd screen
   lcd.setCursor(3, 0);
   lcd.print("MULTIMETER");
   lcd.createChar(1, Right_arrow);
   lcd.createChar(2, Left_arrow);
   lcd.setCursor(1, 0);
   lcd.write((byte)1); // it print Right_arrow on lcd screen
   lcd.setCursor(14, 0);
   lcd.write((byte)2); //it print Left_arrow on lcd screen
   delay(2000);
   lcd.clear();
}

void loop()
{
 
 Capacitor_meter();
}

void Capacitor_meter()
{
    delay(1000);
    lcd.setCursor(8,0);
    lcd.print("C=");
    lcd.print("0.0uf ");
    while(1)
    {
      Ohm_meter();
      Voltage_meter(); 
      pinMode(charge,INPUT);
      pinMode(discharge,OUTPUT);
      digitalWrite(discharge,LOW);
      
      while(voltage>2.0)
      {
       voltage=measure();
       delay(1000);
       answer=voltage * (99.0 / 1023.0);

      }
      delay(800);
      
      pinMode(discharge,INPUT);
      pinMode(charge,OUTPUT);
      digitalWrite(charge,HIGH);
      start_timer=micros();
      
      while(measure()<648)
      {
        measure()*(100.0/1023.0);
      }
      
      stop_timer=micros();
      duration=stop_timer-start_timer;
      answer=duration/10000;
  
      lcd.setCursor(8,0);
      lcd.print("C=");
      lcd.print(answer,1);
      lcd.print("uf "); 
      delay(2000);
      
    }
}


int measure (void)
{
  int value;
  value=analogRead(analogPin1);
  return value;
}


void Ohm_meter()
{
  raw = analogRead(analogPin);
  if(raw)
  {
    buffer = raw * Vin;
    Vout = (buffer)/1024.0;
    buffer = (Vin/Vout) - 1;
    R2_2= (R1_1 * buffer)/1000;
    Serial.print("Vout: ");
    Serial.println(Vout);
    Serial.print("R2: ");
    Serial.println(R2_2);
    lcd.setCursor(0,1);
    lcd.print("R=");
    lcd.print(R2_2);
    lcd.print("K     ");
    delay(500);
  }
}


void Voltage_meter()
{
   // read the value at analog input
   value = analogRead(analogInput);
   vout = (value * 5.0) / 1024.0; // see text
   vin = vout / (R2/(R1+R2)); 
   if (vin<0.09) 
   {
     vin=0.0;//statement to quash undesired reading !
   } 
   lcd.setCursor(0,0);
   lcd.print("V=");
   lcd.print(vin,1);
   delay(500);
}
