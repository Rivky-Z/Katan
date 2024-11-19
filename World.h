#pragma once
#include<vector>
using namespace std;
#include <string>
#include "ReadFromJson.h"
#include "Tile.h"

class World
{
private:
	vector<vector<Tile*>> worldMap;
	int row;
	int col;
	int rain;
	ReadFromJson data;


public:
	World(vector<vector<string>> & map);
	string selectedCtegory(int x, int y);
	string selectedResource(int x, int y);
	string selectedComplete(int x, int y, int wait);
	int selectedPeople(int x, int y);
	int selectedVehicle(int x,int y,string type);
	void updateResource(int x, int y, int cnt,string resource);
	void addPeople(int x,int y,int cnt);
	void fallRain(int milisec);
	void work(int xsource,int ysource,int xdest,int ydest);
	void build(int x, int y, string type, bool isSteps);
	int count(string categoryName);
	void makeEmpty(int x,int y);
	void manufucture(int x,int y,string type);
};

