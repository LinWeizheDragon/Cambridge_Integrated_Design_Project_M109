#include <iostream>
#include <map>
#include <list>
#include <robot_delay.h>
#include <cstdlib>

using namespace std;

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

//Task List Definition
#define TASK_SCAN_A 1
#define TASK_SCAN_B 2
#define TASK_RETRACK 3
#define TASK_WAITING 0

list<int> task_list; // store tasks to be executed

//Operation List Definition
#define GO_STRAIGHT 0
#define TURN_LEFT -1
#define TURN_RIGHT 1
#define TURN_BACK 2

//Error Code Definition
#define ERROR_LOSE_WAY 0
#define ERROR_PICKUP_FAIL 1

list<int> operation_list; // stores operations to be executed

//Error Code Definition
#define ERROR_LOSE_WAY 0
#define ERROR_PICKUP_FAIL 1

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
            cout<<"up is:"<<this->up->name<<endl;
        }
        if (this->down!= NULL){
            cout<<"down is:"<<this->down->name<<endl;
        }
        cout<<endl;
    }
};
class Direction{
public:
    int direction = RIGHT;
    void updateDirection(int x){
        this->direction+=x;
        if (this->direction < 0)
            this->direction += 4;
        if (this->direction > 3)
            this->direction -= 4;
        //-1 to turn left, 1 to turn right
    }
    int getDirection(){
        return this->direction;
    }
};
Node S1, S2, A1, A2, A5, A6, A7, A8, B1, B2, B5, B6, B7, B8;
Node F1, F2, F3, F4;
Node C1, C2;
Node D1, D2, D3, D4, D5, D6, D7, D8;
Node E1, E2, E3, E4, E5, E6, E7, E8;

// a map of Nodes
map<string, Node*>node_storage;
// current node
Node* current_node;
Node* previous_node;

// current direction
Direction current_direction;
int GetTaskId(){
    if (!task_list.empty()){
        return task_list.front();
    }else{
        return -1;
    }
}
void NextTask(){
    if (!task_list.empty()){
        task_list.pop_front();
    }
}
int GetOperationId(){
    if (!operation_list.empty()){
        return operation_list.front();
    }else{
        return 0;
    }
}
void NextOperation(){
    if (!operation_list.empty()){
        operation_list.pop_front();
    }
}
void UpdateNode(){
    int operation_id = -1;
    if (!operation_list.empty()){
        operation_id = operation_list.front();
    }
    cout<<"Direction changing from "<<current_direction.getDirection();
    current_direction.updateDirection(operation_id);
    cout<<" to "<<current_direction.getDirection()<<endl;
    
    cout<<"Previous Node changing from "<<previous_node->name;
    previous_node = current_node;
    cout<<" to "<<previous_node->name<<endl;
    
    cout<<"Current Node changing from "<<current_node->name;
    switch(current_direction.getDirection()){
        case LEFT:
            current_node = current_node->left;
            break;
        case UP:
            current_node = current_node->up;
            break;
        case RIGHT:
            current_node = current_node->right;
            break;
        case DOWN:
            current_node = current_node->down;
            break;
    }
    cout<<" to "<<current_node->name<<endl;
    NextOperation();
}


int front_left_sensor_reading, front_right_sensor_reading, middle_sensor_reading, back_sensor_reading;
int previous_state = 0;
bool crossing_detected = false;
bool turning_process = false;
int motor_turning_time = 1000;
int motor_pre_turing_time = 1500;

list< list<Node*> > queue;


static int min_steps = 0;
static int min_turns = 999;
static list<int> min_op_list;

bool VerrifyQueue(list<Node*> target_list){
    
    list<int> op_list;
    
    // check whether there are duplicated nodes.
    list<Node*> check_duplication_list = target_list;
    check_duplication_list.sort();
    check_duplication_list.unique();
    if (check_duplication_list.size() != target_list.size()){
        return false;
    }
    
    Node* last_node=NULL;
    Direction direction;
    direction.direction = current_direction.direction;
    for (list<Node*>::iterator it = target_list.begin(); it != target_list.end(); it++)
    {
        if (last_node == NULL){
            last_node = (*it);
        }else{
            //cout<<"last node: "<<last_node->name<<endl;
            //cout<<"current node: "<<(*it)->name<<endl;
            
            int new_direction=-1;
            if (last_node->right != NULL && last_node->right->name == (*it)->name){
                new_direction = RIGHT;
            }
            if (last_node->left != NULL && last_node->left->name == (*it)->name){
                new_direction = LEFT;
            }
            if (last_node->up != NULL && last_node->up->name == (*it)->name){
                new_direction = UP;
            }
            if (last_node->down != NULL && last_node->down->name == (*it)->name){
                new_direction = DOWN;
            }
            //cout<<"new direction "<<new_direction<<endl;
            if (new_direction!=-1){
                int operation_value = new_direction - direction.direction;
                if (operation_value < -2)
                    operation_value += 4;
                if (operation_value > 2)
                    operation_value -= 4;
                if (operation_value == -2)
                    operation_value = 2;
                //cout<<"new operation: "<<operation_value<<" at "<<last_node->name<<endl;
                
                // operations that can't be conducted
                if (operation_value == TURN_LEFT && last_node->name == "A1" && new_direction == RIGHT){
                    return false;
                }
                if (operation_value == TURN_LEFT && last_node->name == "B1" && new_direction == RIGHT){
                    return false;
                }
                if (operation_value == TURN_LEFT && last_node->name == "C1" && new_direction == RIGHT){
                    return false;
                }
                if (operation_value == TURN_LEFT && last_node->name == "D1" && new_direction == RIGHT){
                    return false;
                }
                op_list.push_back(operation_value);
                direction.direction = new_direction;
            }
            last_node = (*it);
        }
    }
    
    int turn_counter = 0;
    for (list<int>::iterator iter = op_list.begin(); iter != op_list.end(); iter++)
    {
        turn_counter += abs((*iter));
    }
    cout<<"turn_counter:"<<turn_counter<<endl;
    if (turn_counter < min_turns){
        min_steps = (int)target_list.size();
        min_op_list = op_list;
        min_turns = turn_counter;
    }
    return true;
}
void BFS(Node* to_node){
    bool found = false;
    
    // init the minimum step storage
    min_op_list.clear();
    min_steps=0;
    min_turns=999;
    
    while (!found){
        //cout<<"examining "<<queue.front().back()->name<<endl;
        if (queue.front().back()->name == to_node->name){
            cout<<"found result: ";
            for (list<Node*>::iterator it = queue.front().begin(); it != queue.front().end(); it++)
            {
                cout << (*it)->name << " " ;
            }
            cout << endl;
            
            
            if (queue.front().size() > min_steps && min_op_list.size() != 0){
                found = true;
            }
            if (VerrifyQueue(queue.front())){
                cout<<"valid solution"<<endl;
            }else{
                cout<<"invalid solution"<<endl;
            }
            queue.pop_front();
        }else{
            Node* this_node = queue.front().back();
            if (this_node->left != NULL){
                //cout<<"add "<<this_node->left->name<<endl;
                list<Node*> new_queue = queue.front();
                new_queue.push_back(this_node->left);
                queue.push_back(new_queue);
            }
            if (this_node->right!= NULL){
                //cout<<"add "<<this_node->right->name<<endl;
                list<Node*> new_queue = queue.front();
                new_queue.push_back(this_node->right);
                queue.push_back(new_queue);
            }
            if (this_node->up!= NULL){
                //cout<<"add "<<this_node->up->name<<endl;
                list<Node*> new_queue = queue.front();
                new_queue.push_back(this_node->up);
                queue.push_back(new_queue);
            }
            if (this_node->down!= NULL){
                //cout<<"add "<<this_node->down->name<<endl;
                list<Node*> new_queue = queue.front();
                new_queue.push_back(this_node->down);
                queue.push_back(new_queue);
            }
            //cout<<"delete "<<queue.front().back()->name<<endl;
            queue.pop_front();
        }
    }
    cout<<"SOLUTION:"<<endl;
    for (list<int>::iterator iter = min_op_list.begin(); iter != min_op_list.end(); iter++)
    {
        cout << (*iter) << " " ;
        operation_list.push_back(*iter);
    }
    cout << endl;
    cout<<operation_list.size()<<endl;
}
void FindRoute(Node* from_node, Node* to_node){
    list<Node*> init_list;
    init_list.push_back(from_node);
    queue.clear();
    queue.push_back(init_list);
    BFS(to_node);
    
}
