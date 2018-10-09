//
//  main.cpp
//  TestMap
//
//  Created by 林炜哲 on 06/10/2018.
//  Copyright © 2018 Dragon. All rights reserved.
//

#include <iostream>
#include "autopilot.h"
using namespace std;


void InitializeNode(string name, Node* target, Node* left, Node* up, Node* right, Node* down){
    cout<<up<<endl;
    target->name = name;
    target->left = left;
    target->up = up;
    //cout<<(string)typeof(target->up) <<endl;
    target->right = right;
    target->down = down;
}

int main(int argc, const char * argv[]) {
    A2.name = "A2";
    
    //////////////////////////left//up//right/down//////
    cout<<&S2<<endl;
    InitializeNode("S2", &S2, NULL, NULL, &A2, &S1);
    InitializeNode("S1", &S1, NULL, &S2, &A1, NULL);
    InitializeNode("A2", &A2, &S2, NULL, &B2, &A1);
    InitializeNode("A1", &A1, &S1, &A2, &B1, NULL);
    
    
    
    return 0;
}
