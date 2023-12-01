//
// Created by User on 12/1/2023.
//
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <iostream>
using namespace std;
class Node{
public :
    Node* node[256];//ASCII
    int start;
    int end;
    Node(){
        start=-1;
        end=-1;
        *node= nullptr;
    }
//    ~Node();
};
class SuffixTree{
private:
    Node* Root;
    char* STR;
public:
    SuffixTree(){

    }
    ~SuffixTree();
    void insertSuffix();
    void search();
};