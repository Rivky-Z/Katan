#include "ReadFromJson.h"
ReadFromJson::ReadFromJson() {

        ifstream f("configuration.json");
        f >> config;

}



//ReadFromJson* ReadFromJson::getJson() {
//    if (readFromJson == NULL)
//        readFromJson = new ReadFromJson();
//    return readFromJson;
//}


