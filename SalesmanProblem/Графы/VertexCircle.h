#pragma once
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <string>
using namespace sf;
using namespace std;

const double PI = 3.14;
const Color VertexDefaultColor = Color(128, 0, 128);
const Color VertexPressedColor = Color(128, 255, 128);

class VertexCircle : public Drawable {
public:
	CircleShape Crcl;
	Text text;
	Font font;
public:
	VertexCircle(const string& name) {
		bool f = font.loadFromFile("Undertale.ttf");
		if (!f) exit(777);
		Crcl.setRadius(30);
		Crcl.setFillColor(VertexDefaultColor);
		Crcl.setOutlineColor(Color::Black);
		Crcl.setOutlineThickness(3);
		Crcl.setOrigin(Crcl.getRadius(), Crcl.getRadius());
		Crcl.setPosition(100, 100);
		text.setFont(font);
		text.setCharacterSize(Crcl.getRadius() / 2);
		text.setString(name);
		text.setFillColor(Color::Black);
		text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
		text.setPosition(Crcl.getPosition().x, Crcl.getPosition().y - text.getCharacterSize() / 4);
	}
	void setColor(const Color& color) {
		Crcl.setFillColor(color);
	}
	void set_position(const float& x, const float& y) {
		Crcl.setPosition(x, y);
		text.setPosition(Crcl.getPosition().x, Crcl.getPosition().y - text.getCharacterSize() / 4);
	}
	bool contain(const Vector2i& mouse) {
		return pow((mouse.x - Crcl.getPosition().x), 2) + pow((mouse.y - Crcl.getPosition().y), 2) < pow(Crcl.getRadius(), 2);
	}
	string getText() {
		return text.getString();
	}
	void setText(const string& s) {
		text.setString(s);
		FloatRect textRect = text.getLocalBounds();
		if (textRect.width / 2 > Crcl.getRadius()) {
			Crcl.setRadius(textRect.width / 2);
		}
		else if (Crcl.getRadius() > 50) {
			Crcl.setRadius(50);
		}
		Crcl.setOrigin(Crcl.getRadius(), Crcl.getRadius());
		text.setOrigin(textRect.width / 2, textRect.height / 2);
		text.setPosition(Crcl.getPosition().x, Crcl.getPosition().y - text.getCharacterSize() / 4);
	}
	Vector2f getPosition() {
		return Crcl.getPosition();
	}
	Color getColor() {
		return Crcl.getFillColor();
	}
	int getRadius() {
		return Crcl.getRadius();
	}
	void draw(RenderTarget& target, RenderStates states = RenderStates::Default) const override {
		target.draw(Crcl);
		target.draw(text);
	}
};

void DrawVertexes(RenderWindow& Window, const vector<VertexCircle*>& vertexes) {
	for (int i = 0; i < vertexes.size(); i++)
		Window.draw(*vertexes[i]);
}

bool not_intersection(const vector<VertexCircle*>& v, const Vector2i& mouse) {
	for (int i = 0; i < v.size(); i++) {
		if (v[i]->contain(mouse)) return false;
	}
	return true;
}

void VertexesDefaultColor(vector<VertexCircle*>& verts) {
	for (int i = 0; i < verts.size(); i++) {
		verts[i]->setColor(VertexDefaultColor);
	}
}

void CheckVertecies(vector<VertexCircle*>& TmpVertexes) {
	if (TmpVertexes.size() > 0) {
		VertexesDefaultColor(TmpVertexes);
		TmpVertexes.clear();
	}
}