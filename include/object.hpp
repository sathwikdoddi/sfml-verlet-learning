#include <SFML/Graphics.hpp>
#include <cstdlib>

struct VerletObject {
    sf::CircleShape shape;
    sf::Vector2f position;
    sf::Vector2f position_last;
    sf::Vector2f acceleration;
    sf::Vector2f velocity;
    sf::Clock time_alive;

    float checkableCollisions;

    VerletObject(float radius, sf::Vector2f v) {
        acceleration = sf::Vector2f(0, 0);

        shape.setFillColor(sf::Color::White);
        shape.setRadius(radius);
        
        sf::Vector2f initPosition(500, 250);
        shape.setOrigin(shape.getRadius(), shape.getRadius());
        shape.setPosition(initPosition);
        shape.setOutlineThickness(1);
        shape.setOutlineColor(sf::Color::Black);

        velocity = v;
        position = initPosition + velocity;
        position_last = initPosition;

        sf::Clock time;
        time_alive = time;
        checkableCollisions = 0.1;
    }

    void update(float dt) {
        velocity = position - position_last;
        position_last = position;
        position = position + velocity + acceleration * dt * dt;

        acceleration = sf::Vector2f();

        shape.setPosition(position);
    }
    
    void accelerate(sf::Vector2f a) {
        acceleration += a;
    }

    void setPosition(sf::Vector2f pos) {
        position = pos;
    }

    sf::Vector2f getPos() {
        return position;
    }

    sf::CircleShape getShape() {
        return shape;
    }

    float getRadius() {
        return shape.getRadius();
    }

    bool appropriateTimeAlive() {
        return time_alive.getElapsedTime().asSeconds() > checkableCollisions;
    }
};