#include "ArduinoJson/Internals/JsonParser.hpp"

#include <stdlib.h> // for strtol, strtod
#include <ctype.h>

#include "ArduinoJson/JsonBuffer.hpp"
#include "ArduinoJson/Internals/QuotedString.hpp"

using namespace ArduinoJson::Internals;

bool JsonParser::isArrayStart()
{
    return *_ptr == '[';
}

bool JsonParser::isArrayStop()
{
    return *_ptr == ']';
}

bool JsonParser::isBoolean()
{
    return *_ptr == 't' || *_ptr == 'f';
}

bool JsonParser::isComma()
{
    return *_ptr == ',';
}

bool JsonParser::isColon()
{
    return *_ptr == ':';
}

bool JsonParser::isDouble()
{
    char* ptr = _ptr;

    // skip all digits
    while (isdigit(*ptr))
        ptr++;

    // same position => 0 digits => not a number
    if (ptr == _ptr)
        return false;

    // stopped on a decimal separator => ok it's a double
    return *ptr == '.';
}

bool JsonParser::isEnd()
{
    return *_ptr == 0;
}

bool JsonParser::isLong()
{
    char* ptr = _ptr;

    // skip all digits
    while (isdigit(*ptr))
        ptr++;

    // same position => 0 digits => not a number
    if (ptr == _ptr) 
        return false;

    // stopped on a decimal separator => not a long but a double
    return *ptr != '.';
}

bool JsonParser::isNull()
{
    return *_ptr == 'n';
}

bool JsonParser::isObjectStart()
{
    return *_ptr == '{';
}

bool JsonParser::isObjectStop()
{
    return *_ptr == '}';
}

bool JsonParser::isSpace()
{
    return *_ptr == ' ' || *_ptr == '\t' || *_ptr == '\n' || *_ptr == '\r';
}

void JsonParser::skipOneChar()
{
    _ptr++;
}

void JsonParser::skipSpaces()
{
    while(isSpace()) skipOneChar();
}

JsonNode* JsonParser::parseAnything()
{
    skipSpaces();

    if (isArrayStart())
        return parseArray();

    if (isBoolean())
        return parseBoolean();
        
    if (isDouble())
        return parseDouble();

    if (isLong())    
        return parseLong();

    if (isNull())
        return parseNull();

    if (isObjectStart())
        return parseObject();

    return parseString();
}

JsonNode* JsonParser::parseArray()
{
    JsonNode* node = _buffer->createArrayNode();

    skipOneChar(); // skip the '['
    skipSpaces();

    if (isEnd())
        return 0;

    if (isArrayStop())
    {
        skipOneChar(); // skip the ']'
        return node;
    }

    for(;;)
    {
        node->addChild(parseAnything());

        skipSpaces();

        if (isArrayStop())
            return node;

        if (!isComma())
            return 0;

        skipOneChar(); // skip the ','
    }
}

JsonNode *JsonParser::parseBoolean()
{
    bool value = *_ptr == 't';

    _ptr += value ? 4 : 5;
    // 4 = strlen("true")
    // 5 = strlen("false");

    return _buffer->createBoolNode(value);
}

JsonNode *JsonParser::parseDouble()
{
    double value = strtod(_ptr, &_ptr);

    int decimals = 0;
    while(_ptr[1-decimals] != '.')
        decimals++;

    return _buffer->createDoubleNode(value, decimals);
}

JsonNode* JsonParser::parseLong()
{
    long value = strtol(_ptr, &_ptr, 10);

    return _buffer->createLongNode(value);
}

JsonNode* JsonParser::parseNull()
{
    _ptr += 4; // strlen("null")

    return _buffer->createStringNode(0);
}

JsonNode* JsonParser::parseObject()
{
    JsonNode* node = _buffer->createObjectNode();

    skipOneChar(); // skip the '{'
    skipSpaces();

    if (isEnd())
        return 0;

    if (isObjectStop())
    {
        skipOneChar(); // skip the '}'
        return node;
    }

    for(;;)
    {
        JsonNode* keyValueNode = parseObjectKeyValue();

        if (!keyValueNode)
            return 0;

        node->addChild(keyValueNode);

        skipSpaces();

        if (isObjectStop())
            return node;

        if (!isComma())
            return 0;

        skipOneChar(); // skip the ','
    }
}

JsonNode* JsonParser::parseObjectKeyValue()
{
    const char* key = QuotedString::extractFrom(_ptr, &_ptr);

    if (!key)
        return 0;

    skipSpaces();

    if (!isColon())
        return 0;

    skipOneChar(); // skip the :
    skipSpaces();

    JsonNode* value = parseAnything();

    return _buffer->createObjectKeyValueNode(key, value);
}

JsonNode* JsonParser::parseString()
{
    const char* s = QuotedString::extractFrom(_ptr, &_ptr);
    return _buffer->createStringNode(s);
}