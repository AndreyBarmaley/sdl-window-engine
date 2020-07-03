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
#include <initializer_list>

#include "swe_types.h"
#include "swe_colors.h"
#include "swe_cunicode_color.h"
#include "swe_cstring.h"
#include "swe_rect.h"
#include "jsmn/jsmn.h"

namespace SWE
{

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

        virtual int		getType(void) const
        {
            return TypeNull;
        }
        virtual std::string	toString(void) const
        {
            return "null";
        }

        bool		isNull(void) const
        {
            return getType() == TypeNull;
        }
        bool		isBoolean(void) const
        {
            return getType() == TypeBoolean;
        }
        bool		isInteger(void) const
        {
            return getType() == TypeInteger;
        }
        bool		isDouble(void) const
        {
            return getType() == TypeDouble;
        }
        bool		isString(void) const
        {
            return getType() == TypeString;
        }
        bool		isObject(void) const
        {
            return getType() == TypeObject;
        }
        bool		isArray(void) const
        {
            return getType() == TypeArray;
        }

        virtual int		getInteger(void) const
        {
            return 0;
        }
        virtual std::string	getString(void)  const
        {
            return "";
        }
        virtual double	getDouble(void)  const
        {
            return 0;
        }
        virtual bool	getBoolean(void) const
        {
            return false;
        }
    };

    const JsonValue & operator>> (const JsonValue &, int &);
    const JsonValue & operator>> (const JsonValue &, std::string &);
    const JsonValue & operator>> (const JsonValue &, double &);
    const JsonValue & operator>> (const JsonValue &, bool &);

    template<typename T1, typename T2>
    const JsonValue & operator>> (const JsonValue & jv, std::pair<T1, T2> & val)
    {
        return jv >> val.first >> val.second;
    }

    class JsonPrimitive : public JsonValue
    {
    public:
        JsonPrimitive() {}
        virtual ~JsonPrimitive() {}

        std::string		toString(void) const
        {
            return getString();
        }
    };

    class JsonString : public JsonPrimitive
    {
        std::string		content;

    public:
        JsonString(const std::string & val) : content(val) {}

        int			getInteger(void) const override
        {
            return String::toInt(content);
        }
        std::string		getString(void)  const override
        {
            return content;
        }
        double		getDouble(void)  const override
        {
            return String::toDouble(content);
        }
        bool		getBoolean(void) const override;

        int			getType(void) const override
        {
            return TypeString;
        }
        std::string		toString(void) const override;
    };

    class JsonDouble : public JsonPrimitive
    {
        double		content;
        int			prec;

    public:
        JsonDouble(const double & val, int prc = 8) : content(val), prec(prc) {}

        int			getInteger(void) const override
        {
            return content;
        }
        std::string		getString(void)  const override
        {
            return String::number(content, prec);
        }
        double		getDouble(void)  const override
        {
            return content;
        }
        bool		getBoolean(void) const override
        {
            return content;
        }

        int			getType(void) const override
        {
            return TypeDouble;
        }
    };

    class JsonInteger : public JsonPrimitive
    {
        int			content;

    public:
        JsonInteger(const int & val) : content(val) {}

        int			getInteger(void) const override
        {
            return content;
        }
        std::string		getString(void)  const override
        {
            return String::number(content);
        }
        double		getDouble(void)  const override
        {
            return content;
        }
        bool		getBoolean(void) const override
        {
            return content;
        }

        int			getType(void) const override
        {
            return TypeInteger;
        }
    };

    class JsonBoolean : public JsonPrimitive
    {
        bool		content;

    public:
        JsonBoolean(const bool & val) : content(val) {}

        int			getInteger(void) const override
        {
            return content;
        }
        std::string		getString(void)  const override
        {
            return String::Bool(content);
        }
        double		getDouble(void)  const override
        {
            return content;
        }
        bool		getBoolean(void) const override
        {
            return content;
        }

        int			getType(void) const override
        {
            return TypeBoolean;
        }
    };

    class JsonContainer : public JsonValue
    {
    public:
        JsonContainer() {}

        virtual bool	isValid(void) const
        {
            return false;
        }
        virtual int		count(void) const = 0;
        virtual void	clear(void) = 0;
    };

    class JsonArray : public JsonContainer
    {
        int			getInteger(void) const override
        {
            return 0;
        }
        std::string		getString(void) const override
        {
            return "";
        }
        double		getDouble(void) const override
        {
            return 0;
        }
        bool		getBoolean(void) const override
        {
            return false;
        }

    protected:
        std::vector<JsonValue*>
        content;
        friend class JsonContent;

    public:
        JsonArray() {}
        JsonArray(const std::initializer_list<int> &);
        JsonArray(const std::initializer_list<std::string> &);
        JsonArray(const JsonArray &);
        JsonArray(JsonArray && ja) noexcept;

        ~JsonArray();

        JsonArray 	&	operator=(const JsonArray &);
        JsonArray 	&	operator=(JsonArray && ja) noexcept;

        int			count(void) const override;
        void			clear(void) override;
        int			getType(void) const override;

        const JsonValue*	getValue(size_t index) const;
        const JsonObject*	getObject(size_t index) const;
        const JsonArray*	getArray(size_t index) const;

        std::string		toString(void) const override;

        int			getInteger(size_t index) const;
        std::string		getString(size_t index)  const;
        double			getDouble(size_t index)  const;
        bool			getBoolean(size_t index) const;
        bool			isValid(void) const override;
        bool			isValid(size_t index) const;

        void			addInteger(const int &);
        void			addString(const std::string &);
        void			addDouble(const double &);
        void			addBoolean(const bool &);

        void			addArray(const JsonArray &);
        void			addObject(const JsonObject &);

        template<typename T>
        std::vector<T> toStdVector(void) const
        {
            std::vector<T> res;
            res.reserve(content.size());

            for(auto it = content.begin(); it != content.end(); ++it)
            {
                const JsonValue* jv = *it;

                if(jv)
                {
                    res.push_back(T());
                    *jv >> res.back();
                }
            }

            return res;
        }

        template<typename T>
        std::list<T> toStdList(void) const
        {
            std::list<T> res;

            for(auto it = content.begin(); it != content.end(); ++it)
            {
                const JsonValue* jv = *it;

                if(jv)
                {
                    res.push_back(T());
                    *jv >> res.back();
                }
            }

            return res;
        }

        template<class T>
        const JsonArray & operator>> (std::vector<T> & v) const
        {
            for(auto it = content.begin(); it != content.end(); ++it)
            {
                const JsonValue* jv = *it;

                if(jv)
                {
                    v.push_back(T());
                    *jv >> v.back();
                }
            }

            return *this;
        }

        template<class T>
        const JsonArray & operator>> (std::list<T> & v) const
        {
            for(auto it = content.begin(); it != content.end(); ++it)
            {
                const JsonValue* jv = *it;

                if(jv)
                {
                    v.push_back(T());
                    *jv >> v.back();
                }
            }

            return *this;
        }
    };

    JsonArray & operator<< (JsonArray &, const int &);
    JsonArray & operator<< (JsonArray &, const std::string &);
    JsonArray & operator<< (JsonArray &, const double &);
    JsonArray & operator<< (JsonArray &, const bool &);

    class JsonObject : public JsonContainer
    {
        int			getInteger(void) const override;
        std::string		getString(void) const override;
        double			getDouble(void) const override;
        bool			getBoolean(void) const override;

    protected:
        std::unordered_map<std::string, JsonValue*>
        content;
        friend class JsonContent;

    public:
        JsonObject() {}
        JsonObject(const JsonObject &);
        JsonObject(JsonObject && jo) noexcept;

        ~JsonObject();

        JsonObject &	operator=(const JsonObject &);
        JsonObject &	operator=(JsonObject && jo) noexcept;

        int			count(void) const override;
        void			clear(void) override;
        int			getType(void) const override;
        bool			isValid(void) const override;

        std::string		toString(void) const override;
        bool			hasKey(const std::string &) const;
        StringList		keys(void) const;

        const JsonValue*	getValue(const std::string &) const;
        const JsonObject*	getObject(const std::string &) const;
        const JsonArray*	getArray(const std::string &) const;

        bool			isNull(const std::string &) const;
        bool			isBoolean(const std::string &) const;
        bool			isInteger(const std::string &) const;
        bool			isDouble(const std::string &) const;
        bool			isString(const std::string &) const;
        bool			isObject(const std::string &) const;
        bool			isArray(const std::string &) const;

        int			getType(const std::string &) const;
        int                 	getInteger(const std::string &, int def = 0) const;
        std::string         	getString(const std::string &, std::string def = "") const;
        double              	getDouble(const std::string &, double def = 0) const;
        bool                	getBoolean(const std::string &, bool def = false) const;

        void			addNull(const std::string &);
        void			addInteger(const std::string &, const int &);
        void			addString(const std::string &, const std::string &);
        void			addDouble(const std::string &, const double &);
        void			addBoolean(const std::string &, const bool &);

        void			addArray(const std::string &, const JsonArray &);
        void			addObject(const std::string &, const JsonObject &);

        StringList getStringList(const std::string & key) const
        {
            return getStdList<std::string>(key);
        }

        template<typename T>
        std::vector<T> getStdVector(const std::string & key) const
        {
            const JsonArray* jarr = getArray(key);
            return jarr ? jarr->toStdVector<T>() : std::vector<T>();
        }

        template<typename T>
        std::list<T> getStdList(const std::string & key) const
        {
            const JsonArray* jarr = getArray(key);
            return jarr ? jarr->toStdList<T>() : std::list<T>();
        }
    };

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

        bool			parseString(const std::string &);
        bool			parseBinary(const char*, size_t);
        bool			readFile(const std::string &);

        bool			isObject(void) const;
        bool			isArray(void) const;

        JsonArray		toArray(void) const;
        JsonObject		toObject(void) const;

        bool			isValid(void) const;
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

} // SWE
#endif
#endif
