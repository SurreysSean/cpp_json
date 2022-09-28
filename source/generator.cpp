#include "generator.hpp"
#include "tools.hpp"

void jsonGenerator::generate(const jsonContent &input)
{
    jsonType inputType = input.getType();

    switch (inputType)
    {
    case json_null:
        res += "null";
        break;
    case json_true:
        res += "true";
        break;
    case json_false:
        res += "false";
        break;
    case json_number:
        if (input.getNum() - static_cast<int>(input.getNum()))
            res += std::to_string(input.getNum());
        else
            res += std::to_string(static_cast<int>(input.getNum()));
        break;
    case json_string:
        res += "\"" + input.getStr() + "\"";
        break;
    case json_array:
        gen_array(input);
        break;
    case json_object:
        gen_object(input);
        break;
    }
}

void jsonGenerator::format()
{
    int curIndexAmount = 0;
    string RET_INDEX;
    for (size_t i = 0; i < res.size(); ++i)
    {
        switch (res[i])
        {
        case ',':
            format_res += res[i] + RET_INDEX;
            break;
        case '[':
        case '{':
            ++curIndexAmount;
            RET_INDEX = "\n" + multiIndex(curIndexAmount);
            format_res += res[i] + RET_INDEX;
            break;
        case ']':
        case '}':
            --curIndexAmount;
            RET_INDEX = "\n" + multiIndex(curIndexAmount);
            format_res += RET_INDEX + res[i];
            break;
        case ':':
            format_res += res[i] + string(" ");
            break;
        default:
            format_res += res[i];
        }
    }
}

void jsonGenerator::gen_array(const jsonContent &i)
{
    res += '[';
    jsType_Arr arr(i.getArr());
    for (auto iter = arr.begin(); iter != arr.end(); ++iter)
    {
        generate(*iter);
        res += ',';
    }
    res[res.size() - 1] = ']';
}

void jsonGenerator::gen_object(const jsonContent &i)
{
    res += '{';
    jsType_Obj obj(i.getObj());
    for (auto iter = obj.begin(); iter != obj.end(); ++iter)
    {
        res += "\"" + iter->first + "\"" + ':';
        generate(iter->second);
        res += ',';
    }
    res[res.size() - 1] = '}';
}
