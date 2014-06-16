#include <iostream>

#include <RCF/RCF.hpp>
#include <cstdlib>
#include <vector>
#include <SF/vector.hpp>

using namespace std;

RCF_BEGIN(I_HelloWorld, "I_HelloWorld")
    RCF_METHOD_V1(void, Print, const std::string &)
    RCF_METHOD_V0(void, inc)
    RCF_METHOD_V0(void, dec)
RCF_END(I_HelloWorld)

RCF_BEGIN(I_Board, "I_Board")
    RCF_METHOD_V1(void, turn, const int &)
    RCF_METHOD_R0(vector< vector<double> >, getBoard)
    RCF_METHOD_R1(int, addPlayer, const string &)
    RCF_METHOD_V1(void, removePlayer, const int &)
    RCF_METHOD_R1(bool, checkMyTurn, const int &)
RCF_END(I_Board)

class Position {
    
    public:
        int x;
        int y;
        
        Position(int x, int y) {
            this->x = x;
            this->y = y;
        }
    
};

class Block {
    
    public:
        int val;
        bool adv;
        Block(int val){
            this->val = val;
            this->adv = false;
        }
};

class Player{
    public:
        int id;
        string nick;
        int score;

        Player(string _nick){
            this->nick = _nick;
            score = 0;
        }

        void setId(int val){
            this->id = val;
        }
};

class ClientBoard{
    public:

    int size;

    vector< vector<Block> > board;
    vector<Position> freeTiles;

    ClientBoard(){

        this->size = 4;

        for (int i = 0; i < size; i++) {
            vector<Block> row;
            
            for(int j = 0; j < size; j++){
                Block block(0);
                row.push_back(block);
            }

            board.push_back(row);

        }
    }

    void showBoard() {
        printf("\033[2J\033[1;1H");

        for (int i = 0; i < board.size(); i++) {
            for (int j = 0; j < board.size(); j++) {
                //System.out.print(board.get(i).get(j).val + "\t");
                if(board[i][j].val != 0){
                    cout << board[i][j].val << "\t";
                }else{
                    cout << "." << "\t";
                }
            }

            cout << endl;
        }

        //cout << "--------------------" << endl;

        
    }

    vector< vector<double> > getBoard(){
        
        vector< vector<double> > res;

        for(int i = 0; i < board.size(); i++){
            vector<double> row;
            for(int j = 0; j < board.size(); j++){
                row.push_back(board[i][j].val);
            }
            res.push_back(row);
        }
        
        return res;
    }

    void updateBoard(RcfClient<I_Board> _client){
        vector< vector<double> > newBoard = _client.getBoard(); 

        for(int i = 0; i < board.size(); i++)
            for(int j = 0; j < board.size(); j++)
                this->board[i][j].val = newBoard[i][j];
    }

};

int main()
{
    RCF::RcfInitDeinit rcfInit;

    string nick = "Anonim";

    //std::cout << "Calling the I_HelloWorld Print() method." << std::endl;
    //RcfClient<I_HelloWorld> client( RCF::TcpEndpoint(50001) );
    RcfClient<I_Board> client( RCF::TcpEndpoint(50001) );

    Player myself(nick);

    myself.setId(client.addPlayer(nick));

    ClientBoard board;

    char decision;
    bool quit = false;
        
    while (!quit) {
        //board.updateBoard(client);
        //board.showBoard();
        
        while(!client.checkMyTurn(myself.id)){
            RCF::sleepMs(500);
            board.updateBoard(client);
            board.showBoard();  
        }

        board.updateBoard(client);
        board.showBoard();

        cin >> decision;
        switch (decision) {
        case 'w':
            client.turn(0);
            break;
        case 's':
            client.turn(2);
            break;
        case 'a':
            client.turn(3);
            break;
        case 'd':
            client.turn(1);
            break;
        case 'q':
            quit = true;
        }
    }

    client.removePlayer(myself.id);
    

    /*bool quit = false;

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

    client.dec();*/

    return 0;
}