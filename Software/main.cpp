#include <iostream>
using namespace std;
#include <robot_instr.h>
#include <robot_link.h>
#include <stopwatch.h>
#include <robot_delay.h>
#include "autopilot.h"
#include "led_control.h"
#include "clamp_control.h"
#include "object_recognition.h"
#include <list>

#define ROBOT_NUM 10   // The id number (see below)
int motor_turning_speed = 75;
int adjustment_power_decrement = 5;
int motor_common_speed = 127; //127 max
int motor_passing_crosing_time = 300;
int motor_pre_turing_time = 2600;
int motor_middle_turing_time = 2700;
int state = 0;
int item_to_pick_1 = 5;
int item_picked = 0;
int crossings_passed = 0;

string spcial_crossings[12] = {"D8", "E8", "E6", "E5", "E4", "E3", "E2", "E1", "D1", "C1", "B1", "A1"};

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
    
    InitializeNode("A2", &A2, &S2, &A5, &B2, &A1);
    InitializeNode("A1", &A1, &S1, &A2, &B1, NULL);
    InitializeNode("A5", &A5, NULL, &A6, &B5, &A2);
    InitializeNode("A6", &A6, NULL, &A7, &B6, &A5);
    InitializeNode("A7", &A7, NULL, &A8, &B7, &A6);
    InitializeNode("A8", &A8, NULL, NULL, &B8, &A7);
    
    InitializeNode("B2", &B2, &A2, &B5, &C2, &B1);
    InitializeNode("B1", &B1, &A1, &B2, &C1, NULL);
    InitializeNode("B5", &B5, &A5, &B6, &D5, &B2);
    InitializeNode("B6", &B6, &A6, &B7, &D6, &B5);
    InitializeNode("B7", &B7, &A7, &B8, &D7, &B6);
    InitializeNode("B8", &B8, &A8, NULL, &D8, &B7);
    
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
    task_list.push_back(TASK_WAITING);
    task_list.push_back(TASK_GOTO_E7);
    task_list.push_back(TASK_SCAN_A);
    
    //operation list initialization
    
    /*operation_list.push_back(TURN_RIGHT);
    operation_list.push_back(TURN_RIGHT); 
    operation_list.push_back(TURN_LEFT);
    operation_list.push_back(GO_STRAIGHT);
    operation_list.push_back(GO_STRAIGHT); 
    operation_list.push_back(GO_STRAIGHT); 
    operation_list.push_back(GO_STRAIGHT); 
    operation_list.push_back(GO_STRAIGHT); 
    operation_list.push_back(GO_STRAIGHT); 
    operation_list.push_back(GO_STRAIGHT); 
    //operation_list.push_back(GO_STRAIGHT);//*/
    
    ///////////Settings here////////////////////
    current_node = &D7;
    previous_node = &B7;
    current_direction.direction = RIGHT;
    /*
    current_direction.direction = RIGHT;
    FindRoute(&S2, &E7);
    current_direction.direction = RIGHT;
    FindRoute(&E7, &E1);
    cout<<"Route: ";
    for (list<int>::iterator iter = operation_list.begin(); iter != operation_list.end(); iter++)
    {
        cout << (*iter) << " " ;
    }//*/
    ////////////////////////////////////////////
    
    
    cout<<"Total Operations = "<<operation_list.size()<<endl;
    cout<<"Task Initialization completed."<<endl;
}

void ObjectInitialization(){
    //Object Initialization
    InitializeObject("red", &OBJECT_RED, &B5);
    InitializeObject("white", &OBJECT_WHITE, &B6);
    InitializeObject("green", &OBJECT_GREEN, &B5);
    InitializeObject("wood", &OBJECT_WOOD, &B6);
    InitializeObject("transparent", &OBJECT_TRANS, &B6);
    InitializeObject("unknown", &OBJECT_UNKNOWN, NULL);
 
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
            else
				state = -1; // head on track but body off track
        }
        else if (middle_sensor_reading == 1)
            state = 2; // deviation towards right
        else
			state = 5; // large deviation towards right
    }
    else if (front_right_sensor_reading == 1){
        if (middle_sensor_reading == 1)
            state = 1; // deviation towards left
        else 
        state = 4; // large deviation towards left
    }
    else if (middle_sensor_reading == 1){
        state = 6; // large deviation
    }
    else
    state = 7; // very large deviation
}

void motor_control(int left_wheel_power, int right_wheel_power){
    if (left_wheel_power < 128){
        rlink.command(MOTOR_1_GO, left_wheel_power - 18);
        rlink.command(MOTOR_2_GO, right_wheel_power + 128);
    }
    else{
        rlink.command(MOTOR_1_GO, left_wheel_power);
        rlink.command(MOTOR_2_GO, right_wheel_power - 128);
    }
}

void motor_turn(int speed, int direction){ // -1 for left, 1 for right
	if (direction == -1){
        rlink.command(MOTOR_1_GO, speed);
        rlink.command(MOTOR_2_GO, speed + 18);
	}
	else if (direction == 1){
        rlink.command(MOTOR_1_GO, speed + 128 + 11);
        rlink.command(MOTOR_2_GO, speed + 128);
	}
	else
		cout << "turning direction error";
}

void hault(void){
        rlink.command(MOTOR_1_GO, 0);
        rlink.command(MOTOR_2_GO, 0);
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

void line_following(int motor_speed, int mode, int adjustment){ // mode 0 for normal, 1 for strict, 2 for back
    get_state();
    if (mode == 2)
        motor_speed += 128;
    if (state != previous_state){
		switch(state){
            case 1:
                if (mode == 0)
                    motor_control(motor_speed - adjustment, motor_speed);
                else
                    reposition(0);
                break;
            case 4:
                reposition(0);
                break;
            case 2:
                if (mode == 0)
                    motor_control(motor_speed, motor_speed - adjustment);
                else
                    reposition(1);
                break;
            case 5:
                reposition(1);
                break;
            case 6:
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

bool find(string crossing){
    for (int a = 0; a < 12; a++){
        if (spcial_crossings[a] == crossing)
            return true;
    }
    return false;
}

void crossing_action(int action_index, int turning_speed){ // 0: pass, -1: go left, 1: go right
	if (action_index != 0){
		LedDisplayOperation(LED_TURNING, true);
		rlink.command(WRITE_PORT_7, LedReading());
	}
    watch.start();
    int etime = watch.read();
    if (action_index == 0){
        motor_control(motor_common_speed, motor_common_speed);
        while (etime < motor_passing_crosing_time){
            etime = watch.read();
        }
    }
    else{
        motor_turn(turning_speed, action_index);
        if (current_node -> name == "A1")
			while (etime < motor_pre_turing_time + (action_index+1) * 600)
				etime = watch.read();
        else
			while (etime < motor_pre_turing_time)
				etime = watch.read();
		watch.stop();
		get_wheel_reading();
		while ((front_left_sensor_reading == 0 && action_index == 1) || (front_right_sensor_reading == 0 && action_index == -1))
			get_wheel_reading();
		motor_control(motor_common_speed, motor_common_speed);
		while (back_sensor_reading == 1)
			get_wheel_reading();
        cout<<"turn complete"<<endl;
    }
    LedDisplayOperation(LED_TURNING, false);
	rlink.command(WRITE_PORT_7, LedReading());
}

void traverse(){
	rlink.command(RAMP_TIME, 0);
	rlink.command(BOTH_MOTORS_GO_OPPOSITE, motor_common_speed);
	LedDisplayOperation(LED_FOLLOWING_LINE, true);
	rlink.command(WRITE_PORT_7, LedReading());
    while (!operation_list.empty()){
        int action_index = GetOperationId();
        get_wheel_reading();
        while (back_sensor_reading != 1)
            line_following(motor_common_speed, 0, adjustment_power_decrement);
        cout<<"action:"<<action_index<<endl;
        crossing_action(action_index, motor_turning_speed);
        UpdateNode();
    }
    LedDisplayOperation(LED_FOLLOWING_LINE, false);
	rlink.command(WRITE_PORT_7, LedReading());
}

void DetectObject(int num){
    /*
     This function recognize the object and setup LED display.
     */
    cout<<"Object Recognition: starting..."<<endl;
    list<int> params;
    int mean_distance=0;
    for (int i = 0; i<RECOGNITION_SAMPLE_NUMBER; i++){
        params.push_back(rlink.request(ADC1));
        mean_distance += rlink.request(ADC2);
    }
    mean_distance = mean_distance / RECOGNITION_SAMPLE_NUMBER;
    // change current object
    current_object = ObjectRecognition(params, mean_distance, num);
    cout<<"Object Recognition: Displaying..."<<endl;
    string name = current_object->name;
    if (name == "red"){
		LedDisplayObject(LED_OBJECT_RED);
	}
	if (name == "green"){
		LedDisplayObject(LED_OBJECT_GREEN);
	}
	if (name == "red"){
		LedDisplayObject(LED_OBJECT_RED);
	}
	if (name == "white"){
		LedDisplayObject(LED_OBJECT_WHITE);
	}
	if (name == "wood"){
		LedDisplayObject(LED_OBJECT_WOOD);
	}
	if (name == "transparent"){
		LedDisplayObject(LED_OBJECT_TRANS);
	}
	if (name == "unknown"){
		LedDisplayObject(LED_OBJECT_NONE);
	}
	
    // display LED
    rlink.command(WRITE_PORT_7, LedReading());
    cout<<"Object Recognition: finished."<<endl;
}

void pick(void){
    
}

void pick_line_action(void){
	bool distance_sensor_activated = false;
	clamp.OpenClamp();
	int v = rlink.request(READ_PORT_0);
	rlink.command(WRITE_PORT_0, clamp.GetReading(v));
	clamp.ExtendArm();
	v = rlink.request(READ_PORT_0);
	rlink.command(WRITE_PORT_0, clamp.GetReading(v));

    while (!operation_list.empty()){
		get_wheel_reading();
		if (clamp.clamp_status == CLAMP_OPEN){
			int distance_value = -1;
			bool found = false;
			if (distance_sensor_activated == true){
				
				
				if (crossings_passed > item_picked){
					cout<<"detect a junction, do it"<<endl;
					found = true;
					item_picked += 1;
				}
				if (item_picked == 5)
					item_picked = 0;
				
				distance_value = rlink.request(ADC2);
				cout<<"distance: "<<distance_value<<endl;
				if (distance_value < 155){
					int mean_value = 0;
					for (int i = 0; i < 50; i++){
						mean_value += rlink.request(ADC2);
					}
					mean_value = mean_value / 50;
					cout<<"mean value: "<<mean_value<<endl;
					if (mean_value < 155){
						cout<<"object found!"<<endl;
						found = true;
					}
				}
				if (found){
					hault();
					clamp.CloseClamp();
					v = rlink.request(READ_PORT_0);
					rlink.command(WRITE_PORT_0, clamp.GetReading(v));
					delay(3000);
					clamp.ShrinkArm();
					v = rlink.request(READ_PORT_0);
					rlink.command(WRITE_PORT_0, clamp.GetReading(v));
					delay(5000);
					DetectObject(1);
				}
				
			}
		}
		
	
        int action_index = GetOperationId();
        get_wheel_reading();
        line_following(motor_common_speed, 0, adjustment_power_decrement);
		if (back_sensor_reading == 1){
			crossing_action(action_index, motor_turning_speed);
			UpdateNode();
			if (distance_sensor_activated == true){
				crossings_passed += 1;
			}
		}
        if (current_node->name == "E5" && distance_sensor_activated == false){
			cout<<"distance sensor activated!"<<endl;
			distance_sensor_activated = true;
			crossings_passed += 1;
		}
	}
}

void get_some_put_signal(void){ // TO DO
    
}

void put(void){
    
}
/*
void put_line_action(void){
    get_some_put_signal();
    get_state();
    while (contact_switch == false){ // CAN BE SOME OTHER CONDITION
        line_following(motor_common_speed, 1, adjustment_power_decrement);
        //get_contact_switch();
    }
    put();
}
*/

void TestIO(){
    //rlink.command(WRITE_PORT_0, 0);
    stopwatch watch;
    watch.start();
    while(true){
		int a;
		cin>>a;
		for (int i = 0; i<a; i++){
			int v=rlink.request (ADC2);
			cout << "time:" << watch.read() << "\tValue="  <<v << endl;
		}
        ErrorHandling() ;
	}
}


    
void line_test(void){
    motor_control(motor_common_speed, motor_common_speed);
    while(true){}
}

void turn_test(int direction){
    motor_turn(motor_turning_speed, direction);
    while(true){}
}

void TestPick(){
	clamp.OpenClamp();
    int v = rlink.request(READ_PORT_0);
    rlink.command(WRITE_PORT_0, clamp.GetReading(v));
    delay(1000);
    
    clamp.ExtendArm();
    v = rlink.request(READ_PORT_0);
    rlink.command(WRITE_PORT_0, clamp.GetReading(v));
    
    int distance_value = -1;
    bool found = false;
    while (!found){
		get_wheel_reading();
		if (back_sensor_reading == 1 && clamp.clamp_status == CLAMP_OPEN){
			cout<<"detect a junction, do it"<<endl;
			found = true;
		}
        distance_value = rlink.request(ADC2);
        cout<<"distance: "<<distance_value<<endl;
        if (distance_value < 155){
            int mean_value = 0;
            for (int i = 0; i < 200; i++){
                mean_value += rlink.request(ADC2);
            }
            mean_value = mean_value / 200;
            cout<<"mean value: "<<mean_value<<endl;
            if (mean_value < 155){
                cout<<"object found!"<<endl;
                found = true;
            }
        }
    }
    if (found){
        clamp.CloseClamp();
        v = rlink.request(READ_PORT_0);
        rlink.command(WRITE_PORT_0, clamp.GetReading(v));
        delay(3000);
        clamp.ShrinkArm();
        v = rlink.request(READ_PORT_0);
        rlink.command(WRITE_PORT_0, clamp.GetReading(v));
        delay(5000);
        DetectObject(1);
    }
}

int main ()
{
    MapInitialization();
    TaskInitialization();
    ObjectInitialization();

    int val;                              // data from microprocessor
    if (!rlink.initialise (ROBOT_NUM)) { // setup the link
        cout << "Cannot initialise link" << endl;
        rlink.print_errs("  ");
        return -1;
    }
    val = rlink.request (TEST_INSTRUCTION); // send test instruction
    if (val == TEST_INSTRUCTION_RESULT) {   // check result
        cout << "Connected..." << endl;
        //TestPick();
        //traverse();
        
        int v=rlink.request (READ_PORT_0);
        clamp.OpenClamp();
        clamp.ShrinkArm();
        rlink.command(WRITE_PORT_0, clamp.GetReading(v));

        delay(3000);
        int a;
        cin>>a;
		         //*/
        //TestPick();
        //TestIO();
        
        while (true){
            if (operation_list.empty()){
				cout<<"Next Task!!!!!"<<endl;
                NextTask();
                // no next operation
                if (GetTaskId() != -1){
                    // init next task
                    InitNextTask(GetTaskId());
                    cout<<"New Task ready"<<endl;
                }else{
                    // finish all tasks
                    return 0;
                }
            }else{
                // do traverse until operation_list is empty.
                if (scan_mode == MODE_NOT_SCANNING){
					cout<<"start traverse"<<endl;
					traverse();
				}
				else if (scan_mode == MODE_SCANNING)
					pick_line_action();
					
                rlink.command(BOTH_MOTORS_GO_OPPOSITE, 0);
                // TODO: pickup and place logic here
            }
        }
        
        //TestIO();
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
