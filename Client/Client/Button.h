#pragma once
#include <SFML/Graphics.hpp>
#include <string>

#define TEXTURE_WIDTH m_texture.getSize().x
#define TEXTURE_HEIGHT m_texture.getSize().y

class Button {
	// <- this is private (by default)
	sf::Texture m_texture;
	sf::RectangleShape m_shape;

public:
	Button();
	sf::Vector2f getPosition();
	void setTexture(const std::string& _texture);
	void setTexture(const char* _texture);
	template<class T> void setSize(T X, T Y) {
		m_shape.setSize(sf::Vector2f(X, Y));
		m_shape.setTextureRect(sf::IntRect(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT));
	}
	template<class T> void setPosition(T X, T Y) {
		m_shape.setPosition(sf::Vector2f(X, Y));
	}

	bool clicked(sf::RenderWindow* window);
	void draw(sf::RenderWindow* window);
};

