#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

const int WIDTH = 20;
const int HEIGHT = 10;

int playerX = WIDTH / 2;
int playerY = HEIGHT / 2;

int enemyX;
int enemyY;

bool running = true;

// ======================= DRAW =======================

void draw() {

    // Clear screen
    system("cls"); // use "clear" for Linux/Mac

    for (int y = 0; y < HEIGHT; y++) {

        for (int x = 0; x < WIDTH; x++) {

            if (x == playerX && y == playerY)
                cout << "P";

            else if (x == enemyX && y == enemyY)
                cout << "E";

            else
                cout << ".";
        }

        cout << endl;
    }

    cout << "\nMove: W A S D\n";
}

// ======================= INPUT =======================

void input() {

    char move;
    cin >> move;

    switch (tolower(move)) {

        case 'w':
            if (playerY > 0)
                playerY--;
            break;

        case 's':
            if (playerY < HEIGHT - 1)
                playerY++;
            break;

        case 'a':
            if (playerX > 0)
                playerX--;
            break;

        case 'd':
            if (playerX < WIDTH - 1)
                playerX++;
            break;
    }
}

// ======================= UPDATE =======================

void update() {

    // Enemy moves randomly
    int dir = rand() % 4;

    switch (dir) {

        case 0:
            if (enemyY > 0)
                enemyY--;
            break;

        case 1:
            if (enemyY < HEIGHT - 1)
                enemyY++;
            break;

        case 2:
            if (enemyX > 0)
                enemyX--;
            break;

        case 3:
            if (enemyX < WIDTH - 1)
                enemyX++;
            break;
    }

    // Collision
    if (playerX == enemyX && playerY == enemyY) {

        draw();

        cout << "\nGAME OVER!\n";

        running = false;
    }
}

// ======================= MAIN =======================

int main() {

    srand(time(0));

    enemyX = rand() % WIDTH;
    enemyY = rand() % HEIGHT;

    while (running) {

        draw();

        input();

        update();
    }

    return 0;
}
