#pragma once
#include <SFML/Graphics.hpp>
#include <string>

#define $Font "..\\Data\\ClearSans-Bold.ttf"
#define TEXTURE_WIDTH m_texture.getSize().x
#define TEXTURE_HEIGHT m_texture.getSize().y

class InputTextBox {
	int maxText;
	int sizeText;

	sf::RectangleShape m_shape;
	sf::Font font;
	
	std::string m_data;
	sf::Color _click, _unclick, _text;

public:
	InputTextBox(int maxlen = 30, int sztext = 30);

	void setColor(sf::Color text_color, sf::Color click_color, sf::Color unclick_color);

	template<class T> void setSize(T x, T y) {
		m_shape.setSize(sf::Vector2f(x, y));
	}
	template<class T> void setPosition(T x, T y) {
		m_shape.setPosition(sf::Vector2f(x, y));
	}

	bool clicked(sf::RenderWindow* window);
	bool start(sf::RenderWindow* window);

	void draw(sf::RenderWindow* window);
	void input(sf::RenderWindow* window, sf::Event& e);
};

