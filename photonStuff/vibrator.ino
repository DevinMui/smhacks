// This #include statement was automatically added by the Particle IDE.
#include "SparkJson/SparkJson.h"

// This #include statement was automatically added by the Particle IDE.
#include "HttpClient/HttpClient.h"

#include "InternetButton/InternetButton.h"

/* How about we make this interactive? */

InternetButton b = InternetButton();

int led1 = D0;

void setup() {
    // Tell b to get everything ready to go
    // Use b.begin(1); if you have the original SparkButton, which does not have a buzzer or a plastic enclosure
    // to use, just add a '1' between the parentheses in the code below.
    b.begin();
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

    
    Particle.publish("getrqst", "", PRIVATE);
    
    delay(2000);
    // When you click the second button (at the 3 o'clock position) let's turn that LED on
    if(on == 1){
  // Trigger the integration
        
        b.allLedsOn(0,20,20);
        digitalWrite(led1, HIGH);
        delay(2000);
        b.allLedsOff();
        digitalWrite(led1, LOW);
        Serial.println(gtone);
        
        on = 0;
    }
    // And if the button's not on, then the LED should be off
    else {
        
    }
    
    delay(4000);
    /* Much like the LEDs, there are also functions to check if all the buttons are on- b.allButtonsOn()
    or if all the buttons are off- b.allButtonsOff() */
}