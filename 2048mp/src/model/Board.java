package model;

import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.util.ArrayList;
import java.util.Random;

import javax.swing.JPanel;

public class Board extends JPanel implements KeyListener {
	ArrayList<ArrayList<Integer>> board = new ArrayList<ArrayList<Integer>>();
	ArrayList<Position> freeTiles = new ArrayList<Position>();
	int size = 4;

	public Board() {
		super();

		 addKeyListener(this);
		 setFocusable(true); //IMPORTANT!!!

		for (int i = 0; i < size; i++) {
			board.add(new ArrayList<Integer>());
		}

		for (int i = 0; i < board.size(); i++)
			for (int j = 0; j < size; j++)
				board.get(i).add(0);
	}

	void reset() {
		for (int i = 0; i < board.size(); i++)
			for (int j = 0; j < board.get(i).size(); j++)
				board.get(i).set(j, 0);
	}
	
	void updateFreeTiles(){
		freeTiles.clear();
		
		for(int i = 0; i < board.size(); i++)
			for(int j = 0; j < board.size(); j++){
				if(board.get(i).get(j) == 0){
					freeTiles.add(new Position(i,j));
				}
			}
	}

	void moveUp() {
		boolean moved = true;

		while (moved) {
			moved = false;

			for (int i = 1; i < board.size(); i++) {
				for (int j = 0; j < board.size(); j++) {
					if (board.get(i).get(j) != 0) {
						Integer neigh = board.get(i - 1).get(j);
						Integer target = board.get(i).get(j);

						if (neigh == target) {
							board.get(i - 1).set(j, neigh * 2);
							board.get(i).set(j, 0);
							moved = true;
							// System.out.println("move1");
							continue;

						}

						if (neigh == 0) {
							board.get(i - 1).set(j, target);
							board.get(i).set(j, 0);
							moved = true;
							// System.out.println("move2");
						}
					}
				}
			}
		}
	}

	void moveDown() {
		boolean moved = true;

		while (moved) {
			moved = false;

			for (int i = board.size() - 2; i >= 0; i--) {
				for (int j = 0; j < board.size(); j++) {
					if (board.get(i).get(j) != 0) {
						Integer neigh = board.get(i + 1).get(j);
						Integer target = board.get(i).get(j);

						if (neigh == target) {
							board.get(i + 1).set(j, neigh * 2);
							board.get(i).set(j, 0);
							moved = true;
							continue;
						}

						if (neigh == 0) {
							board.get(i + 1).set(j, target);
							board.get(i).set(j, 0);
							moved = true;
						}
					}
				}
			}
		}
	}

	void moveLeft() {
		boolean moved = true;

		while (moved) {
			moved = false;
			
				for (int j = 1; j < board.size(); j++) {
				for (int i = 0; i < board.size(); i++) {
					if (board.get(i).get(j) != 0) {
						Integer neigh = board.get(i).get(j - 1);
						Integer target = board.get(i).get(j);

						if (neigh == target) {
							board.get(i).set(j - 1, neigh * 2);
							board.get(i).set(j, 0);
							moved = true;
							continue;
						}

						if (neigh == 0) {
							board.get(i).set(j - 1, target);
							board.get(i).set(j, 0);
							moved = true;
						}
					}
				}
			}
		}
	}

	void moveRight() {
		boolean moved = true;

		while (moved) {
			moved = false;
			for (int j = board.size() - 2; j > 0; j--) {
				for (int i = 0; i < board.size(); i++) {
					if (board.get(i).get(j) != 0) {
						Integer neigh = board.get(i).get(j + 1);
						Integer target = board.get(i).get(j);

						if (neigh == target) {
							board.get(i).set(j + 1, neigh * 2);
							board.get(i).set(j, 0);
							moved = true;
							continue;
						}

						if (neigh == 0) {
							board.get(i).set(j + 1, target);
							board.get(i).set(j, 0);
							moved = true;
						}
					}
				}
			}
		}
	}

	void spawn(int x, int y, int val) {
		board.get(x).set(y, val);
	}
	
	void randSpawn(){
		updateFreeTiles();
		
		if(freeTiles.isEmpty()){
			return;
		}
		
		int number = new Random().nextInt(2);	//2 or 4
		
		
		Position tile = freeTiles.get(new Random().nextInt(freeTiles.size()));
		
		if(number == 0){
			board.get(tile.x).set(tile.y, 2);
		}else{
			board.get(tile.x).set(tile.y, 4);
		}
		
	}

	void showBoard() {
		for (int i = 0; i < board.size(); i++) {
			for (int j = 0; j < board.size(); j++) {
				System.out.print(board.get(i).get(j));
			}

			System.out.println();
		}

		System.out
				.println("-----------------------------------------------------");
	}

	public static void main(String[] args) {
		Board board = new Board();

		//board.spawn(2, 2, 2);
		//board.spawn(3, 2, 4);
		//board.spawn(0, 0, 4);
		//board.show();
		//board.moveLeft();
	//	board.moveDown();
	//	board.move();
//		board.randSpawn();
//		board.randSpawn();
//		board.randSpawn();
//		board.randSpawn();
//		board.randSpawn();
//		board.randSpawn();
//		board.randSpawn();
//		board.randSpawn();
//		board.randSpawn();
//		board.randSpawn();
//		board.randSpawn();
//		board.randSpawn();
//		board.randSpawn();
//		board.randSpawn();
//		board.randSpawn();
//		board.randSpawn();
//		board.randSpawn();
		board.showBoard();
		
//		Scanner read = new Scanner(System.in); //obiekt do odebrania danych od użytkownika
//		 
//	    
//	    String decision;
		
//		while(true){
//			board.spawn(new Random().nextInt(3), new Random().nextInt(3), 2);
//			board.show();
//			decision = read.nextLine();
//			if(decision == "w"){
//				board.moveUp();
//			}
//		}

	}

	@Override
	public void keyPressed(KeyEvent arg0) {
		// TODO Auto-generated method stub
	}

	@Override
	public void keyReleased(KeyEvent arg0) {
		// TODO Auto-generated method stub

		if (arg0.getKeyCode() == KeyEvent.VK_W)
			moveUp();

		if (arg0.getKeyCode() == KeyEvent.VK_S)
			moveDown();

		if (arg0.getKeyCode() == KeyEvent.VK_A)
			moveLeft();

		if (arg0.getKeyCode() == KeyEvent.VK_D)
			moveRight();

		if (arg0.getKeyCode() == KeyEvent.VK_ENTER)
			reset();

	}

	@Override
	public void keyTyped(KeyEvent arg0) {
		// TODO Auto-generated method stub

	}

}
