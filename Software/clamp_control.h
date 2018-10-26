#include <iostream>

using namespace std;

#define ARM_SHRINK 0
#define ARM_EXTENDED 1
#define CLAMP_OPEN 0
#define CLAMP_CLOSED 1

class Clamp{
public:
    int arm_status = ARM_SHRINK;
    int clamp_status = CLAMP_CLOSED;
    void ShrinkArm(){
        // shrink the arm
        this->arm_status = ARM_SHRINK;
    }
    void ExtendArm(){
        // extend the arm
        this->arm_status = ARM_EXTENDED;
    }
    void CloseClamp(){
        // close the clamp
        this->clamp_status = CLAMP_CLOSED;
    }
    void OpenClamp(){
        // open the clamp
        this->clamp_status = CLAMP_OPEN;
    }
    int GetReading(int current_reading){
        int v = current_reading;
        if (v bitand 0x20){
            v = v - 0x20;
        }
        if (v bitand 0x40){
            v = v - 0x40;
        }
        if (this->clamp_status == CLAMP_OPEN){
            v = v + 0x20;
        }
        if (this->arm_status == ARM_EXTENDED){
            v = v + 0x40;
        }
        return v;
    }
};

Clamp clamp;

