#include <iostream>
#include <list>
#include <typeinfo>
#include <cstdlib>
#include "gc_details.h"
#include "gc_iterator.h"
// #define DEBUG
/*
    Pointer implements a pointer type that uses
    garbage collection to release unused memory.
    A Pointer must only be used to point to memory
    that was dynamically allocated using new.
    When used to refer to an allocated array,
    specify the array size.
*/
template <class T, int size = 0>
class Pointer{
private:
    /** Static variable 'refContainer' maintains the garbage collection list. */
    static std::list<PtrDetails<T> > refContainer;
    static bool first; // 'first' true when first Pointer is created
    /* Variable 'addr' points to the allocated heap memory which this Pointer object currently tracks.*/ 
    T *addr;
    /*  Property 'isArray' is true if & only if this Pointer points
        to an allocated array. 
        Variable 'arraySize' is greater than zero if & only if 'isArray' is true,
        in which case it contains the array's size.
    */
    bool isArray; // 'isArray' true if & only if object points to array
    unsigned arraySize; // 'arraySize' contains the size of the array
    /** 'fintPtrInfo()' returns an iterator to a PtrDetails object in refContainer. */
    typename std::list<PtrDetails<T> >::iterator findPtrInfo(T *ptr);
public:
    /* Define an iterator type for Pointer<T>. */
    typedef Iter<T> GCiterator;
    // Empty constructor
    // NOTE: templates aren't able to have prototypes with default arguments
    // this is why constructor is designed like this:
    Pointer(){
        Pointer(NULL);
    }
    Pointer(T* t);
    // Copy constructor.
    Pointer(const Pointer &);
    // Destructor for Pointer.
    ~Pointer();
    // Collect garbage. Returns true if at least
    // one object was freed.
    static bool collect();
    // Overload assignment of pointer to Pointer.
    T *operator=(T *t);
    // Overload assignment of Pointer to Pointer.
    Pointer &operator=(Pointer &rv);
    // Return a reference to the object pointed
    // to by this Pointer.
    T &operator*(){
        return *addr;
    }
    // Return the address being pointed to.
    T *operator->() { return addr; }
    T * get() const { return addr; }
    // Return a reference to the object at the
    // index specified by i.
    T &operator[](int i){ return addr[i];}
    // Conversion function to T *.
    operator T *() { return addr; }
    // Return an Iter to the start of the allocated memory.
    Iter<T> begin(){
        int _size;
        if (isArray)
            _size = arraySize;
        else
            _size = 1;
        return Iter<T>(addr, addr, addr + _size);
    }
    // Return an Iter to one past the end of an allocated array.
    Iter<T> end(){
        int _size;
        if (isArray)
            _size = arraySize;
        else
            _size = 1;
        return Iter<T>(addr + _size, addr, addr + _size);
    }
    // Return the size of refContainer for this type of Pointer.
    static int refContainerSize() { return refContainer.size(); }
    // A utility function that displays refContainer.
    static void showlist();
    // Clear refContainer when program exits.
    static void shutdown();
};

// STATIC INITIALIZATION
// Creates storage for the static variables
template <class T, int size>
std::list<PtrDetails<T> > Pointer<T, size>::refContainer;
template <class T, int size>
bool Pointer<T, size>::first = true;

/** Constructor for both initialized and uninitialized objects. 
 * Precondition: new Pointer object has no corresponding PtrDetails object 
 * in GC list.
 * Postcondition: new PtrDetails object corresponding to Pointer object is 
 * present in GC list, with reference count 1.
*/
template<class T,int size>
Pointer<T,size>::Pointer(T *t){
    // Register shutdown() as an exit function.
    if (first)
        std::atexit(shutdown);
    first = false;

    // TODO: Implement Pointer constructor
    // Lab: Smart Pointer Project Lab
    auto p = findPtrInfo(t);
    //If t already exists, increment its reference count;
    //otherwise, add pointer details to GC list
    if (p != refContainer.end())
        p->refcount++;
    else {
        // Create PtrDetails object and add to refContainer 
        refContainer.push_front(PtrDetails<T>(t, size));
    }
    this->addr = t;
    this->arraySize = size;
    if (size > 0)
        this->isArray = true;
    else
        this->isArray = false;
#ifdef DEBUG
    std::cout << "Called standard constructor for Pointer object with address: " 
              << t 
              << std::endl;
#endif
}

/** Copy constructor:
 * Preconditions: 
 * - RHS is already initialised, thus corresponds to a PtrDetails object in the GC list with reference count > 0.
 * - LHS Pointer object is also in GC list, with reference count > 0.
 * Postconditions: 
 * - LHS Pointer object has properties of ptrObj,
 * - reference count of previous object pointed to by LHS is decremented by 1 
 * - reference count of RHS PtrDetails object is incremented by 1.
 */
template< class T, int size>
Pointer<T,size>::Pointer(const Pointer &ptrObj){

    // TODO: Implement Pointer constructor
    // Lab: Smart Pointer Project Lab
    // decrement current object refcount
    auto prev = findPtrInfo(addr);
    prev->refcount--;
    // increment new object refcount
    auto rhs = findPtrInfo(ptrObj.get()); //must be present in GC list
    rhs->refcount++;
    //copy data
    this->addr = rhs->memPtr;
    this->isArray = rhs->isArray;
    this->arraySize = rhs->arraySize;
#ifdef DEBUG
    std::cout << "Called standard constructor for Pointer object with address: " 
              << addr 
              << std::endl;
#endif
}
/** Overload assignment of Pointer to Pointer (copy assignment operator):
 * Preconditions: 
 * - RHS 'ptrObj' is an existing Pointer object (call it P_1), not a raw pointer, so there will be a corresponding PtrDetails object in the GC list (with reference count > 0).
 * - LHS variable points to an already existing Pointer object with corresponding PtrDetails object in the GC list.
 *  
 */
template <class T, int size>
Pointer<T, size> &Pointer<T, size>::operator=(Pointer &ptrObj){
    // TODO: Implement operator=
    // LAB: Smart Pointer Project Lab
    //Find LHS and RHS pointer details
    auto lhs_pd = findPtrInfo(this->addr); // must exist
    auto rhs_pd = findPtrInfo(ptrObj.get()); // must exist
    if (lhs_pd != rhs_pd) // Guard against self assignment
    {
        //Decrement reference count for LHS pointer details 
        lhs_pd->refcount--;
        //Increment count for RHS pointer details 
        rhs_pd->refcount++;
        // Copy RHS details to LHS
        this->addr = rhs_pd->memPtr;
        this->isArray = rhs_pd->isArray;
        this->arraySize = rhs_pd->arraySize;
    }
    // Return
    return *this;
}
/** Overload assignment of raw pointer to Pointer:
 * Preconditions: 
 * - 't' is a raw pointer, NOT a Pointer object. If it already corresponds to an item in the GC list, then we do NOT assign (to prevent potential problems managing deletion). If it is not currently in the list, then we create a new PtrDetails object and add it to the GC list.
 * - If LHS is already initialised, then we need to decrement the reference count for its current PtrDetails object before associating it with a new PtrDetails object.
 */
template <class T, int size>
T *Pointer<T, size>::operator=(T *t){

    // TODO: Implement operator=
    // LAB: Smart Pointer Project Lab
    //Check whether LHS and RHS already have entries in GC list
    //Get LHS and RHS pointer details (if any) from GC list
    auto lhs_pd = findPtrInfo(this->addr); 
    auto rhs_pd = findPtrInfo(t); 

    if (rhs_pd != refContainer.end()) // RHS already in GC list: do not assign!
    {
        //Better to throw exception, so change this later
        std::cout << "Improper assignment to pointer." << std::endl;
    }
    else // RHS doesn't exist in GC list, so proceed with assignment
    {
        if (lhs_pd != refContainer.end()){
            --(lhs_pd->refcount);
        }
        // Create PtrDetails object and add to refContainer 
        this->addr = t;
        this->arraySize = size;
        refContainer.push_front(PtrDetails<T>(this->addr, this->arraySize));
    }
    //Return pointer to T
    return t;
}

// Destructor for Pointer.
template <class T, int size>
Pointer<T, size>::~Pointer(){
    
    // TODO: Implement Pointer destructor
    // Lab: New and Delete Project Lab
    //Find pointer details in GC list
    typename std::list<PtrDetails<T> >::iterator p;
    p = findPtrInfo(addr);
    //Decrement refcount of corresponding PtrDetails object
    if (p->refcount){
        p->refcount--;
    }
#ifdef DEBUG
    std::cout << "Pointer going out of scope, destructor called for address: " 
              << addr 
              << std::endl;
#endif
    //Recover memory --call collect()
    collect();
}

// Collect garbage. Returns true if at least
// one object was freed.
template <class T, int size>
bool Pointer<T, size>::collect(){
    // TODO: Implement collect function
    // LAB: New and Delete Project Lab
    // Note: collect() will be called in the destructor
    bool memory_freed = false;
    //Traverse the GC list, checking the refcount of the PtrDetails objects
    typename std::list<PtrDetails<T> >::iterator p;
    do {
        for (p = refContainer.begin(); p != refContainer.end(); p++){
            if (p->refcount > 0) //memory in use, so skip
                continue;
            memory_freed = true;
            //remove from GC list
            refContainer.remove(*p);
            //free memory
            if (p->memPtr){
                if (p->isArray){
                    delete[] p->memPtr;
                }
                else {
                    delete p->memPtr;
                }
            }
            //reset iterator and search for other zero refcounts
            break;
        }
    } while ( p != refContainer.end());
#ifdef DEBUG
    std::cout << "garbage collector called: " 
              << (memory_freed? "memory freed" : "no memory freed")
              << std::endl;
#endif
    return memory_freed;
}

// A utility function that displays refContainer.
template <class T, int size>
void Pointer<T, size>::showlist(){
    typename std::list<PtrDetails<T> >::iterator p;
    std::cout << "refContainer<" << typeid(T).name() << ", " << size << ">:\n";
    std::cout << "memPtr refcount value\n ";
    if (refContainer.begin() == refContainer.end())
    {
        std::cout << " Container is empty!\n\n ";
    }
    for (p = refContainer.begin(); p != refContainer.end(); p++)
    {
        std::cout << "[" << (void *)p->memPtr << "]"
             << " " << p->refcount << " ";
        if (p->memPtr)
            std::cout << " " << *p->memPtr;
        else
            std::cout << "---";
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
// Find a pointer in refContainer.
template <class T, int size>
typename std::list<PtrDetails<T> >::iterator
Pointer<T, size>::findPtrInfo(T *ptr){
    typename std::list<PtrDetails<T> >::iterator p;
    // Find ptr in refContainer.
    for (p = refContainer.begin(); p != refContainer.end(); p++)
        if (p->memPtr == ptr)
            return p;
    return p;
}
// Clear refContainer when program exits.
template <class T, int size>
void Pointer<T, size>::shutdown(){
    if (refContainerSize() == 0)
        return; // list is empty
    typename std::list<PtrDetails<T> >::iterator p;
    for (p = refContainer.begin(); p != refContainer.end(); p++)
    {
        // Set all reference counts to zero
        p->refcount = 0;
    }
    collect();
}
