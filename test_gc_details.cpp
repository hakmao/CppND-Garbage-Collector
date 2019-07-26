#include <iostream>
#include "gc_details.h"

int main(){

    PtrDetails<int> pd1(new int(23));
    std::cout << "Pointer pd1: " << std::endl
              << "\taddress = " << pd1.memPtr << std::endl
              << "\tvalue = " << *(pd1.memPtr) << std::endl
              << "\tisArray = " << pd1.isArray << std::endl
              << "\tarray size = " << pd1.arraySize << std::endl
              << "\tsizeof(memPtr) = " << sizeof(pd1.memPtr) << std::endl;
    PtrDetails<int> pd2(new int(11), 0);
    std::cout << "Pointer pd2: " << std::endl
              << "\taddress = " << pd2.memPtr << std::endl
              << "\tvalue = " << *(pd2.memPtr) << std::endl
              << "\tisArray = " << pd2.isArray << std::endl
              << "\tarray size = " << pd2.arraySize << std::endl
              << "\tsizeof(memPtr) = " << sizeof(pd2.memPtr) << std::endl;
    // Array pointers
    PtrDetails<int> pd3(new int[50], 50);
    std::cout << "Pointer pd3: " << std::endl
              << "\taddress = " << pd3.memPtr << std::endl
              << "\tvalue = " << *(pd3.memPtr) << std::endl
              << "\tisArray = " << pd3.isArray << std::endl
              << "\tarray size = " << pd3.arraySize << std::endl
              << "\tsizeof(memPtr) = " << sizeof(pd3.memPtr) << std::endl;
    PtrDetails<int> pd4(new int[1], 1);
    std::cout << "Pointer pd4: " << std::endl
              << "\taddress = " << pd4.memPtr << std::endl
              << "\tvalue = " << *(pd4.memPtr) << std::endl
              << "\tisArray = " << pd4.isArray << std::endl
              << "\tarray size = " << pd4.arraySize << std::endl
              << "\tsizeof(memPtr) = " << sizeof(pd4.memPtr) << std::endl;


    return 0;
}
