#include <SFML/Graphics.hpp>
#include "vector2f.cpp"
#include <iostream>
#include <vector>

using namespace std;

const float G = 3;
const int PLANETS_NUMBER = 5;

class Planet {
public:
	Planet(float radius, sf::Color c, vector2f init_pos) : shape(radius), cur_pos(vector2f(0.f, 0.f)) {
		shape.setFillColor(c);
		Move(init_pos);
		mass = 314 * radius * radius;
		velocity = vector2f(rand() * 80.0 / RAND_MAX - 40.0, rand() * 80.0 / RAND_MAX - 40.0);
	}

	void Move(vector2f pos) {
		cur_pos += pos;
		shape.setPosition(sf::Vector2f(cur_pos.x, cur_pos.y));
	}

	void Interact(Planet &p, float dt) {
		vector2f R = p.GetPos() - cur_pos;
		vector2f a = R.Normalize() * G * (p.GetMass() / (R.Length() * R.Length()));
		Move(velocity * dt + a * dt * dt * 0.5);
		velocity += a * dt;
	}

	float GetMass() {
		return mass;
	}

	vector2f GetPos() {
		return cur_pos;
	}

	sf::Shape* GetShapeObject() {
		return &shape;
	}
private:
	float mass;
	sf::CircleShape shape;
	vector2f cur_pos, velocity;
};

int main() {
	srand(time(0));
	sf::RenderWindow window(sf::VideoMode(1000, 600), "Planets");
	sf::Clock timer;

	vector<Planet*> planets;

	for (int i = 0; i < PLANETS_NUMBER; i++) {
		planets.push_back(new Planet(rand() % 20, sf::Color::Red, vector2f(rand() % 500 + 200, rand() % 400 + 100)));
	}

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		float dt = timer.getElapsedTime().asSeconds();
		timer.restart();

		for (int i = 0; i < PLANETS_NUMBER; i++) {
			for (int j = 0; j < PLANETS_NUMBER; j++) {
				if (i != j) {
					planets[i]->Interact(*planets[j], dt);
				}
			}
		}

		for (int i = 0; i < PLANETS_NUMBER; i++) {
			window.draw(*(planets[i]->GetShapeObject()));
		}
		window.display();
	}

	return 0;
}
