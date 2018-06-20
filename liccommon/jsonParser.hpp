
#ifndef JSON_PARSER_HPP__ 
#define JSON_PARSER_HPP__ 
#include <json/json.h>
#include <QString>

class JsonParser  
{
public:
    JsonParser(){};
    JsonParser(Json::Value &j):_jValue(j){};
    JsonParser(const Json::Value  &j):_jValue(j){};

    void setJ(Json::Value &j);
    Json::Value getJ();

    QString get(QString key);
    void set(QString key,QString v);

private:
    Json::Value _jValue;
};




#endif
