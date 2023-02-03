#include "unit_test_framework.h"
#include "json.h"

TEST(parse_string_test){
    std::string str = "\"hello world\"";
    StringBuffer buffer(str);
    std::string value = parseString(buffer);
    ASSERT_EQUAL(value, "hello world");
}

TEST(parse_string_test_2){
    std::string str = "\"hello \\\"world\n\\\"\"";// 'hello "world\n"'
    StringBuffer buffer(str);
    std::string value = parseString(buffer);
    ASSERT_EQUAL(value, "hello \"world\n\"");
}

TEST(parse_empty_string){
    std::string str = "\"\"";
    StringBuffer buffer(str);
    std::string value = parseString(buffer);
    ASSERT_EQUAL(value, "");
}

TEST(parse_number){
    std::string str = "123";
    StringBuffer buffer(str);
    double value = parseNumber(buffer);
    ASSERT_EQUAL(value, 123);
}

TEST(parse_number_decimal){
    std::string str = "123.456";
    StringBuffer buffer(str);
    double value = parseNumber(buffer);
    ASSERT_EQUAL(value, 123.456);
}

TEST(parse_number_negative){
    std::string str = "-123.456";
    StringBuffer buffer(str);
    double value = parseNumber(buffer);
    ASSERT_EQUAL(value, -123.456);
}

TEST(parse_number_negative_2){
    std::string str = "-123";
    StringBuffer buffer(str);
    double value = parseNumber(buffer);
    ASSERT_EQUAL(value, -123);
}

TEST(parse_number_negative_3){
    std::string str = "-0.123";
    StringBuffer buffer(str);
    double value = parseNumber(buffer);
    ASSERT_EQUAL(value, -0.123);
}

TEST(parse_boolean){
    std::string str = "true";
    StringBuffer buffer(str);
    bool value = parseBool(buffer);
    ASSERT_EQUAL(value, true);
}

TEST(parse_boolean_2){
    std::string str = "false";
    StringBuffer buffer(str);
    bool value = parseBool(buffer);
    ASSERT_EQUAL(value, false);
}

TEST(parse_null){
    std::string str = "null";
    StringBuffer buffer(str);
    bool value = parseNull(buffer);
    ASSERT_EQUAL(value, true);
}

TEST(parse_json_number){

    std::string str = "123";
    StringBuffer buffer(str);

    JsonData * value = parseToJsonData(buffer);

    ASSERT_EQUAL(value->getType(), JsonType::JSON_NUMBER);
    ASSERT_EQUAL(value->asNumber(), 123);

}

TEST(parse_json_string){

    std::string str = "\"hello world\"";
    StringBuffer buffer(str);

    JsonData * value = parseToJsonData(buffer);

    ASSERT_EQUAL(value->getType(), JsonType::JSON_STRING);
    ASSERT_EQUAL(value->asString(), "hello world");

}

TEST(parse_json_bool){
    
        std::string str = "true";
        StringBuffer buffer(str);
    
        JsonData * value = parseToJsonData(buffer);
    
        ASSERT_EQUAL(value->getType(), JsonType::JSON_BOOL);
        ASSERT_EQUAL(value->asBool(), true);
    
}

TEST(parse_json_bool_2){
    
        std::string str = "false";
        StringBuffer buffer(str);
    
        JsonData * value = parseToJsonData(buffer);
    
        ASSERT_EQUAL(value->getType(), JsonType::JSON_BOOL);
        ASSERT_EQUAL(value->asBool(), false);
    
}

TEST(parse_json_array){
    
        std::string str = "[1,2,3]";
        StringBuffer buffer(str);
    
        JsonData * value = parseToJsonData(buffer);
    
        ASSERT_EQUAL(value->getType(), JsonType::JSON_ARRAY);
        ASSERT_EQUAL(value->asArray().size(), 3);
        ASSERT_EQUAL(value->asArray()[0]->asNumber(), 1);
        ASSERT_EQUAL(value->asArray()[1]->asNumber(), 2);
        ASSERT_EQUAL(value->asArray()[2]->asNumber(), 3);
    
}

TEST_MAIN()