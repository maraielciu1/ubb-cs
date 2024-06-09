//
// Created by Mara Ielciu on 11.04.2024.
//

#ifndef SEMINAR4_914_LINKEDLIST_H
#define SEMINAR4_914_LINKEDLIST_H

#pragma once

#include "List.h"

template <typename T>
class Node {
private:
    T data;
    Node* next;
public:
    Node(T data, Node<T>* next=nullptr){
        this->data = data;
        this->next = next;
    }

    ~Node(){
        delete this->next;
    }

    Node* getNext(){
        return this->next;
    }

    T& getData() {
        return this->data;
    }

    void setNext(Node<T>* new_next){
        this->next = new_next;
    }

};

template <typename T>
class LinkedList: public List<T>
        {
private:
    Node<T>* head;
    int size;
public:
    LinkedList(){
        this->head = nullptr;
        this->size = 0;
    }

    LinkedList(LinkedList& l) {
        Node<T>* current = l.head;
        while (current != nullptr) {
            this->addElement(current->getData());
            current = current->getNext();
        }
    }

    ~LinkedList(){
        Node<T>* current = this->head;
        while (current != nullptr){
            Node<T>* next = current->getNext();
            delete current;
            current = next;
        }
    }
    int getSize() const override{
        int cnt=0;
        Node<T>* current=this->head;
        while(current!= nullptr)
        {
            current = current->getNext();
            cnt++;
        }
        return cnt;
    }
    void add(const T& data) override{
        Node<T>* newNode = new Node<T>(data);
        Node<T>* current = this->head;
        if(this->head== nullptr){
            this->head=newNode;
            this->size++;
            return;
        }
        while (current->getNext() != nullptr)
            current = current->getNext();
        current->setNext(newNode);
        this->size++;
        return;
    }

    void deleteElement(T data){
        Node<T>* current = this->head;
        Node<T>* previous = nullptr;
        while (current != nullptr){
            if (current->data == data){
                if (previous == nullptr){
                    this->head = current->next;
                }
                else{
                    previous->next = current->next;
                }
                current->next = nullptr;
                delete current;
                return;
            }
            previous = current;
            current = current->next;
        }
    }

    T& operator[](int pos) override{
        Node<T>* current = this->head;
        int cnt = 0;
        while (current != nullptr){
            if (cnt == pos){
                return current->getData();
            }
            cnt++;
            current = current->getNext();
        }
    }
};



#endif //SEMINAR4_914_LINKEDLIST_H
