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
int adjust_time = 0;

void line_following(void){
    int temp_speed = speed=rlink.request(MOTOR_1);
    if (wheel_reading == 011){ // right detects the white line
        rlink.command(MOTOR_1_GO, temp_speed + adjustment_power_increment);
        delay (adjust_time);
        rlink.command(MOTOR_1_GO, temp_speed);
    }
    if (wheel_reading == 110){ // left detects the white line
        rlink.command(MOTOR_2_GO, temp_speed + adjustment_power_increment);
        delay (adjust_time);
        rlink.command(MOTOR_2_GO, temp_speed);
    }
}
