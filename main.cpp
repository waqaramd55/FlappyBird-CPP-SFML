#include <SFML/Graphics.hpp>
#include <vector>
#include <optional>
#include <iostream>

enum class State { Start, Playing, Paused, GameOver };

int main() {
    sf::RenderWindow window(sf::VideoMode({400, 600}), "Flappy Bird Pro");
    window.setFramerateLimit(60);

    State gameState = State::Start;
    int score = 0;
    int highScore = 0;

    sf::CircleShape bird(15.f);
    bird.setFillColor(sf::Color::Yellow);
    bird.setPosition({50.f, 300.f});
    
    float velocity = 0.f;
    const float gravity = 0.5f;

    struct Pipe { sf::RectangleShape top, bottom; bool passed = false; };
    std::vector<Pipe> pipes;
    int spawnTimer = 0;

    std::cout << "--- FLAPPY BIRD ---" << std::endl;
    std::cout << "Press SPACE to START" << std::endl;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Space) {
                    if (gameState == State::Start || gameState == State::GameOver) {
                        bird.setPosition({50.f, 300.f});
                        velocity = 0;
                        pipes.clear();
                        score = 0;
                        gameState = State::Playing;
                        std::cout << "\nGame Started!                          " << std::endl;
                    } 
                    else if (gameState == State::Playing) {
                        velocity = -8.f;
                    }
                    else if (gameState == State::Paused) {
                        gameState = State::Playing; 
                    }
                }
                if (keyPressed->code == sf::Keyboard::Key::P && gameState == State::Playing) {
                    gameState = State::Paused;
                    std::cout << "\nPAUSED - Press SPACE to Resume" << std::endl;
                }
                if (keyPressed->code == sf::Keyboard::Key::Escape) window.close();
            }
        }

        if (gameState == State::Playing) {
            velocity += gravity;
            bird.move({0.f, velocity});

            if (bird.getPosition().y + 15.f > 600.f) {
                gameState = State::GameOver;
                if (score > highScore) highScore = score;
                std::cout << "\n[GAME OVER] Final Score: " << score << " | High Score: " << highScore << std::endl;
                std::cout << "Press SPACE to Restart" << std::endl;
            }

            if (spawnTimer++ > 80) {
                float gapY = static_cast<float>(rand() % 250 + 100);
                Pipe p;
                p.top.setSize({50.f, gapY});
                p.top.setFillColor(sf::Color::Green);
                p.top.setPosition({400.f, 0.f});
                p.bottom.setSize({50.f, 600.f - gapY - 150.f});
                p.bottom.setFillColor(sf::Color::Green);
                p.bottom.setPosition({400.f, gapY + 150.f});
                pipes.push_back(p);
                spawnTimer = 0;
            }

            for (auto it = pipes.begin(); it != pipes.end();) {
                it->top.move({-3.5f, 0.f});
                it->bottom.move({-3.5f, 0.f});

                if (bird.getGlobalBounds().findIntersection(it->top.getGlobalBounds()) || 
                    bird.getGlobalBounds().findIntersection(it->bottom.getGlobalBounds())) {
                    gameState = State::GameOver;
                    if (score > highScore) highScore = score;
                    std::cout << "\n[CRASHED] Final Score: " << score << " | High Score: " << highScore << std::endl;
                }

                if (!it->passed && it->top.getPosition().x < bird.getPosition().x) {
                    score += 10;
                    it->passed = true;
                    // This line overwrites the previous score in the terminal
                    std::cout << "\rCurrent Score: " << score << std::flush;
                }

                if (it->top.getPosition().x < -50.f) it = pipes.erase(it);
                else ++it;
            }
        }

        window.clear(sf::Color::Cyan);
        window.draw(bird);
        for (auto& p : pipes) {
            window.draw(p.top);
            window.draw(p.bottom);
        }
        window.display();
    }
    return 0;
}