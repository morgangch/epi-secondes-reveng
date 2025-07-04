#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <algorithm>

const int CELL_SIZE = 40 * 2;
const int ROWS = 10;
const int COLS = 15;

sf::Font font;
sf::Text text;
int developerMode = 0;

int hiddenScore = 1337;
std::string hiddenMessage = "Bravo, vous êtes curieux.";

enum Cell {
    WALL, PATH, START, END, PLAYER, SECRET
};

sf::Clock keyClock;
const sf::Time keyDelay = sf::milliseconds(150); 

bool konamiUnlocked = false;
static std::vector<sf::Keyboard::Key> konamiCode = {
    sf::Keyboard::Up, sf::Keyboard::Up,
    sf::Keyboard::Down, sf::Keyboard::Down,
    sf::Keyboard::Left, sf::Keyboard::Right,
    sf::Keyboard::Left, sf::Keyboard::Right,
    sf::Keyboard::B, sf::Keyboard::A
};
static std::vector<sf::Keyboard::Key> inputHistory;

void __attribute__((used)) ultraSecretEnding() {
    std::cout << "Vous avez percé les abysses de l'exécutable...\n";
}

std::vector<std::vector<Cell>> maze;

// tableau temporaire en int
int mazeData[ROWS][COLS] = {
    {1,0,0,5,0,0,5,0,0,0,0,0,0,0,0},
    {1,1,1,1,0,1,1,1,1,1,1,0,1,1,1},
    {0,0,0,1,0,1,0,0,0,0,1,0,1,0,1},
    {0,1,1,1,1,1,0,1,1,0,1,1,1,0,1},
    {0,1,0,0,0,1,0,1,0,0,0,0,1,0,1},
    {0,1,1,1,0,1,0,1,1,1,1,1,1,0,1},
    {0,0,0,1,0,1,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,0,1,1,1,1,1,1,1,1,1,1},
    {1,5,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

struct Vector2iComparator {
    bool operator()(const sf::Vector2i& lhs, const sf::Vector2i& rhs) const {
        if (lhs.y != rhs.y)
            return lhs.y < rhs.y;
        return lhs.x < rhs.x;
    }
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
        case SECRET: return sf::Color::Black;
    }
    return sf::Color::Magenta;
}

void secretEnding() {
    std::cout << "✨ Vous avez découvert la fin secrète !\n";
}

bool canMove(int x, int y) {
    return x >= 0 && x < COLS && y >= 0 && y < ROWS && maze[y][x] != WALL;
}

static std::vector<sf::Keyboard::Key> rickRollCode = {
    sf::Keyboard::R, sf::Keyboard::I, sf::Keyboard::C, sf::Keyboard::K
};

void displayToScreen(const std::wstring& message, sf::RenderWindow& window) {
    text.setString(message);
    window.clear();
    window.draw(text);
    window.display();
    sf::sleep(sf::seconds(2));
    text.setString("");
    window.clear();
}

void text_creation() {
    if (std::getenv("DEV_MODE")) {
        developerMode = std::stoi(std::getenv("DEV_MODE"));
    }
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Erreur de chargement de la police.\n";
        return;
    }
    text.setFont(font);
    text.setCharacterSize(36);
    text.setFillColor(sf::Color::White);
    text.setPosition(ROWS * CELL_SIZE / 2 - 100, COLS * CELL_SIZE / 2 - 50);
}

int main() {
    std::map<sf::Vector2i, Cell, Vector2iComparator> discoveredSecretZones;
    int totalSecretZones = 0;
    text_creation();
    for (int y = 0; y < ROWS; ++y) {
        std::vector<Cell> row;
        for (int x = 0; x < COLS; ++x) {
            if (mazeData[y][x] == SECRET) {
                totalSecretZones++;
            }
            row.push_back(static_cast<Cell>(mazeData[y][x]));
        }
        maze.push_back(row);
    }

    sf::RenderWindow window(sf::VideoMode(COLS * CELL_SIZE, ROWS * CELL_SIZE), "Maze Game", sf::Style::Close | sf::Style::Resize);
    window.setFramerateLimit(60);

    maze[playerPos.y][playerPos.x] = START;
    maze[endPos.y][endPos.x] = END;

    if (developerMode == 42) {
        std::cout << "Developer mode activé. Spoilers ahead. UwU est le keyword ultime\n";
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::Resized) {
                window.setSize(sf::Vector2u(event.size.width, event.size.height));
            }

            if (event.type == sf::Event::KeyPressed) {
                if (keyClock.getElapsedTime() >= keyDelay) {
                    sf::Keyboard::Key key = event.key.code;
                    inputHistory.push_back(key);

                    for (const auto& k : inputHistory) {
                        std::cout << "Input: " << k << std::endl;
                    }

                    if (inputHistory.size() > konamiCode.size()) {
                        inputHistory.clear();
                    }
                
                    if (inputHistory == konamiCode) {
                        konamiUnlocked = true;
                        displayToScreen(L"Code Konami déverrouillé !", window);
                        inputHistory.clear();
                    }

                    if (inputHistory == rickRollCode) {
                        displayToScreen(L"Jamais tu ne me Rickrolleras !", window);
                        inputHistory.clear();
                    }
                    if (std::find(konamiCode.begin(), konamiCode.end(), key) == konamiCode.end() &&
                        std::find(rickRollCode.begin(), rickRollCode.end(), key) == rickRollCode.end()) {
                        inputHistory.clear();
                    }

                    if (key == sf::Keyboard::Escape) {
                        window.close();
                    }
                    if (key == sf::Keyboard::Up && canMove(playerPos.x, playerPos.y - 1)) {
                        playerPos.y--;
                    } else if (key == sf::Keyboard::Down && canMove(playerPos.x, playerPos.y + 1)) {
                        playerPos.y++;
                    } else if (key == sf::Keyboard::Left && canMove(playerPos.x - 1, playerPos.y)) {
                        playerPos.x--;
                    } else if (key == sf::Keyboard::Right && canMove(playerPos.x + 1, playerPos.y)) {
                        playerPos.x++;
                    }
                    keyClock.restart();
                }
            }
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
        if (maze[playerPos.y][playerPos.x] == static_cast<Cell>(5)) {
            if (discoveredSecretZones.find(playerPos) == discoveredSecretZones.end()) {
                discoveredSecretZones[playerPos] = SECRET;
            }   
        }
        if (discoveredSecretZones.size() == totalSecretZones) {
            displayToScreen(L"Vous avez découvert toutes les zones secrètes !", window);
            discoveredSecretZones.clear();
        }

        if (playerPos == endPos) {
            displayToScreen(L"Vous avez atteint la fin du labyrinthe !", window);
            sf::sleep(sf::seconds(2));
            window.close();
        }
    }

    return 0;
}
