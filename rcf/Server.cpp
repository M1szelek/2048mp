#include <RCF/RCF.hpp>

#include <iostream>
#include <cstdlib>
#include <vector>
#include <SF/vector.hpp>

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
        int val;    /*!< Wartosc bloku */
        bool adv;   /*!< Czy blok awansowal */
        Block(int val){
            this->val = val;
            this->adv = false;
        }
};

/*! Klasa gracza */

class Player{
    public:
        int id;         /*!< ID gracza */
        string nick;    /*!< Nick gracza */
        int score;      /*!< Aktualny wynik gracza */
        bool empty;     /*!< Czy gracz jest aktywny */
        bool check;     /*!< Czy gracz sie odmeldowal na serwerze */
        int wins;       /*!< Ilosc zwyciestw */

        Player(string _nick){
            this->nick = _nick;
            score = 0;
            empty = false;
            wins = 0;
        }

        void setId(int val){
            this->id = val;
        }
};

/*! Klasa glowna modelu */

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

/*! Klasa modelu serwera */

class ServerModel: public Model{
    public:

    ServerModel(){
        srand(time(NULL));
        this->size = 4;

        this->currplayer = 0;
        this->maxplayers = 4;
        this->idcounter = 0;

        this->running = true;

        for (int i = 0; i < size; i++) {
            vector<Block> row;
            
            for(int j = 0; j < size; j++){
                Block block(0);
                row.push_back(block);
            }

            board.push_back(row);

        }

        randSpawn();
    }

    /**  
     *  Dodaje gracza do gry z nickiem podanym w argumencie.
     *  Jesli nie ma miejsca to klient zostaje dodany jako widz.
     */

    int addPlayer(const string _nick){
        Player newPlayer(_nick);
    

        for(int i = 0; i < players.size(); i++){
            if(players[i].empty){
                if(_nick == players[i].nick){
                    players[i].empty = false;
                    cout << players[i].nick << " powraca do gry" << endl;
                }else{
                    players[i].nick = _nick;
                    players[i].empty = false;
                    players[i].wins = 0;
                    players[i].score = 0;
                    cout << newPlayer.nick << " dolacza do gry" << endl;
                }
                return players[i].id;
            }
        }

        if(players.size() < maxplayers){
            newPlayer.setId(idcounter);
            idcounter++;
            players.push_back(newPlayer);
            cout << newPlayer.nick << " dolacza do gry" << endl;
        }
        else{
            newPlayer.setId(-1);
            cout << newPlayer.nick << " dolacza jako widz" << endl;
        }
        
        return newPlayer.id;
    }

    /** 
     *  Usuwa gracza o podanym id.
     */

    void removePlayer(const int _id){
        for(int i = 0; i < players.size(); i++){
            if(players[i].id == _id){
                //if(i == currplayer){
                //    switchPlayer();
                //}

                cout << players[i].nick << " opuszcza gre" << endl;
                //players.erase(players.begin() + i);
                players[i].empty = true;

                if(i == currplayer && players.size() != 1)
                    switchPlayer();

                //if(currplayer >= players.size()){
                //   currplayer = 0;
                //}
            }
        }
    }

    /**  
     *  Zwraca nicki aktualnie grajacych graczy.
     */

    vector<string> getNicks(){
        vector<string> res;

        for(int i = 0; i < players.size(); i++){
            res.push_back(players[i].nick);
        }

        return res;
    }

    /** 
     *  Zwraca wyniki aktualnie grajacych graczy.
     */

    vector<int> getScores(){
        vector<int> res;

        for(int i = 0; i < players.size(); i++){
            res.push_back(players[i].score);
        }

        return res;
    }

    /** 
     *  Zwraca ilosc zwyciestw aktualnie grajacych graczy.
     */

    vector<int> getWins(){
        vector<int> res;

        for(int i = 0; i < players.size(); i++){
            res.push_back(players[i].wins);
        }

        return res;
    }

    /** 
     *  Zwraca ilosc grajacych.
     */

    int getPlayerCount(){
        return players.size();
    }

    /** 
     *  Zwraca numer gracza, ktory ma ture.
     */

    int getCurrPlayer(){
        return currplayer;
    }

    /** 
     *  Przelaczenie tury na nastepnego gracza.
     */

    void switchPlayer(){
        currplayer++;

        if(currplayer == players.size()){
            currplayer = 0;
        }

        while(players[currplayer].empty){
            currplayer++;
        }

        if(currplayer >= players.size()){
            currplayer = 0;
        }

        cout << "Current player: " << currplayer << "\tPlayer count: " << players.size() << endl;
    }

    /**  
     *  Sprawdzenie czy gracz o podanym id aktualnie ma ture.
     */

    bool checkMyTurn(int _id){
        int currid = players[currplayer].id;

        checkPlayer(_id);

        //cout << currid << " " << _id << endl;

        if(_id == currid){
            return true;
        }else{
            return false;
        }
    }

    /** 
     *  Reset planszy.
     */

    void reset() {
        for (int i = 0; i < board.size(); i++)
            for (int j = 0; j < board[i].size(); j++)
                board[i][j].val = 0;

        randSpawn();

        this->currplayer = 0;

        this->running = true;

        this->resetScores();
    }

    /** 
     *  Reset wyników.
     */


    void resetScores(){
        for(int i = 0; i < players.size(); i++){
            players[i].score = 0;
        }
    }

    /** 
     *  Aktualizacja wolnych pol.
     */

    void updateFreeTiles() {
        freeTiles.clear();

        for (int i = 0; i < board.size(); i++)
            for (int j = 0; j < board.size(); j++) {
                if (board[i][j].val == 0) {
                    
                    Position pos(i,j);
                    freeTiles.push_back(pos);
                }
            }
    }

    /**  
     *  Czyszczenie pol adv na calej planszy.
     */
    
    void clearAdv(){
        for(int i = 0; i < board.size(); i++){
            for(int j = 0; j < board.size(); j++){
                board[i][j].adv = false;
            }
        }
    }

    /** 
     *  Ruch w gore. Zwraca czy nastapil ruch.
     */

    bool moveUp() {
        bool moved = true;
        bool everMoved = false;
        
        clearAdv();

        while (moved) {
            moved = false;

            for (int i = 1; i < board.size(); i++) {
                for (int j = 0; j < board.size(); j++) {
                    if (board[i][j].val != 0 && !board[i][j].adv) {
                        //int neigh = board.get(i - 1).get(j).val;
                        //int target = board.get(i).get(j).val;

                        int neigh = board[i-1][j].val;
                        int target = board[i][j].val;

                        if (neigh == target) {
                            //board.get(i - 1).get(j).val = neigh * 2;
                            //board.get(i - 1).get(j).adv = true;
                            //board.get(i).get(j).val = 0;
                            
                            board[i-1][j].val = neigh * 2;
                            players[currplayer].score += neigh * 2;
                            board[i-1][j].adv = true;
                            board[i][j].val = 0;

                            moved = true;
                            everMoved = true;
                            continue;

                        }

                        if (neigh == 0) {
                            //board.get(i - 1).get(j).val = target;
                            //board.get(i).get(j).val = 0;

                            board[i-1][j].val = target;
                            board[i][j].val = 0;
                            moved = true;
                            everMoved = true;
                        }
                    }
                }
            }
        }

        return everMoved;
    }

    /** 
     *  Ruch w dol. Zwraca czy nastapil ruch.
     */

    bool moveDown() {
        bool moved = true;
        bool everMoved = false;

        clearAdv();

        while (moved) {
            moved = false;

            for (int i = board.size() - 2; i >= 0; i--) {
                for (int j = 0; j < board.size(); j++) {
                    if (board[i][j].val != 0 && !board[i][j].adv) {
                        int neigh = board[i+1][j].val;
                        int target = board[i][j].val;

                        if (neigh == target) {
                            board[i+1][j].val = neigh * 2;
                            players[currplayer].score += neigh * 2;
                            board[i+1][j].adv = true;
                            board[i][j].val = 0;
                            moved = true;
                            everMoved = true;
                            continue;
                        }

                        if (neigh == 0) {
                            board[i+1][j].val = target;
                            board[i][j].val = 0;
                            moved = true;
                            everMoved = true;
                        }
                    }
                }
            }
        }

        return everMoved;
    }

    /** 
     *  Ruch w lewo. Zwraca czy nastapil ruch.
     */

    bool moveLeft() {
        bool moved = true;
        bool everMoved = false;
        clearAdv();

        while (moved) {
            moved = false;

            for (int j = 1; j < board.size(); j++) {
                for (int i = 0; i < board.size(); i++) {
                    if (board[i][j].val != 0 && !board[i][j].adv) {
                        int neigh = board[i][j-1].val;
                        int target = board[i][j].val;

                        if (neigh == target) {
                            board[i][j-1].val = neigh * 2;
                            players[currplayer].score += neigh * 2;
                            board[i][j-1].adv = true;
                            board[i][j].val = 0;
                            moved = true;
                            everMoved = true;
                            continue;
                        }

                        if (neigh == 0) {
                            board[i][j-1].val = target;
                            board[i][j].val = 0;
                            moved = true;
                            everMoved = true;
                        }
                    }
                }
            }
        }

        return everMoved;
    }

    /** 
     *  Ruch w prawo. Zwraca czy nastapil ruch.
     */

    bool moveRight() {
        bool moved = true;
        bool everMoved = false;
        clearAdv();

        while (moved) {
            moved = false;
            for (int j = board.size() - 2; j >= 0; j--) {
                for (int i = 0; i < board.size(); i++) {
                    if (board[i][j].val != 0 && !board[i][j].adv) {
                        int neigh = board[i][j+1].val;
                        int target = board[i][j].val;

                        if (neigh == target) {
                            board[i][j+1].val = neigh * 2;
                            players[currplayer].score += neigh * 2;
                            board[i][j+1].adv = true;
                            board[i][j].val = 0;
                            moved = true;
                            everMoved = true;
                            continue;
                        }

                        if (neigh == 0) {
                            board[i][j+1].val = target;
                            board[i][j].val = 0;
                            moved = true;
                            everMoved = true;
                        }
                    }
                }
            }
        }

        return everMoved;
    }

    /** 
     *  Sprawdzenie czy nie ma wolnych pol na planszy.
     */

    bool checkOccupy(){

        for(int i = 0; i < board.size(); i++)
            for(int j = 0; j < board.size(); j++){
                if(board[i][j].val == 0){
                    return false;
                }
            }

        return true;
    }

    /** 
     *  Sprawdza mozliwosc ruchu.
     */

    bool checkMove(){
        if(!checkOccupy()){
            return true; 
        }

        for(int i = 0; i < board.size(); i++){
            for(int j = 0; j < board.size(); j++){
                
                if(i != board.size()-1)
                if(board[i][j].val == board[i+1][j].val){
                    return true;
                }

                if(i != 0)
                if(board[i][j].val == board[i-1][j].val){
                    return true;
                }

                if(j != 0)
                if(board[i][j].val == board[i][j-1].val){
                    return true;
                }

                if(j != board.size()-1)
                if(board[i][j].val == board[i][j+1].val){
                    return true;
                }
            }
        }

        return false;
    }

    /** 
     *  Sprawdza koniec gry.
     */

    bool checkEndGame(){
        if(!checkMove()){
            cout << "Koniec gry! Wygral " << getWinner().nick << endl;
            return true;
            
        }

        return false;

        
    }

    /** 
     *  Zwraca gracza zwyciezce.
     */

    Player getWinner(){
        int best = 0;
        Player winner("winner");

        for(int i = 0; i < players.size(); i++){
            if(players[i].score > best){
                best = players[i].score;
                winner = players[i];
            }
        }

        addWin(winner);

        return winner;
    }

    /** 
     *  Dodanie zwyciestwa dla gracza.
     */

    void addWin(Player player){
        for(int i = 0; i < players.size(); i++){
            if(players[i].id == player.id){
                players[i].wins++;
            } 
        }
    }

//  void spawn(int x, int y, int val) {
//      board.get(x).set(y, val);
//  }

    /** 
     *  Ogolna funkcja obslugujaca ruch.
     */

    bool move(int dir){
        switch(dir){
            case 0: return moveUp();
            case 1: return moveRight();
            case 2: return moveDown();
            case 3: return moveLeft();
        }

        return false;
    }

    /** 
     *  Wykonanie tury.
     */

    void turn(const int dir){

        if(checkEndGame()){
            this->reset();
            return;
        }

        if(move(dir) && this->running){
            randSpawn();
            switchPlayer();
        }

    }

    /** 
     *  Losowanie nowego numeru na planszy.
     */

    void randSpawn() {
        updateFreeTiles();

        if (freeTiles.empty()) {
            return;
        }

        //int number = new Random().nextInt(2); // 2 or 4

        int number = rand() % 2;

        //Position tile = freeTiles.get(new Random().nextInt(freeTiles.size()));

        Position tile = freeTiles[rand()%freeTiles.size()];

        if (number == 0) {
            board[tile.x][tile.y].val = 2;
        } else {
            board[tile.x][tile.y].val = 4;
        }

    }

    /** 
     *  Wyswietla plansze.
     */

    void showboard() {
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

        cout << "--------------------" << endl;
        
    }

    /** 
     *  Zwraca plansze.
     */

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

    /** 
     *  Sprawdzenie listy obecnosci.
     */

    void checkPlayer(int _id){
        for(int i = 0; i < players.size(); i++){
            if(_id == players[i].id){
                players[i].check = true;
            }
        }
    }

    /** 
     *  Reset listy obecnosci.
     */

    void resetChecks(){
        for(int i = 0; i < players.size(); i++){
            players[i].check = false;
        }
    }

    /** 
     *  Detekcja martwych klientow.
     */

    void checkAlivePlayers(){
        while(true){
            RCF::sleepMs(5000);
            //cout << players.size() << endl;
            for(int i = 0; i < players.size(); i++){
                if(!players[i].empty && !players[i].check){
                    removePlayer(players[i].id);
                }
            }
            resetChecks();

        }


    }

};


int main()
{

    RCF::RcfInitDeinit rcfInit;

    ServerModel model;
    RCF::RcfServer server( RCF::TcpEndpoint("0.0.0.0", 50001) );

    RCF::ThreadPoolPtr tpPtr( new RCF::ThreadPool(1, 25) );
    server.setThreadPool(tpPtr);

    server.start();
   
    server.bind<I_Model>(model);

    cout << "Serwer wystartowal" << endl;

    server.start();

    model.checkAlivePlayers();

    return 0;
}