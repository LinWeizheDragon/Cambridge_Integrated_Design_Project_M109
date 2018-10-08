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
// current node
Node* current_node;
Node* previous_node;
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
// clamp control
void ClampControl(****);
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
**** //some features like color/weight
string target; //target node name
}
Object* current_object=NULL;
```
### Methods
```
// return object pointer
Object* ObjectRecognition();
```

## LED Control
### definition
```
#define COLOR_NAME color_value
```
### Methods
```
void SetLed(****);
```

## Failure Detection and Processing
TODO
