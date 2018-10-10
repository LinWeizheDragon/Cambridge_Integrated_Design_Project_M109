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

/*
 Initialize a node, store into storage
 */
void InitializeNode(string name, Node* target, Node* left, Node* up, Node* right, Node* down){
    target->name = name;
    target->left = left;
    target->up = up;
    target->right = right;
    target->down = down;
    node_storage[name] = target;
}

int main(int argc, const char * argv[]) {
    
    // map initialization
    InitializeNode("S2", &S2, &F1, &F2, &F3, &F4);
    InitializeNode("S1", &S1, NULL, &F4, &A1, NULL);
    InitializeNode("F1", &F1, NULL, NULL, &S2, NULL);
    InitializeNode("F2", &F2, NULL, NULL, NULL, &S2);
    InitializeNode("F3", &F3, &S2, NULL, &A2, &F4);
    InitializeNode("F4", &F4, NULL, &S2, NULL, &S1);
    
    InitializeNode("A2", &A2, &S2, NULL, &B2, &A1);
    InitializeNode("A1", &A1, &S1, &A2, &B1, NULL);
    
    InitializeNode("B2", &B2, &A2, &B5, &C2, &B1);
    InitializeNode("B1", &B1, &A1, &B2, &C1, NULL);
    InitializeNode("B5", &B5, NULL, &B6, &D5, &B2);
    InitializeNode("B6", &B6, NULL, &B7, &D6, &B5);
    InitializeNode("B7", &B7, NULL, &B8, &D7, &B6);
    InitializeNode("B8", &B8, NULL, NULL, &D8, &B7);
    
    InitializeNode("C2", &C2, &B2, NULL, &D2, &C1);
    InitializeNode("C1", &C1, &B1, &C2, &D1, NULL);
    
    InitializeNode("D1", &D1, &C1, &D2, &E1, NULL);
    InitializeNode("D2", &D2, &C2, &D3, &E2, &D1);
    InitializeNode("D3", &D3, NULL, &D4, &E3, &D2);
    InitializeNode("D4", &D4, NULL, &D5, &E4, &D3);
    InitializeNode("D5", &D5, &B5, &D6, &E5, &D4);
    InitializeNode("D6", &D6, &B6, &D7, &E6, &D5);
    InitializeNode("D7", &D7, &B7, &D8, &E7, &D6);
    InitializeNode("D8", &D8, &B8, NULL, &E8, &D7);
    
    InitializeNode("E1", &E1, &D1, &E2, NULL, NULL);
    InitializeNode("E2", &E2, &D2, &E3, NULL, &E1);
    InitializeNode("E3", &E3, &D3, &E4, NULL, &E2);
    InitializeNode("E4", &E4, &D4, &E5, NULL, &E3);
    InitializeNode("E5", &E5, &D5, &E6, NULL, &E4);
    InitializeNode("E6", &E6, &D6, &E7, NULL, &E5);
    InitializeNode("E7", &E7, &D7, &E8, NULL, &E6);
    InitializeNode("E8", &E8, &D8, NULL, NULL, &E7);
    
    map<string, Node*>::iterator  iter;
    for(iter = node_storage.begin(); iter != node_storage.end(); iter++){
        iter->second->print();
    }
    
    return 0;
}
