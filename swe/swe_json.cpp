/***************************************************************************
 *   Copyright (C) 2017 by SWE team <sdl.window.engine@gmail.com>          *
 *                                                                         *
 *   Part of the SWE: SDL Window Engine:                                   *
 *   https://github.com/AndreyBarmaley/sdl-window-engine                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifdef SWE_WITH_JSON

#include <sstream>
#include <cctype>
#include <algorithm>
#include <utility>

#include "swe_systems.h"
#include "swe_json_ext.h"

namespace SWE
{
    JsmnToken::JsmnToken()
    {
        jsmntok_t::type = JSMN_PRIMITIVE;
        jsmntok_t::start = -1;
        jsmntok_t::end = -1;
        jsmntok_t::size = 0;
    }

    int JsmnToken::counts(void) const
    {
        return jsmntok_t::size;
    }

    int JsmnToken::start(void) const
    {
        return jsmntok_t::start;
    }

    int JsmnToken::size(void) const
    {
        return jsmntok_t::end - jsmntok_t::start;
    }

    bool JsmnToken::isKey(void) const
    {
        return isString() && counts() == 1;
    }

    bool JsmnToken::isValue(void) const
    {
        return isPrimitive() || isObject() || isArray() || (isString() && counts() == 0);
    }

    bool JsmnToken::isPrimitive(void) const
    {
        return jsmntok_t::type == JSMN_PRIMITIVE;
    }

    bool JsmnToken::isString(void) const
    {
        return jsmntok_t::type == JSMN_STRING;
    }

    bool JsmnToken::isArray(void) const
    {
        return jsmntok_t::type == JSMN_ARRAY;
    }

    bool JsmnToken::isObject(void) const
    {
        return jsmntok_t::type == JSMN_OBJECT;
    }

    const char* JsmnToken::typeString(void) const
    {
        switch(jsmntok_t::type)
        {
            case JSMN_PRIMITIVE:
                return "primitive";

            case JSMN_OBJECT:
                return "object";

            case JSMN_ARRAY:
                return "array";

            case JSMN_STRING:
                return "string";

            default:
                break;
        }

        return "unknown";
    }

    /* JsonValue */
    JsonArray & operator<< (JsonArray & jv, const int & st)
    {
        jv.addInteger(st);
        return jv;
    }

    JsonArray & operator<< (JsonArray & jv, const std::string & st)
    {
        jv.addString(st);
        return jv;
    }

    JsonArray & operator<< (JsonArray & jv, const double & st)
    {
        jv.addDouble(st);
        return jv;
    }

    JsonArray & operator<< (JsonArray & jv, const bool & st)
    {
        jv.addBoolean(st);
        return jv;
    }


    const JsonValue & operator>> (const JsonValue & jv, int & val)
    {
        val = jv.getInteger();
        return jv;
    }

    const JsonValue & operator>> (const JsonValue & jv, std::string & val)
    {
        val = jv.getString();
        return jv;
    }

    const JsonValue & operator>> (const JsonValue & jv, double & val)
    {
        val = jv.getDouble();
        return jv;
    }

    const JsonValue & operator>> (const JsonValue & jv, bool & val)
    {
        val = jv.getBoolean();
        return jv;
    }

    /* JsonString */
    bool JsonString::getBoolean(void) const
    {
        if(content.compare(0, 4, "fals") == 0)
            return false;
        else if(content.compare(0, 4, "true") == 0)
            return true;

        return getInteger();
    }

    std::string JsonString::escapeChars(std::string str)
    {
        for(auto ch : { '"', '\\' })
            str = String::escapeChar(str, ch);

        return str;
    }

    std::string JsonString::toString(void) const
    {
        return StringFormat("\"%1\"").arg(escapeChars(content));
    }

    /* JsonValuePtr */
    JsonValuePtr::JsonValuePtr()
    {
	reset(new JsonValue());
    }

    JsonValuePtr::JsonValuePtr(int v)
    {
	reset(new JsonInteger(v));
    }

    JsonValuePtr::JsonValuePtr(bool v)
    {
	reset(new JsonBoolean(v));
    }

    JsonValuePtr::JsonValuePtr(double v)
    {
	reset(new JsonDouble(v));
    }

    JsonValuePtr::JsonValuePtr(const std::string & v)
    {
	reset(new JsonString(v));
    }

    JsonValuePtr::JsonValuePtr(const JsonArray & v)
    {
	reset(new JsonArray(v));
    }

    JsonValuePtr::JsonValuePtr(const JsonObject & v)
    {
	reset(new JsonObject(v));
    }

    JsonValuePtr::JsonValuePtr(JsonValue* v)
    {
	reset(v);
    }

    JsonValuePtr::JsonValuePtr(const JsonValuePtr & v)
    {
	assign(v);
    }

    JsonValuePtr::JsonValuePtr(JsonValuePtr && v) noexcept
    {
	swap(v);
    }

    JsonValuePtr & JsonValuePtr::operator=(const JsonValuePtr & v)
    {
	assign(v);
	return *this;
    }

    JsonValuePtr & JsonValuePtr::operator=(JsonValuePtr && v) noexcept
    {
	swap(v);
	return *this;
    }

    void JsonValuePtr::assign(const JsonValuePtr & v)
    {
	JsonValue *val = nullptr;
	switch(v->getType())
	{
	    default:
	    case JsonType::Null:
		val = new JsonValue();
		break;

	    case JsonType::Integer:
		val = new JsonInteger(*static_cast<JsonInteger*>(v.get()));
		break;

	    case JsonType::Boolean:
		val = new JsonBoolean(*static_cast<JsonBoolean*>(v.get()));
		break;

	    case JsonType::Double:
		val = new JsonDouble(*static_cast<JsonDouble*>(v.get()));
		break;

	    case JsonType::String:
		val = new JsonString(*static_cast<JsonString*>(v.get()));
		break;

	    case JsonType::Object:
		val = new JsonObject(*static_cast<JsonObject*>(v.get()));
		break;

	    case JsonType::Array:
		val = new JsonArray(*static_cast<JsonArray*>(v.get()));
		break;
	}
	reset(val);
    }

    /* JsonContent */
    jsmntok_t* JsonContent::toJsmnTok(void)
    {
        return size() ? reinterpret_cast<jsmntok_t*>(& front()) : nullptr;
    }

    bool JsonContent::isValid(void) const
    {
        return size();
    }

    bool JsonContent::parseString(const std::string & str)
    {
        return parseBinary(str.c_str(), str.size());
    }

    bool JsonContent::parseBinary(const char* str, size_t len)
    {
        int counts = 0;

        do
        {
            jsmn_parser parser;
            jsmn_init(& parser);
            resize(size() + 128, JsmnToken());
            counts = jsmn_parse(& parser, str, len, toJsmnTok(), size());
        }
        while(counts == JSMN_ERROR_NOMEM);

        if(counts == JSMN_ERROR_INVAL)
        {
            ERROR("Invalid character inside JSON content");
            clear();
            return false;
        }
        else if(counts == JSMN_ERROR_PART)
        {
            ERROR("The content is not a full JSON packet, more bytes expected");
            clear();
            return false;
        }
        else if(counts < 0)
        {
            ERROR("Unknown error");
            clear();
            return false;
        }

        content.assign(str, len);
        resize(counts);

/*
        if(0)
        {
            VERBOSE("-------- tokens dump ----------");

            for(auto it = begin(); it != end(); ++it)
            {
                const JsmnToken & tok = *it;
                VERBOSE(std::distance(begin(), it) << ": " << tok.typeString() << ": " << content.substr(tok.start(), tok.size()) << ", counts: " << tok.counts());
            }
        }
*/

        return true;
    }

    bool JsonContent::readFile(const std::string & file)
    {
        BinaryBuf buf = Systems::readFile(file);
        return buf.size() ? parseBinary(reinterpret_cast<const char*>(buf.data()), buf.size()) : false;
    }

    std::string JsonContent::stringToken(const JsmnToken & tok) const
    {
        return content.substr(tok.start(), tok.size());
    }

    bool JsonContent::isArray(void) const
    {
        return isValid() && front().isArray();
    }

    bool JsonContent::isObject(void) const
    {
        return isValid() && front().isObject();
    }

    std::pair<JsonValue*, int>
    JsonContent::getValue(const const_iterator & it, JsonContainer* cont) const
    {
        const JsmnToken & tok = *it;

        if(tok.isArray())
        {
            int counts = tok.counts();
            int skip = 1;
            auto itval = it + skip;
            JsonArray* arr = cont ? static_cast<JsonArray*>(cont) : new JsonArray();

            while(counts-- && itval != end())
            {
                auto valp = getValue(itval, nullptr);

                if(valp.first)
                    arr->content.emplace_back(valp.first);

                skip += valp.second;
                itval = it + skip;
            }

    	    return std::make_pair(arr, skip);
        }
    
        if(tok.isObject())
        {
            int counts = tok.counts();
            int skip = 1;
            auto itkey = it + skip;
            auto itval = itkey + 1;
            JsonObject* obj = cont ? static_cast<JsonObject*>(cont) : new JsonObject();

    	    while(counts-- && itval != end())
    	    {
                if(!(*itkey).isKey()) ERROR("not key, index: " << std::distance(begin(), itkey) << ", key: \"" << stringToken(*itkey) << "\"");

                std::string key = stringToken(*itkey);
        	key = String::replace(key, "\\\"", "\"");
        	key = String::replace(key, "\\\\", "\\");

                auto valp = getValue(itval, nullptr);

                if(valp.first)
            	    obj->content.emplace(key, valp.first);

                skip += 1 + valp.second;
                itkey = it + skip;
                itval = itkey + 1;
    	    }

    	    return std::make_pair(obj, skip);
        }

        JsonValue* res = nullptr;
        int skip = 0;

        if(tok.isPrimitive())
        {
            const std::string & val = stringToken(tok);

            if(!(*it).isValue()) ERROR("not value, index: " << std::distance(begin(), it) << ", value: \"" << val << "\"");

            size_t dotpos = val.find(".");
            if(std::string::npos != dotpos)
            {
                bool convok = false;
                double vald = String::toDouble(val, & convok);
                int prec = val.size() - dotpos - 1;
                if(convok)
		    res = new JsonDouble(vald, prec);
            }
            else
            {
                bool convok = false;
                int vali = String::toInt(val, &convok);
                if(convok)
		    res = new JsonInteger(vali);
            }

            if(! res)
            {
                if(String::toLower(val).compare(0, 5, "false") == 0)
                    res = new JsonBoolean(false);
                else if(String::toLower(val).compare(0, 4, "true") == 0)
                    res = new JsonBoolean(true);
                else
                    res = new JsonValue();
            }

            skip = 1;
        }
        else
        {
            std::string val = stringToken(tok);

            if(!(*it).isValue())
		ERROR("not value, index: " << std::distance(begin(), it) << ", value: \"" << val << "\"");

            val = String::replace(val, "\\\"", "\"");
            val = String::replace(val, "\\n", "\n");
            val = String::replace(val, "\\r", "\r");
            val = String::replace(val, "\\t", "\t");
            val = String::replace(val, "\\\\", "\\");

            res = new JsonString(val);
            skip = 1;
        }

        return std::make_pair(res, skip);
    }

    JsonObject JsonContent::toObject(void) const
    {
        JsonObject res;

        if(isObject())
        {
            auto valp = getValue(begin(), & res);

            if(valp.first && valp.first != & res) delete valp.first;
        }

        return res;
    }

    JsonArray JsonContent::toArray(void) const
    {
        JsonArray res;

        if(isArray())
        {
            auto valp = getValue(begin(), & res);

            if(valp.first && valp.first != & res) delete valp.first;
        }

        return res;
    }

    /* JsonObject */
    JsonObject::JsonObject(const JsonObject & jo)
    {
        content.insert(jo.content.begin(), jo.content.end());
    }

    JsonObject::JsonObject(JsonObject && jo) noexcept
    {
        content.swap(jo.content);
    }

    JsonObject & JsonObject::operator=(const JsonObject & jo)
    {
	if(this != & jo)
	{
    	    content.clear();
    	    content.insert(jo.content.begin(), jo.content.end());
	}
        return *this;
    }

    JsonObject & JsonObject::operator=(JsonObject && jo) noexcept
    {
        content.swap(jo.content);
        return *this;
    }

    bool JsonObject::isValid(void) const
    {
	return ! content.empty();
    }

    int JsonObject::count(void) const
    {
        return content.size();
    }

    void JsonObject::clear(void)
    {
        return content.clear();
    }

    JsonType JsonObject::getType(void) const
    {
        return JsonType::Object;
    }

    int JsonObject::getInteger(void) const
    {
        return 0;
    }

    std::string JsonObject::getString(void) const
    {
        return "";
    }

    double JsonObject::getDouble(void) const
    {
        return 0;
    }

    bool JsonObject::getBoolean(void) const
    {
        return false;
    }

    bool JsonObject::hasKey(const std::string & key) const
    {
        return content.end() != content.find(key);
    }

    StringList JsonObject::keys(void) const
    {
        StringList res;

        for(auto it = content.begin(); it != content.end(); ++it)
            res << ((*it).first);

        return res;
    }

    bool JsonObject::isNull(const std::string & key) const
    {
        const JsonValue* jv = getValue(key);
        return jv && jv->isNull();
    }

    bool JsonObject::isBoolean(const std::string & key) const
    {
        const JsonValue* jv = getValue(key);
        return jv && jv->isBoolean();
    }

    bool JsonObject::isInteger(const std::string & key) const
    {
        const JsonValue* jv = getValue(key);
        return jv && jv->isInteger();
    }

    bool JsonObject::isDouble(const std::string & key) const
    {
        const JsonValue* jv = getValue(key);
        return jv && jv->isDouble();
    }

    bool JsonObject::isString(const std::string & key) const
    {
        const JsonValue* jv = getValue(key);
        return jv && jv->isString();
    }

    bool JsonObject::isObject(const std::string & key) const
    {
        const JsonValue* jv = getValue(key);
        return jv && jv->isObject();
    }

    bool JsonObject::isArray(const std::string & key) const
    {
        const JsonValue* jv = getValue(key);
        return jv && jv->isArray();
    }

    const JsonValue* JsonObject::getValue(const std::string & key) const
    {
        auto it = content.find(key);
        return it != content.end() ? (*it).second.get() : nullptr;
    }

    JsonType JsonObject::getType(const std::string & key) const
    {
        const JsonValue* jv = getValue(key);
        return jv ? jv->getType() : JsonType::Null;
    }

    int JsonObject::getInteger(const std::string & key, int def) const
    {
        const JsonValue* jv = getValue(key);
        return jv ? jv->getInteger() : def;
    }

    std::string JsonObject::getString(const std::string & key, std::string def) const
    {
        const JsonValue* jv = getValue(key);
        return jv ? jv->getString() : def;
    }

    double JsonObject::getDouble(const std::string & key, double def) const
    {
        const JsonValue* jv = getValue(key);
        return jv ? jv->getDouble() : def;
    }

    bool JsonObject::getBoolean(const std::string & key, bool def) const
    {
        const JsonValue* jv = getValue(key);
        return jv ? jv->getBoolean() : def;
    }

    const JsonObject* JsonObject::getObject(const std::string & key) const
    {
        auto jv = dynamic_cast<const JsonObject*>(getValue(key));
        return jv;
    }

    const JsonArray* JsonObject::getArray(const std::string & key) const
    {
        auto jv = dynamic_cast<const JsonArray*>(getValue(key));
        return jv;
    }

    std::string JsonObject::toString(void) const
    {
        std::ostringstream os;
        os << "{ ";

        for(auto it = content.begin(); it != content.end(); ++it)
        {
            if((*it).second)
            {

                os << "\"" << JsonString::escapeChars((*it).first) << "\": " << (*it).second->toString();
                if(std::next(it) != content.end()) os << ", ";
            }
        }

        os << " }";
        return os.str();
    }

    void JsonObject::addNull(const std::string & key)
    {
        auto it = content.find(key);
        if(it != content.end())
            (*it).second = JsonValuePtr();
        else
            content.emplace(key, JsonValuePtr());
    }

    void JsonObject::addInteger(const std::string & key, const int & val)
    {
	addValue<int>(key, val);
    }

    void JsonObject::addString(const std::string & key, const std::string & val)
    {
	addValue<std::string>(key, val);
    }

    void JsonObject::addDouble(const std::string & key, const double & val)
    {
	addValue<double>(key, val);
    }

    void JsonObject::addBoolean(const std::string & key, const bool & val)
    {
	addValue<bool>(key, val);
    }

    void JsonObject::addArray(const std::string & key, const JsonArray & val)
    {
	addValue<JsonArray>(key, val);
    }

    void JsonObject::addObject(const std::string & key, const JsonObject & val)
    {
	addValue<JsonObject>(key, val);
    }

    void JsonObject::join(const JsonObject & jo)
    {
        for(auto & pair : jo.content)
        {
            if(pair.second->isArray())
            {
                auto it = content.find(pair.first);
                if(it != content.end() && (*it).second->isArray())
                    static_cast<JsonArray*>((*it).second.get())->join(static_cast<const JsonArray &>(*pair.second.get()));
                else
                    content.emplace(pair.first, pair.second);
            }
            else
            if(pair.second->isObject())
            {
                auto it = content.find(pair.first);
                if(it != content.end() && (*it).second->isArray())
                    static_cast<JsonObject*>((*it).second.get())->join(static_cast<const JsonObject &>(*pair.second.get()));
                else
                    content.emplace(pair.first, pair.second);
            }
            else
            {
                auto it = content.find(pair.first);
                if(it != content.end())
                    (*it).second = pair.second;
                else
                    content.emplace(pair.first, pair.second);
            }
        }
    }

    /* JsonArray */
    JsonArray::JsonArray(const JsonArray & ja)
    {
        content.assign(ja.content.begin(), ja.content.end());
    }

    JsonArray::JsonArray(JsonArray && ja) noexcept
    {
        content.swap(ja.content);
    }

    JsonArray & JsonArray::operator=(const JsonArray & ja)
    {
        content.assign(ja.content.begin(), ja.content.end());
        return *this;
    }

    JsonArray & JsonArray::operator=(JsonArray && ja) noexcept
    {
        content.swap(ja.content);
        return *this;
    }

    int JsonArray::count(void) const
    {
        return content.size();
    }

    void JsonArray::clear(void)
    {
        return content.clear();
    }

    JsonType JsonArray::getType(void) const
    {
        return JsonType::Array;
    }

    int JsonArray::getInteger(size_t index) const
    {
        return isValid(index) ? content[index]->getInteger() : 0;
    }

    std::string JsonArray::getString(size_t index)  const
    {
        return isValid(index) ? content[index]->getString() : "";
    }

    double JsonArray::getDouble(size_t index)  const
    {
        return isValid(index) ? content[index]->getDouble() : 0;
    }

    bool JsonArray::getBoolean(size_t index) const
    {
        return isValid(index) ? content[index]->getBoolean() : false;
    }

    bool JsonArray::isValid(void) const
    {
        return ! content.empty();
    }

    bool JsonArray::isValid(size_t index) const
    {
        return index < content.size();
    }

    const JsonValue* JsonArray::getValue(size_t index) const
    {
        return index < content.size() ? content[index].get() : nullptr;
    }

    const JsonObject* JsonArray::getObject(size_t index) const
    {
        auto jo = dynamic_cast<const JsonObject*>(getValue(index));
        return jo;
    }

    const JsonArray* JsonArray::getArray(size_t index) const
    {
        auto ja = dynamic_cast<const JsonArray*>(getValue(index));
        return ja;
    }

    std::string JsonArray::toString(void) const
    {
        std::ostringstream os;
        os << "[ ";

        for(auto it = content.begin(); it != content.end(); ++it)
        {
            os << (*it)->toString();
            if(std::next(it) != content.end()) os << ", ";
        }

        os << " ]";
        return os.str();
    }

    void JsonArray::addInteger(const int & val)
    {
        content.emplace_back(val);
    }

    void JsonArray::addString(const std::string & val)
    {
        content.emplace_back(val);
    }

    void JsonArray::addDouble(const double & val)
    {
        content.emplace_back(val);
    }

    void JsonArray::addBoolean(const bool & val)
    {
        content.emplace_back(val);
    }

    void JsonArray::addArray(const JsonArray & val)
    {
        content.emplace_back(val);
    }

    void JsonArray::addObject(const JsonObject & val)
    {
        content.emplace_back(val);
    }

    void JsonArray::join(const JsonArray & ja)
    {
        if(content.size() > ja.content.size())
        { 
            for(int pos = 0; pos < ja.content.size(); ++pos)
            {
                auto & ptr1 = content[pos];
                auto & ptr2 = ja.content[pos];

                if(ptr2->isArray())
                {
                    if(ptr1->isArray())
                        static_cast<JsonArray*>(ptr1.get())->join(static_cast<const JsonArray &>(*ptr2.get()));
                    else
                        ptr1.assign(ptr2);
                }
                else
                if(ptr2->isObject())
                {
                    if(ptr1->isObject())
                        static_cast<JsonObject*>(ptr1.get())->join(static_cast<const JsonObject &>(*ptr2.get()));
                    else
                        ptr1.assign(ptr2);
                }
            }
        }
        else
        {
            content = ja.content;
        }
    }

    /* JsonContentFile */
    JsonContentFile::JsonContentFile(const std::string & file)
    {
        readFile(file);
    }

    /* JsonContentString */
    JsonContentString::JsonContentString(const std::string & str)
    {
        parseString(str);
    }
}

#endif
