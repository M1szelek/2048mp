#include<iostream>
#include<vector>
#include<cstdlib>

using namespace std;

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

class Board{
	//ArrayList<ArrayList<Block>> board = new ArrayList<ArrayList<Block>>();
	//ArrayList<Position> freeTiles = new ArrayList<Position>();
	public:

	int size;

	vector< vector<Block> > board;
	vector<Position> freeTiles;

	Board(){

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

	void reset() {
		for (int i = 0; i < board.size(); i++)
			for (int j = 0; j < board[i].size(); j++)
				board[i][j].val = 0;
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

	void moveUp() {
		bool moved = true;
		
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
							continue;

						}

						if (neigh == 0) {
							//board.get(i - 1).get(j).val = target;
							//board.get(i).get(j).val = 0;

							board[i-1][j].val = target;
							board[i][j].val = 0;
							moved = true;
						}
					}
				}
			}
		}
	}

	void moveDown() {
		bool moved = true;
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
							continue;
						}

						if (neigh == 0) {
							board[i+1][j].val = target;
							board[i][j].val = 0;
							moved = true;
						}
					}
				}
			}
		}
	}

	void moveLeft() {
		bool moved = true;
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
							continue;
						}

						if (neigh == 0) {
							board[i][j-1].val = target;
							board[i][j].val = 0;
							moved = true;
						}
					}
				}
			}
		}
	}

	void moveRight() {
		bool moved = true;
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
							continue;
						}

						if (neigh == 0) {
							board[i][j+1].val = target;
							board[i][j].val = 0;
							moved = true;
						}
					}
				}
			}
		}
	}

//	void spawn(int x, int y, int val) {
//		board.get(x).set(y, val);
//	}

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

			//System.out.println();
			//System.out.println();

			cout << endl;
		}

		cout << "--------------------" << endl;
		//System.out
		//		.println("-----------------------------------------------------");
	}

	

	/*@Override
	public void keyPressed(KeyEvent arg0) {
		// TODO Auto-generated method stub
	}

	@Override
	public void keyReleased(KeyEvent arg0) {
		// TODO Auto-generated method stub

		if (arg0.getKeyCode() == KeyEvent.VK_W) {
			moveUp();
			showBoard();
		}

		if (arg0.getKeyCode() == KeyEvent.VK_S) {
			moveDown();
			showBoard();
		}

		if (arg0.getKeyCode() == KeyEvent.VK_A) {
			moveLeft();
			showBoard();
		}

		if (arg0.getKeyCode() == KeyEvent.VK_D) {
			moveRight();
			showBoard();
		}

		if (arg0.getKeyCode() == KeyEvent.VK_ENTER) {
			reset();
			showBoard();
		}

	}

	@Override
	public void keyTyped(KeyEvent arg0) {
		// TODO Auto-generated method stub

	}*/

};


int main() {
		
		srand(time(NULL));
		Board board;


		// board.spawn(2, 2, 2);
		// board.spawn(3, 2, 4);
		// board.spawn(0, 0, 4);
		// board.show();
		// board.moveLeft();
		// board.moveDown();
		// board.move();
		// board.randSpawn();
		// board.randSpawn();
		// board.randSpawn();
		// board.randSpawn();
		// board.randSpawn();
		// board.randSpawn();
		// board.randSpawn();
		// board.randSpawn();
		// board.randSpawn();
		// board.randSpawn();
		// board.randSpawn();
		// board.randSpawn();
		// board.randSpawn();
		// board.randSpawn();
		// board.randSpawn();
		// board.randSpawn();
		// board.randSpawn();

		// board.showBoard();

		//Scanner read = new Scanner(System.in);



		char decision;
		
		while (true) {
			board.randSpawn();
			board.showBoard();
			//decision = read.nextLine();
			cin >> decision;
			switch (decision) {
			case 'w':
				board.moveUp();
				break;
			case 's':
				board.moveDown();
				break;
			case 'a':
				board.moveLeft();
				break;
			case 'd':
				board.moveRight();
				break;
			}
		}

	}