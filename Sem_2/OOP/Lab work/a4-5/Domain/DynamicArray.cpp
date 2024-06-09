//
// Created by Mara Ielciu on 24.03.2024.
//

#include "DynamicArray.h"

//void DynamicArray::resize() {
//    this->capacity *= 2;
//    Tutorial* newElements = new Tutorial[this->capacity];
//    for (int i = 0; i < this->size; i++)
//        newElements[i] = this->elements[i];
//    delete[] this->elements;
//    this->elements = newElements;
//}
//
//DynamicArray::DynamicArray(const DynamicArray &dynamicArray) {
//    this->size = dynamicArray.size;
//    this->capacity = dynamicArray.capacity;
//    this->elements = new Tutorial[this->capacity];
//    for (int i = 0; i < this->size; i++)
//        this->elements[i] = dynamicArray.elements[i];
//}
//
//DynamicArray::~DynamicArray() {
//    delete[] this->elements;
//}
//
//void DynamicArray::addElementArray(const Tutorial &tutorial) {
//    if (this->size == this->capacity)
//        this->resize();
//    this->elements[this->size++] = tutorial;
//}
//
//void DynamicArray::deleteElementArray(int position) {
//    if(position <0 || position >= this->size)
//        throw std::invalid_argument("Invalid position");
//    for (int i = position; i < this->size - 1; i++)
//        this->elements[i] = this->elements[i + 1];
//    this->size--;
//}
//
//void DynamicArray::updateElementArray(int position, const Tutorial &tutorial) {
//    if(position <0 || position >= this->size)
//        throw std::invalid_argument("Invalid position");
//    this->elements[position] = tutorial;
//}
//
//Tutorial *DynamicArray::getAllElementsArray() const {
//    return this->elements;
//}
//
//int DynamicArray::getSizeArray() const {
//    return this->size;
//}
//
//Tutorial DynamicArray::getElementArray(int position) {
//    if(position <0 || position >= this->size)
//        throw std::invalid_argument("Invalid position");
//    return this->elements[position];
//}
//
//DynamicArray &DynamicArray::operator=(const DynamicArray &dynamicArray) {
//    if (this == &dynamicArray)
//        return *this;
//    this->size = dynamicArray.size;
//    this->capacity = dynamicArray.capacity;
//    delete[] this->elements;
//    this->elements = new Tutorial[this->capacity];
//    for (int i = 0; i < this->size; i++)
//        this->elements[i] = dynamicArray.elements[i];
//    return *this;
//}
//
//int DynamicArray::getCapacityArray() const {
//    return this->capacity;
//}
//
//int DynamicArray::returnPosition(const Tutorial &tutorial) {
//    for (int i = 0; i < this->size; i++)
//        if (this->elements[i] == tutorial)
//            return i;
//    return -1;
//}


