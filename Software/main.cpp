#include <iostream>
using namespace std;
#include <robot_instr.h>
#include <robot_link.h>
#include <stopwatch.h>
#include <robot_delay.h>
#include "autopilot.h"

#define ROBOT_NUM 10   // The id number (see below)
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
void MapInitialization(){
    cout<<"Map Initializing..."<<endl;
    // map initialization
    InitializeNode("S2", &S2, &F1, &F2, &F3, &F4);
    InitializeNode("S1", &S1, NULL, &F4, &A1, NULL);
    InitializeNode("F1", &F1, NULL, NULL, &S2, NULL);
    InitializeNode("F2", &F2, NULL, NULL, NULL, &S2);
    InitializeNode("F3", &F3, &S2, NULL, &A2, &F4);
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
    operation_list.push_back(TURN_RIGHT);
    operation_list.push_back(TURN_LEFT);
    operation_list.push_back(GO_STRAIGHT);
    operation_list.push_back(GO_STRAIGHT);
    current_node = &F3;
    previous_node = &S2;
}
void TestIO(){
    //rlink.command(WRITE_PORT_3, 255);
    stopwatch watch;
    watch.start();
    while(true){
			int v=rlink.request (READ_PORT_3);
			cout << "time:" << watch.read() << "\tValue="  <<v << endl;
        ErrorHandling();
	}
}

void get_wheel_reading(void){
	int v;
	wheel_reading = 0;
	v = rlink.request (READ_PORT_3);
	if (v bitand 0x01){
		wheel_reading += 100;
	} 
	v = v>>1;
	if (v bitand 0x01)
		wheel_reading += 10;
	v = v>>1;
	wheel_reading += v;
}

int get_state(void){
	cout<<"wheel_reading:"<<wheel_reading<<endl;
    if (wheel_reading == 10)
        return 0; // on track
    else if (wheel_reading == 11)
        return 1; // right detects the white line
    else if (wheel_reading == 110)
        return 2; // left detects the white line
    else if (wheel_reading == 111)
        return 3; // a crossing detected
    else if (wheel_reading == 1)
        return 4; // large deviation on the right
    else if (wheel_reading == 100)
        return 5; // large deviation on the left
    else if (wheel_reading == 0)
        return 6;
    else if (wheel_reading == 101)
        return 7;
    return 6;
}

void line_following(int state, int motor_speed){ // 000 101 return the current state
	if (state != previous_state){
		if (state == 1){
			rlink.command(MOTOR_1_GO, motor_speed + adjustment_power_increment);
			rlink.command(MOTOR_2_GO, motor_speed);
		}
		else if (state == 2){
			rlink.command(MOTOR_1_GO, motor_speed);
			rlink.command(MOTOR_2_GO, motor_speed + adjustment_power_increment);
		}
		else if (state == 0){
			rlink.command(BOTH_MOTORS_GO_SAME, motor_speed);
		}
		else if (state == 3){
			cout<<"crossing_detected";
		}
		else if (state == 4){
			rlink.command(MOTOR_1_GO, motor_speed + adjustment_power_increment * 2);
			rlink.command(MOTOR_2_GO, motor_speed);
		}
		else if (state == 5){
			rlink.command(MOTOR_1_GO, motor_speed);
			rlink.command(MOTOR_2_GO, motor_speed + adjustment_power_increment * 2);
		}
		else if (state == 6)
			cout<<"error: state 6"<<endl;
		else if (state == 7)
			cout<<"error: state 7"<<endl;  
	}
	cout<<"motor1: "<<rlink.request(MOTOR_1)<<endl<<"motor2: "<<rlink.request(MOTOR_2)<<endl;
    previous_state = state;
}

void crossing_action(int action_index, int turning_speed){ // 0: pass, -1: go left, 1: go right
	if (rotated == false){
		if (action_index == -1){
			rlink.command(BOTH_MOTORS_GO_OPPOSITE, turning_speed);
		}
		if (action_index == 1){
			rlink.command(MOTOR_1_GO, turning_speed + 128);
			rlink.command(MOTOR_2_GO, turning_speed);
		}
	}
			cout<<"motor1: "<<rlink.request(MOTOR_1)<<endl<<"motor2: "<<rlink.request(MOTOR_2)<<endl;
	rotated = true;
}

void traverse(Node* destination){
	rlink.command(BOTH_MOTORS_GO_SAME, 70);
    while (current_node -> name != destination -> name){
		get_wheel_reading();
        int action_index = GetOperationId();
        int state = get_state();
        while (state != 3){
            line_following(state, 70); // motor speed needs further modification
            get_wheel_reading();
            state = get_state();
        }
        while (state == 3){
			cout<<"action:"<<action_index<<endl;
            crossing_action(action_index, 50); // turning speed needs further modification
            get_wheel_reading();
            state = get_state();
        }
        while (state != 3 && action_index != 0){
            get_wheel_reading();
            state = get_state();
        }
        while (state == 3 && action_index != 0){
            rlink.command(BOTH_MOTORS_GO_SAME, 70); // motor speed needs further modification
            get_wheel_reading();
            state = get_state();
        }
        rotated = false;
        UpdateNode();
    }
}

int main ()
{
    MapInitialization();
    TaskInitialization();
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
        traverse(&C1);
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
