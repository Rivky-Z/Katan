#include "World.h"
#include <string>
#include <iostream>
#include "Tile.h"
#include "City.h"
#include "Village.h"
#include "Road.h"
#include "ReadFromJson.h"
using namespace std;


World::World(vector<vector<string>> & grid) {
	
	//data = ReadFromJson::getJson();
	int x = data.config["Sizes"]["Tile"][0];
	int y = data.config["Sizes"]["Tile"][1];
	row = grid.size() * x;
	col = grid[0].size() * y;
	rain = 0;
	worldMap.resize(row + 1, vector<Tile*>(col + 1));
	string categoryName = "";

	for (int i = 0; i < grid.size(); i++) {
		for (int j = 0; j < grid[0].size(); j++) {
			worldMap[i * x][j * y] = new Tile();

			//init category
			worldMap[i * x][j * y]->category = stoi(grid[i][j]);

			//init categoryName
			for (auto item : data.config["Tiles"].items()) {
					if (item.value() == stoi(grid[i][j])) {
						categoryName = item.key();
					}
			} 
			if (categoryName == "") {
				throw invalid_argument("no category match to number");
			}
			worldMap[i*x][j*y]->categoryName = categoryName;

			//init resourceName
			for (auto item : data.config["ResourceInCategory"].items()) {
				if (item.key() == categoryName) {
					worldMap[i*x][j*y]->resourceName = item.value();
				}
			}

			//resize and init zero map source
			worldMap[i * x][j * y]->resource = make_shared<map<string, int>>();
			int size = data.config["ResourceTypes"].size()-1;
			for (int res = 0; res < size; res++) {
				string s = data.config["ResourceTypes"][res];
				worldMap[i * x][j * y]->resource->emplace(s, 0); 
			}
			
			//init StartingResource
			for (auto it : data.config["StartingResources"].items()){
				if (it.key() == worldMap[i * x][j * y]->categoryName) {
					string s = worldMap[i * x][j * y]->resourceName;
					(* (worldMap[i * x][j * y]->resource))[s] = it.value();
				}
			}

			//init people
			worldMap[i * x][j * y]->people = 0;

		}
	}

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (i % x != 0 || j % y != 0) {
				
				worldMap[i][j] = worldMap[i - (i % x)][j - (j % y)];
			}
		}
	}

	////for (int i = 0; i < row; i++) {
	////	for (int j = 0; j < col; j++) {
	////		/*for(auto i : *(worldMap[i][j].resource))
	////			cout<<i.first<<" : "<<i.second<<" ";
	////		cout << "    ";*/
	////		cout << worldMap[i][j].resource << " ";
	////	}
	////	cout << endl;
	////}
}

string World::selectedCtegory(int x, int y) {
	x--;
	y--;
	if (y > row || x > col || x < 0 || y < 0) {
		return "error";
         }
	return worldMap[y][x]->categoryName;
}

string World::selectedResource(int x, int y) {
	x--;
	y--;
	if (y > row || x > col || x < 0 || y < 0) {
		return "error";
	}

	string res = "";

	map<int,int> vals;
	for (auto i : (* (worldMap[y][x]->resource))) {
		vals[worldMap[y][x]->getIndexOfResource(i.first)] = i.second;
	}
	for(auto i: vals)
	     res = res + to_string(i.second) + " ";

	return res;
}

void World::updateResource(int x,int y ,int cnt,string resource) {
	//TODO int capacity = data->config["Capacities"]
	y--;
	x--;
	worldMap[y][x]->updateResource(cnt, resource);
}

void World::addPeople(int x, int y, int cnt) {
	y--;
	x--;
	worldMap[y][x]->addPeople(cnt);
}

void World::fallRain(int milisec) {
	//TODO - rain left
	//milisec += rain;

	int fromDown = 1;
	int x = data.config["Sizes"]["Tile"][0];
	int y = data.config["Sizes"]["Tile"][1];

	vector<int> times;
	for (auto i : data.config["Rains"].items()) {
		times.push_back(i.value());
	}

	vector<string> resources;
	for (auto i : data.config["Rains"].items()) {
		resources.push_back(i.key());
	}

	while (fromDown <= milisec) {
	    for (int i = 0; i < row/x; i++) {
		    for (int j = 0; j < col/y; j++) {
				for (int t = 0;t<times.size();t++) {
					if (fromDown % times[t] == 0 && worldMap[i*x][j*y]->resourceName == resources[t]) {
						worldMap[i*x][j*y]->resourceGrow();
					}
				}
			}
		}
		fromDown++;
	}
}

void World::work(int xsource, int ysource, int xdest, int ydest) {
	ydest--; xdest--; ysource--; xsource--;
	if (worldMap[ydest][xdest]->people <= 0) {
		throw invalid_argument("no people to work");
	}
	int speed = data.config["Speeds"]["People"];
	speed *= 50000;
	while (speed-- >= 0);
	int res = worldMap[ysource][xsource]->takeResourcesByWork();
	speed = data.config["Speeds"]["People"];
	speed *= 50000;
	while (speed-- >= 0);
	worldMap[ydest][xdest]->addResourceByWork(res, worldMap[ysource][xsource]->resourceName);
}

void World::build(int x, int y, string type,bool isSteps) {
	y--;
	x--;
	int tilex = data.config["Sizes"]["Tile"][0];
	int tiley = data.config["Sizes"]["Tile"][1];
	int sizex = data.config["Sizes"][type][0];
	int sizey = data.config["Sizes"][type][1];
	y -= y % sizey;
	x -= x % sizex;

	//check if it came from start or steps
	/*int flag = 1;
	if (isSteps) {
		if (row - y < sizey || col - x < sizex)
			return;
		for (int i = y - 1; i <= y + sizey && flag; i++) {
			for (int j = x - 1; j <= x + sizex && flag; j++) {
				if ((i == y - 1 || i == y + sizey) && i>0 && i!=row) {
					if (worldMap[i][j]->categoryName == "Road")
						flag = 0;
				}
				if ((j == x - 1 || j == x + sizex) && j!=0 && j!=col) {
					if (worldMap[i][j]->categoryName == "Road")
						flag = 0;
				}
			}
		}

		if (flag)
			return;
	}*/

	if (!isSteps || x>0 && worldMap[y][x-1]->categoryName == "Road" || 
		worldMap[y][x - 1]->categoryName == "City" || 
		worldMap[y][x - 1]->categoryName == "Village") {

		//sum all resource of infrastructure
		for (int i = y; i < y + sizey; i += tiley) {
			for (int j = x; j < x + sizex; j += tilex) {
				if (i != y || j != x) {
					for (auto res : (*(worldMap[i][j]->resource)))
						(*(worldMap[y][x]->resource))[res.first] += res.second;
				}
			}
		}

		Tile* c = new Tile();
		if (type == "City") {
			c = new City();
		}
		if (type == "Road") {
			c = new Road();
		}
		if (type == "Village") {
			c = new Village();
		}
		c->resource = make_shared<map<string, int>>();

		//init c object with resources
		for (auto res : (*(worldMap[y][x]->resource)))
			(*(c->resource)).emplace(res.first, res.second);

		//count people init tiles
		int people = 0;
		for (int i = y; i < y + sizey; i++) {
			for (int j = x; j < x + sizex; j++) {
				people += worldMap[i][j]->people;
				worldMap[i][j] = c;
			}
		}

		//add people to all pointers to our infrastructure
		c->addPeople(people);
	}
}

int World::count(string categoryName) {
	int tilex = data.config["Sizes"]["Tile"][0];
	int tiley = data.config["Sizes"]["Tile"][1];
	int x = data.config["Sizes"][categoryName][0];
	int y = data.config["Sizes"][categoryName][1];
	int cnt = 0;

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (worldMap[i][j]->categoryName == categoryName) {
				cnt++;
			}
		}
	}
	return cnt / x / y;
}

string World::selectedComplete(int x, int y, int wait){
	//time to build from configuration
	if (wait >= 4)
		return "true";
	return "false";
}

int World::selectedPeople(int x, int y) {
	y--;
	x--;
	return worldMap[y][x]->people;
}

void World::makeEmpty(int x, int y) {
	y--;
	x--;
	worldMap[y][x]->people = 0;
}

void World::manufucture(int x, int y, string type) {
	y--;
	x--;
	if (type == "Car") {
		worldMap[y][x]->vehicles.push_back(new Car);
	}
	if (type == "Truck") {
		worldMap[y][x]->vehicles.push_back(new Truck);
	}
	if (type == "Helicopter") {
		worldMap[y][x]->vehicles.push_back(new Helicopter);
	}
}

int World::selectedVehicle(int x, int y, string type) {
	y--;
	x--;
	int cnt = 0;
	for (Vehicle* i : worldMap[y][x]->vehicles) {
		if (i->type == type) {
			cnt++;
		}
	}
	return cnt;
}
