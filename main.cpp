#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
using namespace sf;

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 500;

//Base Entity Class
// Abstract base class for game objects
class Entity : public Drawable {
public:
    virtual void update() {}
    virtual FloatRect getBounds() const = 0;
};

// Paddle Class
// Represents the player-controlled paddle
class Paddle : public Entity {
private:
    Sprite sprite;
public:
    Paddle(const Texture& texture) {
        sprite.setTexture(texture);
        sprite.setPosition(WINDOW_WIDTH / 2 - 40, WINDOW_HEIGHT - 40);
    }

    void moveLeft() {
        if (sprite.getPosition().x > 0)
            sprite.move(-7, 0);
    }

    void moveRight() {
        if (sprite.getPosition().x + sprite.getGlobalBounds().width < WINDOW_WIDTH)
            sprite.move(7, 0);
    }

    void draw(RenderTarget& target, RenderStates states) const override {
        target.draw(sprite, states);
    }

    FloatRect getBounds() const override {
        return sprite.getGlobalBounds();
    }

    void setPosition(float x, float y) {
        sprite.setPosition(x, y);
    }

    Vector2f getPosition() const {
        return sprite.getPosition();
    }

    Sprite& getSprite() { return sprite; }
};

// Ball Class
// Controls the ball's movement and bounce logic
class Ball : public Entity {
private:
    Sprite sprite;
    Vector2f velocity;
public:
    Ball(const Texture& texture) {
        sprite.setTexture(texture);
        sprite.setScale(0.5f, 0.5f);
        sprite.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
        velocity = { 3.f, -3.f };
    }

    void update() override {
        sprite.move(velocity);
    }

    void bounceX() { velocity.x = -velocity.x; }
    void bounceY() { velocity.y = -velocity.y; }

    void draw(RenderTarget& target, RenderStates states) const override {
        target.draw(sprite, states);
    }

    FloatRect getBounds() const override {
        return sprite.getGlobalBounds();
    }

    Vector2f getVelocity() const { return velocity; }
    void setVelocity(Vector2f v) { velocity = v; }
    Vector2f getPosition() const { return sprite.getPosition(); }
    void setPosition(float x, float y) { sprite.setPosition(x, y); }
};

// Block Class
// Represents breakable blocks in the game
class Block : public Entity {
private:
    RectangleShape shape;
    bool destroyed = false;
public:
    Block(float x, float y, const Texture& texture) {
        shape.setSize({ 40.f, 20.f });
        shape.setPosition(x, y);
        shape.setTexture(&texture);
    }

    void draw(RenderTarget& target, RenderStates states) const override {
        if (!destroyed)
            target.draw(shape, states);
    }

    FloatRect getBounds() const override {
        return shape.getGlobalBounds();
    }

    bool isDestroyed() const { return destroyed; }
    void destroy() { destroyed = true; }
};

int main() {
    // Recoginize random seed and create game window
    srand(static_cast<unsigned>(time(0)));
    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Block Bash!");
    window.setFramerateLimit(80);

    // Load textures and font
    Texture texBall, texPaddle, texBackground;
    Font font;
    if (!texBall.loadFromFile("resources/ball.png")) return -1;
    if (!texPaddle.loadFromFile("resources/paddle.png")) return -1;
    if (!texBackground.loadFromFile("resources/background.jpg")) return -1;
    if (!font.loadFromFile("resources/arial.ttf")) return -1;

    // Load block textures
    std::vector<Texture> blockTextures(5);
    if (!blockTextures[0].loadFromFile("resources/block.png")) return -1;
    if (!blockTextures[1].loadFromFile("resources/block02.png")) return -1;
    if (!blockTextures[2].loadFromFile("resources/block03.png")) return -1;
    if (!blockTextures[3].loadFromFile("resources/block04.png")) return -1;
    if (!blockTextures[4].loadFromFile("resources/block05.png")) return -1;

    // Load sounds and music
    SoundBuffer hitBuffer, gameOverBuffer;
    Music backgroundMusic;
    if (!hitBuffer.loadFromFile("resources/hit.wav")) return -1;
    if (!gameOverBuffer.loadFromFile("resources/gameover.wav")) return -1;
    if (!backgroundMusic.openFromFile("resources/music.ogg")) return -1;

    Sound hitSound(hitBuffer);
    Sound gameOverSound(gameOverBuffer);
    backgroundMusic.setLoop(true);
    backgroundMusic.setVolume(30);
    backgroundMusic.play();

    // Set up game objects
    Sprite background(texBackground);
    Paddle paddle(texPaddle);
    Ball ball(texBall);

    int score = 0, level = 1, highScore = 0;

    // Load high score from file
    std::ifstream fin("highscore.txt");
    if (fin.is_open()) {
        fin >> highScore;
        fin.close();
    }

    // Setup UI text elements
    Text scoreText, levelText, highScoreText, gameOverText;
    scoreText.setFont(font);
    levelText.setFont(font);
    highScoreText.setFont(font);
    gameOverText.setFont(font);

    scoreText.setCharacterSize(18);
    levelText.setCharacterSize(18);
    highScoreText.setCharacterSize(18);
    gameOverText.setCharacterSize(22);

    scoreText.setFillColor(Color::Red);
    levelText.setFillColor(Color::Red);
    highScoreText.setFillColor(Color::Red);
    gameOverText.setFillColor(Color::Red);
    gameOverText.setStyle(Text::Bold);
    gameOverText.setPosition(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 50);

    scoreText.setPosition(10, 10);
    levelText.setPosition(500, 10);
    highScoreText.setPosition(220, 10);

    // Lambda to update text values
    auto updateText = [&]() {
        scoreText.setString("Score: " + std::to_string(score));
        levelText.setString("Level: " + std::to_string(level));
        highScoreText.setString("High Score: " + std::to_string(highScore));
    };

    std::vector<Block> blocks;

    // Lambda to generate a grid of blocks
    auto createBlocks = [&](int rows, int cols) {
        blocks.clear();
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                int texIndex = rand() % blockTextures.size();
                blocks.emplace_back(50 + j * 45, 50 + i * 25, blockTextures[texIndex]);
            }
        }
    };

    createBlocks(4, 10); // Initial level blocks
    updateText();

    bool gameOver = false;

    // Game Loop
    while (window.isOpen()) {
        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed)
                window.close();
        }

        // Handle game over state
        if (gameOver) {
            gameOverText.setString("Game Over! Final Score: " + std::to_string(score) + "\nPress R to Restart");
            window.clear();
            window.draw(background);
            window.draw(gameOverText);
            window.display();

            std::ofstream fout("highscore.txt");
            if (fout.is_open()) {
                fout << highScore;
                fout.close();
            }

            // Restart game
            if (Keyboard::isKeyPressed(Keyboard::R)) {
                gameOver = false;
                score = 0;
                level = 1;
                createBlocks(4, 10);
                ball.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
                ball.setVelocity({ 3.f, -3.f });
            }

            continue;
        }

        // Handle paddle movement
        if (Keyboard::isKeyPressed(Keyboard::Left)) paddle.moveLeft();
        if (Keyboard::isKeyPressed(Keyboard::Right)) paddle.moveRight();

        // Update ball position
        ball.update();
        auto bounds = ball.getBounds();

        // Wall collisions
        if (bounds.left <= 0 || bounds.left + bounds.width >= WINDOW_WIDTH)
            ball.bounceX();
        if (bounds.top <= 0)
            ball.bounceY();
        if (bounds.top + bounds.height >= WINDOW_HEIGHT) {
            gameOver = true;
            gameOverSound.play();
            if (score > highScore) highScore = score;
        }

        // Paddle collision
        if (ball.getBounds().intersects(paddle.getBounds()))
            ball.setVelocity({ ball.getVelocity().x, -std::abs(ball.getVelocity().y) });

        // Block collision
        for (auto& block : blocks) {
            if (!block.isDestroyed() && ball.getBounds().intersects(block.getBounds())) {
                block.destroy();
                ball.bounceY();
                hitSound.play();
                score += 10;
                break;
            }
        }

        // Remove destroyed blocks
        blocks.erase(std::remove_if(blocks.begin(), blocks.end(),
            [](Block& b) { return b.isDestroyed(); }), blocks.end());

        // Increase level with score
        int expectedLevel = (score / 100) + 1;
        if (expectedLevel > level) {
            level = expectedLevel;
            createBlocks(3 + level, 10);
            Vector2f vel = ball.getVelocity();
            vel.x *= 1.1f; vel.y *= 1.1f;
            ball.setVelocity(vel);
        }

        updateText();

        // Draw everything
        window.clear();
        window.draw(background);
        window.draw(paddle);
        window.draw(ball);
        for (auto& b : blocks) window.draw(b);
        window.draw(scoreText);
        window.draw(levelText);
        window.draw(highScoreText);
        window.display();
    }

    return 0;
}
