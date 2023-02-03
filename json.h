#ifndef JSON_H
#define JSON_H

#include <string>
#include <map> 
#include <vector>

class JsonValue;
class JsonObject;
class JsonArray;
class JsonString;
class JsonNumber;
class JsonBool;
class JsonNull;
class JsonData;
class StringBuffer;

JsonData * parseToJsonData(StringBuffer &buffer);

static bool parseError = false;
static std::string parseErrorString = "";

bool hasError(){
    return parseError;
}

std::string getError(){
    return parseErrorString;
}

enum class JsonType
{
    JSON_STRING,
    JSON_NUMBER,
    JSON_BOOL,
    JSON_OBJECT,
    JSON_ARRAY,
    JSON_NULL
};

class StringBuffer
{
public:

    inline StringBuffer(const std::string &str)
    : str(str), index(0)
    {};

    inline char next(){
        if (index >= str.length())
            return '\0';
        return str[index++];
    }

    inline char peek(){
        if (index >= str.length())
            return '\0';
        return str[index];
    }

    inline void skipWhitespace(){
        while(peek() == ' '){
            next();
        }
    }

private:
        const std::string & str;
        uint index;
};

inline bool isDelimiter(char c){
    return c == ' ' 
    || c == '}' 
    || c == ']'
    || c == ',';
}; 

inline char readEscape(char c){
    switch (c){
        case 'b': return '\b';
        case 'f': return '\f';
        case 'n': return '\n';
        case 'r': return '\r';
        case 't': return '\t';
        case '"': return '"';
        case '\\': return '\\';
        default: return '\0';
    }
}

inline std::string parseString (StringBuffer &buffer){

    parseError = false; 

    std::string str = "";

    if(buffer.peek() != '"'){
        parseError = true;
        return str;
    }

    buffer.next();

    while(buffer.peek() != '"' && buffer.peek() != '\0'){
        char next = buffer.next(); 

        if(next == '\\'){
            next = buffer.next();
            next = readEscape(next);

            if(next == '\0'){
                parseError = true;
                return str;
            }
        }

        str += next;
    }

    if(buffer.peek() == '\0'){
        parseError = true;
        return str;
    }

    buffer.next();

    return str;
}

inline double parseNumber (StringBuffer &buffer){
    
    parseError = false;

    std::string str = "";
    
    bool isNegative = false;
    bool isDecimal = false;

    if(buffer.peek() == '-'){
        isNegative = true;
        buffer.next();
    }

    while(!isDelimiter(buffer.peek()) && buffer.peek() != '\0'){
        char next = buffer.next();

        if(next == '.'){
            if(isDecimal){
                parseError = true;
                return 0;
            }
            isDecimal = true;
        }

        str += next;
    }

    if(isNegative)
        return -std::stod(str);

    return std::stod(str);
    
}

inline bool parseBool (StringBuffer &buffer){
    
    parseError = false;

    std::string str = "";

    if(buffer.peek() == 't'){
        str += buffer.next();
        str += buffer.next();
        str += buffer.next();
        str += buffer.next();

        if(!isDelimiter(buffer.peek()) && buffer.peek() != '\0'){
            parseError = true;
            return false;
        }

        if(str == "true")
            return true;
        
        parseError = true;
        return false;
    }

    if(buffer.peek() == 'f'){
        str += buffer.next();
        str += buffer.next();
        str += buffer.next();
        str += buffer.next();
        str += buffer.next();
        
        if(!isDelimiter(buffer.peek()) && buffer.peek() != '\0'){
            parseError = true;
            return false;
        }

        if(str == "false")
            return false;
        
        parseError = true;
        return false;
    }

    parseError = true;
    return false;
}

bool parseNull (StringBuffer &buffer){
    
    parseError = false;

    std::string str = "";

    str += buffer.next();
    str += buffer.next();
    str += buffer.next();
    str += buffer.next();

    if(!isDelimiter(buffer.peek()) && buffer.peek() != '\0'){
        parseError = true;
        return false;
    }

    if(str == "null")
        return true;
    
    parseError = true;
    return false;
}

class JsonData
{
public: 
    virtual std::string asString()
    {
        return "";
    };

    virtual double asNumber()
    {
        return 0;
    };

    virtual bool asBool()
    {
        return false;
    };

    virtual JsonData *asObject()
    {
        return nullptr;
    };

    virtual std::vector<JsonData*> asArray()
    {
        return std::vector<JsonData*>();
    };

    virtual JsonData *operator[](const std::string &key)
    {
        return nullptr;
    };

    virtual JsonData *operator[](int index)
    {
        return nullptr;
    };

    virtual int size()
    {
        return 0;
    };

    virtual JsonType getType()
    {
        return JsonType::JSON_NULL;
    };

    virtual ~JsonData(){};
};

class JsonString : public JsonData
{
public:

    inline JsonString(StringBuffer &buffer) : buffer(buffer){
        str = parseString(buffer);
        if(parseError){
            parseErrorString = "Error parsing string";
        }
    };

    inline std::string asString() override
    {
        return str;
    };

    inline JsonType getType() override
    {
        return JsonType::JSON_STRING;
    };

private:

    StringBuffer &buffer;
    std::string str; 
};

class JsonNumber : public JsonData
{
public:

    inline JsonNumber(StringBuffer &buffer) : buffer(buffer){
        num = parseNumber(buffer);
        if(parseError){
            parseErrorString = "Error parsing number";
        }
    };

    inline double asNumber() override
    {
        return num;
    };

    inline JsonType getType() override
    {
        return JsonType::JSON_NUMBER;
    };

private:
    
        StringBuffer &buffer;
        double num; 
};

class JsonBool : public JsonData
{
public:

    inline JsonBool(StringBuffer &buffer) : buffer(buffer){
        b = parseBool(buffer);
        if(parseError){
            parseErrorString = "Error parsing bool";
        }
    };

    inline bool asBool() override
    {
        return b;
    };

    inline JsonType getType() override
    {
        return JsonType::JSON_BOOL;
    };

private:
        StringBuffer &buffer;
        bool b;
};

class JsonNull : public JsonData
{

public:

    inline JsonNull(StringBuffer &buffer) : buffer(buffer){
        if(!parseNull(buffer)){
            parseError = true;
            parseErrorString = "Error parsing null";
        }
    };

    inline JsonType getType() override
    {
        return JsonType::JSON_NULL;
    };

private:
        StringBuffer &buffer;

};

class JsonObject : public JsonData
{
public:

    inline JsonObject(StringBuffer &buffer) : buffer(buffer){
    };

    inline JsonData *operator[](const std::string &key) override
    {
    };

    inline JsonType getType() override
    {
        return JsonType::JSON_OBJECT;
    };

    inline int size() override
    {
    };

private:

    StringBuffer &buffer;
    std::map<std::string, JsonData *> data;

};

class JsonArray : public JsonData
{
public:

    inline JsonArray(StringBuffer &buffer) : buffer(buffer){

        buffer.skipWhitespace(); //skip whitespace

        if(buffer.next() != '['){
            parseError = true;
            parseErrorString = "Error parsing array";
            return;
        }

        buffer.skipWhitespace(); //skip whitespace

        while(buffer.peek() != ']' ){
            data.push_back(parseToJsonData(buffer));
            if(parseError){
                parseErrorString = "Error parsing array";
                return;
            }
            buffer.skipWhitespace(); //skip whitespace

            if(buffer.peek() == ','){
                buffer.next();
            }
            else if(buffer.peek() != ']'){
                parseError = true;
                parseErrorString = "Error parsing array";
                return;
            }
            else{
                break;
            }

            buffer.skipWhitespace(); 
        }

        buffer.next(); //skip ']'

    };

    inline JsonData *operator[](int index) override
    {
        return data[index];
    };

    inline JsonType getType() override
    {
        return JsonType::JSON_ARRAY;
    };

    inline int size() override
    {
        return data.size();
    };

private:
    
        StringBuffer &buffer;
        std::vector<JsonData *> data;
    
};

inline JsonData * parseToJsonData(StringBuffer &buffer){
        
        parseError = false;

        buffer.skipWhitespace(); //skip whitespace
    
        if(buffer.peek() == '"'){
            return new JsonString(buffer);
        }
    
        if(buffer.peek() == '-' || (buffer.peek() >= '0' && buffer.peek() <= '9')){
            return new JsonNumber(buffer);
        }
    
        if(buffer.peek() == 't' || buffer.peek() == 'f'){
            return new JsonBool(buffer);
        }
    
        if(buffer.peek() == 'n'){
            return new JsonNull(buffer);
        }
    
        if(buffer.peek() == '{'){
            return new JsonObject(buffer);
        }
    
        if(buffer.peek() == '['){
            return new JsonArray(buffer);
        }
    
        parseError = true;
        return nullptr;
}

#endif