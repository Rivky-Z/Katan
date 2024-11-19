#pragma once
#include <vector>
using namespace std;
#include "ReadFromJson.h"
#include "Car.h"
#include "Helicopter.h"
#include "Truck.h"

class Tile
{
protected:

	ReadFromJson data;

public:

	int category;
	string categoryName;
	shared_ptr<map<string,int>> resource;
	string resourceName;
	int people;
	vector<Vehicle*> vehicles;
	//Coordinates
	//bool complete;
	//bool robber;

	Tile& operator=(Tile const & other);
	void updateResource(int cnt, string resource);
    virtual void addPeople(int cnt);
	void resourceGrow();
	int takeResourcesByWork();
	void addResourceByWork(int res,string resourceName);
	int getIndexOfResource(string resourceName);
};

