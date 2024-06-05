#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include "object.hpp"

class Solver {
    private:
        std::vector<VerletObject> m_objects;
        sf::Vector2f acceleration;
        sf::CircleShape constraint;
        sf::Clock runtime;
        float PI;
        float max_angle;
        float shooting_offset;
        float substeps;

        void updatePositions(float dt) {
            for (int i = 0; i < m_objects.size(); ++i) {
                m_objects[i].update(dt);
                m_objects[i].accelerate(acceleration);
            }
        }

        void applyConstraint() {
            sf::Vector2f position = constraint.getPosition();
            float radius = constraint.getRadius();

            for (int i = 0; i < m_objects.size(); ++i) {
                VerletObject& object = m_objects[i];

                sf::Vector2f to_object = object.getPos() - position;
                float distance = pow(pow(to_object.x, 2) + pow(to_object.y, 2), 0.5);
                if (distance > (radius - object.getRadius())) {
                    sf::Vector2f n = to_object / distance;
                    object.setPosition(position + n * (radius - object.getRadius()));
                }
            }
        }

        void solveCollisions() {
            for (int i = 0; i < m_objects.size(); i++) {
                for (int j = i + 1; j < m_objects.size(); j++) {
                    VerletObject& object1 = m_objects[i];
                    VerletObject& object2 = m_objects[j];

                    sf::Vector2f collision_axis = object1.getPos() - object2.getPos();
                    float minDist = object1.getRadius() + object2.getRadius();
                    float distance = pow(pow(collision_axis.x, 2) + pow(collision_axis.y, 2), 0.5);

                    if (distance < minDist && object1.appropriateTimeAlive() && object2.appropriateTimeAlive()) {
                        sf::Vector2f n = collision_axis / distance;
                        float delta = minDist - distance;
                        object1.setPosition(object1.getPos() + 0.5f*delta*n);
                        object2.setPosition(object2.getPos() - 0.5f*delta*n);
                    }
                }
            }
        }
    public:
        Solver() {
            srand(time(NULL));

            constraint.setPosition(sf::Vector2f(500, 500));
            constraint.setPointCount(100);
            constraint.setRadius(300);
            constraint.setOrigin(constraint.getRadius(), constraint.getRadius());
            constraint.setFillColor(sf::Color::Black);

            acceleration = sf::Vector2f(0, 1000);
            substeps = 8;

            PI = M_PI;
            max_angle = 3 * PI / 4;
            shooting_offset = - PI / 4.5;
        }
        void addObject() {
            float t = runtime.getElapsedTime().asSeconds();
            float speed = 4.0f;

            if (t > max_angle) {
                runtime.restart();
            } else if (t > max_angle / 2) {
                t = max_angle - t;
            }

            VerletObject object(rand() % 13 + 3, speed * sf::Vector2f(cos(t - shooting_offset), sin(t - shooting_offset)));
            m_objects.push_back(object);
        }

        void renderObjects(sf::RenderWindow* target, float dt) {
            target->draw(constraint);

            float sub_dt = dt / substeps;
            for (int i = 0; i < substeps; i++) {
                applyConstraint();
                solveCollisions();
                updatePositions(sub_dt);
            }
            
            for (int i = 0; i < m_objects.size(); ++i) {
                target->draw(m_objects[i].getShape());
            }
        }

        int numObjects() {
            return m_objects.size();
        }
};