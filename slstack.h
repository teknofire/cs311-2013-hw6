/*
 * slstack.h
 *
 *  Created on: Apr 8, 2014
 *      Author: Will
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

template <typename T>
SLStack<T>::SLStack()//: _list(SList<T>())
{

}

template <typename T>
SLStack<T>::SLStack(const SLStack<T> & other): _list(other._list)
{

}

template <typename T>
SLStack<T> & SLStack<T>::operator=(const SLStack<T> & other)
{
	_list = other._list;
	return *this;
}

template <typename T>
SLStack<T>::~SLStack()
{

}

template <typename T>
bool SLStack<T>::empty() const
{
	return _list.size() == 0;
}

template <typename T>
T & SLStack<T>::top()
{
	return _list.front();
}
template <typename T>
const T & SLStack<T>::top() const
{
	return _list.front();
}

template <typename T>
void SLStack<T>::push(const T & value)
{
	_list.push_front(value);
}

template <typename T>
void SLStack<T>::pop()
{
	_list.pop_front();
}
#endif /* SLSTACK_H_ */
