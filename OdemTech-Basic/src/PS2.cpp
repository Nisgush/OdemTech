#include <Arduino.h>

class Controller{
    private:
    const int DATA_PIN;
    const int CMD_PIN;
    const int CLK_PIN;
    const int ATT_PIN;

    uint8_t sendCommand(uint8_t cmd) {
        uint8_t response = 0;

        for (int i = 0; i < 8; i++) {
            // Write command bit by bit
            digitalWrite(CMD_PIN, (cmd & (1 << i)) ? HIGH : LOW);

            // Pulse the clock
            digitalWrite(CLK_PIN, LOW);
            delayMicroseconds(10);

            // Read the response bit
            if (digitalRead(DATA_PIN)) {
            response |= (1 << i);
            }

            digitalWrite(CLK_PIN, HIGH);
            delayMicroseconds(10);
            }
        return response;
    }

    float power(float a, int b){
        float result = 1;
        for(int i = 0; i<b; i++){
            result*=a;
        }
    return result;
    }





    public:
    Controller(int DATA, int CMD, int CLK, int ATT) : DATA_PIN(DATA), CMD_PIN(CMD), CLK_PIN(CLK), ATT_PIN(ATT){
        
        pinMode(DATA_PIN, INPUT_PULLUP);
        pinMode(CMD_PIN, OUTPUT);
        pinMode(CLK_PIN, OUTPUT);
        pinMode(ATT_PIN, OUTPUT);

        digitalWrite(CMD_PIN, HIGH);
        digitalWrite(CLK_PIN, HIGH);
        digitalWrite(ATT_PIN, HIGH);
    }

    bool triangle = false; //true = pressed, false = released
    bool cross = false; //true = pressed, false = released
    bool circle = false; //true = pressed, false = released
    bool square = false; //true = pressed, false = released
    float left_joystick_X = 0; //value between 0 to 255
    float left_joystick_Y = 0; //value between 0 to 72
    bool left_joystick_button = false; //true = pressed, false = released
    bool right_joystick_button = false; //true = pressed, false = released
    float right_joystick_X = 0; //value between 0 to 255
    float right_joystick_Y = 0; //value between 0 to 72
    bool up = false; //true = pressed, false = released
    bool right = false; //true = pressed, false = released
    bool down = false; //true = pressed, false = released
    bool left = false; //true = pressed, false = released
    bool select = false; //true = pressed, false = released
    bool start = false; //true = pressed, false = released
    bool left_trigger = false; //true = pressed, false = released
    bool left_bumper = false; //true = pressed, false = released
    bool right_trigger = false; //true = pressed, false = released
    bool right_bumper = false; //true = pressed, false = released

    void updateData() {
        uint8_t data[8]; // Array to store PS2 response

        // Start communication
        digitalWrite(ATT_PIN, LOW); // Select controller
        sendCommand(0x01);          // Start byte
        data[0] = sendCommand(0x42); // Poll controller (0x42 for "read" command)
        data[1] = sendCommand(0x00); // Status byte
        for (int i = 2; i < 8; i++) {
            data[i] = sendCommand(0x00); // Receive button/joystick data
        }
        digitalWrite(ATT_PIN, HIGH); // Deselect controller

        // Print raw data
        /*Serial.print("Raw Data: ");
        for (int i = 0; i < 8; i++) {
            Serial.print(data[i], HEX);
            Serial.print(" ");
        }
        Serial.println();
        */
        // Parse button data (data[3] and data[2] contain button states)
        triangle = (data[3] & 0x10) == 0;
        circle = (data[3] & 0x20) == 0;
        cross = (data[3] & 0x40) == 0;
        square = (data[3] & 0x80) == 0;

        select = (data[2] & 0x01) == 0;
        left_joystick_button = (data[2] & 0x02) == 0;
        right_joystick_button = (data[2] & 0x04) == 0;
        start = (data[2] & 0x08) == 0;

        up = (data[2] & 0x10) == 0;
        right = (data[2] & 0x20) == 0;
        down = (data[2] & 0x40) == 0;
        left = (data[2] & 0x80) == 0;

        left_trigger = (data[3] & 0x01) == 0;
        right_trigger = (data[3] & 0x02) == 0;
        left_bumper = (data[3] & 0x04) == 0;
        right_bumper = (data[3] & 0x08) == 0;

        // Parse joystick data
        left_joystick_X = data[6];
        left_joystick_Y = data[7];
        right_joystick_X = data[4];
        right_joystick_Y = data[5];
    }
};

