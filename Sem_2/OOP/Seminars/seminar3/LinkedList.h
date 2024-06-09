//
// Created by Mara Ielciu on 28.03.2024.
//

#ifndef SEMINAR3_914_LINKEDLIST_H
#define SEMINAR3_914_LINKEDLIST_H
#pragma once

template <typename T>
class Node {
private:
    T data;
    Node* next;
public:
    Node(T data, Node<T>* next){
        this->data = data;
        this->next = next;
    }

    ~Node(){
        delete this->next;
    }

    Node* getNext(){
        return this->next;
    }

    T getData() {
        return this->data;
    }

    void setNext(Node<T>* new_next){
        this->next = new_next;
    }

};

template <typename T>
class LinkedList {
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
            Node<T>* next = current->next;
            delete current;
            current = next;
        }
    }
    int get_size(){
        int cnt=0;
        Node<T>* current=this->head;
        while(current!= nullptr)
        {
            current = current->getNext();
            cnt++;
        }
        return cnt;
    }
    void addElement(T data){
        Node<T>* newNode = new Node<T>(data,this->head);
        this->head=newNode;
        this->size++;
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
};


#endif //SEMINAR3_914_LINKEDLIST_H
