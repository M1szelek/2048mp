#include <iostream>
#include <fstream>

#include <RCF/RCF.hpp>
#include <cstdlib>
#include <vector>
#include <SF/vector.hpp>
#include <thread>

using namespace std;

RCF_BEGIN(I_Model, "I_Model")
    RCF_METHOD_V1(void, turn, const int &)
    RCF_METHOD_R0(vector< vector<double> >, getBoard)
    RCF_METHOD_R1(int, addPlayer, const string &)
    RCF_METHOD_V1(void, removePlayer, const int &)
    RCF_METHOD_R1(bool, checkMyTurn, const int &)
    RCF_METHOD_R0(vector<string>, getNicks)
    RCF_METHOD_R0(vector<int>, getScores)
    RCF_METHOD_R0(vector<int>, getWins)
    RCF_METHOD_R0(int, getPlayerCount)
    RCF_METHOD_R0(int, getCurrPlayer)
RCF_END(I_Model)

/*! Klasa pomocnicza */

class Position {
    
    public:
        int x;
        int y;
        
        Position(int x, int y) {
            this->x = x;
            this->y = y;
        }
    
};

/*! Klasa pojedynczego bloku na planszy */

class Block {
    
    public:
        int val;
        bool adv;
        Block(int val){
            this->val = val;
            this->adv = false;
        }
};

/*! Klasa gracza */

class Player{
    public:
        int id;
        string nick;
        int score;
        int wins;

        Player(string _nick, int _score, int _wins){
            this->nick = _nick;
            this->score = _score;
            this->wins = _wins;
        }

        void setId(int val){
            this->id = val;
        }
};

/*! Klasa glowna modelu*/

class Model{
    public:

    int size;   /*!< Wielkosc planszy */

    int currplayer;     /*!< Aktualny gracz ktory ma ture */
    int maxplayers;     /*!< Maksymalna ilosc graczy */

    int idcounter;      /*!< Zmienna z ktorej przydzielane sa id dla graczy */

    bool running;       /*!< Flaga oznaczajaca przebieg rozgrywki */
    

    vector< vector<Block> > board;  /*! Plansza */
    vector<Position> freeTiles;     /*! Wolne pola na planszy */

    vector<Player> players;         /*! Lista graczy */
    vector<Player> spectators;      /*! Lista widzow */
};

/*! Klasa modelu klienta */

class ClientModel: public Model{
    public:

    ClientModel(){

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

    /**  
     *  Funkcja renderujaca cala aplikacje.
     */

    void render(){
        showBoard();
        showPlayers();
    }

    /**  
     *  Wyswietla plansze.
     */

    void showBoard() {
        printf("\033[2J\033[1;1H");

        for (int i = 0; i < board.size(); i++) {
            for (int j = 0; j < board.size(); j++) {
                //System.out.print(Model.get(i).get(j).val + "\t");
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

    /**  
     *  Wyswietla liste graczy.
     */

    void showPlayers(){
        cout << endl;

        for(int i = 0; i < players.size(); i++){
            if(currplayer == i){
                cout << players[i].nick << ":\t" << players[i].score << "\t" << players[i].wins << " <<<" << endl;
            }else{
                cout << players[i].nick << ":\t" << players[i].score << "\t" << players[i].wins << endl;
            }
        }
    }

    /**  
     *  Pobiera aktualny stan planszy z serwera.
     */

    void updateBoard(RcfClient<I_Model> _client){
        vector< vector<double> > newBoard = _client.getBoard(); 

        for(int i = 0; i < board.size(); i++)
            for(int j = 0; j < board.size(); j++)
                this->board[i][j].val = newBoard[i][j];
    }

    /**  
     *  Pobiera aktualna liste graczy.
     */

    void updatePlayers(RcfClient<I_Model> _client){
        
        players.clear();

        vector<string> nicks = _client.getNicks();
        vector<int> scores = _client.getScores();
        vector<int> wins = _client.getWins();
        int count = _client.getPlayerCount();
        this->currplayer = _client.getCurrPlayer();

        for(int i = 0; i < count; i++){
            Player _player(nicks[i],scores[i],wins[i]);
            players.push_back(_player);
        }
    }

    void process(RcfClient<I_Model> _client){
        this->updateBoard(_client);
        this->updatePlayers(_client);
        this->render(); 
    }

};

bool quit = false;
bool myTurn = false;

void updateGameStatus(RcfClient<I_Model> _client, ClientModel _Model, Player _myself){
    try{
        while(!quit){
                    if(_client.checkMyTurn(_myself.id)){
                        myTurn = true;
                    }
                    RCF::sleepMs(500);
                    _Model.process(_client);
        }
    }
    catch(const RCF::Exception & e){
        std::cout << "Utracono polaczenie z serwerem!" << std::endl;
        quit = true;
        exit(1);
    }
}

void control(RcfClient<I_Model> _client){
    char decision;
    try{
        while (!quit) {
            
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
        std::cout << "Utracono polaczenie z serwerem!" << std::endl;
        quit = true;
    }

}

int main()
{
    //thread tr(&fun);
    //tr.join();

    ifstream myReadFile;
    myReadFile.open("config");
    char output[100];
    if (myReadFile.is_open()) {
        while (!myReadFile.eof()) {


        myReadFile >> output;
        //cout<<output;


        }
    }
    myReadFile.close();

    cout << output << endl;

    string nick;

    cout << "Podaj nick: ";
    cin >> nick;


    RCF::RcfInitDeinit rcfInit;

    RcfClient<I_Model> client_send( RCF::TcpEndpoint(output,50001) );
    RcfClient<I_Model> client_recv( RCF::TcpEndpoint(output,50001) );

    // 5 second timeout when establishing network connection.
    client_send.getClientStub().setConnectTimeoutMs(5*1000);
    client_recv.getClientStub().setConnectTimeoutMs(5*1000);

    // 60 second timeout when waiting for remote call response from the server.
    client_send.getClientStub().setRemoteCallTimeoutMs(60*1000);
    client_recv.getClientStub().setRemoteCallTimeoutMs(60*1000);

    Player myself(nick,0,0);

    try{
        myself.setId(client_send.addPlayer(myself.nick));
    }catch(const RCF::Exception & e){
        cout << "Nie mozna polaczyc sie z serwerem." << endl;
        return 1;
    }

    ClientModel model;

    char decision;
    
       
    thread tr(&updateGameStatus, client_send, model, myself);
    thread tr2(&control, client_recv);
    tr.join();
    tr2.join();

    try{
        client_send.removePlayer(myself.id);
    }
    catch(const RCF::Exception & e){
        std::cout << "Brak polaczenia za serwerem!" << std::endl;
        return 1;
    }

    return 0;
}