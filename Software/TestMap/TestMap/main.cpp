//
//  main.cpp
//  TestMap
//
//  Created by 林炜哲 on 06/10/2018.
//  Copyright © 2018 Dragon. All rights reserved.
//

#include <iostream>
using namespace std;
class FieldMap{
public:
    string name="none";
    FieldMap* left=NULL;
    FieldMap* up=NULL;
    FieldMap* right=NULL;
    FieldMap* down=NULL;
    void print(){
        if (this->left != NULL){
            cout<<"left is:"<<this->left->name<<endl;
        }
        if (this->right!= NULL){
            cout<<"right is:"<<this->right->name<<endl;
        }
    }
};
int main(int argc, const char * argv[]) {
    // insert code here...
    cout << "Hello, World!\n";
    FieldMap A,B;
    A.name = "This is A";
    B.name = "This is B";
    A.left = &B;
    B.right = &A;
    A.print();
    B.print();
    return 0;
}
