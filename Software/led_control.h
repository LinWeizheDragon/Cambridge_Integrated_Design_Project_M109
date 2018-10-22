#include <iostream>

using namespace std;

/**
 LED Display Definition
 Operation:
 bit 0: Following Line
 bit 1: Turning
 bit 2: Error Recovering
 bit 3-4:
 00: Going to scan C2, height 2
 01: Going to scan C1, height 1
 10: Going to lorry D1
 11: Going to lorry D2
 bit 5-7
 Object:
 000: red
 001: white
 010: green
 011: wood
 100: transparent
 **/

// LED Definition
#define LED_FOLLOWING_LINE 0x01
#define LED_TURNING 0x02
#define LED_ADJUSTING 0x04
#define LED_RECOVERING 0x08
#define LED_SCAN_C2 0x10
#define LED_SCAN_C1 0x20
#define LED_GOTO_D1 0x40
#define LED_GOTO_D2 0x80

#define LED_OBJECT_RED 0x01
#define LED_OBJECT_WHITE 0x02
#define LED_OBJECT_GREEN 0x04
#define LED_OBJECT_WOOD 0x08
#define LED_OBJECT_TRANS 0x10

int led_value_A, led_value_B;

void LedClear(){
    // clear led value
    led_value_A = 0x00;
    led_value_B = 0x00;
}
int LedReadingOperation(){
    return led_value_A;
}
int LedReadingObject(){
    return led_value_B;
}
void LedDisplayOperation(int led_type, bool led_switch){
    if (led_switch){
        if ((led_value_A & led_type) == 0){
            // if this bit is 0
            led_value_A = led_value_A ^ led_type;
        }
    }else{
        if ((led_value_A & led_type) != 0){
            // if this bit is 1
            led_value_A = led_value_A ^ led_type;
        }
    }
    cout<<"Operation LED set to: ";
    for (int i = 0; i<8; i++){
        if (led_value_A & (0x01 << i))
            cout<<1;
        else
            cout<<0;
    }
    cout<<endl;
}
void LedDisplayObject(int led_type, bool led_switch){
    if (led_switch){
        if ((led_value_B & led_type) == 0){
            // if this bit is 0
            led_value_B = led_value_B ^ led_type;
        }
    }else{
        if ((led_value_B & led_type) != 0){
            // if this bit is 1
            led_value_B = led_value_B ^ led_type;
        }
    }
    cout<<"Object LED set to: ";
    for (int i = 0; i<8; i++){
        if (led_value_B & (0x01 << i))
            cout<<1;
        else
            cout<<0;
    }
    cout<<endl;
}
