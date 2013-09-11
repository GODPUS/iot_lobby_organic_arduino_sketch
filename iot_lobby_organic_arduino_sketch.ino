/*
Sketch for Arduino FIO v3 boards with accelerometers soldered on them 
to detect when they've been picked up and report that activity to a web application.

if the printing is triggering too easily, increase the gThreshold
and if it's too difficult to trigger, decrease the gThreshold

assumes the X,Y,Z analog outputs of the Accelerometer 
are wired up to analog in pins 1,2,3 on the Arduino Fio

based on an example from http://wiring.org.co/learning/basics/accelerometer.html
for Triple Axis Accelerometer Breakout - ADXL335 https://www.sparkfun.com/products/9269
tested on FIO v3 boards https://www.sparkfun.com/products/11520
*/

String objectBrand = "quaker";
//String applicationAddress = "http://afternoon-fjord-6952.herokuapp.com/";
String applicationAddress = "http://192.168.23.61:5000/";
int gThreshold = 5;
int x, y, z, xOld, yOld, zOld;

void setup()
{
  //init the serial line for the accelerometer
  Serial.begin(9600);
  
  //init the serial line for the xbee radio
  //example on the xig site state faster is better for the xbee 
  //but when i tried higher data rates Serial1.availible() would evalutate to false
  Serial1.begin(9600);
  if(!Serial1.available()){
    Serial.println("Serial1 not availible");  
  }
}

void loop()
{
  //get new values from the Accelerometer 
  x = (analogRead(A1)-510)+90; //conversion to degrees then add 90 so that the device corresponds to the way you hold it
  y = (analogRead(A2)-510)*-1; //conversion to degrees
  z = (analogRead(A3)-510)*-1; //conversion to degrees
  
  //we only want to take action if there's been significant movement, hence the threshold test
  if(abs(xOld - x) > gThreshold || abs(yOld - y) > gThreshold)
  {
    //build the url to call, final url looks something like
    //http://afternoon-fjord-6952.herokuapp.com/movement?object=nike&x=22&y=57&z=90
    String url = applicationAddress+"movement?object="+objectBrand+"&x="+(x)+"&y="+(y)+"&z="+(z);
    
    //call the url via xbee through iDigi connect port
    Serial.println(url);
    Serial1.println(url);    
    
    //Serial1.print("http://afternoon-fjord-6952.herokuapp.com/movement?object=nike"); 
  }
      
  //store old values to compare against next loop
  xOld = x;
  yOld = y;
  zOld = z;
  
  delay(500);              // wait 100ms for next reading
}

