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

#ifdef WITH_JSON

#include <sstream>
#include <cctype>
#include <algorithm>

#include "systems.h"
#include "json_wrapper.h"

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
        case JSMN_PRIMITIVE:    return "primitive";
        case JSMN_OBJECT:       return "object";
        case JSMN_ARRAY:        return "array";
        case JSMN_STRING:       return "string";
        default: break;
    }

    return "unknown";
}

/* JsonValue */
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

const JsonValue & operator>> (const JsonValue & jv, Point & val)
{
    val = jv.getPoint();
    return jv;
}

const JsonValue & operator>> (const JsonValue & jv, ZPoint & val)
{
    val = jv.getZPoint();
    return jv;
}

const JsonValue & operator>> (const JsonValue & jv, Size & val)
{
    val = jv.getSize();
    return jv;
}

const JsonValue & operator>> (const JsonValue & jv, Rect & val)
{
    val = jv.getRect();
    return jv;
}

const JsonValue & operator>> (const JsonValue & jv, Color & val)
{
    val = jv.getColor();
    return jv;
}

/* JsonPrimitive */
void JsonPrimitive::setInteger(const int & val)
{
    ptr = std::make_shared<void*>(new int(val));
}

void JsonPrimitive::setString(const std::string & val)
{
    ptr = std::make_shared<void*>(new std::string(val));
}

void JsonPrimitive::setDouble(const double & val)
{
    ptr = std::make_shared<void*>(new double(val));
}

/* JsonString */
JsonString::~JsonString()
{
    if(isValid() && ptr.unique())
    {
	std::string* res = reinterpret_cast<std::string*>(*ptr.get());
	delete res;
    }
}

int JsonString::getInteger(int def) const
{
    const std::string & val = getString();
    bool ok = false;
    int res = String::toInt(val, & ok);
    return ok ? res : def;
}

std::string JsonString::getString(std::string def) const
{
    const JsonPrimitive & val = *this;
    const std::string* res = reinterpret_cast<const std::string*>(val());
    return res ? *res : def;
}

double JsonString::getDouble(double def) const
{
    const std::string & val = getString();
    bool ok = false;
    double res = String::toDouble(val, & ok);
    return ok ? res : def;
}

bool JsonString::getBoolean(bool def) const
{
    const std::string & val = getString();
    if(val.compare(0, 4, "fals") == 0)
	return false;
    else
    if(val.compare(0, 4, "true") == 0)
	return true;

    return getInteger(0);
}

std::string JsonString::toString(void) const
{
    return StringFormat("\"%1\"").arg(getString());
}

Color JsonString::getColor(Color def) const
{
    return Color(getString());
}

/* JsonDouble */
JsonDouble::~JsonDouble()
{
    if(isValid() && ptr.unique())
    {
	double* res = reinterpret_cast<double*>(*ptr.get());
	delete res;
    }
}

int JsonDouble::getInteger(int def) const
{
    return isValid() ? getDouble() : def;
}

std::string JsonDouble::getString(std::string def) const
{
    return isValid() ? String::number(getDouble(), prec) : def;
}

double JsonDouble::getDouble(double def) const
{
    const JsonPrimitive & val = *this;
    const double* res = reinterpret_cast<const double*>(val());
    return res ? *res : def;
}

bool JsonDouble::getBoolean(bool def) const
{
    return isValid() ? getInteger() : def;
}

/* JsonInteger */
JsonInteger::~JsonInteger()
{
    if(isValid() && ptr.unique())
    {
	int* res = reinterpret_cast<int*>(*ptr.get());
	delete res;
    }
}

int JsonInteger::getInteger(int def) const
{
    const JsonPrimitive & val = *this;
    const int* res = reinterpret_cast<const int*>(val());
    return res ? *res : def;
}

std::string JsonInteger::getString(std::string def) const
{
    return isValid() ? String::number(getInteger()) : def;
}

double JsonInteger::getDouble(double def) const
{
    return isValid() ? getInteger() : def;
}

bool JsonInteger::getBoolean(bool def) const
{
    return isValid() ? getInteger() : def;
}

Color JsonInteger::getColor(Color def) const
{
    return Color(ARGB(getInteger()));
}

/* JsonBoolean */
std::string JsonBoolean::getString(std::string def) const
{
    return isValid() ? (getBoolean() ? "true" : "false") : def;
}

/* JsonContent */
jsmntok_t* JsonContent::toJsmnTok(void)
{
    return size() ? reinterpret_cast<jsmntok_t*>(& front()) : NULL;
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
        return false;
    }
    else
    if(counts == JSMN_ERROR_PART)
    {
        ERROR("The content is not a full JSON packet, more bytes expected");
        return false;
    }
    else
    if(counts < 0)
    {
        ERROR("Unknown error");
        return false;
    }

    content.assign(str, len);
    resize(counts);

    if(0)
    {
	VERBOSE("-------- tokens dump ----------");

	for(auto it = begin(); it != end(); ++it)
	{
    	    const JsmnToken & tok = *it;
    	    VERBOSE(std::distance(begin(), it) << ": " << tok.typeString() << ": " << content.substr(tok.start(), tok.size()) << ", counts: " << tok.counts());
	}
    }

    return true;
}

bool JsonContent::readFile(const std::string & file)
{
    BinaryBuf buf = Systems::readFile(file);
    return buf.size() ? parseBinary(reinterpret_cast<const char*>(buf.data()), buf.size()) : false;
}

std::string JsonContent::stringTocken(const JsmnToken & tok) const
{
    return content.substr(tok.start(), tok.size());
}

bool JsonContent::isArray(void) const
{
    return size() && front().isArray();
}

bool JsonContent::isObject(void) const
{
    return size() && front().isObject();
}

std::pair<JsonValue*, int>
JsonContent::getValue(const const_iterator & it, JsonContainer* cont) const
{
    const JsmnToken & tok = *it;
    JsonValue* res = NULL;
    int skip = 0;

    if(tok.isArray())
    {
	int counts = tok.counts();
	skip = 1;
	auto itval = it + skip;
	JsonArray* arr = cont ? static_cast<JsonArray*>(cont) : new JsonArray();

	while(counts-- && itval != end())
	{
	    auto valp = getValue(itval);
	    arr->content.push_back(valp.first);
	    skip += valp.second;
	    itval = it + skip;
	}

	res = arr;
    }
    else
    if(tok.isObject())
    {
	int counts = tok.counts();
	skip = 1;
	auto itkey = it + skip;
	auto itval = itkey + 1;
	JsonObject* obj = cont ? static_cast<JsonObject*>(cont) : new JsonObject();

	while(counts-- && itval != end())
	{
	    if(! (*itkey).isKey()) ERROR("not key, index: " << std::distance(begin(), itkey) << ", key: \"" << stringTocken(*itkey) << "\"");

	    std::string key = stringTocken(*itkey);
	    auto valp = getValue(itval);
	    obj->content.insert(key, valp.first);
	    skip += 1 + valp.second;
	    itkey = it + skip;
	    itval = itkey + 1;
	}

	res = obj;
    }
    else
    if(tok.isPrimitive())
    {
	const std::string & val = stringTocken(tok);
	if(! (*it).isValue()) ERROR("not value, index: " << std::distance(begin(), it) << ", value: \"" << val <<"\"");

	size_t dotpos = val.find(".");

	if(std::string::npos != dotpos)
	{
	    bool convok = false;
	    double vald = String::toDouble(val, &convok);
	    int prec = val.size() - dotpos - 1;
    	    if(convok) res = new JsonDouble(vald, prec);
	}
	else
	{
	    bool convok = false;
	    int vali = String::toInt(val, &convok);
    	    if(convok) res = new JsonInteger(vali);
	}

	if(! res)
	{
	    if(String::toLower(val).compare(0, 5, "false") == 0)
    		res = new JsonBoolean(false);
	    else
	    if(String::toLower(val).compare(0, 4, "true") == 0)
    		res = new JsonBoolean(true);
	    else
	    if(String::toLower(val).compare(0, 4, "null") == 0)
    		res = new JsonValue();
	}

	skip = 1;
    }
    else
    {
	const std::string & val = stringTocken(tok);
	if(! (*it).isValue()) ERROR("not value, index: " << std::distance(begin(), it) << ", value: \"" << val <<"\"");
	res = new JsonString(val);
	skip = 1;
    }

    return std::make_pair(res, skip);
}

JsonObject JsonContent::toObject(void) const
{
    JsonObject res;

    if(isObject())
	getValue(begin(), & res);

    return res;
}


JsonArray JsonContent::toArray(void) const
{
    JsonArray res;

    if(isArray())
	getValue(begin(), & res);

    return res;
}

/* JsonObject */
JsonObject::~JsonObject()
{
    if(content.unique())
    {
	for(auto it = content.begin(); it != content.end(); ++it)
	    delete (*it).second;
	content.clear();
    }
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
    return it != content.end() ? (*it).second : NULL;
}

int JsonObject::getInteger(const std::string & key, int def) const
{
    const JsonValue* jv = getValue(key);
    return jv ? jv->getInteger(def) : def;
}

std::string JsonObject::getString(const std::string & key, std::string def) const
{
    const JsonValue* jv = getValue(key);
    return jv ? jv->getString(def) : def;
}

double JsonObject::getDouble(const std::string & key, double def) const
{
    const JsonValue* jv = getValue(key);
    return jv ? jv->getDouble(def) : def;
}

bool JsonObject::getBoolean(const std::string & key, bool def) const
{
    const JsonValue* jv = getValue(key);
    return jv ? jv->getBoolean(def) : def;
}

Point JsonObject::getPoint(const std::string & key, Point def) const
{
    const JsonContainer* jv = static_cast<const JsonContainer*>(getValue(key));
    return jv ? jv->getPoint(def) : def;
}

ZPoint JsonObject::getZPoint(const std::string & key, ZPoint def) const
{
    const JsonContainer* jv = static_cast<const JsonContainer*>(getValue(key));
    return jv ? jv->getZPoint(def) : def;
}

Size JsonObject::getSize(const std::string & key, Size def) const
{
    const JsonContainer* jv = static_cast<const JsonContainer*>(getValue(key));
    return jv ? jv->getSize(def) : def;
}

Rect JsonObject::getRect(const std::string & key, Rect def) const
{
    const JsonContainer* jv = static_cast<const JsonContainer*>(getValue(key));
    return jv ? jv->getRect(def) : def;
}

Color JsonObject::getColor(const std::string & key, Color def) const
{
    const JsonValue* jv = getValue(key);
    return jv ? jv->getColor() : def;
}

const JsonObject* JsonObject::getObject(const std::string & key) const
{
    const JsonObject* jv = static_cast<const JsonObject*>(getValue(key));
    return jv;
}

const JsonArray* JsonObject::getArray(const std::string & key) const
{
    const JsonArray* jv = static_cast<const JsonArray*>(getValue(key));
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
	    os << "\"" << (*it).first << "\": " << (*it).second->toString();
	    auto itend = it;
	    if(++itend != content.end()) os << ", ";
	}
    }
    os << " }";
    return os.str();
}

Point JsonObject::getPoint(Point def) const
{
    const JsonValue* val = NULL;

    val = getValue("posx");
    if(! val) val = getValue("x");
    if(val) def.x = val->getInteger(def.x);

    val = getValue("posy");
    if(! val) val = getValue("y");
    if(val) def.y = val->getInteger(def.y);

    return def;
}

ZPoint JsonObject::getZPoint(ZPoint def) const
{
    Point pt = getPoint(def);
    def.x = pt.x;
    def.y = pt.y;

    const JsonValue* val = getValue("posz");
    if(! val) val = getValue("z");
    if(val) def.z = val->getInteger(def.z);

    return def;
}

Size JsonObject::getSize(Size def) const
{
    const JsonValue* val = NULL;

    val = getValue("width");
    if(! val) val = getValue("w");
    if(val) def.w = val->getInteger(def.w);

    val = getValue("height");
    if(! val) val = getValue("h");
    if(val) def.h = val->getInteger(def.h);

    return def;
}

Rect JsonObject::getRect(Rect def) const
{
    Point pt = getPoint(def.toPoint());
    Size sz = getSize(def.toSize());

    def.x = pt.x;
    def.y = pt.y;
    def.w = sz.w;
    def.h = sz.h;

    return def;
}

FBColors JsonObject::getFBColors(FBColors def) const
{
    const JsonValue* val = NULL;

    val = getValue("fg");
    if(! val) val = getValue("foreground");
    if(val) def.setfg(val->getColor(def.fgcolor()).toColorIndex());

    val = getValue("bg");
    if(! val) val = getValue("background");
    if(val) def.setbg(val->getColor(def.bgcolor()).toColorIndex());

    return def;
}

UnicodeColor JsonObject::getUnicodeColor(UnicodeColor def) const
{
    const JsonValue* val = NULL;

    val = getValue("symbol");
    if(! val) val = getValue("sym");
    if(val) def.unicode(val->getInteger());

    val = getValue("colors");
    if(val)
    {
	if(val->isArray())
	{
	    const JsonArray* ja = static_cast<const JsonArray*>(val);
	    def.colors(ja->getFBColors(def.colors()));
	}
	else
	if(val->isObject())
	{
	    const JsonObject* jo = static_cast<const JsonObject*>(val);
	    def.colors(jo->getFBColors(def.colors()));
	}
    }

    return def;
}

Points JsonObject::getPoints(const std::string & key) const
{
    return getArray<Point>(key);
}

Rects JsonObject::getRects(const std::string & key) const
{
    return getArray<Rect>(key);
}

StringList JsonObject::getStringList(const std::string & key) const
{
    return getList<std::string>(key);
}

FBColors JsonObject::getFBColors(const std::string & key) const
{
    FBColors res;
    const JsonValue* val = NULL;

    val = getValue(key);
    if(val)
    {
	if(val->isArray())
	{
	    const JsonArray* ja = static_cast<const JsonArray*>(val);
	    res = ja->getFBColors();
	}
	else
	if(val->isObject())
	{
	    const JsonObject* jo = static_cast<const JsonObject*>(val);
	    res = jo->getFBColors();
	}
    }

    return res;
}

UnicodeColor JsonObject::getUnicodeColor(const std::string & key) const
{
    UnicodeColor res;
    const JsonValue* val = NULL;

    val = getValue("symbol");
    if(! val) val = getValue("sym");
    if(val) res.unicode(val->getInteger());

    val = getValue("colors");
    if(val)
    {
	if(val->isArray())
	{
	    const JsonArray* ja = static_cast<const JsonArray*>(val);
	    res.colors(ja->getFBColors());
	}
	else
	if(val->isObject())
	{
	    const JsonObject* jo = static_cast<const JsonObject*>(val);
	    res.colors(jo->getFBColors());
	}
    }

    return res;
}

void JsonObject::addInteger(const std::string & key, const int & val)
{
    content.insert(key, new JsonInteger(val));
}

void JsonObject::addString(const std::string & key, const std::string & val)
{
    content.insert(key, new JsonString(val));
}

void JsonObject::addDouble(const std::string & key, const double & val)
{
    content.insert(key, new JsonDouble(val));
}

void JsonObject::addBoolean(const std::string & key, const bool & val)
{
    content.insert(key, new JsonBoolean(val));
}

void JsonObject::addColor(const std::string & key, const Color & val)
{
    content.insert(key, new JsonString(val.toString()));
}

void JsonObject::addPoint(const std::string & key, const Point & val)
{
    JsonArray* ja = new JsonArray();
    if(ja)
    {
	ja->addInteger(val.x);
	ja->addInteger(val.y);
	content.insert(key, ja);
    }
}

void JsonObject::addZPoint(const std::string & key, const ZPoint & val)
{
    JsonArray* ja = new JsonArray();
    if(ja)
    {
	ja->addInteger(val.x);
	ja->addInteger(val.y);
	ja->addInteger(val.z);
	content.insert(key, ja);
    }
}

void JsonObject::addSize(const std::string & key, const Size & val)
{
    JsonArray* ja = new JsonArray();
    if(ja)
    {
	ja->addInteger(val.w);
	ja->addInteger(val.h);
	content.insert(key, ja);
    }
}

void JsonObject::addRect(const std::string & key, const Rect & val)
{
    JsonArray* ja = new JsonArray();
    if(ja)
    {
	ja->addInteger(val.x);
	ja->addInteger(val.y);
	ja->addInteger(val.w);
	ja->addInteger(val.h);
	content.insert(key, ja);
    }
}

void JsonObject::addArray(const std::string & key, const JsonArray & val)
{
    JsonArray* ja = new JsonArray(val);
    if(ja) content.insert(key, ja);
}

void JsonObject::addObject(const std::string & key, const JsonObject & val)
{
    JsonObject* jo = new JsonObject(val);
    if(jo) content.insert(key, jo);
}

/* JsonArray */
JsonArray::~JsonArray()
{
    if(content.unique())
    {
	for(auto it = content.begin(); it != content.end(); ++it)
	    delete *it;
	content.clear();
    }
}

const JsonValue* JsonArray::getValue(size_t index) const
{
    return index < content.size() ? content[index] : NULL;
}

const JsonObject* JsonArray::getObject(size_t index) const
{
    const JsonObject* jo = static_cast<const JsonObject*>(getValue(index));
    return jo;
}

const JsonArray* JsonArray::getArray(size_t index) const
{
    const JsonArray* ja = static_cast<const JsonArray*>(getValue(index));
    return ja;
}

std::string JsonArray::toString(void) const
{
    std::ostringstream os;
    os << "[ ";
    for(auto it = content.begin(); it != content.end(); ++it)
    {
	if(*it)
	{
	    os << (*it)->toString();
	    if(it + 1 != content.end()) os << ", ";
	}
    }
    os << " ]";
    return os.str();
}

Points JsonArray::toPoints(void) const
{
    Points res;
    res.resize(count());

    for(int it = 0; it < count(); ++it)
    {
	const JsonValue* jv = getValue(it);
	if(jv)  *jv >> res[it];
    }

    return res;
}

Rects JsonArray::toRects(void) const
{
    Rects res;
    res.resize(count());

    for(int it = 0; it < count(); ++it)
    {
	const JsonValue* jv = getValue(it);
	if(jv) *jv >> res[it];
    }

    return res;
}

Point JsonArray::getPoint(Point def) const
{
    const JsonValue* val = NULL;

    val = getValue(0);
    if(val) def.x = val->getInteger(def.x);

    val = getValue(1);
    if(val) def.y = val->getInteger(def.y);

    return def;
}

ZPoint JsonArray::getZPoint(ZPoint def) const
{
    Point pt = getPoint(def);
    def.x = pt.x;
    def.y = pt.y;

    const JsonValue* val = getValue(2);
    if(val) def.z = val->getInteger(def.z);

    return def;
}

Size JsonArray::getSize(Size def) const
{
    const JsonValue* val = NULL;

    val = getValue(0);
    if(val) def.w = val->getInteger(def.w);

    val = getValue(1);
    if(val) def.h = val->getInteger(def.h);

    return def;
}

Rect JsonArray::getRect(Rect def) const
{
    const JsonValue* val = NULL;

    val = getValue(0);
    if(val) def.x = val->getInteger(def.x);

    val = getValue(1);
    if(val) def.y = val->getInteger(def.y);

    val = getValue(2);
    if(val) def.w = val->getInteger(def.w);

    val = getValue(3);
    if(val) def.h = val->getInteger(def.h);

    return def;
}

Color JsonArray::getColor(Color def) const
{
    const JsonValue* val = NULL;
    int rv = 0;
    int gv = 0;
    int bv = 0;

    val = getValue(0);
    if(val) rv = val->getInteger(def.r());

    val = getValue(1);
    if(val) gv = val->getInteger(def.g());

    val = getValue(2);
    if(val) bv = val->getInteger(def.b());

    val = getValue(3);
    return val ? Color(rv, gv, bv, val->getInteger(def.a())) : Color(rv, gv, bv);
}

FBColors JsonArray::getFBColors(FBColors def) const
{
    const JsonValue* val = NULL;

    val = getValue(0);
    if(val) def.setfg(val->getColor(def.fgcolor()).toColorIndex());

    val = getValue(1);
    if(val) def.setbg(val->getColor(def.bgcolor()).toColorIndex());

    return def;
}

void JsonArray::addInteger(const int & val)
{
    content.push_back(new JsonInteger(val));
}

void JsonArray::addString(const std::string & val)
{
    content.push_back(new JsonString(val));
}

void JsonArray::addDouble(const double & val)
{
    content.push_back(new JsonDouble(val));
}

void JsonArray::addBoolean(const bool & val)
{
    content.push_back(new JsonBoolean(val));
}

void JsonArray::addColor(const Color & val)
{
    content.push_back(new JsonString(val.toString()));
}

void JsonArray::addPoint(const Point & val)
{
    JsonArray* ja = new JsonArray();
    if(ja)
    {
	ja->addInteger(val.x);
	ja->addInteger(val.y);
	content.push_back(ja);
    }
}

void JsonArray::addZPoint(const ZPoint & val)
{
    JsonArray* ja = new JsonArray();
    if(ja)
    {
	ja->addInteger(val.x);
	ja->addInteger(val.y);
	ja->addInteger(val.z);
	content.push_back(ja);
    }
}

void JsonArray::addSize(const Size & val)
{
    JsonArray* ja = new JsonArray();
    if(ja)
    {
	ja->addInteger(val.w);
	ja->addInteger(val.h);
	content.push_back(ja);
    }
}

void JsonArray::addRect(const Rect & val)
{
    JsonArray* ja = new JsonArray();
    if(ja)
    {
	ja->addInteger(val.x);
	ja->addInteger(val.y);
	ja->addInteger(val.w);
	ja->addInteger(val.h);
	content.push_back(ja);
    }
}

void JsonArray::addArray(const JsonArray & val)
{
    JsonArray* ja = new JsonArray(val);
    if(ja) content.push_back(ja);
}

void JsonArray::addObject(const JsonObject & val)
{
    JsonObject* jo = new JsonObject(val);
    if(jo) content.push_back(jo);
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

/* JsonPack */
JsonArray JsonPack::points(const Points & st)
{
    JsonArray ja;
    for(auto it = st.begin(); it != st.end(); ++it)
        ja.addPoint(*it);
    return ja;
}

JsonArray JsonPack::rects(const Rects & st)
{
    JsonArray ja;
    for(auto it = st.begin(); it != st.end(); ++it)
        ja.addRect(*it);
    return ja;
}

JsonArray JsonPack::stringList(const StringList & v)
{
    JsonArray ja;
    for(auto it = v.begin(); it != v.end(); ++it)
	ja.addString(*it);
    return ja;
}

JsonObject JsonPack::fbColors(const FBColors & v)
{
    JsonObject jo;
    jo.addString("fg", v.fgcolor().toString());
    jo.addString("bg", v.bgcolor().toString());
    return jo;
}

#endif /* WITH_JSON */
