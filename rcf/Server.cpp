#include <RCF/RCF.hpp>

#include <iostream>
#include <cstdlib>
#include <vector>
#include <SF/vector.hpp>

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

        Player(string _nick){
            this->nick = _nick;
            score = 0;
        }

        void setId(int val){
            this->id = val;
        }
};

class Board{
    public:

    int size;

    int currplayer;
    int maxplayers;

    int idcounter;

    vector< vector<Block> > board;
    vector<Position> freeTiles;

    vector<Player> players;
    vector<Player> spectators;

    Board(){
        srand(time(NULL));
        this->size = 4;

        this->currplayer = 0;
        this->maxplayers = 4;
        this->idcounter = 0;

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

    int addPlayer(const string _nick){
        Player newPlayer(_nick);
        
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

    void removePlayer(const int _id){
        for(int i = 0; i < players.size(); i++){
            if(players[i].id == _id){
                cout << players[i].nick << " opuszcza gre" << endl;
                players.erase(players.begin() + i);
            }
        }
    }

    vector<string> getNicks(){
        vector<string> res;

        for(int i = 0; i < players.size(); i++){
            res.push_back(players[i].nick);
        }

        return res;
    }

    vector<int> getScores(){
        vector<int> res;

        for(int i = 0; i < players.size(); i++){
            res.push_back(players[i].score);
        }

        return res;
    }

    int getPlayerCount(){
        return players.size();
    }

    int getCurrPlayer(){
        return currplayer;
    }

    void switchPlayer(){
        currplayer++;

        if(currplayer == players.size()){
            currplayer = 0;
        }

        cout << "Current player: " << currplayer << "\tPlayer count: " << players.size() << endl;
    }

    bool checkMyTurn(int _id){
        int currid = players[currplayer].id;

        //cout << currid << " " << _id << endl;

        if(_id == currid){
            return true;
        }else{
            return false;
        }
    }

    void reset() {
        for (int i = 0; i < board.size(); i++)
            for (int j = 0; j < board[i].size(); j++)
                board[i][j].val = 0;

        randSpawn();

        currplayer = 0;
    }

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
    
    void clearAdv(){
        for(int i = 0; i < board.size(); i++){
            for(int j = 0; j < board.size(); j++){
                board[i][j].adv = false;
            }
        }
    }

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

    bool checkEndGame(){
        //if(!moveLeft() && !moveRight() && !moveUp() && !moveDown()){
        //    cout << "Koniec gry! Wygral " << getWinner().nick << endl;
        //    return true;
        //}

        return false;
    }

    Player getWinner(){
        int best = 0;
        Player winner("winner");

        for(int i = 0; i < players.size(); i++){
            if(players[i].score > best){
                best = players[i].score;
                winner = players[i];
            }
        }

        return winner;
    }

//  void spawn(int x, int y, int val) {
//      board.get(x).set(y, val);
//  }

    bool move(int dir){
        switch(dir){
            case 0: return moveUp();
            case 1: return moveRight();
            case 2: return moveDown();
            case 3: return moveLeft();
        }

        return false;
    }

    void turn(const int dir){
        
        if(move(dir) && !checkEndGame()){
            randSpawn();
            switchPlayer();
        }

    }

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

    void showBoard() {
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

};


int main()
{

    RCF::RcfInitDeinit rcfInit;

    //HelloWorldImpl helloWorld;
    Board board;
    RCF::RcfServer server( RCF::TcpEndpoint(50001) );
    //server.bind<I_HelloWorld>(helloWorld);
    server.bind<I_Board>(board);

    server.start();
    
    //int input;
    //bool quit;

   /* while(true){
        cin >> input;
        switch(input){
            case 1: helloWorld.inc();break;
            case 2: helloWorld.dec();break;
            case 3: quit = true;break;
        }

        if(quit){
            break;
        }
    }*/

    //std::cout << "Press Enter to exit..." << std::endl;
    //board.showBoard();
    std::cin.get();

    return 0;
}