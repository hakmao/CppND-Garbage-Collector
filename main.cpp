#include "gc_pointer.h"
#include "LeakTester.h"

int main()
{
    //Standard Constructor
    std::cout << "Main scope: declare Pointer<float>" << std::endl;
    Pointer<float> a(new float(1.1));
    {
        std::cout << "Inside block: " << std::endl;
        Pointer<float> b(new float(2.2));
        Pointer<float> c(new float(3.3));
        a.showlist();
    }
    std::cout << "Main scope:" << std::endl;
    a.showlist();
    //
    std::cout << "Main scope: declare Pointer<int>" << std::endl;
    Pointer<int> p(new int(11));
    Pointer<int> q(new int(22));
    p.showlist();
    {
        std::cout << "Inside block: " << std::endl;
        //Copy constructor
        Pointer<int> r(p);
        r.showlist();
    }
    std::cout << "Main scope:" << std::endl;
    p.showlist();
    {
        std::cout << "Inside block: copy assignment (Pointer to Pointer)" << std::endl;
        Pointer<int> s = p;
        Pointer<int> t; //Constructor
        //Copy assignment operator= (Pointer to Pointer)
        q = p;
        *t = 88;
        p.showlist();
    }
    std::cout << "Main scope:" << std::endl;
    p.showlist();
    // List
    std::cout << "Main scope: declare list pointer Pointer<int,10>" << std::endl;
    Pointer<int,10> ls1;
    ls1.showlist();
    {
        std::cout << "Inside block: initialise list" << std::endl;
        ls1 = new int[10];
        for (int i = 0; i < 10; i++){
            ls1[i] = 2*i;
        }
        std::cout << "Inside block: print list" << std::endl;
        for (int i = 0; i < 10; i++){
            std::cout << ls1[i] << std::endl;
        }
    }
    std::cout << "Main scope: print list" << std::endl;
    ls1.showlist();
    //Pointer<int> s = new int(19);
    //Reassignment using operator = 
    //s = new int(21);
    //s.showlist();

    return 0;
}
