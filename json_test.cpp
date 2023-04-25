#include "unit_test_framework.h"
#include "json.h"

TEST(parse_string_test)
{
    std::string str = "\"hello world\"";
    StringBuffer buffer(str);
    std::string value = parseString(buffer);
    ASSERT_EQUAL(value, "hello world");
}

TEST(parse_string_test_2)
{
    std::string str = "\"hello \\\"world\\n\\\"\""; // 'hello "world\n"'
    std::cout << str << std::endl;
    StringBuffer buffer(str);
    std::string value = parseString(buffer);
    ASSERT_EQUAL(value, "hello \\\"world\\n\\\"");
}


TEST(parse_empty_string)
{
    std::string str = "\"\"";
    StringBuffer buffer(str);
    std::string value = parseString(buffer);
    ASSERT_EQUAL(value, "");
}

TEST(parse_number)
{
    std::string str = "123";
    StringBuffer buffer(str);
    double value = parseNumber(buffer);
    ASSERT_EQUAL(value, 123);
}

TEST(parse_number_decimal)
{
    std::string str = "123.456";
    StringBuffer buffer(str);
    double value = parseNumber(buffer);
    ASSERT_EQUAL(value, 123.456);
}

TEST(parse_number_negative)
{
    std::string str = "-123.456";
    StringBuffer buffer(str);
    double value = parseNumber(buffer);
    ASSERT_EQUAL(value, -123.456);
}

TEST(parse_number_negative_2)
{
    std::string str = "-123";
    StringBuffer buffer(str);
    double value = parseNumber(buffer);
    ASSERT_EQUAL(value, -123);
}

TEST(parse_number_negative_3)
{
    std::string str = "-0.123";
    StringBuffer buffer(str);
    double value = parseNumber(buffer);
    ASSERT_EQUAL(value, -0.123);
}

TEST(parse_boolean)
{
    std::string str = "true";
    StringBuffer buffer(str);
    bool value = parseBool(buffer);
    ASSERT_EQUAL(value, true);
}

TEST(parse_boolean_2)
{
    std::string str = "false";
    StringBuffer buffer(str);
    bool value = parseBool(buffer);
    ASSERT_EQUAL(value, false);
}

TEST(parse_null)
{
    std::string str = "null";
    StringBuffer buffer(str);
    bool value = parseNull(buffer);
    ASSERT_EQUAL(value, true);
}

TEST(parse_json_number)
{

    std::string str = "123";
    StringBuffer buffer(str);

    JsonData *value = parseToJsonData(buffer);

    ASSERT_EQUAL(value->getType(), JsonType::JSON_NUMBER);
    ASSERT_EQUAL(value->asNumber(), 123);

    delete value;
}

TEST(parse_json_string)
{

    std::string str = "\"hello world\"";
    StringBuffer buffer(str);

    JsonData *value = parseToJsonData(buffer);

    ASSERT_EQUAL(value->getType(), JsonType::JSON_STRING);
    ASSERT_EQUAL(value->asString(), "hello world");

    delete value;
}

TEST(parse_json_string2){
    std::string str = "\"'\"";
    StringBuffer buffer(str);

    JsonData * value = parseToJsonData(buffer);
    ASSERT_EQUAL(value->getType(), JsonType::JSON_STRING);
    ASSERT_EQUAL(value->asString(), "'");

    delete value;
}

TEST(parse_json_bool)
{

    std::string str = "true";
    StringBuffer buffer(str);

    JsonData *value = parseToJsonData(buffer);

    ASSERT_EQUAL(value->getType(), JsonType::JSON_BOOL);
    ASSERT_EQUAL(value->asBool(), true);

    delete value;
}

TEST(parse_json_bool_2)
{

    std::string str = "false";
    StringBuffer buffer(str);

    JsonData *value = parseToJsonData(buffer);

    ASSERT_EQUAL(value->getType(), JsonType::JSON_BOOL);
    ASSERT_EQUAL(value->asBool(), false);

    delete value;
}

TEST(parse_json_array)
{

    std::string str = "[1,2,3]";
    StringBuffer buffer(str);

    JsonData *value = parseToJsonData(buffer);

    ASSERT_EQUAL(value->getType(), JsonType::JSON_ARRAY);
    ASSERT_EQUAL(value->asArray()->size(), 3);
    ASSERT_EQUAL((*value->asArray())[0]->asNumber(), 1);
    ASSERT_EQUAL((*value->asArray())[1]->asNumber(), 2);
    ASSERT_EQUAL((*value->asArray())[2]->asNumber(), 3);

    delete value;
}

TEST(parse_json_array2)
{

    std::string str = "[1,2,3,\n[4,5,6]]";
    StringBuffer buffer(str);

    JsonData *value = parseToJsonData(buffer);

    ASSERT_EQUAL(value->getType(), JsonType::JSON_ARRAY);
    ASSERT_EQUAL((*value->asArray()).size(), 4);
    ASSERT_EQUAL((*value->asArray())[0]->asNumber(), 1);
    ASSERT_EQUAL((*value->asArray())[1]->asNumber(), 2);
    ASSERT_EQUAL((*value->asArray())[2]->asNumber(), 3);
    ASSERT_EQUAL((*value->asArray())[3]->asArray()->size(), 3);
    ASSERT_EQUAL((*value->asArray())[3]->asArray()->operator[](0)->asNumber(), 4);
    ASSERT_EQUAL((*value->asArray())[3]->asArray()->operator[](1)->asNumber(), 5);
    ASSERT_EQUAL((*value->asArray())[3]->asArray()->operator[](2)->asNumber(), 6);

    delete value;
}

TEST(parse_json_object)
{

    std::string str = "{\"name\":\"hello world\", \"age\": 18}";
    StringBuffer buffer(str);

    JsonData *value = parseToJsonData(buffer);

    ASSERT_EQUAL(value->getType(), JsonType::JSON_OBJECT);
    ASSERT_EQUAL(value->size(), 2);
    ASSERT_EQUAL(value->asObject()->operator[]("name")->asString(), "hello world");
    ASSERT_EQUAL(value->asObject()->operator[]("age")->asNumber(), 18);

    delete value;
}

TEST(parse_json_object2)
{

    std::string str = "{\"name\":\"hello world\", \"age\": 18, \"address\": {\"city\": \"beijing\", \"country\": \"china\"}}";
    StringBuffer buffer(str);

    JsonData *value = parseToJsonData(buffer);

    ASSERT_EQUAL(value->getType(), JsonType::JSON_OBJECT);
    ASSERT_EQUAL(value->size(), 3);
    ASSERT_EQUAL(value->asObject()->operator[]("name")->asString(), "hello world");
    ASSERT_EQUAL(value->asObject()->operator[]("age")->asNumber(), 18);
    ASSERT_EQUAL(value->asObject()->operator[]("address")->asObject()->operator[]("city")->asString(), "beijing");
    ASSERT_EQUAL(value->asObject()->operator[]("address")->asObject()->operator[]("country")->asString(), "china");

    delete value;
}

TEST(parse_json_object3)
{

    std::string str = "{\"name\":\"hello world\", \"age\": 18, \"address\": {\"city\": \"beijing\", \"country\": \"china\", \"street\": [\"a\", \"b\", \"c\"]}}";
    StringBuffer buffer(str);

    JsonData *value = parseToJsonData(buffer);

    ASSERT_EQUAL(value->getType(), JsonType::JSON_OBJECT);
    ASSERT_EQUAL(value->size(), 3);
    ASSERT_EQUAL(value->asObject()->operator[]("name")->asString(), "hello world");
    ASSERT_EQUAL(value->asObject()->operator[]("age")->asNumber(), 18);
    ASSERT_EQUAL(value->asObject()->operator[]("address")->asObject()->operator[]("city")->asString(), "beijing");
    ASSERT_EQUAL(value->asObject()->operator[]("address")->asObject()->operator[]("country")->asString(), "china");
    ASSERT_EQUAL(value->asObject()->operator[]("address")->asObject()->operator[]("street")->asArray()->size(), 3);
    ASSERT_EQUAL(value->asObject()->operator[]("address")->asObject()->operator[]("street")->asArray()->operator[](0)->asString(), "a");
    ASSERT_EQUAL(value->asObject()->operator[]("address")->asObject()->operator[]("street")->asArray()->operator[](1)->asString(), "b");
    ASSERT_EQUAL(value->asObject()->operator[]("address")->asObject()->operator[]("street")->asArray()->operator[](2)->asString(), "c");

    delete value;
}

TEST(parse_json_object4_array_of_objects)
{

    std::string str = "{\"name\":\"hello world\", \"age\": 18, \"address\": {\"city\": \"beijing\", \"country\": \"china\", \"street\": [{\"name\": \"a\"}, {\"name\": \"b\"}, {\"name\": \"c\"}]}}";
    StringBuffer buffer(str);

    JsonData *value = parseToJsonData(buffer);

    ASSERT_EQUAL(value->getType(), JsonType::JSON_OBJECT);
    ASSERT_EQUAL(value->size(), 3);
    ASSERT_EQUAL(value->asObject()->operator[]("name")->asString(), "hello world");
    ASSERT_EQUAL(value->asObject()->operator[]("age")->asNumber(), 18);
    ASSERT_EQUAL(value->asObject()->operator[]("address")->asObject()->operator[]("city")->asString(), "beijing");
    ASSERT_EQUAL(value->asObject()->operator[]("address")->asObject()->operator[]("country")->asString(), "china");
    ASSERT_EQUAL(value->asObject()->operator[]("address")->asObject()->operator[]("street")->asArray()->size(), 3);
    ASSERT_EQUAL(value->asObject()->operator[]("address")->asObject()->operator[]("street")->asArray()->operator[](0)->asObject()->operator[]("name")->asString(), "a");
    ASSERT_EQUAL(value->asObject()->operator[]("address")->asObject()->operator[]("street")->asArray()->operator[](1)->asObject()->operator[]("name")->asString(), "b");
    ASSERT_EQUAL(value->asObject()->operator[]("address")->asObject()->operator[]("street")->asArray()->operator[](2)->asObject()->operator[]("name")->asString(), "c");

    delete value;
}

TEST(parse_and_emit)
{

    std::string str = "{\"name\":\"hello world\", \"age\": 18, \"address\": {\"city\": \"beijing\", \"country\": \"china\", \"street\": [{\"name\": \"a\"}, {\"name\": \"b\"}, {\"name\": \"c\"}]}}";
    StringBuffer buffer(str);

    JsonData *value = parseToJsonData(buffer);

    std::string str2 = value->emit();

    StringBuffer buffer2(str2);

    JsonData *value2 = parseToJsonData(buffer2);

    ASSERT_EQUAL(value2->getType(), JsonType::JSON_OBJECT);
    ASSERT_EQUAL(value2->size(), 3);
    ASSERT_EQUAL(value2->asObject()->operator[]("name")->asString(), "hello world");
    ASSERT_EQUAL(value2->asObject()->operator[]("age")->asNumber(), 18);
    ASSERT_EQUAL(value2->asObject()->operator[]("address")->asObject()->operator[]("city")->asString(), "beijing");
    ASSERT_EQUAL(value2->asObject()->operator[]("address")->asObject()->operator[]("country")->asString(), "china");
    ASSERT_EQUAL(value2->asObject()->operator[]("address")->asObject()->operator[]("street")->asArray()->size(), 3);
    ASSERT_EQUAL(value2->asObject()->operator[]("address")->asObject()->operator[]("street")->asArray()->operator[](0)->asObject()->operator[]("name")->asString(), "a");
    ASSERT_EQUAL(value2->asObject()->operator[]("address")->asObject()->operator[]("street")->asArray()->operator[](1)->asObject()->operator[]("name")->asString(), "b");
    ASSERT_EQUAL(value2->asObject()->operator[]("address")->asObject()->operator[]("street")->asArray()->operator[](2)->operator[]("name")->asString(), "c");

    JSON_dumpf(value2, "test.json");

    delete value;
    delete value2;

}


TEST(json_create_object){
    auto value = new JsonObject();

    value->set("test", toJsonData(4));

    ASSERT_EQUAL(value->getType(), JsonType::JSON_OBJECT);
    ASSERT_EQUAL(value->size(), 1);
    ASSERT_EQUAL(value->asObject()->operator[]("test")->asNumber(), 4);

    delete value;
}

TEST(json_push_pop){
    auto value = JSON("{'arr':[1, 2, 3]}"); // parse json literal

    std::cout << "LENGTH: " << value->get("arr")->asArray()->size() << std::endl; 

    value->get("arr")->asArray()->push_back(toJsonData(4));
    value->get("arr")->asArray()->push_back(toJsonData(5));

    std::cout << "LENGTH: " << value->get("arr")->asArray()->size() << std::endl; 

    ASSERT_EQUAL(value->getType(), JsonType::JSON_OBJECT);
    ASSERT_EQUAL(value->get("arr")->asArray()->size(), 5);
    ASSERT_EQUAL(value->asObject()->operator[]("arr")->asArray()->operator[](0)->asNumber(), 1);
    ASSERT_EQUAL(value->asObject()->operator[]("arr")->asArray()->operator[](1)->asNumber(), 2);
    ASSERT_EQUAL(value->asObject()->operator[]("arr")->asArray()->operator[](2)->asNumber(), 3);
    ASSERT_EQUAL(value->asObject()->operator[]("arr")->asArray()->operator[](3)->asNumber(), 4);
    ASSERT_EQUAL(value->asObject()->operator[]("arr")->asArray()->operator[](4)->asNumber(), 5);

    value->get("arr")->asArray()->pop_back();

    ASSERT_EQUAL(value->getType(), JsonType::JSON_OBJECT);
    ASSERT_EQUAL(value->get("arr")->asArray()->size(), 4);
    ASSERT_EQUAL(value->asObject()->operator[]("arr")->asArray()->operator[](0)->asNumber(), 1);
    ASSERT_EQUAL(value->asObject()->operator[]("arr")->asArray()->operator[](1)->asNumber(), 2);
    ASSERT_EQUAL(value->asObject()->operator[]("arr")->asArray()->operator[](2)->asNumber(), 3);
    ASSERT_EQUAL(value->asObject()->operator[]("arr")->asArray()->operator[](3)->asNumber(), 4);

    delete value;

}

TEST_MAIN()