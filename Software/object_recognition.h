
#include <iostream>

using namespace std;

class Object{
public:
    string name = "None";
    Node* target = NULL;
};

Object OBJECT_RED, OBJECT_WHITE, OBJECT_GREEN, OBJECT_WOOD, OBJECT_TRANS;
Object* current_object;

Object* ObjectRecognition(int param){
    // if else selection, return Object*
    return &OBJECT_RED;
}
