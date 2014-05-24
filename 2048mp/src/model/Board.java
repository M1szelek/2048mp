package model;

import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.util.ArrayList;
import java.util.Random;
import java.util.Scanner;

import javax.swing.JPanel;

public class Board extends JPanel implements KeyListener {
	ArrayList<ArrayList<Block>> board = new ArrayList<ArrayList<Block>>();
	ArrayList<Position> freeTiles = new ArrayList<Position>();
	int size = 4;

	public Board() {
		super();

		addKeyListener(this);
		setFocusable(true); // IMPORTANT!!!

		for (int i = 0; i < size; i++) {
			board.add(new ArrayList<Block>());
		}

		for (int i = 0; i < board.size(); i++)
			for (int j = 0; j < size; j++)
				board.get(i).add(new Block(0));
	}

	void reset() {
		for (int i = 0; i < board.size(); i++)
			for (int j = 0; j < board.get(i).size(); j++)
				board.get(i).get(j).val = 0;
	}

	void updateFreeTiles() {
		freeTiles.clear();

		for (int i = 0; i < board.size(); i++)
			for (int j = 0; j < board.size(); j++) {
				if (board.get(i).get(j).val == 0) {
					freeTiles.add(new Position(i, j));
				}
			}
	}
	
	void clearAdv(){
		for(int i = 0; i < board.size(); i++){
			for(int j = 0; j < board.size(); j++){
				board.get(i).get(j).adv = false;
			}
		}
	}

	void moveUp() {
		boolean moved = true;
		
		clearAdv();

		while (moved) {
			moved = false;

			for (int i = 1; i < board.size(); i++) {
				for (int j = 0; j < board.size(); j++) {
					if (board.get(i).get(j).val != 0 && !board.get(i).get(j).adv) {
						Integer neigh = board.get(i - 1).get(j).val;
						Integer target = board.get(i).get(j).val;

						if (neigh == target) {
							board.get(i - 1).get(j).val = neigh * 2;
							board.get(i - 1).get(j).adv = true;
							board.get(i).get(j).val = 0;
							moved = true;
							continue;

						}

						if (neigh == 0) {
							board.get(i - 1).get(j).val = target;
							board.get(i).get(j).val = 0;
							moved = true;
						}
					}
				}
			}
		}
	}

	void moveDown() {
		boolean moved = true;
		clearAdv();

		while (moved) {
			moved = false;

			for (int i = board.size() - 2; i >= 0; i--) {
				for (int j = 0; j < board.size(); j++) {
					if (board.get(i).get(j).val != 0 && !board.get(i).get(j).adv) {
						Integer neigh = board.get(i + 1).get(j).val;
						Integer target = board.get(i).get(j).val;

						if (neigh == target) {
							board.get(i + 1).get(j).val = neigh * 2;
							board.get(i + 1).get(j).adv = true;
							board.get(i).get(j).val = 0;
							moved = true;
							continue;
						}

						if (neigh == 0) {
							board.get(i + 1).get(j).val = target;
							board.get(i).get(j).val = 0;
							moved = true;
						}
					}
				}
			}
		}
	}

	void moveLeft() {
		boolean moved = true;
		clearAdv();

		while (moved) {
			moved = false;

			for (int j = 1; j < board.size(); j++) {
				for (int i = 0; i < board.size(); i++) {
					if (board.get(i).get(j).val != 0 && !board.get(i).get(j).adv) {
						Integer neigh = board.get(i).get(j - 1).val;
						Integer target = board.get(i).get(j).val;

						if (neigh == target) {
							board.get(i).get(j - 1).val = neigh * 2;
							board.get(i).get(j - 1).adv = true;
							board.get(i).get(j).val = 0;
							moved = true;
							continue;
						}

						if (neigh == 0) {
							board.get(i).get(j - 1).val = target;
							board.get(i).get(j).val = 0;
							moved = true;
						}
					}
				}
			}
		}
	}

	void moveRight() {
		boolean moved = true;
		clearAdv();

		while (moved) {
			moved = false;
			for (int j = board.size() - 2; j >= 0; j--) {
				for (int i = 0; i < board.size(); i++) {
					if (board.get(i).get(j).val != 0 && !board.get(i).get(j).adv) {
						Integer neigh = board.get(i).get(j + 1).val;
						Integer target = board.get(i).get(j).val;

						if (neigh == target) {
							board.get(i).get(j + 1).val = neigh * 2;
							board.get(i).get(j + 1).adv = true;
							board.get(i).get(j).val = 0;
							moved = true;
							continue;
						}

						if (neigh == 0) {
							board.get(i).get(j + 1).val = target;
							board.get(i).get(j).val = 0;
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

		if (freeTiles.isEmpty()) {
			return;
		}

		int number = new Random().nextInt(2); // 2 or 4

		Position tile = freeTiles.get(new Random().nextInt(freeTiles.size()));

		if (number == 0) {
			board.get(tile.x).get(tile.y).val = 2;
		} else {
			board.get(tile.x).get(tile.y).val = 4;
		}

	}

	void showBoard() {
		for (int i = 0; i < board.size(); i++) {
			for (int j = 0; j < board.size(); j++) {
				System.out.print(board.get(i).get(j).val + "\t");
			}

			System.out.println();
			System.out.println();
		}

		System.out
				.println("-----------------------------------------------------");
	}

	public static void main(String[] args) {
		Board board = new Board();

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

		Scanner read = new Scanner(System.in);

		String decision;
		
		while (true) {
			board.randSpawn();
			board.showBoard();
			decision = read.nextLine();
			switch (decision) {
			case "w":
				board.moveUp();
				break;
			case "s":
				board.moveDown();
				break;
			case "a":
				board.moveLeft();
				break;
			case "d":
				board.moveRight();
				break;
			}
		}

	}

	@Override
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

	}

}
