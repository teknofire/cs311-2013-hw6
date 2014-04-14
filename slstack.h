/*
 *  slstack.h
 *  CS311 Homework #6 Exercise b
 *  Created on: April 8, 2014
 *      Authors: Will Fisher, Karl Oder, Cody Gaines
 */

#ifndef SLSTACK_H_
#define SLSTACK_H_

#include "slist.h"

template <typename T>
class SLStack
{
	SList<T> _list;

public:
	SLStack();
	explicit SLStack(const SLStack<T> & other);
	SLStack & operator=(const SLStack<T> & other);
	~SLStack();

	bool empty() const;

	T & top();
	const T & top() const;

	void push(const T & value);
	void pop();
};

/*
 * SLStack<T>::SLStack(): Deafult SLStack constructor
 * Preconditions: None
 * Postconditions: This is now an empty SLStack
 */
template <typename T>
SLStack<T>::SLStack()//: _list(SList<T>())
{

}

/*
 * SLStack<T>::SLStack(const SLStack<T> & other): SLStack copy constructor
 * Preconditions: None
 * Postconditions: This now contains copied contents from other
 */
template <typename T>
SLStack<T>::SLStack(const SLStack<T> & other): _list(other._list)
{

}

/*
 * SLStack<T>::operator=(const SLStack<T> & other): SLStack copy assignment
 * Preconditions: None
 * Postconditions: This now contains copied contents from other.
 */
template <typename T>
SLStack<T> & SLStack<T>::operator=(const SLStack<T> & other)
{
	_list = other._list;
	return *this;
}

/*
 * SLStack<T>::~SLStack() Deconstructor
 * Preconditions: None
 * Postconditions: None
 */
template <typename T>
SLStack<T>::~SLStack()
{

}

/*
 * SLStack<T>::empty() Returns if the stack is empty or not
 * Preconditions: None
 * Postconditions: Returns the current size of the stack
 */
template <typename T>
bool SLStack<T>::empty() const
{
	return _list.size() == 0;
}

/*
 * SLStack<T>::top() Returns a reference the top item on the stack
 * Preconditions: None
 * Postconditions: Returns a reference to the top item on the stack. Throws std::range_error(from SList) if stack is empty.
 */
template <typename T>
T & SLStack<T>::top()
{
	try
	{
		return _list.front();
	}
	catch(...)
	{
		throw;
	}
}

/*
 * SLStack<T>::top() Returns a reference the top item on the stack
 * Preconditions: None
 * Postconditions: Returns a const reference to the top item on the stack. Throws std::range_error(from SList) if stack is empty.
 * Exception Guarantee: Strong
 */
template <typename T>
const T & SLStack<T>::top() const
{
	try
	{
		return _list.front();
	}
	catch(...)
	{
		throw;
	}
}

/*
 * SLStack<T>::push() Pushes an item on to the stack
 * Preconditions: None
 * Postconditions: None
 * Exception Guarantee: Strong, may throw std::bad_alloc
 */
template <typename T>
void SLStack<T>::push(const T & value)
{
	try
	{
		_list.push_front(value);
	}
	catch(...)
	{
		throw;
	}
}

/*
 * void SLStack<T>::pop(): Pops the stack
 * Preconditions: None
 * Postcondtions: Removes the top of the stack, if there is one
 * Exception Guarantee: No-Throw
 */
template <typename T>
void SLStack<T>::pop()
{
	_list.pop_front();
}
#endif /* SLSTACK_H_ */
