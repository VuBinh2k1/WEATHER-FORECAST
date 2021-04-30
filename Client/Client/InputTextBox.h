#pragma once
#include <SFML/Graphics.hpp>
#include <string>

#define $Font "..\\Data\\ClearSans-Bold.ttf"
#define TEXTURE_WIDTH m_texture.getSize().x
#define TEXTURE_HEIGHT m_texture.getSize().y

class InputTextBox {
	int maxText;
	int sizeText;
	bool hideText;

	sf::RectangleShape m_shape;
	sf::Font font;
	
	std::string m_data;
	sf::Color _click, _unclick, _text;

public:
	void Init(int maxlen = 30, int sztext = 30, bool hidetxt = 0);

	std::string getText();
	bool size();
	void clear();

	void setColor(sf::Color text_color, sf::Color click_color, sf::Color unclick_color);
	void setText(std::string txt);

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

