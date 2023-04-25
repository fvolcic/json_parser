# Micro Json - a C++ JSON Library

This is a C++ library for JSON. It is a header-only library, so you can just include the header file in your project and use it. The JSON parser is easy to use, and does validate the JSON syntax. The goal of micro json is to provide a dependency free json parser. This parser is less than 1000 lines, and provides support for loading, validating, editing, and dumping json. This json library is also quite fast, parsing json faster than projects such as nohlmann json. If you need a performant JSON parser, it is still recommended to use the simdjson parser, which is abbout 9x faster than micro josn. Micro json is great if you would like to customize a json parser for your needs. Micro json is implemented as a recuirsive descent parser.

## Usage

To use, simply copy the header file into your project, and include it. The library is header-only, so you don't need to compile anything.

```c++
#include "json.h"

// Load the json from test.json
JsonData * data = JSON_loadf("test.json");

// Print the value of the key "key"
cout << data->asObject()->get("key")->asString();

// save the json to test2.json
JSON_dumpf(data, "test2.json");
```

## Documentation

```c++

// Load a json string
JsonData * JSON(std::string json);
JsonData * JSON(const char * json);

// Dump a json string
std::string JSON_emit(JsonData * data);

// Load a json file
JsonData * JSON_loadf(std::string filename);

// Save a json file
void JSON_dumpf(JsonData * data, std::string filename);

// Get the type of the json data
JsonData->getType();

// Get the value of the json data
JsonData->asBool();
JsonData->asNumber();
JsonData->asString();
JsonData->asArray();
JsonData->asObject();

// Get data from object
JsonData->asObject()->get(std::string key);

// Get data from array
JsonData->asArray()->get(int index);

// Get the size of the JSON array or object
JsonData->size();

// Turn values into JSON data
toJsonData(bool value);
toJsonData(int value);
toJsonData(double value);
toJsonData(std::string value);

// Set values
JsonData->set(std::string key, JsonData * value);
JsonData->set(int index, JsonData * value);

// Push and pop values from arrays
JsonData->asArray()->push(JsonData * value);
JsonData->asArray()->pop();
