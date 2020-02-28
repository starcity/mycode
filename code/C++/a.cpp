/* j#if __cplusplus < 201103L */
/*     #error "use c++11" */
/* #endif */
_Pragma ("once")
#include <iostream> 
#include <ctime>
#include <typeinfo>


class A
{
public:
    A(){std::cout<<"A"<<std::endl;}
    void f(double i){std::cout<<__func__<<"A"<<std::endl;}
};

class B:public A
{
    using A::f;

public:
    B(int dd){b=dd;std::cout<<"B"<<b<<std::endl;b=dd;}

    void f(int f){std::cout<<__func__<<"B"<<std::endl;}

private:
    int b = 1;
    int c {1};
};

int main()
{int * p = nullptr;
        delete p;



    static_assert(true,"This static assert");

    time_t t = 1472524402;
    int tmp_days = (std::time(nullptr) - t) / (3600 * 24);
    int remainder = (std::time(nullptr) - t) % (3600 * 24);
    std::string  days = std::to_string((remainder == 0) ? tmp_days : tmp_days + 1);
    std::cout << days << " "<<std::time(nullptr)<<std::endl;

    B b{4};
    b.f(1);
    std::cout<<typeid(decltype(b)).name()<<std::endl;


    std::cout<<__STDC_HOSTED__<<" "<<__STDC__<<" "<<std::endl;

        int && ia = 342332;
        ia = 32;

        int bb[] {3,4,5,6,7,8,};

        std::cout<<sizeof(bb)<<std::endl;
            return 0;
}


