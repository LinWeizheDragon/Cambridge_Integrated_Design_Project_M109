//
//  main.cpp
//  TestMap
//
//  Created by 林炜哲 on 06/10/2018.
//  Copyright © 2018 Dragon. All rights reserved.
//

#include <iostream>
using namespace std;
int current_color = 0; // current color of the brick that's going to cath
int motor_power[3] = {0, 0, 0}; // hand left right motor
int motor_angle[3] = {0, 0, 0};
int left_wheel_reading = 0;
int right_wheel_reading = 0;

class FieldMap{
public:
    string name="none";
    FieldMap* left=NULL;
    FieldMap* up=NULL;
    FieldMap* right=NULL;
    FieldMap* down=NULL;
    void print(){
        if (this->left != NULL){
            cout<<"left is:"<<this->left->name<<endl;
        }
        if (this->right!= NULL){
            cout<<"right is:"<<this->right->name<<endl;
        }
    }
};

void motor(int power, int angle, int index){
    // controls the motor at the hand of the robot
    motor_power[index] = power;
    motor_angle[index] = angle;
}

int eye_sensor_reading(void){
    // use different int for different colors
    return 0;
}

bool catch_failure(void){
    // at failure, catch_brick can be called several times
    return (eye_sensor_reading() != current_color);
}

void catch_brick(void){
    motor(0, 0, 0);
}

void path_adjustment(void){
    int adjustment_power_increment = 0; // some values found out in experiment
    int adjustment_angle_used = 0;
    int addressed_motor = 1;
    if (left_wheel_reading > 0) // detects the white line
        addressed_motor = 2;
    if (left_wheel_reading > 0 || right_wheel_reading > 0){
        int temp_power = motor_power[addressed_motor];
        int temp_angle = motor_angle[addressed_motor];
        motor(temp_power + adjustment_power_increment, adjustment_angle_used, addressed_motor);
        motor(temp_power, temp_angle - adjustment_angle_used, addressed_motor);
    }
}

void path_retrack(void){
    // called when no crossing is detected when angle more than enough has been rotated
    // currently no idea how to retrack. more information is needed
}

int main(int argc, const char * argv[]) {
    // insert code here...
    cout << "Hello, World!\n";
    FieldMap A,B;
    A.name = "This is A";
    B.name = "This is B";
    A.left = &B;
    B.right = &A;
    A.print();
    B.print();
    return 0;
}
