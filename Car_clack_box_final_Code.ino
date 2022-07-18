#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <Wire.h>

LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
SoftwareSerial Bluetooth(2, 3); // RX, TX
/****************************************************************/
   int acc   = 1;
   int alch  = 1;
   int flame = 1;
   int pt,ot,dt=0;
   int alert =0;
   int mov =0;
/****************************************************************/
  void get_Sensor_data();
  void Timer();

  void stop_car();
  void Forward_car();
  void Reverse_car();
  void Left_car();
  void Right_car();
  void bt_get_data();
  void Serial_re();
/****************************************************************/
void setup() 
{
  Serial.begin(9600);
  Bluetooth.begin(9600);
  
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  
  lcd.begin(16, 2);
  lcd.print(" Accident Alert");
  lcd.setCursor(0, 1);
  lcd.print("    System");
  
  pinMode(A2,INPUT);
  pinMode(A1,INPUT);
  pinMode(A0,INPUT);
  pinMode(7,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(4,OUTPUT);
  stop_car();
  pt=ot=millis()/100;
}
/****************************************************************/
void loop()
{  
  get_Sensor_data();
  Timer();
  bt_get_data();
  Serial_re();
  delay(100);
}
/****************************************************************/
/****************************************************************/
   void get_Sensor_data()
    {
      int a,b,c;
      a = digitalRead(A0);
      delay(25);
      b  = digitalRead(A1);
      delay(25);
      c = digitalRead(A2);
      delay(25);

      if(a == 0)
      {
        acc = 0;
        dt=0;
        stop_car();
        alert = 1;
      }
      if(b == 0)
      {
        alch = 0;
        dt=1;
        stop_car();
        alert = 2;
      }
      if(c == 0)
      {
        flame = 0;
        dt=0;
        stop_car();
        alert = 3;
      }
      
      if(acc ==0 && flame ==0)
      {
        dt=0;
        stop_car();
        alert = 4;
      }
      if(acc ==0 && alch ==0)
      {
        dt=0;
        stop_car();
        alert = 5;
      }
      if(acc ==0 && flame ==0 && alch ==0)
      {
        dt=0;
        stop_car();
        alert = 6;
      }
      
    }
/****************************************************************/
   void Timer()
    {
      pt=millis()/100;
      if(pt-ot>9)
      {
        ot=pt;
            dt++;
            if(dt==1)
            {
                lcd.begin(16, 2);
                lcd.print("Accident ");
                    if(acc == 0)
                    {
                      lcd.print("Alert!");
                    }
                    else
                    {
                      lcd.print("Safe");
                    }
                lcd.setCursor(0, 1);
                lcd.print("Flame    ");
                    if(flame == 0)
                    {
                      lcd.print("Alert!");
                    }
                    else
                    {
                      lcd.print("Safe");
                    }
            }
            else if(dt==2)
            {
                lcd.begin(16, 2);
                lcd.print("Alcohol  ");
                    if(alch == 0)
                    {
                      lcd.print("Alert!");
                    }
                    else
                    {
                      lcd.print("Safe");
                    }
                lcd.setCursor(0, 1);
                lcd.print("Alert    ");
                    if(alert == 0)
                    {
                      lcd.print("Safe");
                    }
                    else if(alert == 1)
                    {
                      lcd.print("Accident");
                    }
                    else if(alert == 2)
                    {
                      lcd.print("Alcohol");
                    }
                    else if(alert == 3)
                    {
                      lcd.print("Flame");
                    }
                    else if(alert == 4)
                    {
                      lcd.print("Acc FL");
                    }
                    else if(alert == 5)
                    {
                      lcd.print("Acc Alch");
                    }
                    else if(alert == 6)
                    {
                      lcd.print("Acc Alch FL");
                    }
                    dt=0;
            }
      }
    }
/****************************************************************/
  void stop_car()
  {
    digitalWrite(7,LOW);
    digitalWrite(6,LOW);
    digitalWrite(5,LOW);
    digitalWrite(4,LOW);
    //Serial.println("Stop");    
  }
/****************************************************************/
  void Forward_car()
  {
    digitalWrite(7,HIGH);
    digitalWrite(6,LOW);
    digitalWrite(5,HIGH);
    digitalWrite(4,LOW);
    //Serial.println("Forward");
  }
/****************************************************************/
  void Reverse_car()
  {
    digitalWrite(7,LOW);
    digitalWrite(6,HIGH);
    digitalWrite(5,LOW);
    digitalWrite(4,HIGH);
    //Serial.println("Reverse");
  }
/****************************************************************/
  void Left_car()
  {
    digitalWrite(7,HIGH);
    digitalWrite(6,LOW);
    digitalWrite(5,LOW);
    digitalWrite(4,HIGH);
    //Serial.println("Left");
  }
/****************************************************************/
  void Right_car()
  {
    digitalWrite(7,LOW);
    digitalWrite(6,HIGH);
    digitalWrite(5,HIGH);
    digitalWrite(4,LOW);
    //Serial.println("Right");
  }
/****************************************************************/
  void bt_get_data()
  {
    char x;
    if (Bluetooth.available())
     {
          x=Bluetooth.read();
          //Serial.write(x);
          if(x=='q')
              {
                alert =0;
                acc   = 1;
                alch  = 1;
                flame = 1;
                Bluetooth.println("System set to safty");   
                lcd.clear();
                lcd.print("System enabled");
                lcd.setCursor(0,1);
                lcd.print(" Good to go");  
                delay(1000);           
              }
          else if(alert == 0)
          {
              if(x=='s')
              {
                mov=0;
                stop_car();
              }
              else if(x=='a')
              {
                mov=1;
                Forward_car();
              }
              else if(x=='b')
              {
                mov=1;
                Reverse_car();
              }
              else if(x=='c')
              {
                mov=1;
                Left_car();
              }
              else if(x=='d')
              {
                mov=1;
                Right_car();
              }
          }
          else
          {
            Bluetooth.println("System not safe please set to safty");
            lcd.clear();
            lcd.print("Sys Err please");
            lcd.setCursor(0,1);
            lcd.print("Enable System");
            delay(1000);
          }
          
     }
  }
/****************************************** RCV EVENT *************************/
    void receiveEvent(int howMany) 
    {
     while (0 <Wire.available()) 
     {
        char c = Wire.read();      /* receive byte as a character */
        //Serial.print(c);           /* print the character */
      }
     //Serial.println();             /* to newline */
    }
/****************************************** Request EVENT **********************/

void requestEvent() 
{
  char buffer[40];
  sprintf(buffer, "A%dB%dC%dD%d     ",acc,alch,flame,alert);  
  Wire.write(buffer);  /*send string on request */
}

/*******************************************************************************/
  void Serial_re()
  {
    char d;
    if(Serial.available())
    {
      d=Serial.read();
      if(d=='u')
      {
        char buffer[40];
        sprintf(buffer, "A%dB%dC%dD%d     ",acc,alch,flame,alert);  
        Serial.println(buffer);  /*send string on request */
        delay(10);
      }
    }
  }
/****************************************************************/
