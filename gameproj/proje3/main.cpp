#include <SFML/Graphics.hpp>
#include <vector>
#include <tuple>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <SFML/Audio.hpp>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int GAME_WIDTH = 1600;
const int GAME_HEIGHT = 1200;

enum GameState {
    MENU,
    GAME,
    WIN,
    EXIT
};

enum Difficulty {
    NORMAL,
    HARD
};

class Button {
public:
    sf::RectangleShape shape;
    sf::Text text;

    Button(const std::string& label, const sf::Font& font, float x, float y) {
        text.setFont(font);
        text.setString(label);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);
        text.setPosition(x + 20, y + 10);

        shape.setSize(sf::Vector2f(250, 50));
        shape.setFillColor(sf::Color::Red);
        shape.setPosition(x, y);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
        window.draw(text);
    }

    bool isClicked(sf::Vector2i mousePos) {
        return shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
    }

    void setLabel(const std::string& label) {
        text.setString(label);
    }
};

class Win {
private:
    std::vector<Button> WinButtons;
public:
    Win(sf::Font& font) {
        WinButtons.push_back(Button("Menu", font, 275, 400));
        WinButtons.push_back(Button("Play again", font, 275, 500));
    }

    void draw(sf::RenderWindow& window) {
        for (auto& button : WinButtons) {
            button.draw(window);
        }
    }

    int handleClick(sf::RenderWindow& window, sf::Vector2i mousePos) {
        sf::Vector2f worldPos = window.mapPixelToCoords(mousePos, window.getDefaultView());
        for (size_t i = 0; i < WinButtons.size(); ++i) {
            if (WinButtons[i].isClicked(static_cast<sf::Vector2i>(worldPos))) {
                return i;
            }
        }
        return -1;
    }
};

class SubMenu {
public:
    SubMenu(sf::Font& font) {
        buttons.push_back(Button("Normal", font, 300, 400));
        buttons.push_back(Button("Hard", font, 300, 500));
    }

    void draw(sf::RenderWindow& window) {
        for (auto& button : buttons) {
            button.draw(window);
        }
    }

    int handleClick(sf::Vector2i mousePos) {
        for (size_t i = 0; i < buttons.size(); ++i) {
            if (buttons[i].isClicked(mousePos)) {
                return i;
            }
        }
        return -1;
    }

private:
    std::vector<Button> buttons;
};

class Menu {
public:
    Menu(sf::Font& font) : showSubMenu(false), subMenu(font) {
        buttons.push_back(Button("Start", font, 300, 200));
        buttons.push_back(Button("Select Game Mode", font, 300, 300));
        buttons.push_back(Button("Exit", font, 300, 400));
    }

    void draw(sf::RenderWindow& window) {
        for (auto& button : buttons) {
            button.draw(window);
        }
        if (showSubMenu) {
            subMenu.draw(window);
        }
    }

    int handleClick(sf::Vector2i mousePos) {
        if (showSubMenu) {
            int subMenuIndex = subMenu.handleClick(mousePos);
            if (subMenuIndex != -1) {
                showSubMenu = false;
                return subMenuIndex + 3; // Offset by 3 to distinguish from main menu buttons
            }
        }
        else {
            for (size_t i = 0; i < buttons.size(); ++i) {
                if (buttons[i].isClicked(mousePos)) {
                    if (i == 1) {
                        showSubMenu = true;
                        return -1;
                    }
                    return i;
                }
            }
        }
        return -1;
    }

private:
    std::vector<Button> buttons;
    bool showSubMenu;
    SubMenu subMenu;
};

class Player {
public:
    sf::Sprite sprite;
    sf::Texture texture;
    float velocityY;
    float velocityX;
    bool onGround;
    sf::IntRect currentFrame;
    sf::Clock animationClock;
    float animationTime;
    int currentFrameIndex;
    int jumpCount = 0;

    Player(const std::string& textureFile) : velocityY(0), velocityX(200.0f), onGround(false), currentFrameIndex(0), animationTime(0.1f) {
        if (!texture.loadFromFile(textureFile)) {
            std::cerr << "Error loading player texture!" << std::endl;
        }
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(0, 0, 32, 32)); // Assume each frame is 32x32 pixels
        sprite.setPosition(1, 1100);
    }

    void update(float deltaTime) {
        // Apply gravity
        if (!onGround) {
            velocityY += 981.0f * deltaTime; // gravity in pixels/s^2
        }

        // Apply horizontal movement
        sprite.move(velocityX * deltaTime, velocityY * deltaTime);

        // Update animation
        updateAnimation(deltaTime);
    }

    void jump() {
        if (jumpCount < 2) {
            velocityY = -600.0f; // jump velocity
            onGround = false;
            jumpCount++;
        }
    }

    void moveLeft() {
        velocityX = -200.0f; // move speed
        sprite.setScale(-1, 1); // Flip sprite horizontally
        sprite.setOrigin(sprite.getLocalBounds().width, 0); // Set origin to right side
    }

    void moveRight() {
        velocityX = 200.0f; // move speed
        sprite.setScale(1, 1); // Reset sprite scale
        sprite.setOrigin(0, 0); // Reset origin to default (left side)
    }

    void stop() {
        velocityX = 0.0f;
    }

    void updateAnimation(float deltaTime) {
        // Update only if the time elapsed is greater than animationTime
        if (animationClock.getElapsedTime().asSeconds() >= animationTime) {
            if (onGround) {
                currentFrameIndex = (currentFrameIndex + 1) % 2;
                currentFrame = sf::IntRect(currentFrameIndex * 32, 0 * 32, 32, 32);
            } else {
                currentFrameIndex = (currentFrameIndex + 1) % 8;
                currentFrame = sf::IntRect(currentFrameIndex * 32, 5 * 32, 32, 32);
            }

            sprite.setTextureRect(currentFrame);
            animationClock.restart();
        }
    }

    void handleCollision(const sf::FloatRect& platformBounds) {
        sf::FloatRect playerBounds = sprite.getGlobalBounds();

        if (playerBounds.intersects(platformBounds)) {
            float playerBottom = playerBounds.top + playerBounds.height;
            float platformTop = platformBounds.top;
            float playerTop = playerBounds.top;
            float platformBottom = platformBounds.top + platformBounds.height;
            float playerRight = playerBounds.left + playerBounds.width;
            float platformLeft = platformBounds.left;
            float playerLeft = playerBounds.left;
            float platformRight = platformBounds.left + platformBounds.width;

            float overlapBottom = playerBottom - platformTop;
            float overlapTop = platformBottom - playerTop;
            float overlapRight = playerRight - platformLeft;
            float overlapLeft = platformRight - playerLeft;

            bool fromBottom = overlapBottom < overlapTop && overlapBottom < overlapRight && overlapBottom < overlapLeft;
            bool fromTop = overlapTop < overlapBottom && overlapTop < overlapRight && overlapTop < overlapLeft;
            bool fromRight = overlapRight < overlapLeft && overlapRight < overlapTop && overlapRight < overlapBottom;
            bool fromLeft = overlapLeft < overlapRight && overlapLeft < overlapTop && overlapLeft < overlapBottom;

            if (fromBottom) {
                sprite.setPosition(sprite.getPosition().x, platformBounds.top - playerBounds.height);
                velocityY = 0;
                onGround = true;
                jumpCount = 0;
            } else if (fromTop) {
                sprite.setPosition(sprite.getPosition().x, platformBounds.top + platformBounds.height);
                velocityY = 0;
            } else if (fromRight) {
                sprite.setPosition(platformBounds.left - playerBounds.width, sprite.getPosition().y);
            } else if (fromLeft) {
                sprite.setPosition(platformBounds.left + platformBounds.width, sprite.getPosition().y);
            }
        }
    }

    void caught() {
        sprite.setPosition(1, 1100);
        velocityY = 0;
        velocityX = 200.0f;
        jumpCount = 0;
    }


    void draw(sf::RenderWindow& window) {
        window.draw(sprite);
    }
};


class Attacker {
public:
    Attacker(Difficulty dif) : spawnClock() {
        if(dif == HARD){
            spawnInterval = 0.5f;
        }else if(dif == NORMAL){
            spawnInterval = 1.0f;
        }
        captureSpeed = 200.0f; // Speed at which ghost moves towards the player
        if (!ghostTexture.loadFromFile("E:/szkola/Programowanie/c++/gameproj/proje3/assets/ghost.png")) {
            std::cerr << "Error loading ghost texture!" << std::endl;
        }
    }

    void update(float deltaTime, Player& player, sf::RenderWindow& window) {
        if (spawnClock.getElapsedTime().asSeconds() >= spawnInterval) {
            spawnClock.restart();
            spawnGhost(window);
        }

        for (auto it = ghosts.begin(); it != ghosts.end(); ) {
            moveGhost(*it, deltaTime);
            if (it->getGlobalBounds().intersects(player.sprite.getGlobalBounds())) {
                capturePlayer(player);
                it = ghosts.erase(it); // Remove ghost upon capturing player
            } else if (it->getPosition().y >= 1200) {
                it = ghosts.erase(it); // Remove ghost if it reaches the bottom of the screen
            } else {
                ++it;
            }
        }
    }

    void draw(sf::RenderWindow& window) {
        for (const auto& ghost : ghosts) {
            window.draw(ghost);
        }
    }

private:
    sf::Clock spawnClock;
    float spawnInterval;
    float captureSpeed;
    std::vector<sf::Sprite> ghosts;
    sf::Texture ghostTexture;

    void spawnGhost(sf::RenderWindow& window) {
        sf::Sprite ghost;
        ghost.setTexture(ghostTexture);
        ghost.setTextureRect(sf::IntRect(0, 0, 32, 32));
        float x = static_cast<float>(rand() % window.getSize().x);
        ghost.setPosition(x, 0); // Spawn at the top of the window
        ghosts.push_back(ghost);
        std::cout << "Spawned ghost at x: " << x << std::endl; // Debug output
    }

    void moveGhost(sf::Sprite& ghost, float deltaTime) {
        ghost.move(0, captureSpeed * deltaTime);
    }

    void capturePlayer(Player& player) {
        player.caught();
    }
};


class ParallaxBackground {
private:
    sf::Sprite sprite;
    sf::Texture texture;
    float parallaxFactor;

public:
    ParallaxBackground(const std::string& filePath, float parallaxFactor) : parallaxFactor(parallaxFactor) {
        if (!texture.loadFromFile(filePath)) {
            std::cerr << "Could not load texture" << filePath << "!" << std::endl;
        }
        sprite.setTexture(texture);
        sprite.setScale(
            static_cast<float>(GAME_WIDTH) / texture.getSize().x,
            static_cast<float>(GAME_HEIGHT) / texture.getSize().y
            );
    }

    void update(const sf::View& view) {
        sf::Vector2f viewCenter = view.getCenter();
        sprite.setPosition(viewCenter.x * parallaxFactor - WINDOW_WIDTH * parallaxFactor / 2,
                           viewCenter.y * parallaxFactor - WINDOW_HEIGHT * parallaxFactor / 2);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(sprite);
    }
};

class TileMap : public sf::Drawable, public sf::Transformable {
public:
    bool load(const std::string& tileset, sf::Vector2u tileSize, const std::vector<std::tuple<int, int, int>>& tiles) {
        if (!m_tileset.loadFromFile(tileset))
            return false;

        m_vertices.setPrimitiveType(sf::Quads);
        m_vertices.resize(tiles.size() * 4);

        size_t platformCount = 0;
        size_t decorativeCount = 0;

        for (const auto& tile : tiles) {
            int tileNumber = std::get<2>(tile);
            if (tileNumber >= 0 && tileNumber <= 20) {
                platformCount++;
            }
            else {
                decorativeCount++;
            }
        }

        size_t platformIndex = decorativeCount * 4;
        size_t decorativeIndex = 0;

        for (size_t i = 0; i < tiles.size(); ++i) {
            int x, y, tileNumber;
            std::tie(x, y, tileNumber) = tiles[i];

            int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
            int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

            sf::Vertex* quad;

            if (tileNumber >= 0 && tileNumber <= 20) {
                quad = &m_vertices[platformIndex];
                platformIndex += 4;
                m_collisionRects.push_back(sf::FloatRect(x, y, tileSize.x, tileSize.y));
            }
            else {
                quad = &m_vertices[decorativeIndex];
                decorativeIndex += 4;
            }

            if (tileNumber == 39) {
                m_crownRects.push_back(sf::FloatRect(x, y, tileSize.x, tileSize.y));
            }

            quad[0].position = sf::Vector2f(x, y);
            quad[1].position = sf::Vector2f(x + tileSize.x, y);
            quad[2].position = sf::Vector2f(x + tileSize.x, y + tileSize.y);
            quad[3].position = sf::Vector2f(x, y + tileSize.y);

            quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
            quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
            quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
            quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
        }

        return true;
    }

    const std::vector<sf::FloatRect>& getCollisionRects() const {
        return m_collisionRects;
    }

    const std::vector<sf::FloatRect>& getCrownRects() const {
        return m_crownRects;
    }

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        states.texture = &m_tileset;
        target.draw(m_vertices, states);
    }

    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
    std::vector<sf::FloatRect> m_collisionRects;
    std::vector<sf::FloatRect> m_crownRects;
};



std::vector<std::tuple<int, int, int>> loadTileData(const std::string& filePath) {
    std::vector<std::tuple<int, int, int>> tiles;
    std::ifstream file(filePath);
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        int x, y, tileNumber;
        if (iss >> x >> y >> tileNumber) {
            tiles.emplace_back(x, y, tileNumber);
        }
    }

    return tiles;
}


int main() {

    // sf::Music music;
    // if (!music.openFromFile("E:/szkola/Programowanie/c++/gameproj/proje3/assets/sound.ogg")) {
    //     std::cerr << "Could not load mp3 file" << std::endl;
    //     return -1;
    // }

    std::vector<std::tuple<int, int, int>> tiles = loadTileData("E:/szkola/Programowanie/c++/gameproj/proje3/assets/tile_data.txt");

    TileMap tileMap;
    if (!tileMap.load("E:/szkola/Programowanie/c++/gameproj/proje3/assets/tilset11.png", sf::Vector2u(32, 32), tiles)) {
        std::cerr << "Could not load tileset" << std::endl;
        return -1;
    }

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Jumper Keng");
    window.setFramerateLimit(60);

    ParallaxBackground parallaxBackground("E:/szkola/Programowanie/c++/gameproj/proje3/assets/background1.png", 0.5f);

    sf::Font font;
    if (!font.loadFromFile("E:/szkola/Programowanie/c++/gameproj/proje3/assets/arial.ttf")) {
        std::cerr << "Could not load font" << std::endl;
        return -1;
    }

    sf::Text positionText;
    positionText.setFont(font);
    positionText.setCharacterSize(24);
    positionText.setFillColor(sf::Color::Black);
    positionText.setPosition(10, 10);

    Player player("E:/szkola/Programowanie/c++/gameproj/proje3/assets/AnimationSheet_Character.png");

    std::srand(static_cast<unsigned>(std::time(nullptr)));

    sf::Clock clock;

    Menu menu(font);
    GameState gameState = MENU;
    Difficulty difficulty = NORMAL;

    Win winScreen(font);
    sf::Sprite winSprite;

    sf::Texture winTexture;
    if (!winTexture.loadFromFile("E:/szkola/Programowanie/c++/gameproj/proje3/assets/win.png")) {
        std::cerr << "Couldnt load texture" << std::endl;
        return -1;
    }

    winSprite.setTexture(winTexture);
    winSprite.setPosition((WINDOW_WIDTH - winTexture.getSize().x) / 2, (WINDOW_HEIGHT - winTexture.getSize().y) / 2);

    // music.play();

    sf::View view(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
    window.setView(view);

    Attacker attacker(difficulty);

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (gameState == MENU) {
                if (event.type == sf::Event::MouseButtonPressed) {
                    int buttonIndex = menu.handleClick(sf::Mouse::getPosition(window));
                    if (buttonIndex == 0) {
                        gameState = GAME;
                    } else if (buttonIndex == 1) {

                    } else if (buttonIndex == 2) {
                        gameState = EXIT;
                    } else if (buttonIndex == 3) {
                        difficulty = NORMAL;
                    } else if (buttonIndex == 4) {
                        difficulty = HARD;
                    }
                }
            } else if (gameState == GAME) {
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Space) {
                        player.jump();
                    }
                }
            }
            else if (gameState == WIN) {
                if (event.type == sf::Event::MouseButtonPressed) {
                    int buttonIndex = winScreen.handleClick(window, sf::Mouse::getPosition(window));
                    if (buttonIndex == 0) {
                        gameState = MENU;
                        window.setView(window.getDefaultView()); // Reset view to default
                    }
                    else if (buttonIndex == 1) {
                        gameState = GAME;
                    }
                }
            }
        }

        if (gameState == EXIT) {
            window.close();
        } else if (gameState == GAME) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                player.moveLeft();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                player.moveRight();
            }
            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right) {
                    player.stop();
                }
            }

            for (const auto& rect : tileMap.getCrownRects()) {
                sf::FloatRect playerBounds = player.sprite.getGlobalBounds();
                if (playerBounds.intersects(rect)) {
                    gameState = WIN;
                }
            }

            player.update(deltaTime);

            player.onGround = false;

            for (const auto& rect : tileMap.getCollisionRects()) {
                player.handleCollision(rect);
            }


            sf::View view = window.getView();
            sf::Vector2f playerPosition = player.sprite.getPosition();
            sf::Vector2f viewCenter = playerPosition;

            if (viewCenter.x < WINDOW_WIDTH / 2.0f) {
                viewCenter.x = WINDOW_WIDTH / 2.0f;
            }
            if (viewCenter.x > GAME_WIDTH - WINDOW_WIDTH / 2.0f) {
                viewCenter.x = GAME_WIDTH - WINDOW_WIDTH / 2.0f;
            }

            if (viewCenter.y < WINDOW_HEIGHT / 2.0f) {
                viewCenter.y = WINDOW_HEIGHT / 2.0f;
            }
            if (viewCenter.y > GAME_HEIGHT - WINDOW_HEIGHT / 2.0f) {
                viewCenter.y = GAME_HEIGHT - WINDOW_HEIGHT / 2.0f;
            }

            view.setCenter(viewCenter);
            window.setView(view);


            parallaxBackground.update(view);

            int yDiv100 = static_cast<int>(playerPosition.y) / 100;
            std::ostringstream ss;
            ss << "Y: " << yDiv100;
            positionText.setString(ss.str());

            attacker.update(deltaTime, player, window);
        }

        window.clear();
        parallaxBackground.draw(window);
        window.draw(tileMap);
        window.draw(player.sprite);
        window.draw(positionText);
        attacker.draw(window);

        if (gameState == WIN) {
            window.clear();
            player.sprite.setPosition(1, 1100);
            winSprite.setPosition(view.getCenter().x - 100, view.getCenter().y - 200);
            winSprite.setScale(0.85,0.75);

            window.draw(winSprite);

            window.setView(window.getDefaultView());
            winScreen.draw(window);

            window.setView(view);

        }
        else if (gameState == MENU) {
            window.clear();
            window.setView(window.getDefaultView());
            menu.draw(window);
        }
        window.display();

    }

    return 0;
}
