#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Graph.h"
using namespace sf;
using namespace std;

const Color OffBtnColor = Color(127, 255, 212);
const Color OnBtnColor = Color(127, 0, 212);

class Button : public Drawable {
private:
	RectangleShape Btn;
	Text text;
	Font font;
	bool mode;
public:
	Button(const string& txt, const RenderWindow& Window, const int& correction) {
		bool f = font.loadFromFile("Undertale.ttf");
		if (!f) exit(777);
		text.setCharacterSize(16);
		text.setFont(font);
		text.setString(txt);
		text.setFillColor(Color::Black);
		FloatRect textRect = text.getLocalBounds();
		text.setOrigin(textRect.width / 2, textRect.height / 2);

		Btn.setFillColor(OffBtnColor);
		Btn.setOutlineColor(Color::Black);
		Btn.setOutlineThickness(1);
		Btn.setSize(Vector2f(150, 30));
		FloatRect btnRect = Btn.getLocalBounds();
		Btn.setOrigin(btnRect.width, 0);
		Btn.setPosition(Window.getSize().x - Window.getSize().x + 160, Window.getSize().y - Window.getSize().y + correction);

		text.setPosition(Btn.getPosition().x - btnRect.width / 2, Btn.getPosition().y + btnRect.height / 4);
		mode = false;
	}
	void set_position(const RenderWindow& Window, const int& correction) {
		FloatRect buttonRect = Btn.getLocalBounds();
		Btn.setOrigin(buttonRect.width, 0);
		Btn.setPosition(Window.getSize().x - Window.getSize().x + 160, Window.getSize().y - Window.getSize().y + correction);
		text.setPosition(Btn.getPosition().x - buttonRect.width / 2, Btn.getPosition().y + buttonRect.height / 4);
	}
	bool contain(const Vector2i& mouse) {
		return Btn.getGlobalBounds().contains(mouse.x, mouse.y);
	}
	void set_color(Color color) {
		Btn.setFillColor(color);
	}
	void setMode(const bool& Mode) {
		this->mode = Mode;
		if (Mode == true) Btn.setFillColor(OnBtnColor);
		else if (Mode == false) Btn.setFillColor(OffBtnColor);
	}
	bool getMode() {
		return mode;
	}
	void draw(RenderTarget& target, RenderStates states = RenderStates::Default) const override {
		target.draw(Btn);
		target.draw(text);
	}
};