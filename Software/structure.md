# Code Structure

## Mapping system
### definition
```
class Node{
public:
string name="none";
Node* left=NULL;
Node* up=NULL;
Node* right=NULL;
Node* down=NULL;
}
#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3
class Direction{
public:
int direction = UP;
void updateDirection(int x);//-1 to turn left, 1 to turn right
int getDirection();
}
```
### storage
```
// a map of Nodes
map<string, *Node> node_storage;
```
## Operation/Task System
### Positioning
```
// current node
Node* current_node;
// previous node
Node* previous_node;
```
### Task
```
//Task List Definition
#define TASK_SCAN_A 1
#define TASK_SCAN_B 2
#define TASK_RETRACK 3
#define TASK_WAITING 0

list<int> task_list; // store tasks to be executed

int GetTaskId();
void NextTask();
```
### Operation
```
//Operation List Definition
#define GO_STRAIGHT 0
#define TURN_LEFT -1
#define TURN_RIGHT 1
#define TURN_BACK 2

list<int> operation_list; // stores operations to be executed

int GetOperationId();
void NextOperation();
void UpdateNode();
```
### find route
```
#define GO_STRAIGHT 0
#define TURN_LEFT -1
#define TURN_RIGHT 1
list<int> operation_list; // stores operations to be executed
```
returns a list of operations
```
list<FieldMap *> findRoute(string from, string to)
```

## Moving Control
### Definition
```
// control speed of two motors
SetThrottle(int left_motor, int right_motor)
```
## Line Following
### Methods
```
// loop inside this function, following the line
void FollowLine();
```
When a crossing is detected:
```
// turn or go straight according to operations list
void ExecuteOperation();
```

## Object Picking/Placing
### Clamp
```
#define ARM_SHRINK 0
#define ARM_EXTENDED 1
#define CLAMP_OPEN 0
#define CLAMP_CLOSED 1

class Clamp(){
public:
int arm_status = ARM_SHRINK;
int clamp_status = CLAMP_CLOSED;
void ShrinkArm();
void ExtendArm();
void CloseClamp();
void OpenClamp();
}
Clamp clamp;
```
### Picking
```
// some operations before pick-up
void PrePick();
// some operations after pick-up
void PostPick();
```
### Placing
```
// some operations before placing
void PrePlace();
// some operations after placing
void PostPlace();
```
## Object Recognition
### definition
```
class Object{
public:
string name = "None";
Node* target = NULL;
};

Object OBJECT_RED, OBJECT_WHITE, OBJECT_GREEN, OBJECT_WOOD, OBJECT_TRANS;
Object* current_object; // store current object
```
### Methods
```
// return object pointer
Object* ObjectRecognition(int param)
```

## LED Control
### definition
```
// LED Definition
#define LED_FOLLOWING_LINE 0x01
#define LED_TURNING 0x02
#define LED_ADJUSTING 0x04
#define LED_RECOVERING 0x08
#define LED_SCAN_C2 0x10
#define LED_SCAN_C1 0x20
#define LED_GOTO_D1 0x40
#define LED_GOTO_D2 0x80

#define LED_OBJECT_RED 0x01
#define LED_OBJECT_WHITE 0x02
#define LED_OBJECT_GREEN 0x04
#define LED_OBJECT_WOOD 0x08
#define LED_OBJECT_TRANS 0x10
```
### Methods
```
// read operation led value
int LedReadingOperation();
// read object led value
int LedReadingObject();
// read set led display
void LedDisplayOperation(int led_type, bool led_switch);
void LedDisplayObject(int led_type, bool led_switch);
```

## Failure Detection and Processing
TODO
