#include "Tile.h"
#include "ReadFromJson.h"
using namespace std;

Tile & Tile::operator=(Tile const & other) {
	this->category = other.category;
	this->categoryName = other.categoryName;
	this->resourceName = other.resourceName;
	this->resource = other.resource;
	this->people = other.people;
	return *this;
}

int Tile::getIndexOfResource(string resourceName) {
	int sort = 0 ;
	for (auto i : data.config["ResourceTypes"]) {
		if (i == resourceName) {
			return sort;
		}
		sort++;
	}
	throw invalid_argument("no resource match");
}

void Tile::updateResource(int cnt, string resourceName) {
	(*resource)[resourceName] = cnt;
}

void Tile::addPeople(int cnt) {
	people += cnt;
	int ind = getIndexOfResource("People");
	if (people > data.config["Capacities"][categoryName][ind]) {
		people = data.config["Capacities"][categoryName][ind];
	}
}

void Tile::resourceGrow() {
	(*resource)[resourceName] ++;
}

int Tile::takeResourcesByWork() {
	int res = (*resource)[resourceName];
	(*resource)[resourceName] = 0;
	return res;
}

void Tile::addResourceByWork(int res, string resourceName) {
	(*resource)[resourceName] += res;
}
