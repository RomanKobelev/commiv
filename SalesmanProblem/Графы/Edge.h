#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>
#include "Graph.h"
#include "VertexCircle.h"
using namespace std;
using namespace sf;

Color EdgeDefaultColor = Color::Black;
Color EdgePressedColor = Color::Red;

class EdgeLine : public Drawable {
private:
	VertexCircle* Vstart;
	VertexCircle* Vend;
	vector<Vertex> Line;
	Font font;
	Text weight;
	Color color;
	vector<Vertex> triangle;
	int scale;
	bool AnotherEdge;
public:
	EdgeLine(VertexCircle*& Vstart, VertexCircle*& Vend, bool& AnotherEdge) {
		if (!font.loadFromFile("Undertale.ttf")) exit(777);
		weight.setString("0");
		weight.setCharacterSize(15);
		weight.setFont(font);
		weight.setFillColor(color);
		weight.setOutlineColor(Color(200, 211, 193));
		weight.setOutlineThickness(3);
		this->Vstart = Vstart;
		this->Vend = Vend;
		color = EdgeDefaultColor;
		weight.setOrigin(weight.getGlobalBounds().width / 2, weight.getGlobalBounds().height / 2);
		scale = 10;
		this->AnotherEdge = AnotherEdge;
		if (AnotherEdge) {
			Vector2f endpoint = { (Vstart->getPosition().x + Vend->getPosition().x) / 2 , (Vstart->getPosition().y + Vend->getPosition().y) / 2 };
			Line = { Vertex(Vstart->getPosition(), color), Vertex(endpoint, color) };
			Vector2f vect = { Vstart->getPosition().x - endpoint.x, Vstart->getPosition().y - endpoint.y };
			Vector2f ortvect = { vect.x / sqrt(pow(vect.x,2) + pow(vect.y,2)), vect.y / sqrt(pow(vect.x,2) + pow(vect.y,2)) };
			Vector2f point1 = { endpoint.x, endpoint.y };
			Vector2f perp_ort = { -ortvect.y, ortvect.x };
			Vector2f point2 = { point1.x + scale * ortvect.x + scale * perp_ort.x, point1.y + scale * ortvect.y + scale * perp_ort.y };
			Vector2f point3 = { point1.x + scale * ortvect.x - scale * perp_ort.x, point1.y + scale * ortvect.y - scale * perp_ort.y };
			triangle = { Vertex(point1, color), Vertex(point2, color), Vertex(point3, color) };
			Vector2f startpoint = { vect.x - ortvect.x * Vstart->getRadius(), vect.y - ortvect.y * Vstart->getRadius() };
			weight.setPosition(endpoint.x + ortvect.x * Vstart->getRadius(), endpoint.y + ortvect.y * Vstart->getRadius());
		}
		else {
			Line = { Vertex(Vstart->getPosition(), color), Vertex(Vend->getPosition(), color) };
			weight.setPosition((Vend->getPosition().x + Vstart->getPosition().x) / 2, (Vend->getPosition().y + Vstart->getPosition().y) / 2);
			Vector2f vect = { Vstart->getPosition().x - Vend->getPosition().x, Vstart->getPosition().y - Vend->getPosition().y };
			Vector2f ortvect = { vect.x / sqrt(pow(vect.x,2) + pow(vect.y,2)), vect.y / sqrt(pow(vect.x,2) + pow(vect.y,2)) };
			Vector2f radvect = { Vend->getPosition().x, Vend->getPosition().y };
			Vector2f point1 = { radvect.x + ortvect.x * Vend->getRadius(), radvect.y + ortvect.y * Vend->getRadius() };
			Vector2f perp_ort = { -ortvect.y, ortvect.x };
			Vector2f point2 = { point1.x + scale * ortvect.x + scale * perp_ort.x, point1.y + scale * ortvect.y + scale * perp_ort.y };
			Vector2f point3 = { point1.x + scale * ortvect.x - scale * perp_ort.x, point1.y + scale * ortvect.y - scale * perp_ort.y };
			triangle = { Vertex(point1, color), Vertex(point2, color), Vertex(point3, color) };
		}
	}
	void set_weight(const string& weight) {
		this->weight.setString(weight);
	}
	bool containVert(VertexCircle*& V) {
		return V == Vstart || V == Vend;
	}
	VertexCircle* get_start() {
		return Vstart;
	}
	VertexCircle* get_end() {
		return Vend;
	}
	void set_start(VertexCircle*& V) {
		Vstart = V;
		Line = { Vertex(Vstart->getPosition(), Color::Black), Vertex(Vend->getPosition(), Color::Black) };
		if (AnotherEdge) {
			Vector2f endpoint = { (Vstart->getPosition().x + Vend->getPosition().x) / 2 , (Vstart->getPosition().y + Vend->getPosition().y) / 2 };
			Line = { Vertex(Vstart->getPosition(), color), Vertex(endpoint, color) };
			Vector2f vect = { Vstart->getPosition().x - endpoint.x, Vstart->getPosition().y - endpoint.y };
			Vector2f ortvect = { vect.x / sqrt(pow(vect.x,2) + pow(vect.y,2)), vect.y / sqrt(pow(vect.x,2) + pow(vect.y,2)) };
			Vector2f point1 = { endpoint.x, endpoint.y };
			Vector2f perp_ort = { -ortvect.y, ortvect.x };
			Vector2f point2 = { point1.x + scale * ortvect.x + scale * perp_ort.x, point1.y + scale * ortvect.y + scale * perp_ort.y };
			Vector2f point3 = { point1.x + scale * ortvect.x - scale * perp_ort.x, point1.y + scale * ortvect.y - scale * perp_ort.y };
			triangle = { Vertex(point1, color), Vertex(point2, color), Vertex(point3, color) };
			weight.setPosition(endpoint.x + ortvect.x * Vstart->getRadius(), endpoint.y + ortvect.y * Vstart->getRadius());
		}
		else {
			Line = { Vertex(Vstart->getPosition(), color), Vertex(Vend->getPosition(), color) };
			weight.setPosition((Vend->getPosition().x + Vstart->getPosition().x) / 2, (Vend->getPosition().y + Vstart->getPosition().y) / 2);
			Vector2f vect = { Vstart->getPosition().x - Vend->getPosition().x, Vstart->getPosition().y - Vend->getPosition().y };
			Vector2f ortvect = { vect.x / sqrt(pow(vect.x,2) + pow(vect.y,2)), vect.y / sqrt(pow(vect.x,2) + pow(vect.y,2)) };
			Vector2f radvect = { Vend->getPosition().x, Vend->getPosition().y };
			Vector2f point1 = { radvect.x + ortvect.x * Vend->getRadius(), radvect.y + ortvect.y * Vend->getRadius() };
			Vector2f perp_ort = { -ortvect.y, ortvect.x };
			Vector2f point2 = { point1.x + scale * ortvect.x + scale * perp_ort.x, point1.y + scale * ortvect.y + scale * perp_ort.y };
			Vector2f point3 = { point1.x + scale * ortvect.x - scale * perp_ort.x, point1.y + scale * ortvect.y - scale * perp_ort.y };
			triangle = { Vertex(point1, color), Vertex(point2, color), Vertex(point3, color) };
		}
	}
	void set_end(VertexCircle*& V) {
		Vend = V;
		Line = { Vertex(Vstart->getPosition(), Color::Black), Vertex(Vend->getPosition(), Color::Black) };
		if (AnotherEdge) {
			Vector2f endpoint = { (Vstart->getPosition().x + Vend->getPosition().x) / 2 , (Vstart->getPosition().y + Vend->getPosition().y) / 2 };
			Line = { Vertex(Vstart->getPosition(), color), Vertex(endpoint, color) };
			Vector2f vect = { Vstart->getPosition().x - endpoint.x, Vstart->getPosition().y - endpoint.y };
			Vector2f ortvect = { vect.x / sqrt(pow(vect.x,2) + pow(vect.y,2)), vect.y / sqrt(pow(vect.x,2) + pow(vect.y,2)) };
			Vector2f point1 = { endpoint.x, endpoint.y };
			Vector2f perp_ort = { -ortvect.y, ortvect.x };
			Vector2f point2 = { point1.x + scale * ortvect.x + scale * perp_ort.x, point1.y + scale * ortvect.y + scale * perp_ort.y };
			Vector2f point3 = { point1.x + scale * ortvect.x - scale * perp_ort.x, point1.y + scale * ortvect.y - scale * perp_ort.y };
			triangle = { Vertex(point1, color), Vertex(point2, color), Vertex(point3, color) };
			weight.setPosition(endpoint.x + ortvect.x * Vstart->getRadius(), endpoint.y + ortvect.y * Vstart->getRadius());
		}
		else {
			Line = { Vertex(Vstart->getPosition(), color), Vertex(Vend->getPosition(), color) };
			weight.setPosition((Vend->getPosition().x + Vstart->getPosition().x) / 2, (Vend->getPosition().y + Vstart->getPosition().y) / 2);
			Vector2f vect = { Vstart->getPosition().x - Vend->getPosition().x, Vstart->getPosition().y - Vend->getPosition().y };
			Vector2f ortvect = { vect.x / sqrt(pow(vect.x,2) + pow(vect.y,2)), vect.y / sqrt(pow(vect.x,2) + pow(vect.y,2)) };
			Vector2f radvect = { Vend->getPosition().x, Vend->getPosition().y };
			Vector2f point1 = { radvect.x + ortvect.x * Vend->getRadius(), radvect.y + ortvect.y * Vend->getRadius() };
			Vector2f perp_ort = { -ortvect.y, ortvect.x };
			Vector2f point2 = { point1.x + scale * ortvect.x + scale * perp_ort.x, point1.y + scale * ortvect.y + scale * perp_ort.y };
			Vector2f point3 = { point1.x + scale * ortvect.x - scale * perp_ort.x, point1.y + scale * ortvect.y - scale * perp_ort.y };
			triangle = { Vertex(point1, color), Vertex(point2, color), Vertex(point3, color) };
		}
	}
	bool contain(const Vector2i& mouse) {
		const float l = 10;
		float startx = Vstart->getPosition().x;
		float starty = Vstart->getPosition().y;
		float endx;
		float endy;

		if (AnotherEdge) {
			endx = (Vstart->getPosition().x + Vend->getPosition().x) / 2;
			endy = (Vstart->getPosition().y + Vend->getPosition().y) / 2;
		}
		else {
			endx = Vend->getPosition().x;
			endy = Vend->getPosition().y;
		}

		float edge = sqrt(pow(startx - endx, 2) + pow(starty - endy, 2));
		float tostart = sqrt(pow(startx - mouse.x, 2) + pow(starty - mouse.y, 2));
		float toend = sqrt(pow(mouse.x - endx, 2) + pow(mouse.y - endy, 2));

		if (startx == endx) {
			return tostart < edge&& toend < edge&& mouse.x < startx + l && mouse.x > startx - l;
		}
		else if (starty == endy) {
			return tostart < edge&& toend < edge&& mouse.y < starty + l && mouse.y > starty - l;
		}

		float deltax = endx - startx;
		float deltay = endy - starty;

		float k = deltay / deltax;

		float xx1 = startx + sqrt(l * l / (1 + 1 / (k * k)));
		float xx2 = startx - sqrt(l * l / (1 + 1 / (k * k)));

		float yy1 = starty - (1 / k) * (xx1 - startx);
		float yy2 = starty - (1 / k) * (xx2 - startx);

		float b = starty - k * startx; 
		float deltab = abs(yy1 - k * xx1 - b);

		return mouse.y - k * mouse.x - b - deltab < 0 && mouse.y - k * mouse.x - b + deltab > 0 && tostart < edge&& toend < edge;
	}
	void setColor(const Color& color) {
		this->color = color;
		if (AnotherEdge) {
			Vector2f endpoint = { (Vstart->getPosition().x + Vend->getPosition().x) / 2 , (Vstart->getPosition().y + Vend->getPosition().y) / 2 };
			Line = { Vertex(Vstart->getPosition(), color), Vertex(endpoint, color) };
			Vector2f vect = { Vstart->getPosition().x - endpoint.x, Vstart->getPosition().y - endpoint.y };
			Vector2f ortvect = { vect.x / sqrt(pow(vect.x,2) + pow(vect.y,2)), vect.y / sqrt(pow(vect.x,2) + pow(vect.y,2)) };
			Vector2f point1 = { endpoint.x, endpoint.y };
			Vector2f perp_ort = { -ortvect.y, ortvect.x };
			Vector2f point2 = { point1.x + scale * ortvect.x + scale * perp_ort.x, point1.y + scale * ortvect.y + scale * perp_ort.y };
			Vector2f point3 = { point1.x + scale * ortvect.x - scale * perp_ort.x, point1.y + scale * ortvect.y - scale * perp_ort.y };
			triangle = { Vertex(point1, color), Vertex(point2, color), Vertex(point3, color) };
		}
		else {
			Line = { Vertex(Vstart->getPosition(), color), Vertex(Vend->getPosition(), color) };
			Vector2f vect = { Vstart->getPosition().x - Vend->getPosition().x, Vstart->getPosition().y - Vend->getPosition().y };
			Vector2f ortvect = { vect.x / sqrt(pow(vect.x,2) + pow(vect.y,2)), vect.y / sqrt(pow(vect.x,2) + pow(vect.y,2)) };
			Vector2f radvect = { Vend->getPosition().x, Vend->getPosition().y };
			Vector2f point1 = { radvect.x + ortvect.x * Vend->getRadius(), radvect.y + ortvect.y * Vend->getRadius() };
			Vector2f perp_ort = { -ortvect.y, ortvect.x };
			Vector2f point2 = { point1.x + scale * ortvect.x + scale * perp_ort.x, point1.y + scale * ortvect.y + scale * perp_ort.y };
			Vector2f point3 = { point1.x + scale * ortvect.x - scale * perp_ort.x, point1.y + scale * ortvect.y - scale * perp_ort.y };
			triangle = { Vertex(point1, color), Vertex(point2, color), Vertex(point3, color) };
		}
		weight.setFillColor(color);
	}
	Color getColor() {
		return this->color;
	}
	string get_weight() {
		return weight.getString();
	}
	void setAnotherEdge(const bool& AnotherEdge) {
		this->AnotherEdge = AnotherEdge;
		if (AnotherEdge) {
			Vector2f endpoint = { (Vstart->getPosition().x + Vend->getPosition().x) / 2 , (Vstart->getPosition().y + Vend->getPosition().y) / 2 };
			Line = { Vertex(Vstart->getPosition(), color), Vertex(endpoint, color) };
			Vector2f vect = { Vstart->getPosition().x - endpoint.x, Vstart->getPosition().y - endpoint.y };
			Vector2f ortvect = { vect.x / sqrt(pow(vect.x,2) + pow(vect.y,2)), vect.y / sqrt(pow(vect.x,2) + pow(vect.y,2)) };
			Vector2f point1 = { endpoint.x, endpoint.y };
			Vector2f perp_ort = { -ortvect.y, ortvect.x };
			Vector2f point2 = { point1.x + scale * ortvect.x + scale * perp_ort.x, point1.y + scale * ortvect.y + scale * perp_ort.y };
			Vector2f point3 = { point1.x + scale * ortvect.x - scale * perp_ort.x, point1.y + scale * ortvect.y - scale * perp_ort.y };
			triangle = { Vertex(point1, color), Vertex(point2, color), Vertex(point3, color) };
			weight.setPosition(endpoint.x + ortvect.x * Vstart->getRadius(), endpoint.y + ortvect.y * Vstart->getRadius());
		}
		else {
			Line = { Vertex(Vstart->getPosition(), color), Vertex(Vend->getPosition(), color) };
			weight.setPosition((Vend->getPosition().x + Vstart->getPosition().x) / 2, (Vend->getPosition().y + Vstart->getPosition().y) / 2);
			Vector2f vect = { Vstart->getPosition().x - Vend->getPosition().x, Vstart->getPosition().y - Vend->getPosition().y };
			Vector2f ortvect = { vect.x / sqrt(pow(vect.x,2) + pow(vect.y,2)), vect.y / sqrt(pow(vect.x,2) + pow(vect.y,2)) };
			Vector2f radvect = { Vend->getPosition().x, Vend->getPosition().y };
			Vector2f point1 = { radvect.x + ortvect.x * Vend->getRadius(), radvect.y + ortvect.y * Vend->getRadius() };
			Vector2f perp_ort = { -ortvect.y, ortvect.x };
			Vector2f point2 = { point1.x + scale * ortvect.x + scale * perp_ort.x, point1.y + scale * ortvect.y + scale * perp_ort.y };
			Vector2f point3 = { point1.x + scale * ortvect.x - scale * perp_ort.x, point1.y + scale * ortvect.y - scale * perp_ort.y };
			triangle = { Vertex(point1, color), Vertex(point2, color), Vertex(point3, color) };
		}
	}
	void draw(RenderTarget& target, RenderStates states = RenderStates::Default) const override {
		target.draw(&Line[0], Line.size(), Lines);
		if (weight.getString() != "0")
			target.draw(weight);
		target.draw(&triangle[0], triangle.size(), Triangles);
	}
};

void DrawEdges(RenderWindow& Window, const std::vector<EdgeLine*>& edges) {
	for (int i = 0; i < edges.size(); i++)
		Window.draw(*edges[i]);
}

void EdgesDefaultColor(std::vector<EdgeLine*>& edges) {
	for (int i = 0; i < edges.size(); i++) {
		edges[i]->setColor(EdgeDefaultColor);
	}
}

void CheckEdges(std::vector<EdgeLine*>& TmpEdges) {
	if (TmpEdges.size() > 0) {
		EdgesDefaultColor(TmpEdges);
		TmpEdges.clear();
	}
}