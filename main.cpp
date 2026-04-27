#include <iostream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

class Player {
public:
	int x = 0;
	int y = 0;
	int hp = 100;
	int shield = 95;
	int damage = 40;
	int keys = 0;
	vector<string> keyTypes;

	void playerStats() {
		cout << "HP: " << hp << endl;
		cout << "Shield: " << shield << endl;
		cout << "Damage: " << damage << endl;
		cout << "Keys owned: " << keys << endl;
		cout << "Key types owned: ";
		for (int i = 0; i < keyTypes.size(); i++) {
			cout << keyTypes[i] << " ";
		}
		cout << endl;
	}

	bool hasKeyType(string type) {
		for (int i = 0; i < keyTypes.size(); i++) {
			if (keyTypes[i] == type) {
				return true;
			}
		}
		return false;
	}

	void addKeyType(string type) {
		keyTypes.push_back(type);
		keys = keyTypes.size();
	}

	void removeKeyType(string type) {
		for (int i = 0; i < keyTypes.size(); i++) {
			if (keyTypes[i] == type) {
				keyTypes.erase(keyTypes.begin() + i);
				keys = keyTypes.size();
				return;
			}
		}
	}
};

class Enemy {
public:
	string name;
	int hp;
	int shield;
	int damage;
	int x;
	int y;

	Enemy(string n, int h, int s, int d, int startR = 0, int startC = 0) {
		name = n;
		hp = h;
		shield = s;
		damage = d;
		y = startR;
		x = startC;
	}

	void showStats() {
		cout << "Enemy: " << name << endl;
		cout << "HP: " << hp << endl;
		cout << "Shield: " << shield << endl;
		cout << "Damage: " << damage << endl;
	}

	void takeDamage(int amount) {
		hp -= amount;
		if (hp < 0) {
			hp = 0;
		}
	}

	bool isDead() {
		return hp <= 0;
	}
};

class Map {
public:
	vector<vector<char>> grid;
	vector<vector<string>> tileTypes;
	int rows;
	int cols;

	void setupDungeon(int r, int c) {
		rows = r;
		cols = c;
		grid = vector<vector<char>>(rows, vector<char>(cols, ' '));
		tileTypes = vector<vector<string>>(rows, vector<string>(cols, ""));

	}

	void loadTutorialDungeon() {
		setupDungeon(7, 13);

		for (int i = 0; i < 13; i++) {
			grid[0][i] = '*';
		}

		grid[1][0] = '*';
		grid[1][4] = '!';
		grid[1][7] = '+';
		grid[1][8] = '-';
		grid[1][9] = ';';
		grid[1][10] = 'D';
		grid[1][11] = '@';
		grid[1][12] = '*';

		grid[2][0] = '*';
		for (int i = 5; i < 13; i++) {
			grid[2][i] = '*';
		}

		grid[3][0] = '*';
		grid[3][5] = '^';
		grid[3][12] = '*';

		for (int i = 0; i < 10; i++) {
			grid[4][i] = '*';
		}
		grid[4][10] = '[';
		grid[4][11] = '*';
		grid[4][12] = '*';

		grid[5][0] = '*';
		grid[5][1] = 'G';
		grid[5][12] = '*';

		for (int i = 0; i < 13; i++) {
			grid[6][i] = '*';
		}

		setTileType(3, 5, "blue");
		setTileType(4, 10, "blue");

		setTileType(10, 1, "spikes");
	}

	void loadForestDungeon() {
		setupDungeon(10, 20);

		string sketch[10] = {
			"********************",
			"*^                @*",
			"********[***********",
			"* +-; [            *",
			"*******           ^*",
			"*G    ***!**********",
			"*     !      *;* -+*",
			"*******      * *[***",
			"*^                 *",
			"********************"
		};

		for (int r = 0; r < 10; r++) {
			for (int c = 0; c < 20; c++) {
				grid[r][c] = sketch[r][c];
			}
		}

		setTileType(1, 1, "red");
		setTileType(2, 8, "red");

		setTileType(4, 18, "yellow");
		setTileType(7, 17, "yellow");

		setTileType(8, 1, "blue");
		setTileType(3, 6, "blue");

	

	}

	void buildCustomDungeon(int r, int c) {
		setupDungeon(r, c);
	}

	void showDungeon() {
		for (int r = 0; r < rows; r++) {
			for (int c = 0; c < cols; c++) {
				cout << grid[r][c] << ' ';
			}
			cout << endl;
		}
	}

	void setTile(int row, int col, char symbol) {
		if (row >= 0 && row < rows && col >= 0 && col < cols) {
			grid[row][col] = symbol;
			tileTypes[row][col] = "";
		}
	}

	void setTileType(int row, int col, string type) {
		if (row >= 0 && row < rows && col >= 0 && col < cols) {
			tileTypes[row][col] = type;
		}
	}

	string getTileType(int row, int col) {
		if (row >= 0 && row < rows && col >= 0 && col < cols) {
			if (tileTypes[row][col] == "") {
				return "normal";
			}
			return tileTypes[row][col];
		}
		return "normal";

	}

};

class SavedDungeon {
public:
	string name;
	Map mapData;
	vector<Enemy> enemyData;
	int startX;
	int startY;

	SavedDungeon(string dungeonName, Map m, vector<Enemy> e, int x, int y) {
		name = dungeonName;
		mapData = m;
		enemyData = e;
		startX = x;
		startY = y;
	}
};

class Game {
private:
	Player hero;
	vector<Enemy> enemies;
	Map map;
	vector<SavedDungeon> customDungeons;

public:

	void showMenu() {
		string choice;

		while (choice != "exit") {
			cout << endl;
			cout << "|| MENU ||" << endl;
			cout << "1. Enter a Dungeon" << endl;
			cout << "2. Design a dungeon" << endl;
			cout << "3. Exit" << endl;
			getline(cin >> ws, choice);


			for (int i = 0; i < choice.size(); i++) {
				choice[i] = tolower(choice[i]);
			}

			if (choice == "enter a dungeon") {
				playDungeonMenu();
			}
			else if (choice == "design a dungeon") {
				customDungeon();
			}
			else if (choice == "exit") {
				cout << "You have existed Dungeon Escape, good bye!" << endl;
			}
			else {
				cout << "Invalid choice. Try again!" << endl;
			}
		}
	}

	void playDungeonMenu() {
		string dungeonChoice;

		while (dungeonChoice != "tutorial dungeon" && dungeonChoice != "forest dungeon") {
			cout << endl;
			cout << "Choose a Dungeon:" << endl;
			cout << "1. Tutorial Dungeon" << endl;
			cout << "2. Forest Dungeon" << endl;

			for (int i = 0; i < customDungeons.size(); i++) {
				cout << i + 3 << ". " << customDungeons[i].name << endl;
			}
			getline(cin >> ws, dungeonChoice);


			for (int i = 0; i < dungeonChoice.size(); i++) {
				dungeonChoice[i] = tolower(dungeonChoice[i]);
			}

			if (dungeonChoice == "tutorial dungeon") {
				map.loadTutorialDungeon();
				loadEnemiesForDungeon1();
				hero.x = 11;
				hero.y = 1;
				cout << endl << "Tutorial Dungeon loaded." << endl;
				showDungeonWithPlayer();
				cout << endl;
				showGameStats();
				showDefineObjects();
				movePlayer();
			}
			else if (dungeonChoice == "forest dungeon") {
				map.loadForestDungeon();
				loadEnemiesForDungeon2();
				hero.x = 18;
				hero.y = 1;
				cout << endl << "Forest Dungeon loaded." << endl;
				showDungeonWithPlayer();
				cout << endl;
				showGameStats();
				showDefineObjects();
				movePlayer();
			}
			else {
				bool foundCustomDungeon = false;

				for (int i = 0; i < customDungeons.size(); i++) {
					string customName = customDungeons[i].name;

					for (int j = 0; j < customName.size(); j++) {
						customName[j] = tolower(customName[j]);
					}
					if (dungeonChoice == customName) {
						map = customDungeons[i].mapData;
						enemies = customDungeons[i].enemyData;
						hero.x = customDungeons[i].startX;
						hero.y = customDungeons[i].startY;

						cout << endl << customDungeons[i].name << " loaded." << endl;
						showDungeonWithPlayer();
						cout << endl;
						showGameStats();
						movePlayer();

						foundCustomDungeon = true;
						break;
					}
				}
				if (foundCustomDungeon == false) {
					cout << "Invalid dungeon choice! " << endl;
				}
				else {
					break;
				}
			}
		}
	}

	void saveCustomDungeon() {
		string dungeonName;

		bool matchName = false;

		for (int i = 0; i < customDungeons.size(); i++) {
			if (customDungeons[i].name == dungeonName) {
				matchName = true;
				customDungeons[i] = SavedDungeon(dungeonName, map, enemies, hero.x, hero.y);
				string renameDungeonOption;

				for (int i = 0; i < renameDungeonOption.size(); i++) {
					renameDungeonOption[i] = tolower(renameDungeonOption[i]);
				}

				while (renameDungeonOption != "yes" && renameDungeonOption != "no") {
					cout << "Would you like to rename this dungeon? " << endl;
					cin >> renameDungeonOption;
					if (renameDungeonOption == "yes") {
						cout << "Enter a new name for this dungeon: ";
						cin >> dungeonName;
					}
					else if (renameDungeonOption == "no") {
						break;
					}
				}
			}
		}
		if (matchName == false) {
			cout << "Enter a name for this dungeon: ";
			cin >> dungeonName;

			customDungeons.push_back(SavedDungeon(dungeonName, map, enemies, hero.x, hero.y));


		}

		cout << "Dungeon saved!" << endl;
	}

	void customDungeon() {

		string optionEditingExistingDungeon;

		while (optionEditingExistingDungeon != "yes" && optionEditingExistingDungeon != "no") {

			cout << endl << "Would you like to edit an existing dungeon?" << endl;
			cout << "1. Yes" << endl;
			cout << "2. No" << endl;
			cin >> optionEditingExistingDungeon;

			for (int i = 0; i < optionEditingExistingDungeon.size(); i++) {
				optionEditingExistingDungeon[i] = tolower(optionEditingExistingDungeon[i]);
			}

			if (optionEditingExistingDungeon == "yes") {

				if (customDungeons.size() > 0) {
					cout << endl << "Which Dungeon would you like to edit?" << endl;

					for (int i = 0; i < customDungeons.size(); i++) {
						cout << i << ". " << customDungeons[i].name << endl;
					}
					string editCustomDungeonOption;
					getline(cin >> ws, editCustomDungeonOption);

					for (int i = 0; i < editCustomDungeonOption.size(); i++) {
						editCustomDungeonOption[i] = tolower(editCustomDungeonOption[i]);
					}

					bool foundCustomDungeon = false;
					int editIndex = -1;
					string editedDungeonName = "";

					for (int i = 0; i < customDungeons.size(); i++) {
						string customName = customDungeons[i].name;

						for (int j = 0; j < customName.size(); j++) {
							customName[j] = tolower(customName[j]);
						}

						if (editCustomDungeonOption == customName) {
							map = customDungeons[i].mapData;
							enemies = customDungeons[i].enemyData;
							hero.x = customDungeons[i].startX;
							hero.y = customDungeons[i].startY;

							foundCustomDungeon = true;
							editIndex = i;
							editedDungeonName = customDungeons[i].name;
							break;
						}
					}
					if (foundCustomDungeon == false) {
						cout << "Invalid dungeon choice!" << endl;
					}
					else {
						buildCustomDungeonEditor();

						char saveChoice;
						while (true) {
							cout << "Save this dungeon? (y/n): ";
							cin >> saveChoice;

							saveChoice = tolower(saveChoice);

							if (saveChoice == 'y') {
								customDungeons[editIndex] = SavedDungeon(editedDungeonName, map, enemies, hero.x, hero.y);
								cout << "Dungeon updated!" << endl;
								break;
							}
							else if (saveChoice == 'n') {
								break;
							}
							else {
								cout << "Invalid input!" << endl;
							}
						}
					}
					cout << "Returning to main menu..." << endl;
				}
				else {
					cout << endl << "There are custom dungeons to edit!" << endl << endl;
					cout << "Returning to main menu..." << endl;

				}
			}
			else if (optionEditingExistingDungeon == "no") {

				int rows, cols;

				cout << endl;
				cout << "Enter number of rows: ";
				cin >> rows;
				cout << "Enter number of cols: ";
				cin >> cols;

				map.buildCustomDungeon(rows, cols);
				enemies.clear();
				hero.x = -1;
				hero.y = -1;

				buildCustomDungeonEditor();

				char saveChoice;
				while (true) {
					cout << "Save this dungeon? (y/n): ";
					cin >> saveChoice;

					saveChoice = tolower(saveChoice);

					if (saveChoice == 'y') {
						saveCustomDungeon();
						break;
					}
					else if (saveChoice == 'n') {
						break;
					}
					else {
						cout << "Invalid input!" << endl;
					}
				}
				cout << "Returning to main menu..." << endl;
			}
		}
	}


	void showCustomDungeonWithNumbers() {
		cout << "   ";
		for (int c = 0; c < map.cols; c++) {
			cout << c + 1 << ' ';
		}
		cout << endl;

		for (int r = 0; r < map.rows; r++) {
			cout << r + 1 << ' ';
			if (r + 1 < 10) {
				cout << ' ';
			}

			for (int c = 0; c < map.cols; c++) {
				if (hero.x >= 0 && hero.y >= 0 && r == hero.y && c == hero.x) {
					cout << '@' << ' ';
				}
				else {
					cout << map.grid[r][c] << ' ';
				}
			}
			cout << endl;
		}
	}

	void buildCustomDungeonEditor() {
		int row, col;
		char symbol;

		while (true) {
			cout << endl;
			showCustomDungeonWithNumbers();
			cout << endl;

			cout << "Choose a row (1-" << map.rows << ") to add an object | Enter 0 to finish & save: ";
			cin >> row;

			if (row == 0) {
				break;
			}

			cout << "Choose a col (1-" << map.cols << "): ";
			cin >> col;

			cout << "Choose a symbol to place:" << endl;
			cout << "* = wall" << endl;
			cout << "G = goal" << endl;
			cout << "^ = key" << endl;
			cout << "[ = locked door" << endl;
			cout << "! = enemy" << endl;
			cout << "+ = health potion" << endl;
			cout << "- = strength potion" << endl;
			cout << "; = defense potion" << endl;
			cout << "D = damaging floor" << endl;
			cout << "@ = hero start position" << endl;
			cout << "? = inspect object values" << endl;
			cout << "Enter symbol: ";
			cin >> symbol;

			if (row >= 1 && row <= map.rows && col >= 1 && col <= map.cols) {
				if (symbol == '@') {
					hero.x = col - 1;
					hero.y = row - 1;
					map.setTile(row - 1, col - 1, ' ');
				}
				else if (symbol == '!') {
					string enemyName;

					cout << "Enter enemy name: ";
					cin >> enemyName;

					addCustomEnemy(enemyName, 50, 50, 50, col - 1, row - 1);
					map.setTile(row - 1, col - 1, '!');
				}
				else if (symbol == '^') {
					string keyType;

					cout << "Enter key type: ";
					cin >> keyType;

					map.setTile(row - 1, col - 1, symbol);
					map.setTileType(row - 1, col - 1, keyType);
				}
				else if (symbol == '[') {
					string doorType;

					cout << "Enter door type: ";
					cin >> doorType;

					map.setTile(row - 1, col - 1, symbol);
					map.setTileType(row - 1, col - 1, doorType);
				}
				else if (symbol == 'D' || symbol == 'd') {
					string floorChoice;
					string floorType;

					cout << "Choose damaging floor type:" << endl;
					cout << "1. poison" << endl;
					cout << "2. spikes" << endl;
					cout << "3. lava" << endl;
					cout << "Enter type: ";
					cin >> floorChoice;

					for (int i = 0; i < floorChoice.size(); i++) {
						floorChoice[i] = tolower(floorChoice[i]);
					}

					if (floorChoice == "1" || floorChoice == "poison") {
						floorType = "poison";
					}
					else if (floorChoice == "2" || floorChoice == "spikes") {
						floorType = "spikes";
					}
					else if (floorChoice == "3" || floorChoice == "lava") {
						floorType = "lava";
					}
					else {
						cout << "Invalid damaging floor type. Defaulting to poison." << endl;
						floorType = "poison";
					}

					map.setTile(row - 1, col - 1, 'D');
					map.setTileType(row - 1, col - 1, floorType);
				}
				else if (symbol == '?') {
					char objectSymbol = map.grid[row - 1][col - 1];
					string objectType = map.getTileType(row - 1, col - 1);

					cout << endl;
					cout << "-----| OBJECT VALUES |-----" << endl;
					cout << "Symbol: " << objectSymbol << endl;

					if (objectSymbol == '^') {
						cout << "Object: key" << endl;
						cout << "Key type: " << objectType << endl;
					}
					else if (objectSymbol == '[') {
						cout << "Object: locked door" << endl;
						cout << "Door type: " << objectType << endl;
					}
					else if (objectSymbol == 'D') {
						cout << "Object: damaging floor" << endl;
						cout << "Floor type: " << objectType << endl;
						cout << "Damage: " << getDamagingFloorDamage(objectType) << endl;
					}
					else if (objectSymbol == '!') {
						int enemyIndex = findEnemyAtPosition(col - 1, row - 1);

						cout << "Object: enemy" << endl;

						if (enemyIndex != -1) {
							enemies[enemyIndex].showStats();
						}
					}
					else {
						cout << "Object type: no special values" << endl;
					}
				}
				else {
					map.setTile(row - 1, col - 1, symbol);
				}
			}
			else {
				cout << "Invalid row or col." << endl;
			}
		}
	}

	int findEnemyAtPosition(int x, int y) {
		for (int i = 0; i < enemies.size(); i++) {
			if (enemies[i].x == x && enemies[i].y == y && enemies[i].hp > 0) {
				return i;
			}
		}
		return -1;
	}

	void showDungeonWithPlayer() {
		for (int r = 0; r < map.rows; r++) {
			for (int c = 0; c < map.cols; c++) {
				if (r == hero.y && c == hero.x) {
					cout << '@' << ' ';
				}
				else {
					cout << map.grid[r][c] << ' ';
				}
			}
			cout << endl;
		}
	}

	void loadEnemiesForDungeon1() {
		enemies.clear();

		enemies.push_back(Enemy("Monster", 50, 50, 50, 1, 4));
	}
	
	void loadEnemiesForDungeon2() {
		enemies.clear();
		enemies.push_back(Enemy("One eyed Monster", 45, 40, 50, 5, 9));
		enemies.push_back(Enemy("Three eyed Monster", 55, 60, 55, 6, 6));
	}

	void addCustomEnemy(string name, int hp, int shield, int damage, int x, int y) {
		enemies.push_back(Enemy(name, hp, shield, damage, x, y));
	}

	void showGameStats() {
		cout << "-----| HERO |-----" << endl;
		hero.playerStats();
		cout << endl;

		cout << "-----| ENEMIES |-----" << endl;
		for (int i = 0; i < enemies.size(); i++) {
			enemies[i].showStats();
			cout << endl;
		}
	}

	void showDefineObjects() {
		cout << "* = wall" << endl;
		cout << "G = goal" << endl;
		cout << "^ = key" << endl;
		cout << "[ = locked door" << endl;
		cout << "! = enemy" << endl;
		cout << "+ = health potion" << endl;
		cout << "- = strength potion" << endl;
		cout << "; = defense potion" << endl;
		cout << "D = damaging floor" << endl;
		cout << "@ = hero start position" << endl;
	}

	void playerAttack(int enemyIndex) {
		if (enemyIndex >= 0 && enemyIndex < enemies.size()) {
			int damageAmount = hero.damage;

			if (enemies[enemyIndex].shield >= damageAmount) {
				enemies[enemyIndex].shield -= damageAmount;
			}
			else {
				int leftoverDamage = damageAmount - enemies[enemyIndex].shield;
				enemies[enemyIndex].shield = 0;
				enemies[enemyIndex].hp -= leftoverDamage;

				if (enemies[enemyIndex].hp < 0) {
					enemies[enemyIndex].hp = 0;
				}
			}

			cout << "Hero attacks " << enemies[enemyIndex].name << " for " << hero.damage << " damage" << endl
				<< enemies[enemyIndex].name << " Shield:" << enemies[enemyIndex].shield << endl
				<< enemies[enemyIndex].name << " HP:" << enemies[enemyIndex].hp << endl << endl;
		}
	}

	void enemyAttack(int enemyIndex) {
		if (enemyIndex >= 0 && enemyIndex < enemies.size()) {
			int damageAmount = enemies[enemyIndex].damage;

			if (hero.shield >= damageAmount) {
				hero.shield -= damageAmount;
			}
			else {
				int leftoverDamage = damageAmount - hero.shield;
				hero.shield = 0;
				hero.hp -= leftoverDamage;

				if (hero.hp < 0) {
					hero.hp = 0;
				}
			}

			cout << enemies[enemyIndex].name << " attacks hero for " << enemies[enemyIndex].damage << " damage" << endl 
				<< "Hero Shield:" << hero.shield << endl
				<< "Hero HP:" << hero.hp << endl << endl;
		}
	}

	void battleRound(int enemyIndex) {
		if (enemyIndex >= 0 && enemyIndex < enemies.size()) {
			while (hero.hp > 0 && enemies[enemyIndex].hp > 0) {
				playerAttack(enemyIndex);

				if (enemies[enemyIndex].hp > 0) {
					enemyAttack(enemyIndex);
				}
			}
		}
	}

	void checkGameOver() {
		if (hero.hp <= 0) {
			cout << "Hero lost." << endl;
			return;
		}

		bool allEnemiesDead = true;

		for (int i = 0; i < enemies.size(); i++) {
			if (enemies[i].hp > 0) {
				allEnemiesDead = false;
			}
		}

		if (allEnemiesDead) {
			cout << endl << "All enemies defeated.";
		}
		else {
			cout << "Battle continues." << endl;
		}
	}

	void movePlayer() {
		string move;

		while (true) {
			cout << endl << endl;
			showDungeonWithPlayer();
			cout << endl;
			hero.playerStats();
			cout << endl;

			cout << "Move left(a), right(d), up(w), down(s), quit(q): ";
			cin >> move;

			for (int i = 0; i < move.size(); i++) {
				move[i] = tolower(move[i]);
			}

			if (move == "quit" || move == "q") {
				break;
			}
			else if (move == "left" || move == "a") {
				if (tryMove(hero.x - 1, hero.y)) {
					showDungeonWithPlayer();
					cout << endl;
					hero.playerStats();
					break;
				}
			}
			else if (move == "right" || move == "d") {
				if (tryMove(hero.x + 1, hero.y)) {
					showDungeonWithPlayer();
					cout << endl;
					hero.playerStats();
					break;
				}
			}
			else if (move == "up" || move == "w") {
				if (tryMove(hero.x, hero.y - 1)) {
					showDungeonWithPlayer();
					cout << endl;
					hero.playerStats();
					break;
				}
			}
			else if (move == "down" || move == "s") {
				if (tryMove(hero.x, hero.y + 1)) {
					showDungeonWithPlayer();
					cout << endl;
					hero.playerStats();
					break;
				}
			}
			else {
				cout << "Invalid move." << endl;
			}
		}
	}

	int getDamagingFloorDamage(string floorType) {
		if (floorType == "poison") {
			return 4;
		}
		else if (floorType == "spikes") {
			return 3;
		}
		else if (floorType == "lava") {
			return 7;
		}

		return 0;
	}

	bool tryMove(int newX, int newY) {
		if (newX < 0 || newX >= map.cols || newY < 0 || newY >= map.rows) {
			cout << "Out of bounds." << endl;
			return false;
		}

		char tile = map.grid[newY][newX];

		if (tile == '*') {
			cout << endl << "You hit a wall.";
			return false;
		}

		if (tile == '[') {
			string doorType = map.getTileType(newY, newX);

			if (hero.hasKeyType(doorType)) {
				hero.removeKeyType(doorType);
				cout << endl << "You used a " << doorType << " key to open the " << doorType << " door.";
				map.grid[newY][newX] = ' ';
				map.setTileType(newY, newX, "");
			}
			else {
				cout << endl << "The " << doorType << " door is locked. You need a " << doorType << " key.";
				return false;
			}
		}

		if (tile == '^') {
			string keyType = map.getTileType(newY, newX);

			hero.addKeyType(keyType);
			cout << endl << "You picked up a " << keyType << " key.";
			map.grid[newY][newX] = ' ';
			map.setTileType(newY, newX, "");
		}
		else if (tile == '!') {
			int enemyIndex = findEnemyAtPosition(newX, newY);

			if (enemyIndex != -1) {
				cout << endl << "You encountered " << enemies[enemyIndex].name << "." << endl;
				battleRound(enemyIndex);
				checkGameOver();

				if (hero.hp <= 0) {
					cout << "GAME OVER. Hero is defeated." << endl;
					return true;
				}

				if (enemies[enemyIndex].hp <= 0) {
					map.grid[newY][newX] = ' ';
				}
				else {
					return false;
				}
			}
		}
		else if (tile == '+') {
			cout << endl << "You drank a health potion.";
			int randomHeal = rand() % 5 + 5;
			hero.hp += randomHeal;
			map.grid[newY][newX] = ' ';
		}
		else if (tile == '-') {
			cout << endl << "You drank a strength potion.";
			int randomStrength = rand() % 5 + 5;
			hero.hp += randomStrength;
			map.grid[newY][newX] = ' ';
		}
		else if (tile == ';') {
			cout << endl << "You drank a defense potion.";
			int randomShield = rand() % 5 + 5;
			hero.hp += randomShield;
			map.grid[newY][newX] = ' ';
		}
		else if (tile == 'D') {
			string floorType = map.getTileType(newY, newX);
			int floorDamage = getDamagingFloorDamage(floorType);

			hero.hp -= floorDamage;

			if (hero.hp < 0) {
				hero.hp = 0;
			}

			cout << endl << "You stepped on " << floorType << ".";
			cout << endl << "You took " << floorDamage << " damage.";

			if (hero.hp <= 0) {
				cout << endl << "GAME OVER. Hero is defeated." << endl;
			}
		}
		else if (tile == 'G') {
			cout << endl << "YOU REACHED THE GOAL!" << endl;
			map.grid[newY][newX] = ' ';
			return true;
		}
		if (map.grid[hero.y][hero.x] == '@') {
			map.grid[hero.y][hero.x] = ' ';
		}

		hero.x = newX;
		hero.y = newY;
		return false;
	}
};

int main() {

	Game game;
	game.showMenu();

	return 0;
	
}