#include "City.h"

City::City() {
	categoryName = "City";
	people = data.config["StartingResources"]["City"];
}