#pragma once
#include "Tile.h"

class Road: public Tile
{
public :
	Road();
	virtual void addPeople(int cnt) override;
};

