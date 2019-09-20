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

#ifndef _SWE_JSONWRAPPER_
#define _SWE_JSONWRAPPER_

#ifdef WITH_JSON

#include <utility>
#include <unordered_map>

#include "types.h"
#include "sharedvector.h"
#include "sharedlist.h"
#include "sharedmap.h"
#include "colors.h"
#include "cunicode_color.h"
#include "cstring.h"
#include "rect.h"
#include "jsmn.h"

class JsmnToken : protected jsmntok_t
{
public:
    JsmnToken();

    int counts(void) const;
    int start(void) const;
    int size(void) const;

    bool isKey(void) const;
    bool isValue(void) const;
    bool isPrimitive(void) const;
    bool isString(void) const;
    bool isArray(void) const;
    bool isObject(void) const;

    const char* typeString(void) const;
};

enum JsonType { TypeNull, TypeInteger, TypeDouble, TypeString, TypeBoolean, TypeObject, TypeArray };

class JsonContent;
class JsonObject;
class JsonArray;

class JsonValue
{
public:
    JsonValue() {}
    virtual ~JsonValue() {}

    virtual int		getType(void) const { return TypeNull; }
    virtual std::string	toString(void) const { return "null"; }
    virtual bool	isValid(void) const { return false; }

    bool		isNull(void) const { return getType() == TypeNull; }
    bool		isBoolean(void) const { return getType() == TypeBoolean; }
    bool		isInteger(void) const { return getType() == TypeInteger; }
    bool		isDouble(void) const { return getType() == TypeDouble; }
    bool		isString(void) const { return getType() == TypeString; }
    bool		isObject(void) const { return getType() == TypeObject; }
    bool		isArray(void) const { return getType() == TypeArray; }

    virtual int		getInteger(int def = 0) const { return def; }
    virtual std::string	getString(std::string def = "") const { return def; }
    virtual double	getDouble(double def = 0) const { return def; }
    virtual bool	getBoolean(bool def = false) const { return def; }

    virtual Color	getColor(Color def = Color()) const { return def; }
    virtual Point	getPoint(Point def = Point()) const { return def; }
    virtual ZPoint	getZPoint(ZPoint def = ZPoint()) const { return def; }
    virtual Size	getSize(Size def = Size()) const { return def; }
    virtual Rect	getRect(Rect def = Rect()) const { return def; }

    virtual void	addInteger(const int &) {}
    virtual void	addString(const std::string &) {}
    virtual void	addDouble(const double &) {}
    virtual void	addBoolean(const bool &) {}

    virtual void	addColor(const Color &) {}
    virtual void	addPoint(const Point &) {}
    virtual void	addZPoint(const ZPoint &) {}
    virtual void	addSize(const Size &) {}
    virtual void	addRect(const Rect &) {}
};

const JsonValue & operator>> (const JsonValue &, int &);
const JsonValue & operator>> (const JsonValue &, std::string &);
const JsonValue & operator>> (const JsonValue &, double &);
const JsonValue & operator>> (const JsonValue &, bool &);

const JsonValue & operator>> (const JsonValue &, Point &);
const JsonValue & operator>> (const JsonValue &, ZPoint &);
const JsonValue & operator>> (const JsonValue &, Size &);
const JsonValue & operator>> (const JsonValue &, Rect &);
const JsonValue & operator>> (const JsonValue &, Color &);

JsonValue & operator<< (JsonValue &, const int &);
JsonValue & operator<< (JsonValue &, const std::string &);
JsonValue & operator<< (JsonValue &, const double &);
JsonValue & operator<< (JsonValue &, const bool &);

JsonValue & operator<< (JsonValue &, const Color &);
JsonValue & operator<< (JsonValue &, const Point &);
JsonValue & operator<< (JsonValue &, const ZPoint &);
JsonValue & operator<< (JsonValue &, const Size &);
JsonValue & operator<< (JsonValue &, const Rect &);

template<typename T1, typename T2>
const JsonValue & operator>> (const JsonValue & jv, std::pair<T1, T2> & val)
{
    return jv >> val.first >> val.second;
}

class JsonPrimitive : public JsonValue
{
protected:
    std::shared_ptr<void*> ptr;

    void		setInteger(const int &);
    void		setString(const std::string &);
    void		setDouble(const double &);

public:
    JsonPrimitive() {}
    virtual ~JsonPrimitive() {}

    bool		isValid(void) const { return ptr && ptr.get() && *ptr.get(); }
    const void*		operator() (void) const { return ptr && ptr.get() ? *ptr.get() : NULL; }

    std::string		toString(void) const { return getString(); }
};

class JsonString : public JsonPrimitive
{
public:
    JsonString(const std::string & val) { setString(val); }
    ~JsonString();

    int			getInteger(int def = 0) const;
    std::string		getString(std::string def = "") const;
    double		getDouble(double def = 0) const;
    bool		getBoolean(bool def = false) const;

    Color		getColor(Color def = Color()) const;

    int			getType(void) const { return TypeString; }
    std::string		toString(void) const;
};

class JsonDouble : public JsonPrimitive
{
    int			prec;

public:
    JsonDouble(const double & val, int prc = 8) : prec(prc) { setDouble(val); }
    ~JsonDouble();

    int			getInteger(int def = 0) const;
    std::string		getString(std::string def = "") const;
    double		getDouble(double def = 0) const;
    bool		getBoolean(bool def = false) const;

    int			getType(void) const { return TypeDouble; }
};

class JsonInteger : public JsonPrimitive
{
public:
    JsonInteger(const int & val) { setInteger(val); }
    ~JsonInteger();

    int			getInteger(int def = 0) const;
    std::string		getString(std::string def = "") const;
    double		getDouble(double def = 0) const;
    bool		getBoolean(bool def = false) const;

    Color		getColor(Color def = Color()) const;

    int			getType(void) const { return TypeInteger; }
};

class JsonBoolean : public JsonInteger
{
public:
    JsonBoolean(const bool & val) : JsonInteger(val ? 1 : 0) {}

    std::string		getString(std::string def = "false") const;
    int			getType(void) const { return TypeBoolean; }
};

class JsonContainer : public JsonValue
{
public:
    JsonContainer() {}

    virtual int		count(void) const = 0;
    virtual void	clear(void) = 0;
};

class JsonArray : public JsonContainer
{
protected:
    SharedVector<JsonValue*>
			content;
    friend class JsonContent;

public:
    JsonArray() {}
    ~JsonArray();

    int			count(void) const { return content.size(); }
    void		clear(void) { return content.clear(); }
    int			getType(void) const { return TypeArray; }
    const JsonValue*	getValue(size_t index) const;
    const JsonObject*	getObject(size_t index) const;
    const JsonArray*	getArray(size_t index) const;

    std::string		toString(void) const;
    Points		toPoints(void) const;
    Rects		toRects(void) const;

    bool		isValid(void) const { return ! content.empty(); }

    Point		getPoint(Point def = Point()) const;
    ZPoint		getZPoint(ZPoint def = ZPoint()) const;
    Size		getSize(Size def = Size()) const;
    Rect		getRect(Rect def = Rect()) const;
    Color		getColor(Color def = Color()) const;
    FBColors		getFBColors(FBColors def = FBColors()) const;

    void		addInteger(const int &);
    void		addString(const std::string &);
    void		addDouble(const double &);
    void		addBoolean(const bool &);

    void		addColor(const Color &);
    void		addPoint(const Point &);
    void		addZPoint(const ZPoint &);
    void		addSize(const Size &);
    void		addRect(const Rect &);
    void		addArray(const JsonArray &);
    void		addObject(const JsonObject &);

    template<typename T>
    SharedVector<T>     toArray(void) const
    {
        SharedVector<T> res;
        res.reserve(content.size());
        for(auto it = content.begin(); it != content.end(); ++it)
        {
	    const JsonValue* jv = *it;
	    if(jv) { res.push_back(T()); *jv >> res.back(); }
        }
        return res;
    }

    template<typename T>
    std::vector<T>     toStdVector(void) const
    {
        std::vector<T> res;
        res.reserve(content.size());
        for(auto it = content.begin(); it != content.end(); ++it)
        {
	    const JsonValue* jv = *it;
	    if(jv) { res.push_back(T()); *jv >> res.back(); }
        }
        return res;
    }

    template<typename T>
    SharedList<T>	toList(void) const
    {
        SharedList<T> res;
        for(auto it = content.begin(); it != content.end(); ++it)
        {
	    const JsonValue* jv = *it;
	    if(jv) { res.push_back(T()); *jv >> res.back(); }
        }
        return res;
    }

    template<typename T>
    std::list<T>	toStdList(void) const
    {
        std::list<T> res;
        for(auto it = content.begin(); it != content.end(); ++it)
        {
	    const JsonValue* jv = *it;
	    if(jv) { res.push_back(T()); *jv >> res.back(); }
        }
        return res;
    }

    template<class T>
    const JsonArray & operator>> (std::vector<T> & v) const
    {
        for(auto it = content.begin(); it != content.end(); ++it)
        {
	    const JsonValue* jv = *it;
	    if(jv) { v.push_back(T()); *jv >> v.back(); }
	}
        return *this;
    }

    template<class T>
    const JsonArray & operator>> (std::list<T> & v) const
    {
        for(auto it = content.begin(); it != content.end(); ++it)
        {
	    const JsonValue* jv = *it;
	    if(jv) { v.push_back(T()); *jv >> v.back(); }
	}
        return *this;
    }
};

class JsonObject : public JsonContainer
{
protected:
    SharedMap<std::string, JsonValue*>
			content;
    friend class JsonContent;

public:
    JsonObject() {}
    ~JsonObject();

    int			count(void) const { return content.size(); }
    void		clear(void) { return content.clear(); }
    int			getType(void) const { return TypeObject; }
    std::string		toString(void) const;
    bool		isValid(void) const { return ! content.empty(); }

    bool		hasKey(const std::string &) const;
    StringList		keys(void) const;

    const JsonValue*	getValue(const std::string &) const;
    const JsonObject*	getObject(const std::string &) const;
    const JsonArray*	getArray(const std::string &) const;

    bool		isNull(const std::string &) const;
    bool		isBoolean(const std::string &) const;
    bool		isInteger(const std::string &) const;
    bool		isDouble(const std::string &) const;
    bool		isString(const std::string &) const;
    bool		isObject(const std::string &) const;
    bool		isArray(const std::string &) const;

    int                 getInteger(const std::string &, int def = 0) const;
    std::string         getString(const std::string &, std::string def = "") const;
    double              getDouble(const std::string &, double def = 0) const;
    bool                getBoolean(const std::string &, bool def = false) const;

    Point		getPoint(Point def = Point()) const;
    ZPoint		getZPoint(ZPoint def = ZPoint()) const;
    Size		getSize(Size def = Size()) const;
    Rect		getRect(Rect def = Rect()) const;
    FBColors		getFBColors(FBColors def = FBColors()) const;
    UnicodeColor	getUnicodeColor(UnicodeColor def = UnicodeColor()) const;

    Point		getPoint(const std::string &, Point def = Point()) const;
    ZPoint		getZPoint(const std::string &, ZPoint def = ZPoint()) const;
    Size		getSize(const std::string &, Size def = Size()) const;
    Rect		getRect(const std::string &, Rect def = Rect()) const;
    Color		getColor(const std::string &, Color def = Color()) const;

    void		addInteger(const std::string &, const int &);
    void		addString(const std::string &, const std::string &);
    void		addDouble(const std::string &, const double &);
    void		addBoolean(const std::string &, const bool &);

    void		addColor(const std::string &, const Color &);
    void		addPoint(const std::string &, const Point &);
    void		addZPoint(const std::string &, const ZPoint &);
    void		addSize(const std::string &, const Size &);
    void		addRect(const std::string &, const Rect &);
    void		addArray(const std::string &, const JsonArray &);
    void		addObject(const std::string &, const JsonObject &);

    Points		getPoints(const std::string &) const;
    Rects		getRects(const std::string &) const;
    StringList		getStringList(const std::string &) const;

    FBColors		getFBColors(const std::string &) const;
    UnicodeColor	getUnicodeColor(const std::string &) const;

    template<typename T>
    SharedVector<T>     getArray(const std::string & key) const
    {
	const JsonArray* jarr = getArray(key);
        return jarr ? jarr->toArray<T>() : SharedVector<T>();
    }

    template<typename T>
    SharedList<T>       getList(const std::string & key) const
    {
	const JsonArray* jarr = getArray(key);
        return jarr ? jarr->toList<T>() : SharedList<T>();
    }

    template<typename T>
    std::vector<T>     getStdVector(const std::string & key) const
    {
	const JsonArray* jarr = getArray(key);
        return jarr ? jarr->toStdVector<T>() : std::vector<T>();
    }

    template<typename T>
    std::list<T>      getStdList(const std::string & key) const
    {
	const JsonArray* jarr = getArray(key);
        return jarr ? jarr->toStdList<T>() : std::list<T>();
    }
};

namespace JsonPack
{
    JsonArray points(const Points &);
    JsonArray rects(const Rects &);
    JsonArray stringList(const StringList &);
    JsonObject fbColors(const FBColors &);

    template<typename T>
    JsonArray stdList(const std::list<T> & v)
    {
	JsonArray ja;
	for(auto it = v.begin(); it != v.end(); ++it)
    	    ja << *it;
	return ja;
    }

    template<typename T>
    JsonArray stdVector(const std::vector<T> & v)
    {
	JsonArray ja;
	for(auto it = v.begin(); it != v.end(); ++it)
    	    ja << *it;
	return ja;
    }

    template<typename T>
    JsonArray sharedVector(const SharedVector<T> & v)
    {
	JsonArray ja;
	for(auto it = v.begin(); it != v.end(); ++it)
    	    ja << *it;
	return ja;
    }

    template<typename T>
    JsonArray sharedList(const SharedList<T> & v)
    {
	JsonArray ja;
	for(auto it = v.begin(); it != v.end(); ++it)
    	    ja << *it;
	return ja;
    }
}

class JsonContent : protected std::vector<JsmnToken>
{
    std::string		content;

protected:
    std::string		stringTocken(const JsmnToken &) const;
    jsmntok_t*		toJsmnTok(void);
    std::pair<JsonValue*, int>
    			getValue(const const_iterator &, JsonContainer* cont = NULL) const;

public:
    JsonContent() {}

    bool		parseString(const std::string &);
    bool		parseBinary(const char*, size_t);
    bool		readFile(const std::string &);

    bool		isObject(void) const;
    bool		isArray(void) const;

    JsonArray		toArray(void) const;
    JsonObject		toObject(void) const;

    bool		isValid(void) const { return size(); }
};

class JsonContentString : public JsonContent
{
public:
    JsonContentString(const std::string &);
};

class JsonContentFile : public JsonContent
{
public:
    JsonContentFile(const std::string &);
};

#endif
#endif
