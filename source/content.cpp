#include "content.hpp"
#include <iostream>

jsonContent::jsonContent(const jsonContent &src)
{
    type = src.getType();
    if (type == json_number)
    {
        setNumber(src.getNum());
    }
    else if (type == json_string)
    {
        setString(src.getStr());
    }
    else if (type == json_array)
    {
        setArray(src.getArr());
    }
    else if (type == json_object)
    {
        setObject(src.getObj());
    }
}

void jsonContent::setNumber(const double d)
{
    vp.num = new double(d);
    setType(json_number);
}

void jsonContent::setString(const string &s)
{
    vp.str = new string(s);
    setType(json_string);
}

void jsonContent::setArray(const jsType_Arr &a)
{
    vp.arr = new jsType_Arr(a);
    setType(json_array);
}

void jsonContent::setObject(const jsType_Obj &o)
{
    vp.obj = new jsType_Obj(o);
    setType(json_object);
}

void jsonContent::clear()
{
    if (type == json_number && vp.num)
    {
        delete vp.num;
    }
    else if (type == json_string && vp.str)
    {
        delete vp.str;
    }
    else if (type == json_array && vp.arr)
    {
        vp.arr->clear();
        delete vp.arr;
    }
    else if (type == json_object && vp.obj)
    {

        for (auto iter = vp.obj->begin(); iter != vp.obj->end(); ++iter)
            iter->second.clear();
        delete vp.obj;
    }
    type = json_null;
}

jsonContent &jsonContent::operator=(const jsonContent &src)
{
    if (this == &src)
        return *this;
    clear();
    type = src.getType();
    if (type == json_number)
    {
        setNumber(src.getNum());
    }
    else if (type == json_string)
    {
        setString(src.getStr());
    }
    else if (type == json_array)
    {
        setArray(src.getArr());
    }
    else if (type == json_object)
    {
        setObject(src.getObj());
    }
    return *this;
}

bool operator==(const jsonContent &lhs, const jsonContent &rhs)
{
    if (lhs.type != rhs.type)
        return false;
    jsonType type = lhs.type;
    if (type == json_number)
    {
        return *lhs.vp.num == *rhs.vp.num;
    }
    else if (type == json_string)
    {
        return *lhs.vp.str == *rhs.vp.str;
    }
    else if (type == json_array)
    {
        return *lhs.vp.arr == *rhs.vp.arr;
    }
    else if (type == json_object)
    {
        return *lhs.vp.obj == *rhs.vp.obj;
    }
    return false;
}

ostream &operator<<(ostream &os, const jsonContent &src)
{
    jsonType type = src.getType();
    os << "type: " << type << "\nvalue: ";
    if (type == json_number)
    {
        os << src.getNum();
    }
    else if (type == json_string)
    {
        os << src.getStr();
    }
    else if (type == json_array)
    {
        os << "[\n";
        for (int i = 0; i < src.getArr().size(); ++i)
        {
            operator<<(os, src.getArr().at(i));
            if (i != src.getArr().size())
                os << ',';
            os << "\n";
        }
        os << "]";
    }
    else if (type == json_object)
    {
        os << "{\n";
        jsType_Obj &object = src.getObj();
        for (auto iter = object.begin(); iter != object.end(); ++iter)
        {
            os << "\t" << iter->first << ":";
            operator<<(os, iter->second);
            if (iter != object.end())
                os << ',';
            os << "\n";
        }
        os << "}";
    }

    return os;
}