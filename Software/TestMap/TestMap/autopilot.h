#include <iostream>
#include <map>
#include <list>
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
Node S1, S2, A1, A2, B1, B2, B5, B6, B7, B8;
Node F1, F2, F3, F4;
Node C1, C2;
Node D1, D2, D3, D4, D5, D6, D7, D8;
Node E1, E2, E3, E4, E5, E6, E7, E8;

// a map of Nodes
map<string, Node*>node_storage;
// current node
Node* current_node;
Node* previous_node;
