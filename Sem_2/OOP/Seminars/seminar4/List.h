#pragma once

template <typename T>
class List
{
public:
	virtual void add(const T& elem) = 0; // they are virtual bcs we don't know the implementation/data structure used
	virtual T& operator[](int pos) = 0;
	virtual int getSize() const = 0;
	virtual ~List() {} // cannot be pure virtual
};