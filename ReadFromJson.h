#pragma once
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
using namespace std;
using json = nlohmann::json;

class ReadFromJson
{
public:
    json config;
    ReadFromJson();
    //static ReadFromJson* getJson();

private:
    //static ReadFromJson* readFromJson;
    //ReadFromJson(const ReadFromJson & obj) = delete;
};