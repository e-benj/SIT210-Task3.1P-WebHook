//Name: Ethan Benjamin

//Please not some parts of this script have been adapted from the default DHT script provided by adafruit

// This #include statement was automatically added by the Particle IDE.
#include <JsonParserGeneratorRK.h>

//Name: Ethan Benjamin

#include "Adafruit_DHT.h"

#define DHTPIN A2     // what pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11		// DHT 11 

const pin_t MY_LED = D7;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
	Serial.begin(9600); 
	pinMode(MY_LED, OUTPUT);
	dht.begin();
}

void loop() {
// Reading temperature or humidity takes about 250 milliseconds!
// Sensor readings may also be up to 2 seconds 'old' (its a 
// very slow sensor)
	float hum = dht.getHumidity();
// Read temperature as Celsius
	float temp = dht.getTempCelcius();
	
	//Check that both readings are valid
	if (isnan(hum) || isnan(temp)) {
		Serial.println("Failed to read from DHT sensor!");
		return;
	}
  
	//Switch on LED to show data is being pushed to webook
	digitalWrite(MY_LED, HIGH);
	//Call method to create JSON and output to Thingspeak via webhook
	createJsonPayload(temp, hum);
	//Leave LED on for 1 second
	delay(1s);
	//Switch off LED
	digitalWrite(MY_LED, LOW);
	
	//Wait a total of 30 seconds before next reading
	delay(29s);
}

void createJsonPayload(float temp, float humidity)
{
    JsonWriterStatic<256> jw;
    {
        //Create JSON payload
        JsonWriterAutoObject obj(&jw);
    
        jw.insertKeyValue("temp", temp);
        jw.insertKeyValue("hum", humidity);
    }
    
    //Publish JSON payload via webhook
    Particle.publish("temp_hum_vals", jw.getBuffer(), PRIVATE);
}

