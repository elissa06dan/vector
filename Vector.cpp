#include "Vector.h"
#include <stdexcept>

Vector::Vector(const ValueType* rawArray, const size_t size, float coef)
    : _size(size), _capacity(size), _multiplicativeCoef(coef)
{
    if (size > 0) 
    {
        _data = new ValueType[_capacity];
        for (size_t i = 0; i < size; i++) 
	{
            _data[i] = rawArray[i];
        }
    }
}

Vector::Vector(const Vector& other)
    : _size(other._size), _capacity(other._size), _multiplicativeCoef(other._multiplicativeCoef)
{
    if (_size > 0) 
    {
        _data = new ValueType[_capacity];
        for (size_t i = 0; i < _size; i++) 
	{
            _data[i] = other._data[i];
        }
    }
}

Vector& Vector::operator=(const Vector& other)
{
    if (this != &other) 
    {
        delete[] _data;
        _size = other._size;
        _capacity = other._size;
        _multiplicativeCoef = other._multiplicativeCoef;
        
        if (_size > 0) 
	{
            _data = new ValueType[_capacity];
            for (size_t i = 0; i < _size; i++) 
	    {
                _data[i] = other._data[i];
            }
        } 
	else 
	{
            _data = nullptr;
        }
    }
    return *this;
}

Vector::Vector(Vector&& other) noexcept
    : _data(other._data), _size(other._size), _capacity(other._capacity),
      _multiplicativeCoef(other._multiplicativeCoef)
{
    other._data = nullptr;
    other._size = 0;
    other._capacity = 0;
}

Vector& Vector::operator=(Vector&& other) noexcept
{
    if (this != &other) 
    {
        delete[] _data;
        
        _data = other._data;
        _size = other._size;
        _capacity = other._capacity;
        _multiplicativeCoef = other._multiplicativeCoef;
        
        other._data = nullptr;
        other._size = 0;
        other._capacity = 0;
    }
    return *this;
}

Vector::~Vector()
{
    delete[] _data;
}

void Vector::pushBack(const ValueType& value)
{
    if (_size >= _capacity) 
    {
    	_capacity *= _multiplicativeCoef;
        if (_capacity == 0)
        {
            _capacity = _multiplicativeCoef;
        }
        ValueType* temp = new ValueType[_capacity];
        if (_data != nullptr)
        {
            std::copy(_data, _data + _size, temp);
            delete[] _data;
        }
        _data = temp;
    }
    _data[_size] = value;  
    _size++;

}

void Vector::pushFront(const ValueType& value)
{
    if (_capacity == 0) 
    {
        _capacity = 1;
        if (_multiplicativeCoef > 1) 
        {
            _capacity = (size_t)_multiplicativeCoef;
        }
	
        _data = new ValueType[_capacity];
        _data[0] = value;
        _size = 1;
        return;
    }

    if (_size == _capacity) 
    {

        float newCapacityFloat = _capacity * _multiplicativeCoef;
        size_t newCapacity = (size_t)newCapacityFloat;
            
        ValueType* newData = new ValueType[newCapacity];
            
  
        for (size_t i = 0; i < _size; ++i) 
        {
	    newData[i+1] = _data[i];
        }
	
	delete[] _data;
	_data = newData;
	_capacity = newCapacity;
    } 
    
    else 
    {
  
        for (size_t i = _size; i > 0; --i) 
        {
	    _data[i] = _data[i-1];
        }
    }

    _data[0] = value;
    ++_size;
}

void Vector::insert(const ValueType& value, size_t pos)
{
    if (pos > _size) 
    {
        throw std::out_of_range("Position out of range");
    }
    
    if (_size == _capacity) 
    {
        size_t newCapacity = _capacity == 0 ? 1 : static_cast<size_t>(_capacity * _multiplicativeCoef);
        reserve(newCapacity);
    }
    
    for (size_t i = _size; i > pos; --i) 
    {
        _data[i] = _data[i - 1];
    }
    
    _data[pos] = value;
    ++_size;
}

void Vector::insert(const ValueType* values, size_t size, size_t pos)
{
    if (pos > _size) 
    {
        throw std::out_of_range("Position out of range");
    }
    
    if (_size + size > _capacity) 
    {
        size_t newCapacity = _size + size;
        
	if (newCapacity < static_cast<size_t>(_capacity * _multiplicativeCoef)) 
	{
            newCapacity = static_cast<size_t>(_capacity * _multiplicativeCoef);
        }
        
	reserve(newCapacity);
    }
    
    for (size_t i = _size + size - 1; i >= pos + size; --i) 
    {
        _data[i] = _data[i - size];
    }
    
    for (size_t i = 0; i < size; ++i) 
    {
        _data[pos + i] = values[i];
    }
    
    _size += size;
}

void Vector::insert(const Vector& vector, size_t pos)
{
    insert(vector._data, vector._size, pos);
}

void Vector::popBack()
{
    if (_size == 0) 
    {
        throw std::out_of_range("Vector is empty");
    }
    --_size;
}

void Vector::popFront()
{
    erase(0);
}

void Vector::erase(size_t pos, size_t count)
{
    if (pos >= _size) 
    {
        throw std::out_of_range("Position out of range");
    }
    
    if (count > _size - pos) 
    {
        count = _size - pos;
    }
    
    for (size_t i = pos; i < _size - count; ++i) 
    {
        _data[i] = _data[i + count];
    }
    
    _size -= count;
}

void Vector::eraseBetween(size_t beginPos, size_t endPos)
{
    if (beginPos > endPos || beginPos >= _size) 
    {
        throw std::out_of_range("Invalid range");
    }
    
    endPos = std::min(endPos, _size);
    erase(beginPos, endPos - beginPos);
}

size_t Vector::size() const { return _size; }
size_t Vector::capacity() const { return _capacity; }

double Vector::loadFactor() const
{
    return _capacity == 0 ? 0.0 : static_cast<double>(_size) / _capacity;
}

ValueType& Vector::operator[](size_t idx)
{
    if (idx >= _size) 
    {
        throw std::out_of_range("Index out of range");
    }
    return _data[idx];
}

const ValueType& Vector::operator[](size_t idx) const
{
    if (idx >= _size) 
    {
        throw std::out_of_range("Index out of range");
    }
    return _data[idx];
}

long long Vector::find(const ValueType& value) const
{
    for (size_t i = 0; i < _size; ++i) 
    {
        if (_data[i] == value) 
	{
            return static_cast<long long>(i);
        }
    }
    return -1;
}

void Vector::reserve(size_t capacity)
{
    if (capacity > _capacity) 
    {
        ValueType* newData = new ValueType[capacity];
        for (size_t i = 0; i < _size; ++i) 
	{
            newData[i] = _data[i];
        }
        
	delete[] _data;
        _data = newData;
        _capacity = capacity;
    }
}

void Vector::shrinkToFit()
{
    if (_size < _capacity) 
    {
        if (_size == 0) 
	{
            delete[] _data;
            _data = nullptr;
            _capacity = 0;
            return;
        }
        
        ValueType* newData = new ValueType[_size];
        for (size_t i = 0; i < _size; ++i) 
	{
            newData[i] = _data[i];
        }
        
	delete[] _data;
        _data = newData;
        _capacity = _size;
    }
}

Vector::Iterator::Iterator(ValueType* ptr) : _ptr(ptr) {}

ValueType& Vector::Iterator::operator*() { return *_ptr; }
const ValueType& Vector::Iterator::operator*() const { return *_ptr; }
ValueType* Vector::Iterator::operator->() { return _ptr; }
const ValueType* Vector::Iterator::operator->() const { return _ptr; }

Vector::Iterator Vector::Iterator::operator++()
{
    ++_ptr;
    return *this;
}

Vector::Iterator Vector::Iterator::operator++(int)
{
    Iterator temp = *this;
    ++_ptr;
    return temp;
}

bool Vector::Iterator::operator==(const Iterator& other) const { return _ptr == other._ptr; }
bool Vector::Iterator::operator!=(const Iterator& other) const { return _ptr != other._ptr; }

Vector::Iterator Vector::begin() { return Iterator(_data); }
Vector::Iterator Vector::end() { return Iterator(_data + _size); }

