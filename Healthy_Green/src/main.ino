


/**************************************/

bool SERIAL_MONITOR = false;

/**************************************/


#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);


// Ports
const int Moisture_Sensor = A0;

// const int Dry_Soil_Moisture_Percentage = 35 ;           // 70% is good soil  20% and below is bad

const int Good_Value = 437;

const int Dry_Value = 484;
const int Just_watered = 360;

const int Air_Value = 670;                              
const int Water_Value = 350;

int Moisture_Percentage;
int Moisture_Value, Prev_Moisture_Value;


/**********************************************************/

int Calculate_Moisture(  int Value ) {

  int Percentage;
  Percentage = map(Value, Dry_Value, Just_watered, 0, 100);
  
  //Moisture_Percentage -= 10; // cause of the power shortage from supplying lcd display
  return Percentage;
}

int Calculate_Moist_Value( int Check_times, int delay_time ){
  
  int Value =0;
  for (int i = 0; i <= Check_times; i++) {
    Value += analogRead(Moisture_Sensor);
    delay(delay_time);
  }
  Value /= Check_times;
  
  return Value;
}

void LcdPrint( int Percentage , int Value ){
  lcd.clear();
  lcd.print("Moisture ");
  lcd.print(Percentage);
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print("Value: ");
  lcd.print(Value);
}


/**********************************************************/

void setup(){
  lcd.init();
  lcd.backlight();

  if (SERIAL_MONITOR)
    Serial.begin(9600);
}

void loop() {

  Moisture_Value = Calculate_Moist_Value( 20, 100 );

  if ( Moisture_Value != Prev_Moisture_Value ){

      Moisture_Percentage = Calculate_Moisture( Moisture_Value );
      LcdPrint( Moisture_Percentage, Moisture_Value );
      
      Prev_Moisture_Value = Moisture_Value;
  }

  if (SERIAL_MONITOR){
    Serial.print("Value: ");
    Serial.print(Moisture_Value);
    Serial.print("\t Percentage: ");
    Serial.println(Moisture_Percentage);
  }
}
