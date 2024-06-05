#include <SFML/Graphics.hpp>
#include <iostream>
#include "include/solver.hpp"

int main()
{
    int max_objects = 100;

    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Verlet Integration");
    window.setFramerateLimit(60);

    Solver solver;
    sf::Clock clock;

    solver.addObject();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        if (clock.getElapsedTime().asSeconds() > 0.03 && solver.numObjects() < max_objects) {
            clock.restart();
            solver.addObject();
        }

        window.clear(sf::Color::White);
        solver.renderObjects(&window, 1 / 60.0);
        window.display();
    }

    return 0;
}