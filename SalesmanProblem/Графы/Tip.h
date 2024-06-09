#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>
#include "Graph.h"
#include "VertexCircle.h"
using namespace sf;

class Tip : public Drawable {
private:
	Text text;
	Font font;
public:
	Tip(const RenderWindow& Window, const string& msg) {
		if (!font.loadFromFile("Undertale.ttf")) exit(777);
		text.setFont(font);
		text.setFillColor(Color::Black);
		text.setCharacterSize(16);
		text.setString(msg);
		FloatRect rect = text.getGlobalBounds();
		text.setOrigin(rect.width / 2, rect.height / 2);
		text.setPosition(Window.getSize().x / 2, Window.getSize().y - 50);
	}
	Tip() {};
	void setText(const RenderWindow& Window, const string& msg) {
		text.setString(msg);
		FloatRect rect = text.getGlobalBounds();
		text.setOrigin(rect.width / 2, rect.height / 2);
		text.setPosition(Window.getSize().x / 2, Window.getSize().y - 50);
	}
	string getText(){
		return text.getString();
	}
	void draw(RenderTarget& target, RenderStates states = RenderStates::Default) const override {
		if (text.getString() != "")
			target.draw(text);
	}
};