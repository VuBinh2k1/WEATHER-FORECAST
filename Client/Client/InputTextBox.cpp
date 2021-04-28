#include "stdafx.h"
#include "InputTextBox.h"

InputTextBox::InputTextBox(int maxlen, int sztext) {
	maxText = maxlen;
	sizeText = sztext;

	font.loadFromFile($Font);
	_text = sf::Color::Black;
	_click = sf::Color::White;
	_unclick = sf::Color(128, 128, 128);
	m_shape.setFillColor(_unclick);
}

void InputTextBox::setColor(sf::Color text_color, sf::Color click_color, sf::Color unclick_color) {
	_text = text_color;
	_click = click_color;
	_unclick = unclick_color;
}

bool InputTextBox::clicked(sf::RenderWindow* window) {
	int coorMouse_x = sf::Mouse::getPosition(*window).x;
	int coorMouse_y = sf::Mouse::getPosition(*window).y;
	return (m_shape.getPosition().x <= coorMouse_x && coorMouse_x <= m_shape.getPosition().x + m_shape.getLocalBounds().width
		&& m_shape.getPosition().y <= coorMouse_y && coorMouse_y <= m_shape.getPosition().y + m_shape.getLocalBounds().height);
}

bool InputTextBox::start(sf::RenderWindow* window) {
	if (clicked(window)) {
		m_shape.setFillColor(_click);
		return 1;
	}
	else {
		if (m_data.empty())
			m_shape.setFillColor(_unclick);
		return 0;
	}
}

void InputTextBox::draw(sf::RenderWindow* window) {
	window->draw(m_shape);
}

void InputTextBox::input(sf::RenderWindow* window, sf::Event& e) {
	if (e.text.unicode >= 128 && m_data.size() > maxText) return;
	m_data += static_cast<char>(e.text.unicode);

	sf::Texture tex;
	tex.create(1366, 768);
	tex.update(*window);
	sf::Sprite background(tex);

	window->draw(background);
	window->draw(m_shape);

	sf::Text text;
	text.setFont(font);
	text.setString(m_data);
	text.setCharacterSize(sizeText);
	text.setFillColor(_text);

	sf::Vector2f pos = m_shape.getPosition();
	int TEXT_Y = pos.y - (m_shape.getSize().y - text.getCharacterSize()) / 2;
	text.setPosition(sf::Vector2f(pos.x - 10, TEXT_Y));

	window->draw(text);
	window->display();
}
