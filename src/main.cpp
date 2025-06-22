#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

const int CELL_SIZE = 40;
const int ROWS = 10;
const int COLS = 15;

// enum déjà défini
enum Cell {
    WALL, PATH, START, END, PLAYER
};

std::vector<std::vector<Cell>> maze;

// tableau temporaire en int
int mazeData[ROWS][COLS] = {
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,1,1,1,0,1,1,1,1,1,1,0,1,1,1},
    {0,0,0,1,0,1,0,0,0,0,1,0,1,0,1},
    {0,1,1,1,1,1,0,1,1,0,1,1,1,0,1},
    {0,1,0,0,0,1,0,1,0,0,0,0,1,0,1},
    {0,1,1,1,0,1,0,1,1,1,1,1,1,0,1},
    {0,0,0,1,0,1,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,0,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

sf::Vector2i playerPos = {0, 0};
sf::Vector2i endPos = {14, 9};

sf::Color getColor(Cell cell) {
    switch (cell) {
        case WALL: return sf::Color::Black;
        case PATH: return sf::Color::White;
        case START: return sf::Color::Green;
        case END: return sf::Color::Red;
        case PLAYER: return sf::Color::Blue;
    }
    return sf::Color::Magenta;
}

bool canMove(int x, int y) {
    return x >= 0 && x < COLS && y >= 0 && y < ROWS && maze[y][x] != WALL;
}

int main() {
    // conversion
    for (int y = 0; y < ROWS; ++y) {
        std::vector<Cell> row;
        for (int x = 0; x < COLS; ++x) {
            row.push_back(static_cast<Cell>(mazeData[y][x]));
        }
        maze.push_back(row);
    }

    sf::RenderWindow window(sf::VideoMode(COLS * CELL_SIZE, ROWS * CELL_SIZE), "Maze Game", sf::Style::Close);
    window.setFramerateLimit(60);

    maze[playerPos.y][playerPos.x] = START;
    maze[endPos.y][endPos.x] = END;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Mouvement joueur
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && canMove(playerPos.x, playerPos.y - 1)) {
            playerPos.y--;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && canMove(playerPos.x, playerPos.y + 1)) {
            playerPos.y++;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && canMove(playerPos.x - 1, playerPos.y)) {
            playerPos.x--;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && canMove(playerPos.x + 1, playerPos.y)) {
            playerPos.x++;
        }

        // Affichage
        window.clear();
        for (int y = 0; y < ROWS; y++) {
            for (int x = 0; x < COLS; x++) {
                sf::RectangleShape cellShape(sf::Vector2f(CELL_SIZE - 2, CELL_SIZE - 2));
                cellShape.setPosition(x * CELL_SIZE + 1, y * CELL_SIZE + 1);

                if (x == playerPos.x && y == playerPos.y)
                    cellShape.setFillColor(getColor(PLAYER));
                else
                    cellShape.setFillColor(getColor(maze[y][x]));

                window.draw(cellShape);
            }
        }

        window.display();

        if (playerPos == endPos) {
            std::cout << "🎉 Félicitations, vous avez terminé le labyrinthe !\n";
            sf::sleep(sf::seconds(2));
            window.close();
        }
    }

    return 0;
}
