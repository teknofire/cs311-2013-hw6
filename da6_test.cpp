// da6_test.cpp
// Chris Hartman
// April, 2010

// based on


// da6_test.cpp
// Glenn G. Chappell
// 7 Nov 2009
//
// For CS 311 Fall 2009
// Test program for classes SList, SLStack
// Used in Assignment 6, Exercises A & B

// Includes for code to be tested
#include "slist.h"      // For class template SList
#include "slist.h"      // Double inclusion test
#include "slstack.h"    // For class template SLStack
#include "slstack.h"    // Double inclusion test

// Includes for testing package & code common to all test programs
#include <iostream>     // for std::cout, std::endl, std::cin
#include <string>       // for std::string
#include <stdexcept>    // for std::runtime_error

// Additional includes for this test program
#include <cstdlib>      // for std::size_t, std::ptrdiff_t
#include <deque>        // for std::deque
#include <iterator>     // for std::back_inserter
#include <list>         // for std::list
#include <algorithm>    // for std::equal, std::reverse


// ************************************************************************
// Testing Package:
//     Class Tester - For Tracking Tests
// ************************************************************************


// class Tester
// For extremely simple unit testing.
// Keeps track of number of tests and number of passes.
// Use test (with success/failure parameter) to do a test.
// Get results with numTests, numPassed, numFailed, allPassed.
// Restart testing with reset.
// Invariants:
//     countTests_ == number of tests (calls to test) since last reset.
//     countPasses_ == number of times function test called with true param
//      since last reset.
//     0 <= countPasses_ <= countTests_.
//     tolerance_ >= 0.
class Tester {

// ***** Tester: ctors, dctor, op= *****
public:

    // Default ctor
    // Sets countTests_, countPasses_ to zero, tolerance_ to given value
    // Pre: None.
    // Post:
    //     numTests == 0, countPasses == 0, tolerance_ == abs(theTolerance)
    // Does not throw (No-Throw Guarantee)
    Tester(double theTolerance = 0.0000001)
        :countTests_(0),
         countPasses_(0),
         tolerance_(theTolerance >= 0 ? theTolerance : -theTolerance)
    {}

    // Compiler-generated copy ctor, copy op=, dctor are used

// ***** Tester: general public functions *****
public:

    // test
    // Handles single test, param indicates pass/fail
    // Pre: None.
    // Post:
    //     countTests_ incremented
    //     countPasses_ incremented if (success)
    //     Message indicating test name (if given)
    //      and pass/fail printed to cout
    // Does not throw (No-Throw Guarantee)
    //  - Assuming exceptions have not been turned on for cout.
    void test(bool success,
              const std::string & testName = "")
    {
        ++countTests_;
        if (success) ++countPasses_;

        std::cout << "    ";
        if (testName != "")
        {
            std::cout << "Test: "
                      << testName
                      << " - ";
        }
        std::cout << (success ? "passed" : "********** FAILED **********")
                  << std::endl;
    }

    // ftest
    // Does single floating-point test.
    // Tests passes iff difference of first two values is <= tolerance.
    // Pre: None.
    // Post:
    //     countTests_ incremented
    //     countPasses_ incremented if (abs(val1-val2) <= tolerance_)
    //     Message indicating test name (if given)
    //      and pass/fail printed to cout
    // Does not throw (No-Throw Guarantee)
    void ftest(double val1,
               double val2,
               const std::string & testName = "")
    { test(val1-val2 <= tolerance_ && val2-val1 <= tolerance_, testName); }

    // reset
    // Resets *this to default constructed state
    // Pre: None.
    // Post:
    //     countTests_ == 0, countPasses_ == 0
    // Does not throw (No-Throw Guarantee)
    void reset()
    {
        countTests_ = 0;
        countPasses_ = 0;
    }

    // numTests
    // Returns the number of tests that have been done since last reset 
    // Pre: None.
    // Post:
    //     return == countTests_
    // Does not throw (No-Throw Guarantee)
    int numTests() const
    { return countTests_; }

    // numPassed
    // Returns the number of tests that have passed since last reset
    // Pre: None.
    // Post:
    //     return == countPasses_
    // Does not throw (No-Throw Guarantee)
    int numPassed() const
    { return countPasses_; }

    // numFailed
    // Returns the number of tests that have not passed since last reset
    // Pre: None.
    // Post:
    //     return + countPasses_ == numTests_
    // Does not throw (No-Throw Guarantee)
    int numFailed() const
    { return countTests_ - countPasses_; }

    // allPassed
    // Returns true if all tests since last reset have passed
    // Pre: None.
    // Post:
    //     return == (countPasses_ == countTests_)
    // Does not throw (No-Throw Guarantee)
    bool allPassed() const
    { return countPasses_ == countTests_; }

    // setTolerance
    // Sets tolerance_ to given value
    // Pre: None.
    // Post:
    //     tolerance_ = abs(theTolerance)
    // Does not throw (No-Throw Guarantee)
    void setTolerance(double theTolerance)
    { tolerance_ = (theTolerance >= 0 ? theTolerance : -theTolerance); }

// ***** Tester: data members *****
private:

    int countTests_;    // Number of tests done since last reset
    int countPasses_;   // Number of tests passed since last reset
    double tolerance_;  // Tolerance for floating-point near-equality tests

};  // end class Tester


// ************************************************************************
// Testing Package:
//     Class TypeCheck - Helper Class for Type Checking
// ************************************************************************


// class TypeCheck
// This class exists in order to have static member function check, which
// takes a parameter of a given type, by reference. Objects of type
// TypeCheck<T> cannot be created.
// Usage:
//     TypeCheck<MyType>::check(x)
//     returns true if the type of x is (MyType) or (const MyType),
//     otherwise false.
// Invariants: None.
// Requirements on Types: None.
template<typename T>
class TypeCheck {

private:

    // Uncopyable class. Do not define copy ctor, copy assn.
    TypeCheck(const TypeCheck &);
    TypeCheck<T> & operator=(const TypeCheck &);

    // Compiler-generated dctor is used (but irrelevant).

public:

    // check
    // The function and function template below simulate a single function
    // that takes a single parameter, and returns true iff the parameter has
    // type T or (const T).

    // check (reference-to-const T)
    // Pre: None.
    // Post:
    //     Return is true.
    // Does not throw (No-Throw Guarantee)
    static bool check(const T & param)
    { return true; }

    // check (reference-to-const non-T)
    // Pre: None.
    // Post:
    //     Return is false.
    // Requirements on types: None.
    // Does not throw (No-Throw Guarantee)
    template <typename OtherType>
    static bool check(const OtherType & param)
    { return false; }

};  // End class TypeCheck


// ************************************************************************
// Testing Package:
//     Class Counter - Helper Class for Counting Calls & Objects, Throwing
// ************************************************************************


// class Counter
// Item type for counting ctor, dctor, op= calls, counting existing
//  objects, and possibly throwing on copy. Has operator< (which always
//  returns false), allowing it to be the value type of a sorted container.
// If static member copyThrow_ is set, then copy ctor and copy assn throw
//  std::runtime_error. Exception object's "what" member is set to "C" by
//  the copy ctor and "A" by copy assn.
// Increments static data member ctorCount_ on default construction and
//  successful copy construction. Increments static data member assnCount_
//  on successful copy assignment. Increments static data member
//  dctorCount_ on destruction.
// Increments static data member existing_ on construction, and decrements
//  it on destruction.
// Static data member maxExisting_ is highest value of existing_ since last
//  reset, or start of program if reset has never been called.
// Invariants:
//     Counter::existing_ is number of existing objects of this class.
//     Counter::ctorCount_ is number of successful ctor calls since
//      most recent call to reset, or start of program if reset has never
//      been called.
//     Counter::dctorCount_ is (similarly) number of dctor calls.
//     Counter::assnCount_ is (similarly) number of copy assn calls.
//     Counter::maxExisting_ is (similarly) highest value existing_ has
//      assumed.
class Counter {

// ***** Counter: Ctors, dctor, op= *****
public:

    // Default ctor
    // Pre: None.
    // Post:
    //     (ctorCount_ has been incremented.)
    //     (existing_ has been incremented.)
    // Does not throw (No-Throw Guarantee)
    Counter()
    {
        ++existing_;
        if (existing_ > maxExisting_)
            maxExisting_ = existing_;
        ++ctorCount_;
    }

    // Copy ctor
    // Throws std::runtime_error if copyThrow_.
    // Pre: None.
    // Post:
    //     (ctorCount_ has been incremented.)
    //     (existing_ has been incremented.)
    // May throw std::runtime_error
    // Strong Guarantee
    Counter(const Counter & other)
    {
        if (copyThrow_)
            throw std::runtime_error("C");
        ++existing_;
        if (existing_ > maxExisting_)
            maxExisting_ = existing_;
        ++ctorCount_;
    }

    // Copy assignment
    // Throws std::runtime_error if copyThrow_.
    // Pre: None.
    // Post:
    //     Return value is *this.
    //     (assnCount_ has been incremented.)
    // May throw std::runtime_error
    // Strong Guarantee
    Counter & operator=(const Counter & rhs)
    {
        if (copyThrow_)
            throw std::runtime_error("A");
        ++assnCount_;
        return *this;
    }

    // Dctor
    // Pre: None.
    // Post:
    //     (dctorCount_ has been incremented.)
    //     (existing_ has been decremented.)
    // Does not throw (No-Throw Guarantee)
    ~Counter()
    {
        --existing_;
        ++dctorCount_;
    }

// ***** Counter: Functions dealing with count *****
public:

    // reset
    // Pre: None.
    // Post:
    //     maxExisting_ == existing_.
    //     ctorCount_ == 0.
    //     dctorCount_ == 0.
    //     assnCount_ == 0.
    //     copyThrow_ == shouldThrow.
    // Does not throw (No-Throw Guarantee)
    static void reset(bool shouldThrow = false)
    {
        maxExisting_ = existing_;
        ctorCount_ = 0;
        dctorCount_ = 0;
        assnCount_ = 0;
        copyThrow_ = shouldThrow;
    }

    // getExisting
    // Pre: None.
    // Post:
    //     return == existing_.
    // Does not throw (No-Throw Guarantee)
    static int getExisting()
    { return existing_; }

    // getMaxExisting
    // Pre: None.
    // Post:
    //     return == maxExisting_.
    // Does not throw (No-Throw Guarantee)
    static int getMaxExisting()
    { return maxExisting_; }

    // getCtorCount
    // Pre: None.
    // Post:
    //     return == ctorCount_.
    // Does not throw (No-Throw Guarantee)
    static int getCtorCount()
    { return ctorCount_; }

    // getDctorCount
    // Pre: None.
    // Post:
    //     return == dctorCount_.
    // Does not throw (No-Throw Guarantee)
    static int getDctorCount()
    { return dctorCount_; }

    // getAssnCount
    // Pre: None.
    // Post:
    //     return == assnCount_.
    // Does not throw (No-Throw Guarantee)
    static int getAssnCount()
    { return assnCount_; }

    // setCopyThrow
    // Pre: None.
    // Post:
    //     copyThrow_ == shouldThrow
    // Does not throw (No-Throw Guarantee)
    static void setCopyThrow(bool shouldThrow)
    { copyThrow_ = shouldThrow; }

// ***** Counter: Data Members *****
private:

    static int existing_;     // # of existing objects
    static int maxExisting_;  // Max # of existing objects
    static int ctorCount_;    // # of successful (non-throwing) ctor calls
    static int dctorCount_;   // # of dctor calls
    static int assnCount_;    // # of successful (non-throwing) copy = calls
    static bool copyThrow_;   // true if copy operations (ctor, =) throw

};  // End class Counter

// Definition of static data member of class Counter
int Counter::existing_ = 0;
int Counter::maxExisting_ = 0;
int Counter::ctorCount_ = 0;
int Counter::dctorCount_ = 0;
int Counter::assnCount_ = 0;
bool Counter::copyThrow_ = false;


// operator< (Counter)
// Dummy-ish operator<, forming a strict weak order for Counter class
// Returns false (which is legal for a strict weak order; all objects of
//  type Counter are equivalent).
// Pre: None.
// Post:
//     Return value == false.
// Does not throw (No-Throw Guarantee)
bool operator<(const Counter & a,
               const Counter & b)
{ return false; }


// ************************************************************************
// Utility Functions/Classes for This Testing Program
// ************************************************************************


// inRange
// Checks whether v lies in [a,b]; all values are ints.
// Pre: None.
// Post: Return value is true if a <= v and v <= b,
//     false otherwise.
// Does not throw (No-Throw Guarantee)
bool inRange(int v, int a, int b)
{
    return a <= v && v <= b;
}


// ************************************************************************
// Test Suite Functions
// ************************************************************************


// test_class_SList_types
// Test suite for class SList, types
// Pre: None.
// Post:
//     Pass/fail status of tests have been registered with t.
//     Appropriate messages have been printed to cout.
// Does not throw (No-Throw Guarantee)
void test_class_SList_types(Tester & t)
{
    std::cout << "Test Suite: class SList, types" << std::endl;

    bool correctType1;  // result of type checking
    bool correctType2;  // result of type checking

    const SList<double> sld;

    // MEMBER FUNCTION size
    // Check return type
    correctType1 = TypeCheck<std::size_t>::check(sld.size());
        // ignore const-ness
    correctType2 = TypeCheck<std::ptrdiff_t>::check(sld.size());
        // ignore const-ness
    t.test(correctType1 || correctType2, "Return type, function size");
}


// test_class_SList_default_ctor
// Test suite for class SList, default ctor
// Pre: None.
// Post:
//     Pass/fail status of tests have been registered with t.
//     Appropriate messages have been printed to cout.
// Does not throw (No-Throw Guarantee)
void test_class_SList_default_ctor(Tester & t)
{
    std::cout << "Test Suite: class SList, default ctor" << std::endl;

    const SList<int> sli1;
    t.test(sli1.size() == 0, "Default ctor, const - size is zero");

    std::deque<int> vi1;
    sli1.write(std::back_inserter(vi1));
    t.test(vi1.empty(), "Default ctor, const - write does nothing");

    SList<double> sld1;
    t.test(sld1.size() == 0, "Default ctor, non-const - size is zero");

    std::deque<double> vd1;
    sld1.write(std::back_inserter(vd1));
    t.test(vd1.empty(), "Default ctor, non-const - write does nothing");
}


// test_class_SList_read_write
// Test suite for class SList, read & write
// Pre: None.
// Post:
//     Pass/fail status of tests have been registered with t.
//     Appropriate messages have been printed to cout.
// Does not throw (No-Throw Guarantee)
void test_class_SList_read_write(Tester & t)
{
    std::cout << "Test Suite: class SList, read & write" << std::endl;

    // Data for reading
    const unsigned int DATA1SIZE = 10;
    int data1[DATA1SIZE] = { 5, 2, 4, 3, 9, 3, 6, 6, 8, 2 };
    int data2[DATA1SIZE+2] = { -1, 5, 2, 4, 3, 9, 3, 6, 6, 8, 2, -1 };
    const unsigned int DATA3SIZE = 3;
    int data3[DATA3SIZE] = { 2, 6, 10 };

    // Long read
    SList<int> sli1;
    sli1.read(data1, data1+DATA1SIZE);

    const SList<int> & sli1r(sli1);

    t.test(sli1r.size() == DATA1SIZE, "long read - size");
    std::deque<int> vi1;
    sli1r.write(std::back_inserter(vi1));
    t.test(vi1.size() == DATA1SIZE, "long read/write - size");
    t.test(std::equal(vi1.begin(), vi1.end(), data1), "long read/write - values");

    // Write to std::list
    std::list<int> li1(DATA1SIZE+2, -1);
    std::list<int>::iterator iter1 = li1.begin();
    ++iter1;
    sli1r.write(iter1);
    t.test(std::equal(li1.begin(), li1.end(), data2),
        "long read/write to std::list - values");

    // Read again
    sli1.read(data3, data3+DATA3SIZE);
    t.test(sli1r.size() == DATA3SIZE, "read again - size");
    std::deque<int> vi2;
    sli1r.write(std::back_inserter(vi2));
    t.test(vi2.size() == DATA3SIZE, "read again/write - size");
    t.test(std::equal(vi2.begin(), vi2.end(), data3),
        "read again/write - values");

    // Empty read
    SList<int> sli2;
    sli2.read(data1, data1);

    t.test(sli2.size() == 0, "empty read, size");
    std::deque<int> vi3;
    sli2.write(std::back_inserter(vi3));
    t.test(vi3.size() == 0, "empty read/write - size");

    // Singleton read
    SList<int> sli3;
    sli3.read(data1, data1+1);

    t.test(sli3.size() == 1, "singleton read - size");
    std::deque<int> vi4;
    sli3.write(std::back_inserter(vi4));
    t.test(vi4.size() == 1, "singleton read/write - size");
    t.test(std::equal(vi4.begin(), vi4.end(), data1),
        "singleton read/write - values");

    // Read from std::list
    SList<int> sli4;
    std::list<int> li2(data1, data1+DATA1SIZE);
    sli4.read(li2.begin(), li2.end());
    t.test(sli4.size() == DATA1SIZE, "read from list - size");
    std::deque<int> vi5;
    sli4.write(std::back_inserter(vi5));
    t.test(vi5.size() == DATA1SIZE, "read from list/write - size");
    t.test(std::equal(vi5.begin(), vi5.end(), data1),
        "read from list/write - values");
}


// test_class_SList_copy_ctor
// Test suite for class SList, copy ctor
// Pre: None.
// Post:
//     Pass/fail status of tests have been registered with t.
//     Appropriate messages have been printed to cout.
// Does not throw (No-Throw Guarantee)
void test_class_SList_copy_ctor(Tester & t)
{
    std::cout << "Test Suite: class SList, copy ctor" << std::endl;

    // Data for reading
    const unsigned int DATA1SIZE = 10;
    int data1[DATA1SIZE] = { 5, 2, 4, 3, 9, 3, 6, 6, 8, 2 };
    const unsigned int DATA2SIZE = 4;
    int data2[DATA2SIZE] = { 6, 5, 4, 3 };
    const unsigned int DATA3SIZE = 2;
    int data3[DATA3SIZE] = { 10, 11 };

    SList<int> sli1;
    sli1.read(data1, data1+DATA1SIZE);

    const SList<int> & sli1r(sli1);

    // Make copy from const
    SList<int> sli2(sli1r);

    // Check copy
    t.test(sli2.size() == DATA1SIZE, "Copy ctor - size");
    std::deque<int> vi1;
    sli2.write(std::back_inserter(vi1));
    t.test(vi1.size() == DATA1SIZE, "Copy ctor - write, size");
    t.test(std::equal(vi1.begin(), vi1.end(), data1),
        "Copy ctor - write, values");

    // Check original
    t.test(sli1.size() == DATA1SIZE, "Original - size");
    std::deque<int> vi2;
    sli1.write(std::back_inserter(vi2));
    t.test(vi1.size() == DATA1SIZE, "Original - write, size");
    t.test(std::equal(vi2.begin(), vi2.end(), data1),
        "Original - write, values");

    // Modify original, check copy
    sli1.read(data2, data2+DATA2SIZE);
    t.test(sli2.size() == DATA1SIZE, "Mod original, check copy - size");
    std::deque<int> vi3;
    sli2.write(std::back_inserter(vi3));
    t.test(vi3.size() == DATA1SIZE, "Mod original, check copy - write, size");
    t.test(std::equal(vi3.begin(), vi3.end(), data1),
        "Mod original, check copy - write, values");

    // Modify copy, check original
    sli2.read(data3, data3+DATA3SIZE);
    t.test(sli1.size() == DATA2SIZE, "Mod copy, check original - size");
    std::deque<int> vi4;
    sli1.write(std::back_inserter(vi4));
    t.test(vi4.size() == DATA2SIZE, "Mod copy, check original - write, size");
    t.test(std::equal(vi4.begin(), vi4.end(), data2),
        "Mod copy, check original - write, values");
}


// test_class_SList_copy_assn
// Test suite for class SList, copy assignment
// Pre: None.
// Post:
//     Pass/fail status of tests have been registered with t.
//     Appropriate messages have been printed to cout.
// Does not throw (No-Throw Guarantee)
void test_class_SList_copy_assn(Tester & t)
{
    std::cout << "Test Suite: class SList, copy assignment" << std::endl;

    // Data for reading
    const unsigned int DATA1SIZE = 10;
    int data1[DATA1SIZE] = { 5, 2, 4, 3, 9, 3, 6, 6, 8, 2 };
    const unsigned int DATA2SIZE = 4;
    int data2[DATA2SIZE] = { 6, 5, 4, 3 };
    const unsigned int DATA3SIZE = 2;
    int data3[DATA3SIZE] = { 10, 11 };

    SList<int> sli1;
    sli1.read(data1, data1+DATA1SIZE);

    const SList<int> & sli1r(sli1);

    // Make copy from const
    SList<int> sli2;
    sli2 = sli1r;

    // Check copy
    t.test(sli2.size() == DATA1SIZE, "Copy assn - size");
    std::deque<int> vi1;
    sli2.write(std::back_inserter(vi1));
    t.test(vi1.size() == DATA1SIZE, "Copy assn - write, size");
    t.test(std::equal(vi1.begin(), vi1.end(), data1),
        "Copy ctor - write, values");

    // Check original
    t.test(sli1.size() == DATA1SIZE, "Original - size");
    std::deque<int> vi2;
    sli1.write(std::back_inserter(vi2));
    t.test(vi1.size() == DATA1SIZE, "Original - write, size");
    t.test(std::equal(vi2.begin(), vi2.end(), data1),
        "Original - write, values");

    // Modify original, check copy
    sli1.read(data2, data2+DATA2SIZE);
    t.test(sli2.size() == DATA1SIZE, "Mod original, check copy - size");
    std::deque<int> vi3;
    sli2.write(std::back_inserter(vi3));
    t.test(vi3.size() == DATA1SIZE, "Mod original, check copy - write, size");
    t.test(std::equal(vi3.begin(), vi3.end(), data1),
        "Mod original, check copy - write, values");

    // Modify copy, check original
    sli2.read(data3, data3+DATA3SIZE);
    t.test(sli1.size() == DATA2SIZE, "Mod copy, check original - size");
    std::deque<int> vi4;
    sli1.write(std::back_inserter(vi4));
    t.test(vi4.size() == DATA2SIZE, "Mod copy, check original - write, size");
    t.test(std::equal(vi4.begin(), vi4.end(), data2),
        "Mod copy, check original - write, values");
}


// test_class_SList_item_counts
// Test suite for class SList, item counts
// Pre: None.
// Post:
//     Pass/fail status of tests have been registered with t.
//     Appropriate messages have been printed to cout.
// Does not throw (No-Throw Guarantee)
void test_class_SList_item_counts(Tester & t)
{
    std::cout << "Test Suite: class SList, item counts" << std::endl;

    const int DATA1SIZE = 10;
    Counter data1[DATA1SIZE];

    {
    SList<Counter> slc1;

    t.test(inRange(Counter::getExisting(), DATA1SIZE, DATA1SIZE+1),
        "default ctor - no items created");

    slc1.read(data1, data1+DATA1SIZE);
    t.test(inRange(Counter::getExisting(), DATA1SIZE*2, DATA1SIZE*2+1),
        "read - check number of items created");

    slc1.read(data1, data1+DATA1SIZE);
    t.test(inRange(Counter::getExisting(), DATA1SIZE*2, DATA1SIZE*2+1),
        "read again - check number of items created");

    SList<Counter> slc2(slc1);

    t.test(inRange(Counter::getExisting(), DATA1SIZE*3, DATA1SIZE*3+2),
        "copy ctor - check number of items created");
    }

    t.test(Counter::getExisting() == DATA1SIZE, "dctor - items destroyed");
}


// test_class_SList_reverse
// Test suite for class SList, reverse
// Pre: None.
// Post:
//     Pass/fail status of tests have been registered with t.
//     Appropriate messages have been printed to cout.
// Does not throw (No-Throw Guarantee)
void test_class_SList_reverse(Tester & t)
{
    std::cout << "Test Suite: class SList, reverse" << std::endl;

    // Data for reading
    const unsigned int DATA1SIZE = 10;
    int data1[DATA1SIZE] = { 5, 2, 4, 3, 9, 3, 6, 6, 8, 2 };

    // Read & reverse
    SList<int> sli1;
    sli1.read(data1, data1+DATA1SIZE);
    sli1.reverse();
    std::reverse(data1, data1+DATA1SIZE);
    t.test(sli1.size() == DATA1SIZE, "reverse - size");
    std::deque<int> vi1;
    sli1.write(std::back_inserter(vi1));
    t.test(vi1.size() == DATA1SIZE, "reverse - write, size");
    t.test(std::equal(vi1.begin(), vi1.end(), data1), "reverse - write, values");

    // Read & reverse, number of items
    const int DATA2SIZE = 20;
    Counter data2[DATA2SIZE];
    int numOps;
    {
    SList<Counter> slc1;
    slc1.read(data2, data2+DATA2SIZE);
    numOps = Counter::getCtorCount()
           + Counter::getDctorCount()
           + Counter::getAssnCount();
    slc1.reverse();
    t.test(Counter::getCtorCount()
         + Counter::getDctorCount()
         + Counter::getAssnCount() == numOps,
        "reverse - no value-type operations performed");
    t.test(inRange(Counter::getExisting(), DATA2SIZE*2, DATA2SIZE*2+1),
        "reverse - number of items existing");

    slc1.read(data2, data2+DATA2SIZE);
    t.test(inRange(Counter::getExisting(), DATA2SIZE*2, DATA2SIZE*2+1),
        "reverse/read again - number of items existing");
    }
    t.test(Counter::getExisting() == DATA2SIZE,
        "reverse - dctor, items destroyed");
}


// test_class_SList
// Test suite for class SList
// Uses other test-suite functions
// Pre: None.
// Post:
//     Pass/fail status of tests have been registered with t.
//     Appropriate messages have been printed to cout.
// Does not throw (No-Throw Guarantee)
void test_class_SList(Tester & t)
{
    // Do all the test suites
    std::cout << "TEST SUITES FOR CLASS SList" << std::endl;
    test_class_SList_types(t);
    test_class_SList_default_ctor(t);
    test_class_SList_read_write(t);
    test_class_SList_copy_ctor(t);
    test_class_SList_copy_assn(t);
    test_class_SList_item_counts(t);
    test_class_SList_reverse(t);
}


// test_class_SLStack_types
// Test suite for class SLStack, types
// Pre: None.
// Post:
//     Pass/fail status of tests have been registered with t.
//     Appropriate messages have been printed to cout.
// Does not throw (No-Throw Guarantee)
void test_class_SLStack_types(Tester & t)
{
    std::cout << "Test Suite: class SLStack, types" << std::endl;

    bool correctType;  // result of type checking

    SLStack<double> slsd;
    slsd.push(1.1);  // Make non-empty, so can call SLStack::top
    const SLStack<double> & cslsd = slsd;
    SLStack<int> slsi;
    slsi.push(5);    // Make non-empty, so can call SLStack::top
    const SLStack<int> & cslsi = slsi;

    // MEMBER FUNCTION empty
    // Check return type
    correctType = TypeCheck<bool>::check(cslsd.empty());  // ignore const-ness
    t.test(correctType, "Return type, function empty");

    // MEMBER FUNCTION top
    // Check return type
    correctType = TypeCheck<double>::check(cslsd.top());  // ignore const-ness
    t.test(correctType, "Return type, function top, const, #1");

    correctType = TypeCheck<int>::check(cslsi.top());     // ignore const-ness
    t.test(correctType, "Return type, function top, const, #2");

    correctType = TypeCheck<double>::check(slsd.top());   // ignore const-ness
    t.test(correctType, "Return type, function top, non-const");
}


// test_class_SLStack_const_funcs
// Test suite for class SList, const member functions
// Pre: None.
// Post:
//     Pass/fail status of tests have been registered with t.
//     Appropriate messages have been printed to cout.
// Does not throw (No-Throw Guarantee)
void test_class_SLStack_const_funcs(Tester & t)
{
    std::cout << "Test Suite: class SLStack, const member functions" << std::endl;

    SLStack<int> si1;
    si1.push(17);

    const SLStack<int> & si1r(si1);

    t.test(!si1r.empty(), "const object - not empty");
    t.test(si1r.top() == 17, "const object - top value");
}


// test_class_SLStack_ppte_simple
// Test suite for class SList, push, pop, top, and empty, simple
// Pre: None.
// Post:
//     Pass/fail status of tests have been registered with t.
//     Appropriate messages have been printed to cout.
// Does not throw (No-Throw Guarantee)
void test_class_SLStack_ppte_simple(Tester & t)
{
    std::cout << "Test Suite: class SLStack, push, pop, top, and empty, simple"
              << std::endl;

    SLStack<int> si1;
    t.test(si1.empty(), "default ctor - empty");
    si1.push(2);
    t.test(!si1.empty(), "push - not empty");
    t.test(si1.top() == 2, "top - value");
    si1.push(1);
    t.test(!si1.empty(), "push again - not empty");
    t.test(si1.top() == 1, "top - value");
    si1.pop();
    t.test(!si1.empty(), "pop - not empty");
    t.test(si1.top() == 2, "top - value");
    si1.pop();
    t.test(si1.empty(), "pop again - empty");
}


// test_class_SLStack_ppte_complex
// Test suite for class SList, push, pop, top, and empty, complex
// Pre: None.
// Post:
//     Pass/fail status of tests have been registered with t.
//     Appropriate messages have been printed to cout.
// Does not throw (No-Throw Guarantee)
void test_class_SLStack_ppte_complex(Tester & t)
{
    std::cout << "Test Suite: class SLStack, push, pop, top, and empty, complex"
        << std::endl;

    // Data for reading
    const int DATA1SIZE = 10;
    double data1[DATA1SIZE] =
        { 5.1, 2.2, 4.3, 3.4, 9.5, 3.6, 6.7, 6.8, 8.9, 2.0 };
    int i; // loop counter

    SLStack<double> sd1;
    t.test(sd1.empty(), "default ctor - empty");
    sd1.push(1.1);
    t.test(!sd1.empty(), "push - not empty");
    t.test(sd1.top() == 1.1, "top - value");
    sd1.pop();
    t.test(sd1.empty(), "pop - empty");
    sd1.push(-1.0);
    t.test(!sd1.empty(), "push - not empty");
    t.test(sd1.top() == -1.0, "top - value");

    bool emptyTests = true;
    bool valueTests = true;
    for (i = 0; i < DATA1SIZE; ++i)
    {
        sd1.push(data1[i]);
        emptyTests = (!sd1.empty()) && emptyTests;
        valueTests = (sd1.top() == data1[i]) && valueTests;
    }
    t.test(emptyTests, "multiple pushes - empty tests");
    t.test(valueTests, "multiple pushes - value tests");

    emptyTests = true;
    valueTests = true;
    for (int i = DATA1SIZE-1; i >= 0; --i)
    {
        sd1.pop();
        if (i > 0)
        {
            emptyTests = (!sd1.empty()) && emptyTests;
            valueTests = (sd1.top() == data1[i-1]) && valueTests;
        }
    }
    t.test(emptyTests, "multiple pops - empty tests");
    t.test(valueTests, "multiple pops - value tests");

    t.test(!sd1.empty(), "multiple ops done - not empty");
    t.test(sd1.top() == -1.0, "multiple ops done - top value");
    sd1.pop();
    t.test(sd1.empty(), "last pop - empty");
}


// test_class_SLStack_copy_ctor
// Test suite for class SList, copy ctor
// Pre: None.
// Post:
//     Pass/fail status of tests have been registered with t.
//     Appropriate messages have been printed to cout.
// Does not throw (No-Throw Guarantee)
void test_class_SLStack_copy_ctor(Tester & t)
{
    std::cout << "Test Suite: class SLStack, copy ctor" << std::endl;

    // Data for reading
    const int DATA1SIZE = 10;
    int data1[DATA1SIZE] = { 5, 2, 4, 3, 9, 3, 6, 6, 8, 2 };

    int i;  // loop counter

    SLStack<int> si1;
    for (i = 0; i < DATA1SIZE; ++i)
    {
        si1.push(data1[i]);
    }

    const SLStack<int> & si1r(si1);
    SLStack<int> si2(si1r);

    bool emptyTests = true;
    bool valueTests = true;
    for (i = DATA1SIZE-1; i >= 0; --i)
    {
        emptyTests = (!si2.empty()) && emptyTests;
        valueTests == (si2.top() == data1[i]) && valueTests;
        si2.pop();
    }
    t.test(emptyTests, "copy ctor - multiple pops, empty tests");
    t.test(valueTests, "copy ctor - multiple pops, value tests");
    t.test(si2.empty(), "copy ctor - multiple pops done, empty");
}


// test_class_SLStack_copy_assn
// Test suite for class SList, copy assignment
// Pre: None.
// Post:
//     Pass/fail status of tests have been registered with t.
//     Appropriate messages have been printed to cout.
// Does not throw (No-Throw Guarantee)
void test_class_SLStack_copy_assn(Tester & t)
{
    std::cout << "Test Suite: class SLStack, copy assignment" << std::endl;

    // Data for reading
    const int DATA1SIZE = 10;
    int data1[DATA1SIZE] = { 5, 2, 4, 3, 9, 3, 6, 6, 8, 2 };

    int i;  // loop counter

    SLStack<int> si1;
    for (i = 0; i < DATA1SIZE; ++i)
    {
        si1.push(data1[i]);
    }

    const SLStack<int> & si1r(si1);
    SLStack<int> si2;
    si2 = si1r;

    bool emptyTests = true;
    bool valueTests = true;
    for (i = DATA1SIZE-1; i >= 0; --i)
    {
        emptyTests = (!si2.empty()) && emptyTests;
        valueTests == (si2.top() == data1[i]) && valueTests;
        si2.pop();
    }
    t.test(emptyTests, "copy assn - multiple pops, empty tests");
    t.test(valueTests, "copy assn - multiple pops, value tests");
    t.test(si2.empty(), "copy assn - multiple pops done, empty");
}

// test_class_SLStack_item_counts
// Test suite for class SLStack, item counts
// Pre: None.
// Post:
//     Pass/fail status of tests have been registered with t.
//     Appropriate messages have been printed to cout.
// Does not throw (No-Throw Guarantee)
void test_class_SLStack_item_counts(Tester & t)
{
    std::cout << "Test Suite: class SLStack, item counts" << std::endl;

    {
    SLStack<Counter> slsc1;

    t.test(Counter::getExisting() == 0,
        "default ctor - no items created");

    slsc1.push(Counter());
    t.test(Counter::getExisting()==1,
        "push - check number of items created");

    slsc1.push(Counter());
    t.test((Counter::getExisting()==2),
        "push again - check number of items created");

    SLStack<Counter> slsc2(slsc1);

    t.test((Counter::getExisting()==4),
        "copy ctor - check number of items created");

    slsc1.pop();
    t.test(Counter::getExisting()==3,
        "pop - check number of items created");
    }

    t.test(Counter::getExisting() == 0, "dctor - items destroyed");
}


// test_class_SLStack
// Test suite for class SLStack
// Uses other test-suite functions
// Pre: None.
// Post:
//     Pass/fail status of tests have been registered with t.
//     Appropriate messages have been printed to cout.
// Does not throw (No-Throw Guarantee)
void test_class_SLStack(Tester & t)
{
    // Do all the test suites
    std::cout << "TEST SUITES FOR CLASS SLStack" << std::endl;
    test_class_SLStack_types(t);
    test_class_SLStack_const_funcs(t);
    test_class_SLStack_ppte_simple(t);
    test_class_SLStack_ppte_complex(t);
    test_class_SLStack_copy_ctor(t);
    test_class_SLStack_copy_assn(t);
    test_class_SLStack_item_counts(t);
}


// ************************************************************************
// Main program
// ************************************************************************


// main
// Runs class SList & class SLStack test suite, prints results to cout.
int main()
{
    Tester t;
    test_class_SList(t);
    std::cout << std::endl;
    test_class_SLStack(t);

    std::cout << std::endl;
    if (t.allPassed())
    {
        std::cout << "All tests successful" 
                  << std::endl;
    }
    else
    {
        std::cout << "Tests ********** UNSUCCESSFUL **********"
                  << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Press ENTER to quit ";
    while (std::cin.get() != '\n') ;

    return 0;
}

