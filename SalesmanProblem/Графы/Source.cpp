#include <iostream>
#include <string>
#include "Graph.h"
#include "Button.h"
#include "VertexCircle.h"
#include "Tip.h"
#include "Edge.h"
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

const Color WindowColor = Color(200, 211, 193);
void VertexSettingsWindow(VertexCircle*& Vert, const std::vector<EdgeLine*>& Edges, Graph<int>& G);
void EdgeSettingsWindow(EdgeLine*& Edge, Graph<int>& G, std::vector<EdgeLine*> Edges);
void MoveVertex(const RenderWindow& Window, VertexCircle*& movingVert, std::vector<EdgeLine*>& movingEdges, const Vector2i& mousePos, const Vector2i& prev_mousePos);

int main() {
	setlocale(LC_ALL, "Rus");
	Graph<int> G;
	RenderWindow Window(VideoMode(800, 600), "TRAVELLER");
	Window.setVerticalSyncEnabled(true);

	Button AddVertexButton("Add vertex", Window, 50);
	Button AddEdgeButton("Add edge", Window, 80);
	Button DeleteButton("Delete", Window, 110);
	Button MatrixButton("Matrix", Window, 170);
	Button BFSButton("BFS", Window, 230);
	Button DFSButton("DFS", Window, 260);
	Button DijkstraButton("Dijkstra", Window, 290);
	Button SalesmanButton("TMT", Window, 320);
	vector<VertexCircle*> Vertexes;
	vector<EdgeLine*> Edges;
	VertexCircle* movingVert = nullptr;
	vector<EdgeLine*> movingEdges;
	VertexCircle* AddedVertex = nullptr;
	EdgeLine* AddedEdge = nullptr;
	Tip BMessage(Window, "");
	vector<VertexCircle*> TmpVertexes;
	vector<EdgeLine*> TmpEdges;
	Vector2i prev_mousePos;

	bool AnyButtonWasOn = false;
	int ind = 1;
	Image icon;
	if (!icon.loadFromFile("gus.jpg")) exit(777);
	Window.setIcon(1050, 1050, icon.getPixelsPtr());

	while (Window.isOpen()) {
		Vector2i mousePos = Mouse::getPosition(Window);
		Vector2u prevsize = Window.getSize();
		Vector2i prevWpos = Window.getPosition();

		Window.clear(WindowColor);
		Window.draw(AddVertexButton);
		Window.draw(AddEdgeButton);
		Window.draw(DeleteButton);
		Window.draw(BFSButton);
		Window.draw(DFSButton);
		Window.draw(SalesmanButton);
		Window.draw(MatrixButton);
		Window.draw(DijkstraButton);
		DrawEdges(Window, Edges);
		DrawVertexes(Window, Vertexes);
		Window.draw(BMessage);
		Window.display();

		Event event;
		while (Window.pollEvent(event)) {
			switch (event.type) {
			case Event::Closed:
				Window.close();
				break;
			case Event::MouseButtonPressed:
				if (event.mouseButton.button == Mouse::Left) {
					if (AddVertexButton.contain(mousePos) && not_intersection(Vertexes, mousePos)) {
						if (AnyButtonWasOn) {
							CheckVertecies(TmpVertexes);
							CheckEdges(TmpEdges);
							SalesmanButton.setMode(0);
							DeleteButton.setMode(0);
							AddEdgeButton.setMode(0);
							MatrixButton.setMode(0);
							BFSButton.setMode(0);
							DFSButton.setMode(0);
							DijkstraButton.setMode(0);
						}
						AddVertexButton.setMode(1);
						BMessage.setText(Window, "Click and hold the LMB to move the vertex.\nClick RMB to edit a vertex or edge.");
						AddedVertex = new VertexCircle(to_string(ind));
						AddedVertex->set_position(250 + rand() % 350, 60 + rand() % 400);
						AddedVertex->setColor(VertexPressedColor);
						Vertexes.push_back(AddedVertex);
						G.addVertex(to_string(ind));
						ind++;
					}
					if (AddEdgeButton.contain(mousePos) && not_intersection(Vertexes, mousePos)) {
						if (AddEdgeButton.getMode() == 1) {
							if (Vertexes.size() > 0) BMessage.setText(Window, "Click and hold the LMB to move the vertex.\nClick RMB to edit a vertex or edge.");
							else BMessage.setText(Window, "");
							AddEdgeButton.setMode(0);
							CheckVertecies(TmpVertexes);
							CheckEdges(TmpEdges);
							AnyButtonWasOn = false;
						}
						else if (AddEdgeButton.getMode() == 0) {
							if (AnyButtonWasOn) {
								CheckVertecies(TmpVertexes);
								CheckEdges(TmpEdges);
								DeleteButton.setMode(0);
								SalesmanButton.setMode(0);
								MatrixButton.setMode(0);
								BFSButton.setMode(0);
								DFSButton.setMode(0);
								DijkstraButton.setMode(0);
							}
							BMessage.setText(Window, "Click on two vertices to creare an edge between them.");
							AddEdgeButton.setMode(1);
							AnyButtonWasOn = true;
						}
					}
					else if (AddEdgeButton.getMode() == 1) {
						bool Continue = true;
						for (int i = Vertexes.size() - 1; i >= 0 && Continue; i--) {
							if (Vertexes[i]->contain(mousePos)) {
								if (Vertexes[i]->getColor() == VertexDefaultColor) {
									if (TmpVertexes.size() < 2) {
										Vertexes[i]->setColor(VertexPressedColor);
										TmpVertexes.push_back(Vertexes[i]);
									}
								}
								else {
									Vertexes[i]->setColor(VertexDefaultColor);
									vector<VertexCircle*>::iterator it = TmpVertexes.begin();
									for (it; it != TmpVertexes.end() && *it != Vertexes[i]; it++) {}
									TmpVertexes.erase(it);
								}
								Continue = false;
							}
						}
						if (TmpVertexes.size() == 2) {
							switch (Vertexes.size()) {
							case 2:
								if (Edges.size() <= 1) {
									if (!G.isEdge(TmpVertexes[0]->getText(), TmpVertexes[1]->getText())) {
										bool AnotherEdge = G.isEdge(TmpVertexes[1]->getText(), TmpVertexes[0]->getText());
										AddedEdge = new EdgeLine(TmpVertexes[0], TmpVertexes[1], AnotherEdge);
										AddedEdge->setColor(EdgePressedColor);
										Edges.push_back(AddedEdge);
										G.addEdge(TmpVertexes[0]->getText(), TmpVertexes[1]->getText(), 0, true);
										if (AnotherEdge) {
											int i = 0;
											while (i < Edges.size() && (Edges[i]->get_start() != TmpVertexes[1] || Edges[i]->get_end() != TmpVertexes[0]))
												i++;
											Edges[i]->setAnotherEdge(AnotherEdge);
										}
									}
									else
										BMessage.setText(Window, "There is already an edge between this vertices!");
								}
								else
									BMessage.setText(Window, "Graph is already full!");
								break;
							default:
								if (Edges.size() < Vertexes.size() * (Vertexes.size() - 3) + Vertexes.size() * 2) {
									if (!G.isEdge(TmpVertexes[0]->getText(), TmpVertexes[1]->getText())) {
										bool AnotherEdge = G.isEdge(TmpVertexes[1]->getText(), TmpVertexes[0]->getText());
										AddedEdge = new EdgeLine(TmpVertexes[0], TmpVertexes[1], AnotherEdge);
										AddedEdge->setColor(EdgePressedColor);
										Edges.push_back(AddedEdge);
										G.addEdge(TmpVertexes[0]->getText(), TmpVertexes[1]->getText(), 0, true);
										if (AnotherEdge) {
											int i = 0;
											while (i < Edges.size() && (Edges[i]->get_start() != TmpVertexes[1] || Edges[i]->get_end() != TmpVertexes[0]))
												i++;
											Edges[i]->setAnotherEdge(AnotherEdge);
										}
									}
									else
										BMessage.setText(Window, "There is already an edge between this vertices!");
								}
								else
									BMessage.setText(Window, "Graph is already full!");
								break;
							}
							VertexesDefaultColor(TmpVertexes);
							if (AddedEdge != nullptr) {
								AddedEdge->setColor(EdgeDefaultColor);
								AddedEdge = nullptr;
							}
							TmpVertexes.clear();
						}
					}
					if (DeleteButton.contain(mousePos) && not_intersection(Vertexes, mousePos)) {
						if (DeleteButton.getMode() == 1) {
							if (Vertexes.size() > 0) BMessage.setText(Window, "Click and hold the LMB to move the vertex.\nClick RMB to edit a vertex or edge.");
							else BMessage.setText(Window, "");
							DeleteButton.setMode(0);
							CheckVertecies(TmpVertexes);
							CheckEdges(TmpEdges);
							AnyButtonWasOn = false;
						}
						else if (DeleteButton.getMode() == 0) {
							if (AnyButtonWasOn) {
								CheckVertecies(TmpVertexes);
								CheckEdges(TmpEdges);
								AddEdgeButton.setMode(0);
								SalesmanButton.setMode(0);
								MatrixButton.setMode(0);
								BFSButton.setMode(0);
								DFSButton.setMode(0);
								DijkstraButton.setMode(0);
							}
							BMessage.setText(Window, "Click the LMB on the vertex or edge to delete it.");
							DeleteButton.setMode(1);
							AnyButtonWasOn = true;
						}
					}
					else if (DeleteButton.getMode() == 1) {
						bool Continue = true;
						for (int i = Vertexes.size() - 1; i >= 0 && Continue; i--) {
							if (Vertexes[i]->contain(mousePos)) {
								if (Vertexes[i]->getColor() == VertexDefaultColor) {
									Vertexes[i]->setColor(VertexPressedColor);
									TmpVertexes.push_back(Vertexes[i]);
									for (int j = 0; j < Edges.size(); j++) {
										if (Edges[j]->containVert(Vertexes[i]) && Edges[j]->getColor() == EdgeDefaultColor) {
											Edges[j]->setColor(EdgePressedColor);
											TmpEdges.push_back(Edges[j]);
										}
									}
								}
								else {
									Vertexes[i]->setColor(VertexDefaultColor);
									vector<VertexCircle*>::iterator it = TmpVertexes.begin();
									for (it; it != TmpVertexes.end() && *it != Vertexes[i]; it++) {}
									TmpVertexes.erase(it);
									for (int j = 0; j < TmpEdges.size(); j++) {
										if (TmpEdges[j]->get_start() == Vertexes[i]) {
											if (TmpEdges[j]->get_end()->getColor() == VertexDefaultColor) {
												TmpEdges[j]->setColor(EdgeDefaultColor);
												vector<EdgeLine*>::iterator it = TmpEdges.begin() + j;
												TmpEdges.erase(it);
												--j;
											}
										}
										else if (TmpEdges[j]->get_end() == Vertexes[i]) {
											if (TmpEdges[j]->get_start()->getColor() == VertexDefaultColor) {
												TmpEdges[j]->setColor(EdgeDefaultColor);
												vector<EdgeLine*>::iterator it = TmpEdges.begin() + j;
												TmpEdges.erase(it);
												--j;
											}
										}
									}
								}
								Continue = false;
							}
						}

						for (int i = Edges.size() - 1; i >= 0 && Continue; i--) {
							if (Edges[i]->contain(mousePos)) {
								if (Edges[i]->getColor() == EdgeDefaultColor) {
									for (int j = 0; j < TmpVertexes.size(); j++)
										if (TmpVertexes[j] == Edges[i]->get_start() || TmpVertexes[j] == Edges[i]->get_end())
											Continue = false;
									if (Continue) {
										Edges[i]->setColor(EdgePressedColor);
										TmpEdges.push_back(Edges[i]);
									}
								}
								else {
									for (int j = 0; j < TmpVertexes.size(); j++) {
										if (!Edges[i]->containVert(TmpVertexes[j])) {
											Edges[i]->setColor(EdgeDefaultColor);
											vector<EdgeLine*>::iterator it = TmpEdges.begin();
											for (it; it != TmpEdges.end() && *it != Edges[i]; it++) {}
											TmpEdges.erase(it);
										}
									}
								}
								Continue = false;
							}
						}
						if (TmpVertexes.size() > 0 || TmpEdges.size() > 0) {
							for (int i = 0; i < TmpEdges.size(); i++) {
								vector<EdgeLine*>::iterator del;
								del = Edges.begin();
								for (del; del != Edges.end() && (*del != TmpEdges[i]); del++) {}
								G.deleteEdge(TmpEdges[i]->get_start()->getText(), TmpEdges[i]->get_end()->getText());
								if (G.isEdge(TmpEdges[i]->get_end()->getText(), TmpEdges[i]->get_start()->getText())) {
									int j = 0;
									while (j < Edges.size() && (Edges[j]->get_start() != TmpEdges[i]->get_end() || Edges[j]->get_end() != TmpEdges[i]->get_start()))
										j++;
									Edges[j]->setAnotherEdge(false);
								}
								Edges.erase(del);
								delete TmpEdges[i];
								TmpEdges[i] = nullptr;
							}
							TmpEdges.clear();
							for (int i = 0; i < TmpVertexes.size(); i++) {
								vector<VertexCircle*>::iterator del;
								del = Vertexes.begin();
								for (del; del != Vertexes.end() && (*del != TmpVertexes[i]); del++) {}
								Vertexes.erase(del);
								G.deleteVertex(TmpVertexes[i]->getText());
								delete TmpVertexes[i];
								TmpVertexes[i] = nullptr;
							}
							TmpVertexes.clear();
						}
					}
					if (MatrixButton.contain(mousePos) && not_intersection(Vertexes, mousePos)) {
						if (AnyButtonWasOn) {
							CheckVertecies(TmpVertexes);
							CheckEdges(TmpEdges);
							SalesmanButton.setMode(0);
							DeleteButton.setMode(0);
							AddEdgeButton.setMode(0);
							AddVertexButton.setMode(0);
							BFSButton.setMode(0);
							DFSButton.setMode(0);
							DijkstraButton.setMode(0);
						}
						MatrixButton.setMode(1);
						BMessage.setText(Window, "The matrix was printed on the console!");
						G.printMatrix();
					}
					if (BFSButton.contain(mousePos) && not_intersection(Vertexes, mousePos)) {
						if (BFSButton.getMode() == 1) {
							if (Vertexes.size() > 0) BMessage.setText(Window, "Click and hold the LMB to move the vertex.\nClick RMB to edit a vertex or edge.");
							else BMessage.setText(Window, "");
							BFSButton.setMode(0);
							CheckVertecies(TmpVertexes);
							CheckEdges(TmpEdges);
							AnyButtonWasOn = false;
						}
						else if (BFSButton.getMode() == 0) {
							if (AnyButtonWasOn) {
								DeleteButton.setMode(0);
								AddEdgeButton.setMode(0);
								MatrixButton.setMode(0);
								SalesmanButton.setMode(0);
								DFSButton.setMode(0);
								DijkstraButton.setMode(0);
								CheckVertecies(TmpVertexes);
								CheckEdges(TmpEdges);
							}
							BMessage.setText(Window, "Click the LMB on the vertex to see BFS algorithm.");
							BFSButton.setMode(1);
							AnyButtonWasOn = true;
						}
					}
					else if (BFSButton.getMode() == 1) {
						for (int i = 0; i < Vertexes.size(); i++) {
							if (Vertexes[i]->contain(mousePos)) {
								if (Vertexes[i]->getColor() == VertexDefaultColor) {
									if (TmpVertexes.size() == 0) {
										Vertexes[i]->setColor(VertexPressedColor);
										TmpVertexes.push_back(Vertexes[i]);
										vector<GraphVertex<int>*> vroute = G.BFS(Vertexes[i]->getText());
										if (vroute.size() == Vertexes.size()) {
											string sroute = G.getRouteForSearch(vroute);
											BMessage.setText(Window, sroute);
										}
										else
											BMessage.setText(Window, "There's no way for BFS algorithm.");
									}
								}
								else if (Vertexes[i]->getColor() == VertexPressedColor) {
									if (TmpVertexes.size() == 1) {
										Vertexes[i]->setColor(VertexDefaultColor);
										TmpVertexes.erase(vector<VertexCircle*>::iterator(TmpVertexes.begin()));
										BMessage.setText(Window, "Click the LMB to the vertex to see BFS algorithm.");
									}
								}
							}
						}
					}
					if (DFSButton.contain(mousePos) && not_intersection(Vertexes, mousePos)) {
						if (DFSButton.getMode() == 1) {
							if (Vertexes.size() > 0) BMessage.setText(Window, "Click and hold the LMB to move the vertex.\nClick RMB to edit a vertex or edge.");
							else BMessage.setText(Window, "");
							DFSButton.setMode(0);
							CheckVertecies(TmpVertexes);
							CheckEdges(TmpEdges);
							AnyButtonWasOn = false;
						}
						else if (DFSButton.getMode() == 0) {
							if (AnyButtonWasOn) {
								DeleteButton.setMode(0);
								AddEdgeButton.setMode(0);
								MatrixButton.setMode(0);
								SalesmanButton.setMode(0);
								BFSButton.setMode(0);
								DijkstraButton.setMode(0);
								CheckVertecies(TmpVertexes);
								CheckEdges(TmpEdges);
							}
							BMessage.setText(Window, "Click the LMB on the vertex to see DFS algorithm.");
							DFSButton.setMode(1);
							AnyButtonWasOn = true;
						}
					}
					else if (DFSButton.getMode() == 1) {
						for (int i = 0; i < Vertexes.size(); i++) {
							if (Vertexes[i]->contain(mousePos)) {
								if (Vertexes[i]->getColor() == VertexDefaultColor) {
									if (TmpVertexes.size() == 0) {
										Vertexes[i]->setColor(VertexPressedColor);
										TmpVertexes.push_back(Vertexes[i]);
										vector<GraphVertex<int>*> vroute = G.DFS(Vertexes[i]->getText());
										if (vroute.size() == Vertexes.size()) {
											string sroute = G.getRouteForSearch(vroute);
											BMessage.setText(Window, sroute);
										}
										else
											BMessage.setText(Window, "There's no way for DFS algorithm.");
									}
								}
								else if (Vertexes[i]->getColor() == VertexPressedColor) {
									if (TmpVertexes.size() == 1) {
										Vertexes[i]->setColor(VertexDefaultColor);
										TmpVertexes.erase(vector<VertexCircle*>::iterator(TmpVertexes.begin()));
										BMessage.setText(Window, "Click the LMB to the vertex to see DFS algorithm.");
									}
								}
							}
						}
					}
					if (DijkstraButton.contain(mousePos) && not_intersection(Vertexes, mousePos)) {
						if (DijkstraButton.getMode() == 1) {
							if (Vertexes.size() > 0) BMessage.setText(Window, "Click and hold the LMB to move the vertex.\nClick RMB to edit a vertex or edge.");
							else BMessage.setText(Window, "");
							DijkstraButton.setMode(0);
							CheckVertecies(TmpVertexes);
							CheckEdges(TmpEdges);
							AnyButtonWasOn = false;
						}
						else if (DijkstraButton.getMode() == 0) {
							if (AnyButtonWasOn) {
								DeleteButton.setMode(0);
								AddEdgeButton.setMode(0);
								MatrixButton.setMode(0);
								BFSButton.setMode(0);
								DFSButton.setMode(0);
								SalesmanButton.setMode(0);
								CheckVertecies(TmpVertexes);
								CheckEdges(TmpEdges);
							}
							BMessage.setText(Window, "Click the LMB on the vertex to see Dijkstra algorithm.");
							DijkstraButton.setMode(1);
							AnyButtonWasOn = true;
						}
					}
					else if (DijkstraButton.getMode() == 1) {
						for (int i = 0; i < Vertexes.size(); i++) {
							if (Vertexes[i]->contain(mousePos)) {
								if (Vertexes[i]->getColor() == VertexDefaultColor) {
									if (TmpVertexes.size() == 0) {
										Vertexes[i]->setColor(VertexPressedColor);
										TmpVertexes.push_back(Vertexes[i]);
										if (G.isWeighted()) {
											vector<GraphVertex<int>*> vroute = G.BFS(Vertexes[i]->getText());
											if (Vertexes.size() == vroute.size()) {
												G.Dijkstra(Vertexes[i]->getText());
												vroute = G.BFS(Vertexes[i]->getText());
												string sroute = G.getAnswerForDijkstra(vroute);
												BMessage.setText(Window, sroute);
											}
											else
												BMessage.setText(Window, "It is impossible to reach all vertexes!");
										}
										else
											BMessage.setText(Window, "Not all edges have weight!");
									}
								}
								else if (Vertexes[i]->getColor() == VertexPressedColor) {
									if (TmpVertexes.size() == 1) {
										Vertexes[i]->setColor(VertexDefaultColor);
										TmpVertexes.erase(vector<VertexCircle*>::iterator(TmpVertexes.begin()));
										BMessage.setText(Window, "Click the LMB on the vertex to see Dijkstra algorithm.");
									}
								}
							}
						}
					}
					if (SalesmanButton.contain(mousePos) && not_intersection(Vertexes, mousePos)) {
						if (SalesmanButton.getMode() == 1) {
							if (Vertexes.size() > 0) BMessage.setText(Window, "Click and hold the LMB to move the vertex.\nClick RMB to edit a vertex or edge.");
							else BMessage.setText(Window, "");
							SalesmanButton.setMode(0);
							CheckVertecies(TmpVertexes);
							CheckEdges(TmpEdges);
							AnyButtonWasOn = false;
						}
						else if (SalesmanButton.getMode() == 0) {
							if (AnyButtonWasOn) {
								DeleteButton.setMode(0);
								AddEdgeButton.setMode(0);
								MatrixButton.setMode(0);
								BFSButton.setMode(0);
								DFSButton.setMode(0);
								DijkstraButton.setMode(0);
								CheckVertecies(TmpVertexes);
								CheckEdges(TmpEdges);
							}
							BMessage.setText(Window, "Click the LMB on the vertex to see Salesman route.");
							SalesmanButton.setMode(1);
							AnyButtonWasOn = true;
						}
					}
					else if (SalesmanButton.getMode() == 1) {
						for (int i = 0; i < Vertexes.size(); i++) {
							if (Vertexes[i]->contain(mousePos)) {
								if (Vertexes[i]->getColor() == VertexDefaultColor) {
									if (TmpVertexes.size() == 0) {
										Vertexes[i]->setColor(VertexPressedColor);
										TmpVertexes.push_back(Vertexes[i]);
										if (G.isWeighted()) {
											vector<GraphVertex<int>*> vroute = G.TMT(Vertexes[i]->getText());
											if (vroute.size() == Vertexes.size() + 1) {
												string sroute = G.getRouteForTMT(vroute);
												BMessage.setText(Window, sroute);
											}
											else
												BMessage.setText(Window, "There's no way for Salesman.");
										}
										else
											BMessage.setText(Window, "Not all edges have weight.");
									}
								}
								else if (Vertexes[i]->getColor() == VertexPressedColor) {
									if (TmpVertexes.size() == 1) {
										Vertexes[i]->setColor(VertexDefaultColor);
										TmpVertexes.erase(vector<VertexCircle*>::iterator(TmpVertexes.begin()));
										BMessage.setText(Window, "Click the LMB 1 the vertex to see Salesman route.");
									}
								}
							}
						}
					}
					if (!AnyButtonWasOn) {
						bool Continue = true;
						for (int i = Vertexes.size() - 1; i >= 0 && Continue; i--) {
							if (Vertexes[i]->contain(mousePos)) {
								movingVert = Vertexes[i];
								movingVert->setColor(VertexPressedColor);
								vector<VertexCircle*>::iterator it = Vertexes.begin();
								for (it; it != Vertexes.end() && *it != movingVert; it++) {}
								Vertexes.erase(it);
								Vertexes.push_back(movingVert);
								for (int i = 0; i < Edges.size(); i++) {
									if (Edges[i]->containVert(movingVert))
										movingEdges.push_back(Edges[i]);
								}
								Continue = false;
							}
						}
					}
				}
				else if (event.mouseButton.button == Mouse::Right) {
					bool Continue = true;
					for (int i = Vertexes.size() - 1; i >= 0 && Continue; i--) {
						if (Vertexes[i]->contain(mousePos)) {
							if (AnyButtonWasOn) {
								CheckVertecies(TmpVertexes);
								CheckEdges(TmpEdges);
								SalesmanButton.setMode(0);
								AddEdgeButton.setMode(0);
								DeleteButton.setMode(0);
								BFSButton.setMode(0);
								DFSButton.setMode(0);
								DijkstraButton.setMode(0);
								AnyButtonWasOn = false;
							}
							Window.clear(WindowColor);
							DrawEdges(Window, Edges);
							BMessage.setText(Window, "Click and hold the LMB to move the vertex\nClick RMB to edit a vertex or edge");
							Window.clear(WindowColor);
							Window.draw(AddVertexButton);
							Window.draw(AddEdgeButton);
							Window.draw(DeleteButton);
							Window.draw(BFSButton);
							Window.draw(DFSButton);
							Window.draw(SalesmanButton);
							Window.draw(MatrixButton);
							Window.draw(DijkstraButton);
							Vertexes[i]->setColor(VertexPressedColor);
							TmpVertexes.push_back(Vertexes[i]);
							vector<VertexCircle*>::iterator it = Vertexes.begin() + i;
							Vertexes.erase(it);
							Vertexes.push_back(TmpVertexes[0]);
							DrawVertexes(Window, Vertexes);
							Window.display();
							VertexSettingsWindow(TmpVertexes[0], Edges, G);
							Continue = false;
							TmpVertexes[0]->setColor(VertexDefaultColor);
							TmpVertexes.clear();
						}
					}
					for (int i = 0; i < Edges.size() && Continue; i++) {
						if (Edges[i]->contain(mousePos)) {
							if (AnyButtonWasOn) {
								CheckVertecies(TmpVertexes);
								CheckEdges(TmpEdges);
								SalesmanButton.setMode(0);
								AddEdgeButton.setMode(0);
								DeleteButton.setMode(0);
								BFSButton.setMode(0);
								DFSButton.setMode(0);
								DijkstraButton.setMode(0);
								AnyButtonWasOn = false;
							}
							Edges[i]->setColor(EdgePressedColor);
							Window.clear(WindowColor);
							BMessage.setText(Window, "Click and hold the LMB to move the vertex\nClick RMB to edit a vertex or edge");
							DrawEdges(Window, Edges);
							Window.clear(WindowColor);
							Window.draw(AddVertexButton);
							Window.draw(AddEdgeButton);
							Window.draw(DeleteButton);
							Window.draw(BFSButton);
							Window.draw(DFSButton);
							Window.draw(SalesmanButton);
							Window.draw(MatrixButton);
							Window.draw(DijkstraButton);
							DrawVertexes(Window, Vertexes);
							Window.display();
							EdgeSettingsWindow(Edges[i], G, Edges);
							Continue = false;
							if (Edges[i] != nullptr) Edges[i]->setColor(EdgeDefaultColor);
							else {
								vector<EdgeLine*>::iterator del;
								del = Edges.begin() + i;
								Edges.erase(del);
							}
						}
					}
				}
				break;
			case Event::MouseMoved:
				if (movingVert != nullptr)
					MoveVertex(Window, movingVert, movingEdges, mousePos, prev_mousePos);
				break;
			case Event::MouseButtonReleased:
				if (movingVert != nullptr) {
					movingVert->setColor(VertexDefaultColor);
					movingVert = nullptr;
					movingEdges.clear();
				}
				if (AddVertexButton.getMode() == 1) {
					AddVertexButton.setMode(0);
					AddedVertex->setColor(VertexDefaultColor);
					AddedVertex = nullptr;
					AnyButtonWasOn = false;
				}
				else if (MatrixButton.getMode() == 1) {
					MatrixButton.setMode(0);
					AnyButtonWasOn = false;
				}
				break;
			case Event::Resized:
				if (Window.getSize().x < 800) {
					if (Window.getSize().y < 600)
						Window.setSize(Vector2u(800, 600));
					else
						Window.setSize(Vector2u(800, Window.getSize().y));
					Window.setPosition(prevWpos);
				}
				else if (Window.getSize().y < 600) {
					Window.setSize(Vector2u(Window.getSize().x, 600));
					Window.setPosition(prevWpos);
				}
				FloatRect visibleArea(0, 0, Window.getSize().x, Window.getSize().y);
				Window.setView(View(visibleArea));
				AddVertexButton.set_position(Window, 10);
				AddEdgeButton.set_position(Window, 50);
				DeleteButton.set_position(Window, 90);
				MatrixButton.set_position(Window, 130);
				BFSButton.set_position(Window, 170);
				DFSButton.set_position(Window, 210);
				DijkstraButton.set_position(Window, 250);
				SalesmanButton.set_position(Window, 290);
				BMessage.setText(Window, BMessage.getText());
				for (int i = 0; i < Vertexes.size(); i++) {
					Vertexes[i]->set_position((float)Vertexes[i]->getPosition().x / ((float)prevsize.x / (float)Window.getSize().x), (float)Vertexes[i]->getPosition().y / ((float)prevsize.y / (float)Window.getSize().y));
					const int limit = -2;
					const int indent = 5;
					if (Vertexes[i]->getPosition().x - Vertexes[i]->getRadius() < -limit) { //left
						Vertexes[i]->set_position(Vertexes[i]->getRadius() + indent, Vertexes[i]->getPosition().y);
						if (Vertexes[i]->getPosition().y - Vertexes[i]->getRadius() < limit)//left up
							Vertexes[i]->set_position(Vertexes[i]->getPosition().x, Vertexes[i]->getRadius() + indent);
						else if (Vertexes[i]->getPosition().y + Vertexes[i]->getRadius() > Window.getSize().y + limit)//left down
							Vertexes[i]->set_position(Vertexes[i]->getPosition().x, Window.getSize().y - Vertexes[i]->getRadius() - indent);
					}
					else if (Vertexes[i]->getPosition().y - Vertexes[i]->getRadius() < -limit) { //up
						Vertexes[i]->set_position(Vertexes[i]->getPosition().x, Vertexes[i]->getRadius() + indent);
						if (Vertexes[i]->getPosition().x + Vertexes[i]->getRadius() > Window.getSize().x + limit)//up right
							Vertexes[i]->set_position(Window.getSize().x - Vertexes[i]->getRadius() - indent, Vertexes[i]->getPosition().y);
					}
					else if (Vertexes[i]->getPosition().x + Vertexes[i]->getRadius() > Window.getSize().x + limit) { //right
						Vertexes[i]->set_position(Window.getSize().x - Vertexes[i]->getRadius() - indent, Vertexes[i]->getPosition().y);
						if (Vertexes[i]->getPosition().y + Vertexes[i]->getRadius() > Window.getSize().y + limit)//right down
							Vertexes[i]->set_position(Vertexes[i]->getPosition().x, Window.getSize().y - Vertexes[i]->getRadius() - indent);
					}
					else if (Vertexes[i]->getPosition().y + Vertexes[i]->getRadius() > Window.getSize().y + limit)//down
						Vertexes[i]->set_position(Vertexes[i]->getPosition().x, Window.getSize().y - Vertexes[i]->getRadius() - indent);
					for (int j = 0; j < Edges.size(); j++) {
						if (Vertexes[i] == Edges[j]->get_start())	Edges[j]->set_start(Vertexes[i]);
						else if (Vertexes[i] == Edges[j]->get_end()) Edges[j]->set_end(Vertexes[i]);
					}
				}
				break;
			}
		}
		prev_mousePos = mousePos;
	}
	for (int i = 0; i < Vertexes.size(); i++)
		delete Vertexes[i];
	for (int i = 0; i < Edges.size(); i++)
		delete Edges[i];
	Vertexes.clear();
	Edges.clear();
	return 0;
}

void VertexSettingsWindow(VertexCircle*& Vert, const std::vector<EdgeLine*>& Edges, Graph<int>& G) {
	string old_name = Vert->getText();
	RenderWindow VertexSettings(VideoMode(500, 100), "VertexSettings", Style::Close | Style::Titlebar);
	Text Tip;
	Font font;
	if (!font.loadFromFile("Undertale.ttf")) exit(777);
	Tip.setFont(font);
	Tip.setFillColor(Color::Black);
	Tip.setCharacterSize(20);
	Tip.setPosition(2, 0);
	string new_name = old_name;
	Tip.setString("New name:\n" + new_name);

	Text message1;
	message1.setFont(font);
	message1.setCharacterSize(20);
	message1.setPosition(2, 25);
	message1.setFillColor(Color::Black);
	message1.setString("");
	while (VertexSettings.isOpen()) {
		VertexSettings.clear(Color::White);
		VertexSettings.draw(Tip);
		if (message1.getString() != "") {
			VertexSettings.draw(message1);
		}
		VertexSettings.display();
		Event ev;
		while (VertexSettings.pollEvent(ev)) {
			switch (ev.type) {
			case Event::Closed:
				VertexSettings.close();
				break;
			case Event::KeyPressed:
				if (message1.getString() == "") {
					if (ev.key.code >= Keyboard::A && ev.key.code <= Keyboard::Z) {
						new_name += (unsigned char)(ev.key.code + 65);
						Tip.setString("New name:\n" + new_name);
					}
					else if (ev.key.code >= Keyboard::Num0 && ev.key.code <= Keyboard::Num9) {
						new_name += (unsigned char)(ev.key.code + 22);
						Tip.setString("New name:\n" + new_name);
					}
					else if (ev.key.code >= Keyboard::Numpad0 && ev.key.code <= Keyboard::Numpad9) {
						new_name += (unsigned char)(ev.key.code - 27);
						Tip.setString("New name:\n" + new_name);
					}
					else if (ev.key.code == Keyboard::BackSpace) {
						if (new_name.size() != 0) new_name.erase(new_name.size() - 1);
						Tip.setString("New name:\n" + new_name);
					}
					else if (ev.key.code == Keyboard::Enter) {
						if (new_name != "" && new_name != old_name) {
							if (!G.isVertex(new_name)) {
								Vert->setText(new_name);
								for (int i = 0; i < Edges.size(); i++) {
									if (Edges[i]->get_start() == Vert)
										Edges[i]->set_start(Vert);
									else if (Edges[i]->get_end() == Vert)
										Edges[i]->set_end(Vert);
								}
								G.setVertexName(old_name, new_name);
								Tip.setString("Changes saved.");
								Tip.setFillColor(Color::Green);
								message1.setString("New name: " + new_name);
							}
							else {
								Tip.setFillColor(Color::Red);
								Tip.setString("The changes were not saved.");
								message1.setString("There is already a vertex with this name!");
							}
						}
						else if (new_name == "") {
							Tip.setFillColor(Color::Red);
							Tip.setString("The changes were not saved.");
							message1.setString("You haven't entered anything!");
						}
						else if (new_name == old_name) {
							message1.setString("You entered the same name!");
							Tip.setFillColor(Color::Red);
							Tip.setString("The changes were not saved.");

						}
					}
					else if (ev.key.code == Keyboard::Escape) {
						VertexSettings.close();
					}
				}
				else {
					VertexSettings.close();
				}
			}
		}
	}
}

void EdgeSettingsWindow(EdgeLine*& Edge, Graph<int>& G, std::vector<EdgeLine*> Edges) {
	string old_weight = Edge->get_weight();
	RenderWindow EdgeSettings(VideoMode(500, 100), "EdgeSettings", Style::Close | Style::Titlebar);
	Text Tip; Font font;
	if (!font.loadFromFile("Undertale.ttf")) exit(1);
	Tip.setFont(font);
	Tip.setFillColor(Color::Black);
	Tip.setCharacterSize(20);
	Tip.setPosition(2, 0);
	string new_weight;
	if (old_weight == "0") {
		new_weight = "";
		old_weight = "";
	}
	else
		new_weight = old_weight;
	Tip.setString("Input new edge weight:\n" + new_weight);
	Text message1;
	message1.setFont(font);
	message1.setCharacterSize(20);
	message1.setPosition(2, 25);
	message1.setFillColor(Color::Black);
	message1.setString("");

	while (EdgeSettings.isOpen()) {
		EdgeSettings.clear(Color::White);
		EdgeSettings.draw(Tip);
		if (message1.getString() != "")
			EdgeSettings.draw(message1);
		EdgeSettings.display();
		Event ev;
		while (EdgeSettings.pollEvent(ev)) {
			switch (ev.type) {
			case Event::Closed:
				EdgeSettings.close();
				break;
			case Event::KeyPressed:
				if (message1.getString() == "") {
					if (ev.key.code >= Keyboard::Num0 && ev.key.code <= Keyboard::Num9) {
						new_weight += (unsigned char)(ev.key.code + 22);
						Tip.setString("Input new edge weight:\n" + new_weight);
					}
					else if (ev.key.code >= Keyboard::Numpad0 && ev.key.code <= Keyboard::Numpad9) {
						new_weight += (unsigned char)(ev.key.code - 27);
						Tip.setString("Input new edge weight:\n" + new_weight);
					}
					else if (ev.key.code == Keyboard::BackSpace) {
						if (new_weight.size() != 0) new_weight.erase(new_weight.size() - 1);
						Tip.setString("Input new edge weight:\n" + new_weight);
					}
					else if (ev.key.code == Keyboard::Enter) {
						if (new_weight != "" && new_weight != old_weight && new_weight != "0") {
							Edge->set_weight(std::to_string(stoi(new_weight)));
							G.setEdgeWeight(Edge->get_start()->getText(), Edge->get_end()->getText(), stoi(new_weight), true);
							Tip.setFillColor(Color::Green);
							Tip.setString("Changes were saved");
							message1.setString("New edge weight = " + new_weight);
						}
						else {
							if (new_weight == "0")
								message1.setString("Edge weight must be greater than 0!");
							else if (new_weight == old_weight)
								message1.setString("You entered the same value!");
							else if (new_weight == "")
								message1.setString("You haven't entered anything!");
							Tip.setString("Changes are not saved.");
							Tip.setFillColor(Color::Red);
						}
					}
					else if (ev.key.code == Keyboard::Escape)
						EdgeSettings.close();
				}
				else
					EdgeSettings.close();
			}
		}
	}
}

void MoveVertex(const RenderWindow& Window, VertexCircle*& movingVert, std::vector<EdgeLine*>& movingEdges, const Vector2i& mousePos, const Vector2i& prev_mousePos) {
	const int limit = -2;
	const int indent = 5;
	if (movingVert->getPosition().x - movingVert->getRadius() < -limit) { //left
		movingVert->setColor(VertexDefaultColor);
		movingVert->set_position(movingVert->getRadius() + indent, movingVert->getPosition().y);
		if (movingVert->getPosition().y - movingVert->getRadius() < limit) //left up
			movingVert->set_position(movingVert->getPosition().x, movingVert->getRadius() + indent);
		else if (movingVert->getPosition().y + movingVert->getRadius() > Window.getSize().y + limit) //left down
			movingVert->set_position(movingVert->getPosition().x, Window.getSize().y - movingVert->getRadius() - indent);
		movingVert = nullptr;
		movingEdges.clear();
	}
	else if (movingVert->getPosition().y - movingVert->getRadius() < -limit) { //up
		movingVert->setColor(VertexDefaultColor);
		movingVert->set_position(movingVert->getPosition().x, movingVert->getRadius() + indent);
		if (movingVert->getPosition().x + movingVert->getRadius() > Window.getSize().x + limit)//up right
			movingVert->set_position(Window.getSize().x - movingVert->getRadius() - indent, movingVert->getPosition().y);
		movingVert = nullptr;
		movingEdges.clear();
	}
	else if (movingVert->getPosition().x + movingVert->getRadius() > Window.getSize().x + limit) { //right
		movingVert->setColor(VertexDefaultColor);
		movingVert->set_position(Window.getSize().x - movingVert->getRadius() - indent, movingVert->getPosition().y);
		if (movingVert->getPosition().y + movingVert->getRadius() > Window.getSize().y + limit)//right down
			movingVert->set_position(movingVert->getPosition().x, Window.getSize().y - movingVert->getRadius() - indent);
		movingVert = nullptr;
		movingEdges.clear();
	}
	else if (movingVert->getPosition().y + movingVert->getRadius() > Window.getSize().y + limit) { //down
		movingVert->setColor(VertexDefaultColor);
		movingVert->set_position(movingVert->getPosition().x, Window.getSize().y - movingVert->getRadius() - indent);
		movingVert = nullptr;
		movingEdges.clear();
	}
	else {
		movingVert->set_position(movingVert->getPosition().x + (mousePos.x - prev_mousePos.x), (movingVert->getPosition().y + (mousePos.y - prev_mousePos.y)));
		for (int i = 0; i < movingEdges.size(); i++) {
			if (movingVert == movingEdges[i]->get_start())	movingEdges[i]->set_start(movingVert);
			else if (movingVert == movingEdges[i]->get_end()) movingEdges[i]->set_end(movingVert);
		}
	}
	for (int i = 0; i < movingEdges.size(); i++) {
		if (movingVert == movingEdges[i]->get_start())	movingEdges[i]->set_start(movingVert);
		else if (movingVert == movingEdges[i]->get_end()) movingEdges[i]->set_end(movingVert);
	}
}