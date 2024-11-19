#include <iostream>
#include "World.h"
#include "Input.h"


int main()
{
    
    Input inp;
    inp.parse_and_store();
    
    World w(inp.world->data);

    int x;
    int y;
    int wait = 0;

    std::vector<std::shared_ptr<Command>> start = inp.start;
    for (auto i : start) {
        if (i->name == i->RESOURCE) {
            int x = stoi(i->arguments[2]);
            int y = stoi(i->arguments[3]);
            int cnt = stoi(i->arguments[0]);
            string resource = i->arguments[1];
            w.updateResource(x, y, cnt, resource);
        }
        if (i->name == i->PEOPLE) {
            int x = stoi(i->arguments[1]);
            int y = stoi(i->arguments[2]);
            int cnt = stoi(i->arguments[0]);
            w.addPeople(x, y, cnt);
        }
        if (i->name == i->BUILD) {
            int x = stoi(i->arguments[1]);
            int y = stoi(i->arguments[2]);
            string type = i->arguments[0];
            w.build(x, y, type, false);
        }
        if (i->name == i->MAKE_EMPTY) {
            int x = stoi(i->arguments[0]);
            int y = stoi(i->arguments[1]);
            w.makeEmpty(x, y);
        }
        if (i->name == i->MANUFACTURE) {
            string type = i->arguments[0];
            int x = stoi(i->arguments[1]);
            int y = stoi(i->arguments[2]);
            w.manufucture(x, y, type);
        }
    }

    std::vector< std::shared_ptr<Command> > steps = inp.steps;
    for (auto i : steps) {
        if (i->name == i->RESOURCE) {
            int r = stoi(i->arguments[2]);
            int c = stoi(i->arguments[3]);
            int cnt = stoi(i->arguments[0]);
            string resource = i->arguments[1];
            w.updateResource(r, c, cnt, resource);
        }
        if (i->name == i->PEOPLE) {
            int x = stoi(i->arguments[1]);
            int y = stoi(i->arguments[2]);
            int cnt = stoi(i->arguments[0]);
            w.addPeople(x, y, cnt);
        }
        if (i->name == i->SELECT) {
            x = stoi(i->arguments[0]);
            y = stoi(i->arguments[1]);
        }
        if (i->name == i->RAIN) {
            int milisec = stoi(i->arguments[0]);
             w.fallRain(milisec);
        }
        if (i->name == i->WORK) {
            int r = x, c = y;
            x = stoi(i->arguments[0]);
            y = stoi(i->arguments[1]);
            w.work(x, y,r,c);
        }
        if (i->name == i->BUILD) {
            int x = stoi(i->arguments[1]);
            int y = stoi(i->arguments[2]);
            string type = i->arguments[0];
            w.build(x, y, type, true);
        }
        if (i->name == i->WAIT) {
            wait = stoi(i->arguments[0]);
        }
    }

    std::vector<string> asserts = inp.asserts;
    for (auto i : asserts) {
        if (i == "SelectedCategory") {
            cout<< i <<" "<<w.selectedCtegory(x, y)<<endl;
        }
        if (i == "SelectedResource") {
            cout << i << " " << w.selectedResource(x, y) << endl;
        }
        if (i == "SelectedComplete") {
            cout << i << " " << w.selectedComplete(x, y, wait) << endl;
        }
        if (i == "SelectedPeople") {
            cout << i << " " << w.selectedPeople(x, y) << endl;
        }
        if (i == "SelectedCar") {
            cout << i << " " << w.selectedVehicle(x, y, "Car") << endl;
        }
        if (i == "SelectedTruck") {
            cout << i << " " << w.selectedVehicle(x, y, "Truck") << endl;
        }

        if (i == "CityCount") {
            cout << i << " " << w.count("City") << endl;
        }
        if (i == "VillageCount") {
            cout << i << " " << w.count("Village") << endl;
        }
        if (i == "RoadCount") {
            cout << i << " " << w.count("Road") << endl;
        }
    }

}
