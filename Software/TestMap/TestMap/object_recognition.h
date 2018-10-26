#include <iostream>

using namespace std;
#include <map>

#define RECOGNITION_SAMPLE_NUMBER 100
class Object{
public:
    string name = "None";
    Node* target = NULL;
};

Object OBJECT_UNKOWN, OBJECT_RED, OBJECT_WHITE, OBJECT_GREEN, OBJECT_WOOD, OBJECT_TRANS;


/*
 Function to recognize the object
 Get RECOGNITION_SAMPLE_NUMBER samples and take the most frequent one.
 */
Object* ObjectRecognition(int param){
    // map to store samples
    map<Object*, int>samples;
    
    // list to store predefined objects
    list<Object*> object_list;
    object_list.push_back(&OBJECT_UNKOWN);
    object_list.push_back(&OBJECT_WHITE);
    object_list.push_back(&OBJECT_TRANS);
    object_list.push_back(&OBJECT_WOOD);
    object_list.push_back(&OBJECT_GREEN);
    object_list.push_back(&OBJECT_RED);
    for (list<Object*>::iterator it = object_list.begin(); it != object_list.end(); it++)
    {
        Object* this_object = (*it);
        samples[this_object] = 0; // default value is 0
    }
    
    
    
    
    // find the most frequent result
    int max=0;
    Object* max_object=&OBJECT_UNKOWN;
    for (list<Object*>::iterator it = object_list.begin(); it != object_list.end(); it++)
    {
        Object* this_object = (*it);
        if (samples[this_object] > max){
            max = samples[this_object];
            max_object = this_object;
        }
    }
    cout<<"ObjectRecognition: max: "<<max<< " of "<<max_object->name<<endl;
    return max_object;
}
