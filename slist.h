//slist.h
//Karl Oder
//Will Fisher
//Cody Gaines

//April 8, 2014
//For CS 311 Assignment 6 Exercise A

#ifndef _SLIST_H
#define _SLIST_H

#include <cstddef> //size_t

//Item storage class for a double-linked list
//Invariants
//Requirements on type
//	Needs to have operator=
template <typename T>
class LLItem
{
public:
	LLItem<T>* _prev;
	LLItem<T>* _next;
	T _value;
};

template <typename T>
class SListIterator;

template <typename T>
bool operator==(const SListIterator<T> & lhs, const SListIterator<T> & rhs)
{
	return (lhs._itm == rhs._itm);
}

template <typename T>
bool operator!=(const SListIterator<T> & lhs, const SListIterator<T> & rhs)
{
	return !(lhs._itm == rhs._itm);
}

//Simple Iterator for a double-linked list
//May not have all the attributes of a standard iterator
//Used internally for SList
//Requirements on type
//  T needs operator==,
template <typename T>
class SListIterator
{
private:
	LLItem<T>* _itm;
public:
	explicit SListIterator(LLItem<T>* item);
	SListIterator& operator++();
	SListIterator& operator--();
	SListIterator operator++(int);
	SListIterator operator--(int);
	T & operator*();
	LLItem<T>* operator->();

	friend bool operator== <> (const SListIterator<T> & lhs, const SListIterator<T> & rhs);
	friend bool operator!= <> (const SListIterator<T> & lhs, const SListIterator<T> & rhs);
};

template <typename T>
SListIterator<T>::SListIterator(LLItem<T>* item): _itm(item){};

template <typename T>
SListIterator<T>& SListIterator<T>::operator++(){_itm = _itm->_next; return *this;}

template <typename T>
SListIterator<T>& SListIterator<T>::operator--(){_itm = _itm->_prev; return *this;}

template <typename T>
SListIterator<T> SListIterator<T>::operator++(int)
{
	SListIterator tmp(*this);
	operator++();
	return tmp;
}

template <typename T>
SListIterator<T> SListIterator<T>::operator--(int)
{
	SListIterator tmp(*this);
	operator--();
	return tmp;
}

template <typename T>
T & SListIterator<T>::operator*(){return _itm->_value;}

template <typename T>
LLItem<T>* SListIterator<T>::operator->(){return _itm;}


/*
 * SList Class
 * Provides a simple template based linked list
 * Invariants:
 * size_type will be a valid size descriptor
 * _first must either be NULL or point to a valid LLItem, owned by *this
 * _end must either be NULL or point to a valid LLItem, owned by *this
 *
 * Requirements on type
 * 		T needs to have operator=
 */
template <typename T>
class SList
{
public:
	typedef std::size_t size_type;
	typedef SListIterator<T> iterator;

private:
	LLItem<T>* _first;
	LLItem<T>* _end;
	size_type _size;

	void clear(); //Empty the list, doesn't use the iterator for no-throw
	void link_item_end(LLItem<T>* itm); //Links an item to the end of the list

	iterator begin() const;
	iterator end() const;

public:
	~SList();
	SList();
	explicit SList(const SList<T>& other);
	SList& operator=(const SList& other);
	size_type size() const;

	T & front() const;
	void push_front(const T & value);
	void pop_front();

	/*
	 * void read(InputIterator first, InputIterator last)
	 * Preconditions: the given arguments are valid Iterators
	 * Postconditions: this will contain a copy of the given range
	 * Exception Guarantee: Basic, Exception Neutral
	 * May throw std::bad_alloc
	 */
    template <typename InputIterator>
    void read(InputIterator first, InputIterator last);
//    {
//    	clear();
//
//    	LLItem<T>* itm;
//
//    	try
//    	{
//    		for(auto itr = first; itr != last; itr++)
//    		{
//				itm = new LLItem<T>();
//				itm->_value = *itr;
//				link_item_end(itm);
//    		}
//    	}
//    	catch(...)
//    	{
//    		delete itm;
//    		clear();
//    		throw;
//    	}
//    }

    /*
     * void write(OutputIterator dest)
     * Preconditions:
     * 	 dest must be a valid OutputIterator and have enough space to hold the values
     * Postconditions:
     *   dest will contain the copied values from this
     * Exception Guarantee: Basic, Exception-Neutral
     */
    template <typename OutputIterator>
    void write(OutputIterator dest) const
    {
    	try
    	{
    		for(auto i = begin(); i != end(); i++)
    		{
    		*dest++=i->_value;
    		}
    	}
    	catch(...)
    	{
    		throw;
    	}
    }


    void reverse();
};

/*
 * ~SList() : De-Allocates a SList
 * Preconditions: none
 * Postconditions:
 * 	list has been deleted
 * Exception Guarantee: No-Throw
 */
template <typename T>
SList<T>::~SList()
{
	clear();
}


/*
 * SList() : Allocates an Empty SList of the given type
 * Preconditions: none
 * Postconditions:
 * 	this has a size of 0
 * Exception Guarantee: No-Throw
 */
template <typename T>
SList<T>::SList(): _first(0),_end(0), _size(0)
{}


/*
 * SList(const SList<T>& other) : Copies a SList
 * Preconditions:
 * 	none
 * Postconditions:
 * 	this now contains a copy of the contents of other
 * Exception Guarantee: Strong, Exception Neutral
 * May throw std::bad_alloc
 */
template <typename T>
SList<T>::SList(const SList<T>& other): _first(0), _end(0), _size(0)
{
	LLItem<T>* itm = NULL;

	try
	{
		if(other._size)
		{
			auto itr = other._first;
			for(auto i = other.begin(); i != other.end(); i++)
			{
				itm = new LLItem<T>();
				itm->_value = itr->_value;
				link_item_end(itm);
				itr = itr->_next;
			}
		}
	}
	catch(...)
	{
		delete itm;
		throw;
	}
}

/*
 * SList& operator=(const SList& other) : Copies the given SList to the target
 * Preconditions: none
 * Postconditions:
 * 	this will be a copy of other. The original contents will be de-allocated.
 * Exception Guarantee: Basic, Exception Neutral
 * May throw std::bad_alloc
 */
template <typename T>
SList<T>& SList<T>::operator=(const SList& other)
{
	if(this == &other)
		return *this;

	clear();

	LLItem<T>* itm = NULL;

	try
	{
		for(auto i = other.begin(); i != other.end(); ++i)
		{
			itm = new LLItem<T>();
			itm->_value = i->_value;
			link_item_end(itm);
		}
	}
	catch(...)
	{
		delete itm;
		throw;
	}

	return *this;
}

/*
 * size_type size() : Gets the size of the SList
 * Preconditions: none
 * Postconditions: returns size of list
 * Exception Guarantee: No-Throw
 */
template <typename T>
typename SList<T>::size_type SList<T>::size() const
{
	return _size;
}

/* void SList<T>::push_front(const T & value)
 * Preconditions: none
 * Postconditions:
 * 		returns reference to first value in the list
 * Exception Guarantee: No throw
 */
template <typename T>
T & SList<T>::front() const
{
	return _first->_value;
}

/* void SList<T>::push_front(const T & value)
 * Preconditions:
 * Postconditions:
 * 		adds the given value to the front of the list
 * Exception Guarantee: Strong Guarantee
 * May throw std::bad_alloc
 */
template <typename T>
void SList<T>::push_front(const T & value)
{
	LLItem<T>* itm;

	try {
		itm = new LLItem<T>();
	}
	catch(...)
	{
		throw;
	}

	try {
		itm->_value = value;
	}
	catch(...)
	{
		delete itm;
		throw;
	}

	_size++;
	itm->_prev = NULL;
	itm->_next = _first;
	if(_first)
		_first->_prev = itm;
	else
		_end = itm;
	_first = itm;
}

/* void SList<T>::pop_front()
 * Preconditions: none
 * Postconditions:
 * 		removes the first item from the list
 * Strong Guarantee
 */
template <typename T>
void SList<T>::pop_front()
{
	if(_first)
	{
		auto itm = _first;
		if(_first->_next)
			_first->_next->_prev = NULL;
		_first = _first->_next;
		delete itm;
		_size--;
	}
}

template <typename T>
void SList<T>::reverse()
{
	auto itm = _first;
	for(auto i = 0; i < _size; i++)
	{
		auto newitm = itm->_next;
		auto temp = itm->_prev;
		itm->_prev = itm->_next;
		itm->_next = temp;
		itm = newitm;
	}

	auto temp = _first;
	_first = _end;
	_end = temp;

}

template <typename T>
void SList<T>::clear() //Empty the list, doesn't use the iterator for no-throw
{
	if(!_size) //Already done if we are already clear
		return;

	auto start = _first;
	start = start->_next;

	for(auto i = start; i != NULL; i=i->_next) //Walk the list, deleting the previous item
	{
		auto val = i->_prev;
		delete val;
	}
	delete _end; //Delete the last item

	_size = 0; //Reset
	_first = NULL;
	_end = NULL;
}

template <typename T>
void SList<T>::link_item_end(LLItem<T>* itm) //Links an item to the end of the list
{
	if(!_first)
	{
		_first = itm;
		_end = itm;
		itm->_prev = 0;
		itm->_next = 0;
	}
	else
	{
		_end->_next = itm;
		itm->_prev = _end;
		itm->_next = 0;
		_end = itm;
	}
	_size++;
}

/*
 * begin()
 * Preconditions:
 * 	None
 * Postconditions:
 * 	returns an iterator to the first item
 */
template <typename T>
typename SList<T>::iterator SList<T>::begin() const
{
	return iterator(_first);
}

/*
 * end()
 * Preconditions:
 * 	None
 * Postconditions:
 * 	returns an iterator to NULL (end of list)
 */
template <typename T>
typename SList<T>::iterator SList<T>::end() const
{
	return iterator(NULL);
}

/*
 * void read(InputIterator first, InputIterator last)
 * Preconditions: the given arguments are valid Iterators
 * Postconditions: this will contain a copy of the given range
 * Exception Guarantee: Basic, Exception Neutral
 * May throw std::bad_alloc
 */
template <typename T>
template <typename InputIterator>
void SList<T>::read(InputIterator first, InputIterator last)
{
	clear();

	LLItem<T>* itm;

	try
	{
		for(auto itr = first; itr != last; itr++)
		{
			itm = new LLItem<T>();
			itm->_value = *itr;
			link_item_end(itm);
		}
	}
	catch(...)
	{
		delete itm;
		clear();
		throw;
	}
}

#endif
