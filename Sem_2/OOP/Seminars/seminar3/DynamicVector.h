#pragma once

template <typename T> 
class DynamicVector {
private:
	T* elements;
	int size;
	int capacity;

	void resize() {
		this->capacity *= 2;
		T* copy = new T[this->capacity];
		for (int i = 0; i < this->size; i++)
			copy[i] = this->elements[i];
		delete[] this->elements;
		this->elements = copy;
	}

public:
	DynamicVector(int capacity = 20) {
		this->capacity = capacity;
		this->size = 0;
		this->elements = new T[capacity];
	}

	~DynamicVector();

	void add(T element) {
		if (this->size == this->capacity)
			this->resize();
		this->elements[this->size] = element;
		this->size++;
	}

	int getSize() {
		return this->size;
	}

	DynamicVector(const DynamicVector& other)
	{
		this->capacity = other.capacity;
		this->size = other.size;
		this->elements = new T[this->capacity];
		for (int i = 0; i < this->size; i++)
			this->elements[i] = other.elements[i];
	}
	DynamicVector& operator=(const DynamicVector& other)
	{
		if (this == &other)
			return *this;
		this->capacity = other.capacity;
		this->size = other.size;
		delete[] this->elements;
		this->elements = new T[this->capacity];
		for (int i = 0; i < this->size; i++)
			this->elements[i] = other.elements[i];
		return *this;
	}

	T& operator[](int index)
	{
		return this->elements[index];
	}

	class iterator
	{
	private:
		T* ptr;
    public:
		iterator(T* p) : ptr{ p } {}
		T operator*() {
			return *this->ptr;
		}
		bool operator!=(const iterator& it) {
			return this->ptr != it.ptr;
		}
		iterator& operator++() {
			this->ptr++;
			return *this; // dereferencing the pointer this
		}
	};

	iterator begin()
	{
//		iterator it{ this->elements };
//		return it;
//      return iterator{&this->elements[0]};
        return iterator{ this->elements };
	}

	iterator end()
	{
		return iterator{ this->elements + this->size };
//    	return iterator{&this->elements[this->size]};
	}
};

template<typename T>
inline DynamicVector<T>::~DynamicVector()
{
	delete[] this->elements;
}
