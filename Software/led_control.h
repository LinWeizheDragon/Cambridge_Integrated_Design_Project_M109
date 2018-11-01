#include <iostream>

using namespace std;
/**
 LED Display Definition
 Operation:
 bit 0: Following Line
 bit 1: Turning
 bit 2: Error Recovering
 Task:
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
#define LED_RECOVERING 0x04

#define LED_SCAN_C2 0x00
#define LED_SCAN_C1 0x01
#define LED_GOTO_D1 0x02
#define LED_GOTO_D2 0x03

#define LED_OBJECT_NONE 0x00
#define LED_OBJECT_RED 0x01
#define LED_OBJECT_WHITE 0x02
#define LED_OBJECT_GREEN 0x03
#define LED_OBJECT_WOOD 0x04
#define LED_OBJECT_TRANS 0x05

int led_value_operation, led_value_task, led_value_object;

void LedClear(){
    // clear led value
    led_value_operation = 0x00;
    led_value_task = 0x00;
    led_value_object = 0x00;
}
void OutputBinary(int reading){
    for (int i = 7; i>=0; i--){
        if (reading & (0x01 << i))
            cout<<1;
        else
            cout<<0;
    }
    cout<<endl;
}
int LedReading(){
    return ((0x07 - led_value_operation) | ((0x03 - led_value_task) << 3) | ((0x07 - led_value_object) << 5));
}
void LedDisplayOperation(int led_type, bool led_switch){
    if (led_switch){
        if ((led_value_operation & led_type) == 0){
            // if this bit is 0
            led_value_operation = led_value_operation ^ led_type;
        }
    }else{
        if ((led_value_operation & led_type) != 0){
            // if this bit is 1
            led_value_operation = led_value_operation ^ led_type;
        }
    }
    /*
    for (int i = 0; i<3; i++){
        if (led_value_operation & (0x01 << i))
            cout<<1;
        else
            cout<<0;
    }*/
    cout<<endl;
}
void LedDisplayTask(int led_type){
    led_value_task = led_type;
}
void LedDisplayObject(int led_type){
    led_value_object = led_type;
}
