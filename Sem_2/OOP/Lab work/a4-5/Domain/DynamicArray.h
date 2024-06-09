//
// Created by Mara Ielciu on 24.03.2024.
//

#ifndef OOP_A4_5_MARAIELCIU1_DYNAMICARRAY_H
#define OOP_A4_5_MARAIELCIU1_DYNAMICARRAY_H
#pragma once
#include "Tutorial.h"

template <typename T>
class DynamicArray {
private:
    int size;
    int capacity;
    T* elements;

    void resize();
public:
    DynamicArray(int new_capacity=10);
    DynamicArray(const DynamicArray& dynamicArray);
    ~DynamicArray();
    void addElementArray(const T& tutorial);
    void deleteElementArray(int position);
    void updateElementArray(int position, const T& tutorial);
    T* getAllElementsArray() const;
    int getSizeArray() const;
    int getCapacityArray() const;
    T getElementArray(int position);
    DynamicArray& operator=(const DynamicArray& dynamicArray);
    int returnPosition(const T& tutorial);

};

template <typename T>
DynamicArray<T>::DynamicArray(int new_capacity) {
    this->capacity = new_capacity;
    this->size = 0;
    this->elements = new T[this->capacity];
}

template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray &dynamicArray) {
    this->size = dynamicArray.size;
    this->capacity = dynamicArray.capacity;
    this->elements = new T[this->capacity];
    for (int i = 0; i < this->size; i++)
        this->elements[i] = dynamicArray.elements[i];
}

template <typename T>
DynamicArray<T>::~DynamicArray() {
    delete[] this->elements;
}

template <typename T>
void DynamicArray<T>::resize() {
    this->capacity *= 2;
    T* newElements = new T[this->capacity];
    for (int i = 0; i < this->size; i++)
        newElements[i] = this->elements[i];
    delete[] this->elements;
    this->elements = newElements;
}

template <typename T>
void DynamicArray<T>::addElementArray(const T &tutorial) {
    if (this->size == this->capacity)
        this->resize();
    this->elements[this->size++] = tutorial;
}

template <typename T>
void DynamicArray<T>::deleteElementArray(int position) {
    if(position <0 || position >= this->size)
        throw std::invalid_argument("Invalid position");
    for (int i = position; i < this->size - 1; i++)
        this->elements[i] = this->elements[i + 1];
    this->size--;
}

template <typename T>
void DynamicArray<T>::updateElementArray(int position, const T &tutorial) {
    if(position <0 || position >= this->size)
        throw std::invalid_argument("Invalid position");
    this->elements[position] = tutorial;
}

template <typename T>
T* DynamicArray<T>::getAllElementsArray() const {
    return this->elements;
}

template <typename T>
int DynamicArray<T>::getSizeArray() const {
    return this->size;
}

template <typename T>
T DynamicArray<T>::getElementArray(int position) {
    if(position <0 || position >= this->size)
        throw std::invalid_argument("Invalid position");
    return this->elements[position];
}

template <typename T>
DynamicArray<T> &DynamicArray<T>::operator=(const DynamicArray &dynamicArray) {
    if (this == &dynamicArray)
        return *this;
    this->size = dynamicArray.size;
    this->capacity = dynamicArray.capacity;
    delete[] this->elements;
    this->elements = new T[this->capacity];
    for (int i = 0; i < this->size; i++)
        this->elements[i] = dynamicArray.elements[i];
    return *this;
}

template <typename T>
int DynamicArray<T>::getCapacityArray() const {
    return this->capacity;
}

template <typename T>
int DynamicArray<T>::returnPosition(const T &tutorial) {
    for (int i = 0; i < this->size; i++)
        if (this->elements[i] == tutorial)
            return i;
    return -1;
}
#endif //OOP_A4_5_MARAIELCIU1_DYNAMICARRAY_H
