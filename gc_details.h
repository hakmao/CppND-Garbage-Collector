// This class defines an element that is stored
// in the garbage collection information list.
//
template <class T>
class PtrDetails
{
  public:
    unsigned refcount; // current reference count
    T *memPtr;         // pointer to allocated memory
    /* isArray is true if memPtr points
to an allocated array. It is false
otherwise. */
    bool isArray; // true if pointing to array
    /* If memPtr is pointing to an allocated
array, then arraySize contains its size */
    unsigned arraySize; // size of array
    // Here, mPtr points to the allocated memory.
    // If this is an array, then size specifies
    // the size of the array.

    PtrDetails(T *p, unsigned aSize)
    {
        memPtr = p;
        refcount = 1;
        if (aSize > 0){
            arraySize = aSize;
            isArray = true;
        }
        else
        {
            arraySize = 0;
            isArray = false;
        }
    }
    PtrDetails(T *p){
        memPtr = p;
        arraySize = 0;
        isArray = false;
        refcount = 1;
    }
    PtrDetails(void){
        memPtr = nullptr;
        arraySize = 0;
        isArray = false;
        refcount = 0;
    }
};
// Overloading operator== allows two class objects to be compared.
// This is needed by the STL list class.
template <class T>
bool operator==(const PtrDetails<T> &pd1,
                const PtrDetails<T> &pd2)
{
    return pd1.memPtr == pd2.memPtr;
}
