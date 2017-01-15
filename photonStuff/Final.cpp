// This #include statement was automatically added by the Particle IDE.
#include "SparkJson/SparkJson.h"

// This #include statement was automatically added by the Particle IDE.
#include "HttpClient/HttpClient.h"

#include "InternetButton/InternetButton.h"

/* How about we make this interactive? */

InternetButton b = InternetButton();

int led1 = D0;
long previousMillis = 0;
long interval = 2000;

void setup() {
    // Tell b to get everything ready to go
    // Use b.begin(1); if you have the original SparkButton, which does not have a buzzer or a plastic enclosure
    // to use, just add a '1' between the parentheses in the code below.
    b.begin();
    Time.zone(+4);
    Particle.subscribe("hook-response/getrqst", myHandler, MY_DEVICES);
    // Here's another way to limit the overall brightness, so you don't have to do it for each pixel
    b.setBrightness(50);
    pinMode(led1, OUTPUT);
    Serial.begin(9600);

}

String gtone;
String gmorse;
String ggroup;
int on;
String last;
bool delayOff = false;


void myHandler(const char *event, const char *data)
{
  if (data) {

    char* json = strdup(data);
    
    StaticJsonBuffer<200> jsonBuffer;
    
    JsonObject& root = jsonBuffer.parseObject(json);

    const char* group = root["group"];
    const char* morse = root["morse"];
    const char* tone = root["tone"];
    const char* text = root["text"];
    
        if(last != text) {
            on = 1;
            
            gtone = tone;
            gmorse = morse;
            ggroup = group;
        }
        
    last = text;
    }
  else {
    Serial.println("NULL");
    on = 0;
  }
    
    
};


void loop(){
    int hour = Time.hourFormat12();
    int minute = Time.minute()/5;
    if(minute != 12){
        b.ledOff(minute-1);
        b.ledOn(minute, 255, 0, 0);
    }
    if(hour != 12){
        b.ledOff(hour-1);
        b.ledOn(hour, 0, 0, 255);
    }
    if(minute == hour){
        b.ledOn(minute, 255, 0 ,255);
    }
    
    if(b.buttonOn(1)){
        b.playSong("E6,8, ,4,D6,8, ,4,C6,8, ,4,D6.8, ,4,E6,8, ,4,E6,8, ,4,E6,10, ,4,D6,8, ,4,D6,8, ,4,D6,10, ,4,E6,8, ,4,E6,8, ,4,E6,10, ,4,E6,8, ,4,D6,8, ,4,C6,8, ,4,D6.8, ,4,E6,8, ,4,E6,8, ,4,E6,10, ,4,E6,8, ,4,D6,8, ,4,D6,8, ,4,E6,8, ,4,D6,8, ,4,C6,12");
    }
    
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis > interval || delayOff) {
        if(!delayOff){
            Particle.publish("getrqst", "", PRIVATE);
            previousMillis = currentMillis;
        }
        
        // When you click the second button (at the 3 o'clock position) let's turn that LED on
        if(on == 1){
      // Trigger the integration
            delayOff = true;
            b.allLedsOn(0,20,20);
            digitalWrite(led1, HIGH);
            if(currentMillis - previousMillis > interval){
                b.allLedsOff();
                digitalWrite(led1, LOW);
                Serial.println(gtone);
                delayOff = false;
                on = 0;
            }
        }
        // // And if the button's not on, then the LED should be off
        // else {
            
        // }
    }
    // delay(4000);
    /* Much like the LEDs, there are also functions to check if all the buttons are on- b.allButtonsOn()
    or if all the buttons are off- b.allButtonsOff() */
}
