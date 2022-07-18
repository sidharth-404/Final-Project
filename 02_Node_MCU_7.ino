//Node mcu code

#include <Wire.h>

void setup() 
{
     Serial.begin(9600);    
     Wire.begin(D1, D2);    /* join i2c bus with SDA=D1 and SCL=D2 of NodeMCU */
}

void loop() 
{


 Wire.requestFrom(8, 30);       /* request & read data of size 13 from slave */
 while(Wire.available())
 {
    char c = Wire.read();
    Serial.print(c);
 }
 Serial.println();
 delay(3000);
}
