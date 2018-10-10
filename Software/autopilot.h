#include <iostream>
#include <map>
#include <list>
#include <robot_delay.h>
using namespace std;

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

#define GO_STRAIGHT 0
#define TURN_LEFT -1
#define TURN_RIGHT 1
list<int> operation_list; // stores operations to be executed


class Node{
public:
    string name="none";
    Node* left=NULL;
    Node* up=NULL;
    Node* right=NULL;
    Node* down=NULL;
    void print(){
        cout<<"name: "<<this->name<<endl;
        if (this->left != NULL){
            cout<<"left is:"<<this->left->name<<endl;
        }
        if (this->right!= NULL){
            cout<<"right is:"<<this->right->name<<endl;
        }
        if (this->up!= NULL){
            cout<<"up is:"<<this->right->name<<endl;
        }
        if (this->down!= NULL){
            cout<<"down is:"<<this->right->name<<endl;
        }
        cout<<endl;
    }
};
class Direction{
public:
    int direction = UP;
    void updateDirection(int x){
        this->direction+=x;
        if (this->direction < 0)
            this->direction = LEFT;
        if (this->direction > 3)
            this->direction = UP;
        //-1 to turn left, 1 to turn right
    }
    int getDirection(){
        return this->direction;
    }
};

// a map of Nodes
map<string, Node*>node_storage;
// current node
Node* current_node;
Node* previous_node;
int wheel_reading;
int adjustment_power_increment = 0;
bool crossing_detected = false;
bool turning_process = false;

int get_state(void){
    if (wheel_reading == 010)
        return 0; // on track
    if (wheel_reading == 011)
        return 1; // right detects the white line
    if (wheel_reading == 110)
        return 2; // left detects the white line
    if (wheel_reading == 111)
        return 3; // a crossing detected
    if (wheel_reading == 001)
        return 4; // large deviation on the right
    if (wheel_reading == 100)
        return 5; // large deviation on the left
    if (wheel_reading == 000)
        return 6;
    if (wheel_reading == 101)
        return 7;
}

void line_following(int state, int motor_speed){ // 000 101 return the current state
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
        crossing_detected = true;
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

void crossing_action(int action_index, int turning_speed){ // 0: pass, -1: go left, 1: go right
    if (action_index == -1){
        rlink.command(BOTH_MOTORS_GO_OPPOSITE, turning_speed);
    }
    if (action_index == 1){
        rlink.command(MOTOR_1_GO, turning_speed + 128);
        rlink.command(MOTOR_2_GO, turning_speed);
    }
}

void traverse(Node* destination){
    while (current_node -> name != destination -> name){
        int action_index = GetOperationId();
        int state = get_state();
        while (state != 3){
            line_following(state, 10); // motor speed needs further modification
            state = get_state();
        }
        while (state == 3){
            crossing_action(action_index, 10); // turning speed needs further modification
            state = get_state();
        }
        while (state != 3){
            crossing_action(action_index, 10); // turning speed needs further modification
            state = get_state();
        }
        while (state == 3){
            rlink.command(BOTH_MOTORS_GO_SAME, 10); // motor speed needs further modification
            state = get_state();
        }
        UpdateNode();
        NextOperation();
    }
}
