#ifndef FINAL_SRC_HPP
#define FINAL_SRC_HPP

#include <string>
#include <vector>
using namespace std;

class LinkedHashMap;
struct Node{
    Node* next=nullptr;      // next in bucket singly linked list
    Node* timePre=nullptr;   // prev in global time-order doubly list
    Node* timeNext=nullptr;  // next in global time-order doubly list

    int key=0;
    string value;
};

struct Data{
    int key;
    string value;
};

vector<Data> traverseByTime(const LinkedHashMap& obj);
vector<Data> traverseByIndex(const LinkedHashMap& obj);

class LinkedHashMap{
// You may modify beg/current in ctor per your habit.
public:
    int len;
    Node** array;
    Node* beg;      // sentinel head for time-order list
    Node* current;  // tail pointer (last real element) for O(1) append
    // function pointer for traversal
    vector<Data> (*forEachMethod)(const LinkedHashMap&);

    LinkedHashMap(int _len_,bool forEachByTime){
        len=_len_;
        beg=new Node();
        current=beg;
        array=new Node*[len];
        for(int i=0;i<len;i++)
            array[i]=nullptr;
        if(forEachByTime)
            forEachMethod=traverseByTime;
        else
            forEachMethod=traverseByIndex;
    }

    ~LinkedHashMap(){
        // delete all nodes in buckets to free memory once
        for(int i=0;i<len;i++){
            Node* p=array[i];
            while(p){
                Node* nxt=p->next;
                delete p;
                p=nxt;
            }
        }
        delete [] array;
        delete beg;
    }

    void insert(int key,string value){
        if(key<0 || key>=len) return;
        Node* node=new Node();
        node->key=key;
        node->value=value;
        // insert to head of bucket list
        node->next=array[key];
        array[key]=node;
        // append to time-order tail (after current)
        node->timePre=current;
        node->timeNext=nullptr;
        if(current) current->timeNext=node;
        current=node;
    }

    void remove(int key,string value){
        if(key<0 || key>=len) return;
        Node* prev=nullptr;
        Node* p=array[key];
        while(p){
            if(p->value==value){
                // unlink from bucket list
                if(prev) prev->next=p->next;
                else array[key]=p->next;
                // unlink from time list in O(1)
                if(p->timePre) p->timePre->timeNext=p->timeNext;
                if(p->timeNext) p->timeNext->timePre=p->timePre;
                if(current==p) current=p->timePre ? p->timePre : beg;
                delete p;
                return; // only delete the first matching node
            }
            prev=p; p=p->next;
        }
    }

    vector<string> ask(int key)const{
        vector<string> res;
        if(key<0 || key>=len) return res;
        Node* p=array[key];
        while(p){
            res.push_back(p->value);
            p=p->next;
        }
        return res;
    }

    vector<Data> forEach()const{
        return forEachMethod(*this);
    }
};

inline vector<Data> traverseByTime(const LinkedHashMap& obj){
    vector<Data> res;
    Node* p=obj.beg->timeNext;
    while(p){
        Data d; d.key=p->key; d.value=p->value;
        res.push_back(d);
        p=p->timeNext;
    }
    return res;
}

inline vector<Data> traverseByIndex(const LinkedHashMap& obj){
    vector<Data> res;
    for(int i=0;i<obj.len;i++){
        Node* p=obj.array[i];
        while(p){
            Data d; d.key=p->key; d.value=p->value;
            res.push_back(d);
            p=p->next;
        }
    }
    return res;
}

#endif //FINAL_SRC_HPP
