#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "DocykeMotor.h"

#define ENABLE_PIN 5
#define DIRECTION_PIN 12
#define VELOCITY_PIN 13

#define TRIG_PIN 14
#define ECHO_PIN 27

// WiFi settings
const char* ssid = "ODEM";
const char* password = "odem3008";

// MQTT broker settings
const char* mqtt_server = "159.223.248.208"; // Change this to your broker address
const int mqtt_port = 1883;
const char* mqtt_user = "arnon";
const char* mqtt_password = "arnon";

// Topics
const char* topic_motor = "robot/motor";
const char* topic_remote = "robot/remoteIn";
const char* topic_detection = "robot/objectDetection";
const char* topic_distance = "robot/distance";

const char* subscribedTopics[] = {topic_motor,topic_remote,topic_detection,topic_distance};

WiFiClient espClient;
PubSubClient client(espClient);

DocykeMotor motor(ENABLE_PIN, DIRECTION_PIN, VELOCITY_PIN);

// Generate unique ID
String generate_uuid() {
    char uuid[37];
    snprintf(uuid, sizeof(uuid), "%04x%04x-%04x-%04x-%04x-%04x%04x%04x",
             random(0, 0xffff), random(0, 0xffff),
             random(0, 0xffff),
             random(0, 0x0fff) | 0x4000,
             random(0, 0x3fff) | 0x8000,
             random(0, 0xffff), random(0, 0xffff), random(0, 0xffff));
    return String(uuid);
}

// Get current timestamp
String get_timestamp() {
    time_t now = time(nullptr);
    struct tm* timeinfo = localtime(&now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%S", timeinfo);
    return String(buffer);
}

String create_message(const char* msg_type, const char* payload) {
    String message = "{";
    message += "\"id\":\"" + generate_uuid() + "\",";
    message += "\"msgType\":\"" + String(msg_type) + "\",";
    message += "\"timestamp\":\"" + get_timestamp() + "\",";
    message += "\"payload\":\"" + String(payload) + "\"}";
    return message;
}

int recievedInt = 0;
int receivedDistance = 0;
void callback(char* topic, byte* message, unsigned int length) {
    String msg;
    for (unsigned int i = 0; i < length; i++) {
        msg += (char)message[i];
    }
    Serial.printf("Received message on topic %s: %s\n", topic, msg.c_str());
    
    if(String(topic)==String(topic_distance)){
        receivedDistance = msg.toInt();
    }


    

    /*if(*topic==*topic_distance){
        recievedInt=msg.toInt();
    }*/
    
    // Handle the received message (e.g., update motor speed)
    // DynamicJsonDocument doc(1024);
    // deserializeJson(doc, msg);
    // int speed = doc["speed"];
    // motorController.setSpeed(speed);
}

void reconnect() {
    while (!client.connected()) {
        Serial.println("Attempting MQTT connection...");
        if (client.connect("ESP32Client", mqtt_user, mqtt_password)) {
            Serial.println("Connected to MQTT broker");
            for(String topic:subscribedTopics){
                client.subscribe(topic.c_str());
                Serial.print("Subscribed to: ");
                Serial.println(topic);
            }
            
        } else {
            Serial.print("Failed to connect, rc=");
            Serial.print(client.state());
            Serial.println(". Trying again in 5 seconds...");
            delay(5000);
        }
    }
}

void setup() {
    Serial.begin(115200);

    // Connect to WiFi
    Serial.println("Connecting to WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected");

    pinMode(ENABLE_PIN, OUTPUT);
    pinMode(DIRECTION_PIN, OUTPUT);
    pinMode(VELOCITY_PIN, OUTPUT);
    
    Serial.println("Starting motor");

    motor.enable();
    motor.setDirection(false);

    // Configure MQTT
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);


    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}


long measureDistance() {
    // Send a 10us pulse to trigger pin
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    
    // Measure the duration of the echo pulse
    long duration = pulseIn(ECHO_PIN, HIGH);
    return duration;
}

int DistanceCM(){
    long duration = measureDistance();
    return (int)((float)duration / 29 / 2);
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    Serial.print("received distance: ");
    Serial.println(receivedDistance);
    if(receivedDistance<20){
        motor.drive(250,50);
        client.publish(topic_motor,"250");
    }else{
    client.publish(topic_motor,"0");
    motor.drive(0,50);
    }

    int distance = DistanceCM();
    Serial.print("Distance: ");
    Serial.println(distance);
    client.publish(topic_distance,String(distance).c_str());
    
    if(WiFi.status() == WL_CONNECTED){
    client.loop();
    }
}

