#define IN_1  15          // L298N in1 motors Rightx          GPIO15(D8)
  #define IN_2  13          // L298N in2 motors Right           GPIO13(D7)
  #define IN_3  2           // L298N in3 motors Left            GPIO2(D4)
  #define IN_4  0           // L298N in4 motors Left            GPIO0(D3)
  const int trigPin = 12;//D6
const int echoPin = 14;//D5
int buzzer = 5;//D1
int LED = 16;//D0

long duration;              // This is a variable to store the duration of ultrasonic signal
int distance;               // This is a variable to store the distance measured by ultrasonic sensor
  #include <ESP8266WiFi.h>
  #include <WiFiClient.h> 
  #include <ESP8266WebServer.h>
  
  String command;             //String to store app command state.
  int speedCar = 800;         // 400 - 1023.
  int speed_Coeff = 3;
  
  const char* ssid = "Nodemcu5"; // put WIFI SSID for creating AP     
  const char* pass = "1234567898"; // put WIFI password for creating AP
  ESP8266WebServer server(80);
  
  void setup() {

pinMode(trigPin, OUTPUT);         // Set the trigger pin of ultrasonic sensor as Output from UNO board to send signals out
pinMode(echoPin, INPUT);          // Set the echo pin of ultrasonic sensor as input to UNO board to read the recieved signals
 pinMode(IN_1, OUTPUT);
 pinMode(IN_2, OUTPUT);
 pinMode(IN_3, OUTPUT);
 pinMode(IN_4, OUTPUT); 
 pinMode(buzzer,OUTPUT);
 pinMode(LED,OUTPUT);
  
  Serial.begin(115200);
  
// Connecting WiFi

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, pass);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
 
 // Starting WEB-server 
     server.on ( "/", HTTP_handleRoot );
     server.onNotFound ( HTTP_handleRoot );
     server.begin();    
}



void goAhead(){ 
   
      
      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
    
      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
  }

void goBack(){
   
      
      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
    
      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);

  }

void goRight(){ 
     
      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
      
  }

void goLeft(){
     
      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
     

      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
     
  }

void goAheadRight(){
    
      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      
 
      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
   }

void goAheadLeft(){
     
      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
   digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
  }

void goBackRight(){  
      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
       digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);  
  }
void goBackLeft(){ 
      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
     digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);     
  }
void stopRobot(){  
       digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, LOW);
       digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, LOW);  
 }
 void stopit(){  
       digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, LOW);
       digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, LOW);  
 }

void loop() {
  digitalWrite(trigPin, LOW);         // To make trigger pin clear from any values by making it to zero value
  delayMicroseconds(2);               // wait for 2 microseconds after making trigger pin zero
  digitalWrite(trigPin, HIGH);        // Sets the trigPin on HIGH state
  delayMicroseconds(10);              // Hold trigger pin on HIGH state for 10 Microseconds
  digitalWrite(trigPin, LOW);         // Stop the trigger pin or turn off the trigger pin HIGH state
  
                                      //calculate the distance measured by sensor
  duration = pulseIn(echoPin, HIGH);  // Read how much time HIGH value of signal received at echo pin and store it in DURATION variable
  distance = duration*0.034/2;// Calculating the distance based on duration
  Serial.println(distance);
  //delay(100);
  if(distance<=30 && command == "F")     // stop robot if distance is less than 30 cm   
  {
   stopit();
    digitalWrite(buzzer,HIGH);
    delay(100);
    digitalWrite(buzzer,LOW);
    
  }
  
  
      server.handleClient();
    command = server.arg("State");
      if (command == "F" && distance>30) 
      {goAhead();}
      else if (command == "B") {goBack();
      digitalWrite(LED,HIGH);
      delay(100);
      digitalWrite(LED,LOW);
      Serial.println("led");
      }
      else if (command == "L") goLeft();
      else if (command == "R") goRight();
      else if (command == "I") goAheadRight();
      else if (command == "G") goAheadLeft();
      else if (command == "J") goBackRight();
      else if (command == "H") goBackLeft();
      else if (command == "0") speedCar = 400;
      else if (command == "1") speedCar = 470;
      else if (command == "2") speedCar = 540;
      else if (command == "3") speedCar = 610;
      else if (command == "4") speedCar = 680;
      else if (command == "5") speedCar = 750;
      else if (command == "6") speedCar = 820;
      else if (command == "7") speedCar = 890;
      else if (command == "8") speedCar = 960;
      else if (command == "9") speedCar = 1023;
      else if (command == "S") stopRobot();
      
}
void HTTP_handleRoot(void) {

if( server.hasArg("State") ){
       Serial.println(server.arg("State"));
  }
  server.send ( 200, "text/html", "" );
  delay(1);
}
