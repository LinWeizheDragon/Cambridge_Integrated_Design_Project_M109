
#include <iostream>
#include <map>
using namespace std;

#define RECOGNITION_SAMPLE_NUMBER 100

class Object{
public:
    string name = "None";
    Node* target = NULL;
};

Object OBJECT_UNKNOWN, OBJECT_RED, OBJECT_WHITE, OBJECT_GREEN, OBJECT_WOOD, OBJECT_TRANS;
Object* current_object;

/*
 Function to recognize the object
 Get RECOGNITION_SAMPLE_NUMBER samples and take the most frequent one.
 */
Object* ObjectRecognition(list<int> params, int mean_distance, int stack_num){
    // map to store samples
    map<Object*, int>samples;
    
    // list to store predefined objects
    list<Object*> object_list;
    object_list.push_back(&OBJECT_UNKNOWN);
    object_list.push_back(&OBJECT_WHITE);
    object_list.push_back(&OBJECT_TRANS);
    object_list.push_back(&OBJECT_WOOD);
    object_list.push_back(&OBJECT_GREEN);
    object_list.push_back(&OBJECT_RED);
    for (list<Object*>::iterator it = object_list.begin(); it != object_list.end(); it++)
    {
        Object* this_object = (*it);
        samples[this_object] = 0; // default value is 0
        cout<<(*it)->name<<endl;
    }
    cout<<"mean distance:"<<mean_distance<<endl;
    for (list<int>::iterator it = params.begin(); it != params.end(); it++){
		int v = (*it);
		cout<<v<<" ";
		bool found=false;
		if (v >= 30 && v<=70){
			// white or wood
			if (stack_num == 1){
				if (mean_distance>=155){
					samples[&OBJECT_WOOD] +=1;
				}else{
					samples[&OBJECT_WHITE] +=1;
				}
			}else{
				if (mean_distance<=135){
					samples[&OBJECT_WOOD] +=1;
				}else{
					samples[&OBJECT_WHITE] +=1;
				}
			}
			found = true;
		}
		if (v >=150){
			samples[&OBJECT_GREEN] +=1;
			found = true;
		}
		if (v>=70 && v<=150){
			//red or transparent
			if (stack_num == 1){
				if (mean_distance>=149){
					samples[&OBJECT_RED] +=1;
				}else{
					samples[&OBJECT_TRANS] +=1;
				}
			}else{
				if (mean_distance>=149){
					samples[&OBJECT_RED] +=1;
				}else{
					samples[&OBJECT_TRANS] +=1;
				}
			}
			found = true;
		}
		// unknown
		if (!found){
			samples[&OBJECT_UNKNOWN] += 1;
		}
		/*
		if (v >=170){
			samples[&OBJECT_UNKNOWN] += 1;
		}
		else if (v < 150 && v >= 135) {
			samples[&OBJECT_GREEN] += 1;
		}
		else if (v < 120 && v >= 106){
			//cout<<samples[&OBJECT_RED]<<endl;
			samples[&OBJECT_RED] += 1;
		}
		else if (v < 135 && v >= 120){
			samples[&OBJECT_TRANS] += 1;
		}
		else if (v < 88 && v >= 66){
			//cout<<samples[&OBJECT_WHITE]<<endl;
			samples[&OBJECT_WHITE] += 1;
		}
		else if (v < 106 && v >= 88){
			samples[&OBJECT_WOOD] += 1;
		}
		else{
			samples[&OBJECT_UNKNOWN] += 1;
		}*/
	}
    
    
    
    // find the most frequent result
    int max=0;
    Object* max_object=&OBJECT_UNKNOWN;
    for (list<Object*>::iterator it = object_list.begin(); it != object_list.end(); it++)
    {
        Object* this_object = (*it);
        cout<<this_object->name<<samples[this_object]<<endl;
        if (samples[this_object] > max){
            max = samples[this_object];
            max_object = this_object;
        }
    }
    cout<<"Object Recognition: max: "<<max<< " of "<<max_object->name<<endl;
    return max_object;
}
