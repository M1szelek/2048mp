#include <iostream>

#include <RCF/RCF.hpp>
#include <cstdlib>
#include <vector>
#include <SF/vector.hpp>
#include <thread>

using namespace std;

RCF_BEGIN(I_Board, "I_Board")
    RCF_METHOD_V1(void, turn, const int &)
    RCF_METHOD_R0(vector< vector<double> >, getBoard)
    RCF_METHOD_R1(int, addPlayer, const string &)
    RCF_METHOD_V1(void, removePlayer, const int &)
    RCF_METHOD_R1(bool, checkMyTurn, const int &)
    RCF_METHOD_R0(vector<string>, getNicks)
    RCF_METHOD_R0(vector<int>, getScores)
    RCF_METHOD_R0(int, getPlayerCount)
    RCF_METHOD_R0(int, getCurrPlayer)
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

        Player(string _nick, int _score){
            this->nick = _nick;
            this->score = _score;
        }

        void setId(int val){
            this->id = val;
        }
};

class ClientBoard{
    public:

    int size;
    int currplayer;

    vector< vector<Block> > board;
    vector<Position> freeTiles;

    vector<Player> players;

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

    void render(){
        showBoard();
        showPlayers();
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

    void showPlayers(){
        cout << endl;

        for(int i = 0; i < players.size(); i++){
            if(currplayer == i){
                cout << players[i].nick << ":\t" << players[i].score << " <<<" << endl;
            }else{
                cout << players[i].nick << ":\t" << players[i].score << endl;
            }
        }
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

    void updatePlayers(RcfClient<I_Board> _client){
        
        players.clear();

        vector<string> nicks = _client.getNicks();
        vector<int> scores = _client.getScores();
        int count = _client.getPlayerCount();
        this->currplayer = _client.getCurrPlayer();

        for(int i = 0; i < count; i++){
            Player _player(nicks[i],scores[i]);
            players.push_back(_player);
        }
    }

    void process(RcfClient<I_Board> _client){
        this->updateBoard(_client);
        this->updatePlayers(_client);
        this->render(); 
    }

};

void fun(){
    cout << "hello" << endl;
}

bool quit = false;
bool myTurn = false;

void updateGameStatus(RcfClient<I_Board> _client, ClientBoard _board, Player _myself){
    //while(!_client.checkMyTurn(_myself.id)){
    //            RCF::sleepMs(500);
    //            _board.process(_client);
    //}
    try{
        while(!quit){
                    if(_client.checkMyTurn(_myself.id)){
                        myTurn = true;
                    }
                    RCF::sleepMs(500);
                    _board.process(_client);
        }
    }
    catch(const RCF::Exception & e){
        std::cout << "Error: " << e.getErrorString() << std::endl;
        quit = true;
    }
}

void control(RcfClient<I_Board> _client){
    char decision;
    try{
        while (!quit) {
                
                //while(!client.checkMyTurn(myself.id)){
                //    RCF::sleepMs(500);
                //    board.process(client);
                //}

                //board.process(client);



                cin >> decision;

                if(quit){
                    break;
                }
                
                switch (decision) {
                case 'w':
                    if(myTurn){
                    _client.turn(0);
                    myTurn = false;
                    }
                    break;
                case 's':
                    if(myTurn){
                    _client.turn(2);
                    myTurn = false;
                    }
                    break;
                case 'a':
                    if(myTurn){
                    _client.turn(3);
                    myTurn = false;
                    }
                    break;
                case 'd':
                    if(myTurn){
                    _client.turn(1);
                    myTurn = false;
                    }
                    break;
                case 'q':
                    quit = true;
                }
        }
    }
    catch(const RCF::Exception & e){
        std::cout << "Error: " << e.getErrorString() << std::endl;
        quit = true;
    }

}

int main()
{
    //thread tr(&fun);
    //tr.join();

    string nick;

    cout << "Podaj nick: ";
    cin >> nick;


    RCF::RcfInitDeinit rcfInit;

    RcfClient<I_Board> client( RCF::TcpEndpoint(50001) );

    Player myself(nick,0);

    myself.setId(client.addPlayer(myself.nick));

    ClientBoard board;

    char decision;
    
       
    thread tr(&updateGameStatus, client, board, myself);
    thread tr2(&control, client);
    tr.join();
    tr2.join();

    try{
        client.removePlayer(myself.id);
    }
    catch(const RCF::Exception & e){
        std::cout << "Error: " << e.getErrorString() << std::endl;
    }

    /*try{

        while (!quit) {
            
            //while(!client.checkMyTurn(myself.id)){
            //    RCF::sleepMs(500);
            //    board.process(client);
            //}

            //board.process(client);



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

    }
    catch(const RCF::Exception & e)
    {
        std::cout << "Error: " << e.getErrorString() << std::endl;
    }*/

    return 0;
}