/*
* Name: clock and temp project
* Author: Ludvig Larsson
* Date: 2024-22-10
* Description: This project uses a ds3231 to measure time and displays the time to an 1306 oled display,
* Further, it measures temprature with a analog temprature module and displays a mapped value to a 9g-servo-motor
* In the end i decided to add extra componenet for an extra fun feature, I decided to add 3 led lights which are adapted to the temperature, showing if the temp is cold, good or too hot
*/

// Include Libraries
  #include <RTClib.h>
  #include <Wire.h>
  #include <U8glib.h> 
  #include <Servo.h>
  U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);
// Init constants

// Init global variables
// construct objects
  RTC_DS3231 rtc;
  Servo myservo;

void setup() {
  // init communication
  Serial.begin(9600);
  // Init Hardware
  Wire.begin();
  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));
  u8g.setFont(u8g_font_unifont);
  myservo.attach(7);
  pinMode(A1, INPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
}

void loop() {
 
  
  Serial.println(getTemp()); // print the value of the temp to the serial monitor
  oledWrite(0, 33, getTime()); // the value of the time prints out on the oled screen
  servoWrite(getTemp()); // the temperature is mapped to the servo making the value of the temp change the angle on the servo (for example: 30 degrees mappes to 180 degrees on the servo)
  delay(1000);

  if (getTemp() < 26 && getTemp() > 19){ // this part of the code makes different colored lights light up depening if the temperature is "Too cold", "Good" or "Too hot"
    digitalWrite(8, HIGH);
    digitalWrite(9, LOW);
    }
  else if (getTemp() > 26){
    digitalWrite(8, LOW);
    digitalWrite(9, HIGH);
    }
  else if (getTemp() < 19){
    digitalWrite(10, HIGH);
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
  }
  


}


/*
*This function reads time from an ds3231 module and package the time as a String
*Parameters: Void
*Returns: time in hh:mm:ss as String
*/
String getTime(){
  DateTime now = rtc.now();
  //Serial.println("The time is" + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second()));
  String tid= String("Time:") + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
  return(tid);


}

/*
* This function reads an analog pin connected to an analog temprature sensor and calculates the corresponding temp
*Parameters: Void
*Returns: temprature as float
*/
float getTemp(){
 
 int ThermistorPin = A1;
 int Vo;
 float R1 = 10000; // value of R1 on board
 float logR2, R2, T;
 float c1 = 0.001129148, c2 = 0.000234125, c3 = 0.0000000876741;
 
  Vo = analogRead(A1);
  R2 = R1 * (1023.0 / (float)Vo - 1.0); //calculate resistance on thermistor
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2)); // temperature in Kelvin
  T = T - 273.15; //convert Kelvin to Celcius

  return T;
}

/*
* This function takes a string and draws it to an oled display
*Parameters: - text: String to write to display
*Returns: void
*/
void oledWrite(int x, int y, String text){
  u8g.firstPage();  
  do {
    u8g.drawStr(x, y, text.c_str());  
  } while (u8g.nextPage());  
}

/*
* takes a temprature value and maps it to corresppnding degree on a servo
*Parameters: - value: temprature
*Returns: void
*/
void servoWrite(float value){
  myservo.write(map(getTemp(), 15, 30, 0, 180)); 
}