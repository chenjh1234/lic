#include "jsonParser.hpp"
#include "LFileDate.h"
void JsonParser::setJ(Json::Value& j)
{
   _jValue = j;
}
Json::Value JsonParser::getJ()
{
    return _jValue;
}
QString JsonParser::get(QString key)
{
    QString q;
    string v,k;
    k = key.Q2CH;
    v = _jValue[k].asString();
    q = v.c_str();
    return q;
}
void JsonParser::set(QString key,QString value)
{
    QString q;
    string v,k;
    
    k = key.Q2CH;
    v = value.Q2CH;
    _jValue[k] = v;
    //cout <<"jValue = " << _jValue[k];
     
}

