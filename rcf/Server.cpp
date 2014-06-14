#include <RCF/RCF.hpp>

#include <iostream>

using namespace std;


RCF_BEGIN(I_HelloWorld, "I_HelloWorld")
    RCF_METHOD_V1(void, Print, const std::string &)
    RCF_METHOD_V0(void, inc)
    RCF_METHOD_V0(void, dec)
RCF_END(I_HelloWorld)

class HelloWorldImpl
{
public:

    int counter;

    HelloWorldImpl():counter(0){}

    void Print(const std::string & s)
    {
        std::cout << "I_HelloWorld service: " << s << std::endl;
        cout << "Counter: " << counter << endl;
    }

    void inc(){
        counter++;
    }

    void dec(){
        counter--;
    }
};


int main()
{
    RCF::RcfInitDeinit rcfInit;

    HelloWorldImpl helloWorld;
    RCF::RcfServer server( RCF::TcpEndpoint(50001) );
    server.bind<I_HelloWorld>(helloWorld);
    server.start();
    
    int input;
    bool quit;

    while(true){
        cin >> input;
        switch(input){
            case 1: helloWorld.inc();break;
            case 2: helloWorld.dec();break;
            case 3: quit = true;break;
        }

        if(quit){
            break;
        }
    }

    //std::cout << "Press Enter to exit..." << std::endl;
    //std::cin.get();

    return 0;
}