#include <iostream>

#include <RCF/RCF.hpp>

using namespace std;

RCF_BEGIN(I_HelloWorld, "I_HelloWorld")
    RCF_METHOD_V1(void, Print, const std::string &)
    RCF_METHOD_V0(void, inc)
    RCF_METHOD_V0(void, dec)
RCF_END(I_HelloWorld)

int main()
{
    RCF::RcfInitDeinit rcfInit;

    string input;

    std::cout << "Calling the I_HelloWorld Print() method." << std::endl;
    RcfClient<I_HelloWorld> client( RCF::TcpEndpoint(50001) );

    

    bool quit = false;

    client.inc();
    client.Print("Player join the server");

    while(true){	

    	cin >> input;
    	
    	if(input.compare("q") == 0){
    		client.Print("Player quitted");
    		break;
    	}

    	client.Print(input);
    }

    client.dec();

    return 0;
}