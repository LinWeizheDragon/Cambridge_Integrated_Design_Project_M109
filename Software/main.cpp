#include <iostream>
using namespace std;
#include <robot_instr.h>
#include <robot_link.h>
#include <stopwatch.h>
#include <robot_delay.h>
#include "autopilot.h"
#include "led_control.h"
#include "object_recognition.h"

#define ROBOT_NUM 10   // The id number (see below)
int motor_turning_speed = 75;
int adjustment_power_decrement = 5;
int motor_common_speed = 127; //127 max
int motor_passing_crosing_time = 300;
stopwatch watch;
robot_link rlink;      // datatype for the robot link
/*
 Handle errors generated by rlink & program
 */
void ErrorHandling(int error_code=-1){
    if (rlink.any_errs()) {
        if (rlink.fatal_err()){
            //fatal error occurs
            rlink.print_errs("FATAL_ERR:  ");
            return;
        }
        rlink.print_errs("ERR:  ");
    }
    if (error_code != -1){
        switch(error_code){
            case ERROR_LOSE_WAY:
                //TODO:
                cout<<"ERR:  Lose the way, rebooting..."<<endl;
                break;
            case ERROR_PICKUP_FAIL:
                cout<<"ERR:  Pick-up fail, attempting again..."<<endl;
                break;
                //TODO:
            default:
                cout<<"ERR:  Error Code("<<error_code<<") not programmed."<<endl;
        }
    }
}
/*
 Initialize a node, and store it into storage
 */
void InitializeNode(string name, Node* target, Node* left, Node* up, Node* right, Node* down){
    target->name = name;
    target->left = left;
    target->up = up;
    target->right = right;
    target->down = down;
    node_storage[name] = target;
}
/*
 Initialize an object
 */
void InitializeObject(string name, Object* target, Node* deliever_port){
    target->name = name;
    target->target = deliever_port;
}

void MapInitialization(){
    cout<<"Map Initializing..."<<endl;
    // map initialization
    InitializeNode("S2", &S2, &F1, &F2, &F3, &F4);
    InitializeNode("S1", &S1, NULL, &F4, &A1, NULL);
    InitializeNode("F1", &F1, NULL, NULL, &S2, NULL);
    InitializeNode("F2", &F2, NULL, NULL, NULL, &S2);
    InitializeNode("F3", &F3, &S2, NULL, &A2, NULL);
    InitializeNode("F4", &F4, NULL, &S2, NULL, &S1);
    
    InitializeNode("A2", &A2, &S2, NULL, &B2, &A1);
    InitializeNode("A1", &A1, &S1, &A2, &B1, NULL);
    
    InitializeNode("B2", &B2, &A2, &B5, &C2, &B1);
    InitializeNode("B1", &B1, &A1, &B2, &C1, NULL);
    InitializeNode("B5", &B5, NULL, &B6, &D5, &B2);
    InitializeNode("B6", &B6, NULL, &B7, &D6, &B5);
    InitializeNode("B7", &B7, NULL, &B8, &D7, &B6);
    InitializeNode("B8", &B8, NULL, NULL, &D8, &B7);
    
    InitializeNode("C2", &C2, &B2, NULL, &D2, &C1);
    InitializeNode("C1", &C1, &B1, &C2, &D1, NULL);
    
    InitializeNode("D1", &D1, &C1, &D2, &E1, NULL);
    InitializeNode("D2", &D2, &C2, &D3, &E2, &D1);
    InitializeNode("D3", &D3, NULL, &D4, &E3, &D2);
    InitializeNode("D4", &D4, NULL, &D5, &E4, &D3);
    InitializeNode("D5", &D5, &B5, &D6, &E5, &D4);
    InitializeNode("D6", &D6, &B6, &D7, &E6, &D5);
    InitializeNode("D7", &D7, &B7, &D8, &E7, &D6);
    InitializeNode("D8", &D8, &B8, NULL, &E8, &D7);
    
    InitializeNode("E1", &E1, &D1, &E2, NULL, NULL);
    InitializeNode("E2", &E2, &D2, &E3, NULL, &E1);
    InitializeNode("E3", &E3, &D3, &E4, NULL, &E2);
    InitializeNode("E4", &E4, &D4, &E5, NULL, &E3);
    InitializeNode("E5", &E5, &D5, &E6, NULL, &E4);
    InitializeNode("E6", &E6, &D6, &E7, NULL, &E5);
    InitializeNode("E7", &E7, &D7, &E8, NULL, &E6);
    InitializeNode("E8", &E8, &D8, NULL, NULL, &E7);
    
    map<string, Node*>::iterator  iter;
    /*
    for(iter = node_storage.begin(); iter != node_storage.end(); iter++){
        iter->second->print();
    }*/
    cout<<"Map Initialization completed, total:"<<node_storage.size()<<" nodes"<<endl;
    
}

void TaskInitialization(){
    //task initialization
    task_list.push_back(TASK_SCAN_A);
    task_list.push_back(TASK_SCAN_A);
    task_list.push_back(TASK_SCAN_A);
    task_list.push_back(TASK_SCAN_A);
    task_list.push_back(TASK_SCAN_B);
    task_list.push_back(TASK_SCAN_B);
    task_list.push_back(TASK_SCAN_B);
    task_list.push_back(TASK_SCAN_B);
    
    //operation list initialization
    operation_list.push_back(GO_STRAIGHT);
    //operation_list.push_back(TURN_LEFT); 
    operation_list.push_back(GO_STRAIGHT);
    operation_list.push_back(GO_STRAIGHT);
    //operation_list.push_back(TURN_RIGHT); 
    operation_list.push_back(GO_STRAIGHT);
    
    current_node = &A2;
    previous_node = &F3;
    cout<<"Task Initialization completed."<<endl;
}

void ObjectInitialization(){
    //Object Initialization
    InitializeObject("red", &OBJECT_RED, &B5);
    InitializeObject("white", &OBJECT_WHITE, &B6);
    InitializeObject("green", &OBJECT_GREEN, &B5);
    InitializeObject("wood", &OBJECT_WOOD, &B6);
    InitializeObject("transparent", &OBJECT_TRANS, &B6);
    cout<<"Object Initialization completed."<<endl;
}



void get_wheel_reading(void){
	int v;
	v = rlink.request (READ_PORT_0);
    front_left_sensor_reading = v % 2;
    front_right_sensor_reading = (v/2) % 2;
    middle_sensor_reading =  (v/4) % 2;
    back_sensor_reading =  (v/8) % 2;
}

// four light sensors, 0 at front left, 1 at front right, 2 at middle, 3 at the back off the line
void get_state(void){
    get_wheel_reading();
    if (front_left_sensor_reading == 1){
        if (front_right_sensor_reading == 1){
            if (middle_sensor_reading == 1)
                state = 0; // on track
            state = -1; // head on track but body off track
        }
        if (middle_sensor_reading == 1)
            state = 2; // deviation towards right
        state = 5; // large deviation towards right
    }
    if (front_right_sensor_reading == 1){
        if (middle_sensor_reading == 1)
            state = 1; // deviation towards left
        state = 4; // large deviation towards left
    }
    if (middle_sensor_reading == 1){
        state = 6; // large deviation
    }
    state = 7; // very large deviation
}

void get_contact_switch(void){ // TO DO
    contact_switch = false;
}

void motor_control(int left_wheel_power, int right_wheel_power){
    rlink.command(MOTOR_1_GO, left_wheel_power - 10);
    rlink.command(MOTOR_2_GO, right_wheel_power + 128);
}

void motor_turn(int speed, int direction){ // 0 for left, 1 for right
	if (direction == 0){
        rlink.command(MOTOR_1_GO, speed);
        rlink.command(MOTOR_2_GO, speed + 18);
	}
	else if (direction == 1){
        rlink.command(MOTOR_1_GO, speed + 128 + 8);
        rlink.command(MOTOR_2_GO, speed + 128);
	}
	else
		cout << "turning direction error";
}

void reposition(int direction){ // 0 for left, 1 for right
	if (direction == 1){
		rlink.command(MOTOR_1_GO, motor_turning_speed);
		rlink.command(MOTOR_2_GO, motor_turning_speed);
	}
	else if (direction == 0){
		rlink.command(MOTOR_1_GO, motor_turning_speed + 128);
		rlink.command(MOTOR_2_GO, motor_turning_speed + 128);
	}
    while (true){
		get_wheel_reading();
		if(front_left_sensor_reading == 1 && direction == 0)
			break;
		else if (front_right_sensor_reading == 1 && direction == 1)
			break;
	}
}

void line_following(int motor_speed){ // 000 101 return the current state
    get_state();
    if (state != previous_state){
        switch(state){
            case 1:
                motor_control(motor_speed - adjustment_power_decrement, motor_speed);
                break;
            case 2:
                motor_control(motor_speed, motor_speed - adjustment_power_decrement);
                break;
            case 4:
                motor_control(motor_speed - adjustment_power_decrement, motor_speed);
                break;
            case 5:
                motor_control(motor_speed, motor_speed - adjustment_power_decrement);
                break;
            case 6:
				if (previous_state == 1 || previous_state == 4)
					reposition(0);
				else if(previous_state == 2 || previous_state == 5)
					reposition(1);
                break;
            case 7:
				if (previous_state == 1 || previous_state == 4)
					reposition(0);
				else if(previous_state == 2 || previous_state == 5)
					reposition(1);
                break;
            default:
                motor_control(motor_speed, motor_speed);
        }
    }
    previous_state = state;
}

void crossing_action(int action_index, int turning_speed){ // 0: pass, -1: go left, 1: go right
    watch.start();
    int etime = watch.read();
    if (action_index == 0){
        rlink.command(BOTH_MOTORS_GO_OPPOSITE, motor_common_speed);
        while (etime < motor_passing_crosing_time){
            etime = watch.read();
        }
    }
    else{
        if (action_index == -1){
            motor_turn(turning_speed, 0);
        }
        else{
            motor_turn(turning_speed, 1);
        }
        while (etime < motor_pre_turing_time){
            etime = watch.read();
        }
        watch.stop();
        cout<<"time up"<<endl;
        get_wheel_reading();
        while ((front_left_sensor_reading != 1 && action_index == 1) || (front_right_sensor_reading != 1 && action_index == -1)){
            get_wheel_reading();
        }
        cout<<"turn complete"<<endl;
    }
}

void traverse(Node* destination){
	rlink.command(RAMP_TIME, 0);
	rlink.command(BOTH_MOTORS_GO_OPPOSITE, motor_common_speed);
    while (current_node -> name != destination -> name){
        int action_index = GetOperationId();
        get_state();
        while (back_sensor_reading != 1)
            line_following(motor_common_speed);
        cout<<"action:"<<action_index<<endl;
        crossing_action(action_index, motor_turning_speed);
        UpdateNode();
    }
}

void pick(void){
    
}

void pick_line_action(void){
    get_contact_switch();
    get_state();
    while (contact_switch == false){
        line_following(motor_common_speed);
        get_contact_switch();
        if (back_sensor_reading == 1)
            UpdateNode();
    }
    pick();
}

void get_some_put_signal(void){ // TO DO

}

void put(void){
    
}

void put_line_action(void){
    get_some_put_signal();
    get_state();
    while (contact_switch == false){ // CAN BE SOME OTHER CONDITION
        line_following(motor_common_speed);
        get_contact_switch();
    }
    put();
}

void TestIO(){
	//rlink.command(WRITE_PORT_3, 255);
    //rlink.command(WRITE_PORT_0, 255);
    stopwatch watch;
    watch.start();
    while(true){
			int v=rlink.command(WRITE_PORT_0,255);
			int a;
			cin>>a;
			cout << "time:" << watch.read() << "\tValue="  <<v << endl;
        ErrorHandling();
	}
}

void line_test(void){
    motor_control(motor_common_speed);
    while(true){}
}

void turn_test(int direction){
    motor_turn(motor_turning_speed, direction);
    while(true){}
}

int main ()
{
    MapInitialization();
    TaskInitialization();
    ObjectInitialization();
    FindRoute(&S2, &D6);
    int val;                              // data from microprocessor
    if (!rlink.initialise (ROBOT_NUM)) { // setup the link
        cout << "Cannot initialise link" << endl;
        rlink.print_errs("  ");
        return -1;
    }
    val = rlink.request (TEST_INSTRUCTION); // send test instruction
    if (val == TEST_INSTRUCTION_RESULT) {   // check result
        cout << "Test passed" << endl;
        //TestIO();
        traverse(&D6);
        
        /* basic loop
        traverse(&somewhere);
        pick_line_action();
        traverse(&somewhere);
        put_line_action();
        */

        return 0;                            // all OK, finish
    }
    else if (val == REQUEST_ERROR) {
        cout << "Fatal errors on link:" << endl;
        rlink.print_errs();
    }
    else
        cout << "Test failed (bad value returned)" << endl;
    return -1;                          // error, finish
}
