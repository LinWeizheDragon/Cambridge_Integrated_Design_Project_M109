#include <iostream>
using namespace std;

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

#define GO_STRAIGHT 0
#define TURN_LEFT -1
#define TURN_RIGHT 1
//list<int> operation_list; // stores operations to be executed


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
std::map<string, *Node> node_storage;
// current node
Node* current_node;
Node* previous_node;
