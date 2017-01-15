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
long memePrevMillis = 0;
long memeInterval = 500;

void setup() {
    // Tell b to get everything ready to go
    // Use b.begin(1); if you have the original SparkButton, which does not have a buzzer or a plastic enclosure
    // to use, just add a '1' between the parentheses in the code below.
    b.begin();
    Time.zone(+4);
    Particle.subscribe("hook-response/getrqst", myHandler, MY_DEVICES);
    Particle.subscribe("hook-response/sendmeme", memeHandler, MY_DEVICES);
    Particle.subscribe("hook-response/k", kHandler, MY_DEVICES);
    // Here's another way to limit the overall brightness, so you don't have to do it for each pixel
    b.setBrightness(70);
    pinMode(led1, OUTPUT);
    Serial.begin(9600);
}

String gtone;
String gmorse;
String ggroup;
int on;
String last = "";
bool delayOff = false;

void kHandler(const char *event, const char *data){
    // nothing
}

void memeHandler(const char *event, const char *data){
    // nothing
}

void myHandler(const char *event, const char *data)
{
  if (data) {
    Serial.println("my handler method");
    char* json = strdup(data);
    
    StaticJsonBuffer<200> jsonBuffer;
    
    JsonObject& root = jsonBuffer.parseObject(json);

    const char* groupc = root["group"];
    String group = groupc;
    const char* morsec = root["morse"];
    String morse = morsec;
    const char* tonec = root["tone"];
    String tone = tonec;
    const char* textc = root["text"];
    String text = textc;
    Serial.println(last);
    Serial.println(text);
    if(last != text) {
        on = 1;
        Serial.println("on");
        gtone = tone;
        gmorse = morse;
        ggroup = group;
        
    }
        
    last = text;
  } else {
    Serial.println("NULL");
    on = 0;
  }
};


void loop(){
    int hour = Time.hourFormat12();
    int minute = Time.minute()/5;
    unsigned long currentMillis = millis();
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
    
    if(b.buttonOn(3) && (currentMillis - memePrevMillis > memeInterval)){
        memePrevMillis = currentMillis;
        Particle.publish("sendmeme", "", PRIVATE);
    }
    
    if(b.buttonOn(2)){
        // 
    }
    
    if(b.buttonOn(4)){
        Particle.publish("k", "", PRIVATE);
    }

    if(currentMillis - previousMillis > interval || delayOff) {
        if(!delayOff){
            Particle.publish("getrqst", "", PRIVATE);
            previousMillis = currentMillis;
        }
        
        // When you click the second button (at the 3 o'clock position) let's turn that LED on
        if(on == 1){
      // Trigger the integration
            int g = 0;
            int r = 0;
            
            if(gtone == "good") {
                g = 255;
                r = 0;
            } else {
                r = 255;
                g = 0;
            }
            delayOff = true;
            b.allLedsOn(r,g,0);
            digitalWrite(led1, HIGH);
            if(currentMillis - previousMillis > interval){
                b.allLedsOff();
                digitalWrite(led1, LOW);
                Serial.println(gtone);
                delayOff = false;
                on = 0;
            }
        }

    }

}
