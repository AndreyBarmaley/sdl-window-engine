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

#ifndef _SWE_TOOLS_
#define _SWE_TOOLS_

#include <list>
#include <bitset>
#include <vector>
#include <memory>
#include <sstream>
#include <iomanip>
#include <numeric>
#include <iterator>
#include <algorithm>

#include "swe_types.h"
#include "swe_binarybuf.h"
#include "swe_rect.h"
#include "swe_systems.h"
#include "swe_serialize.h"

/// @brief пространство SWE
namespace SWE
{
    namespace Tools
    {
        void		delay(unsigned int ms);
        int		countBits(unsigned long);

        int		rand(int min, int max);
        float		randf(float min, float max);

        u32		crc32b(const std::string &);
        u32		crc32b(const u8*, size_t);
        u32		crc32b(const u8*, size_t, u32 magic);

        int		crc16b(const std::string &);
        int		crc16b(const u8*, size_t);
        int		crc16b(const u8*, size_t, u16 magic);

        u32		ticks(void);
        int		signValue(int v);

        std::pair<int, float>
    			modf(float);

        std::string	stringEncode(const std::string &, const char* charset);

        BinaryBuf	zlibUncompress(const char*, size_t, size_t real = 0);
        BinaryBuf	zlibCompress(const char*, size_t);

        BinaryBuf	zlibUncompress(const u8*, size_t, size_t real = 0);
        BinaryBuf	zlibCompress(const u8*, size_t);

        BinaryBuf	base64Decode(const char*);
        BinaryBuf	base64Decode(const u8*, size_t);

        BinaryBuf	base64Encode(const char*);
        BinaryBuf	base64Encode(const u8*, size_t);

        Points		renderCircle(const Point &, int, bool fill = false);
        Points		renderLine(const Point &, const Point &, int step = 1);

        template<typename InputIterator>
        InputIterator   random_n(InputIterator first, InputIterator last)
        {
            auto dist = std::distance(first, last);
            InputIterator res = first;

            if(1 < dist)
                std::advance(res, rand(0, dist - 1));

            return res;
        }

        template<typename Int>
        std::string buffer2HexString(const Int* data, size_t length, size_t width = 8, const std::string & sep = ",", bool prefix = true)
        {
            std::ostringstream os;
            for(size_t it = 0; it != length; ++it)
            {
                if(prefix)
                    os << "0x";
                os << std::setw(width) << std::setfill('0') << std::uppercase << std::hex << static_cast<int>(data[it]);
                if(sep.size() && it + 1 != length) os << sep;
            }

            return os.str();
        }

	/* RandQueue */
        template<typename T>
        class RandQueue : protected std::vector< std::pair<T, size_t> >
        {
	protected:
            size_t getMaxWeight(void) const
            {
                return std::accumulate(this->begin(), this->end(), 0,
                                    [](size_t v, auto & pair){ return v += pair.second; });
            }

        public:
            RandQueue(size_t size = 0)
            {
                if(size) this->reserve(size);
            }

            void push(const T & value, size_t weight)
            {
                if(weight) this->emplace_back(value, weight);
            }

	    bool isValid(void) const
	    {
		return 0 < this->size();
	    }

            T	 get(void)
            {
                if(isValid())
                {
                    std::vector<size_t> percents;
                    percents.reserve(this->size());
                    // get max
                    size_t max = getMaxWeight();

                    // set weights
                    for(auto & pair : *this)
                        percents.push_back(100 * pair.second / max);

                    // calc max
                    max = std::accumulate(percents.begin(), percents.end(), 0,
                                    [](size_t v, auto & val){ return v += val; });

                    size_t rnd = rand(0, max);
                    size_t amount = 0;

                    // get rnd
                    auto it = std::find_if(percents.begin(), percents.end(),
                                            [&](auto & val){ amount += val; return rnd <= amount; });

                    if(it != percents.end())
                        return this->operator[](std::distance(percents.begin(), it)).first;
                }

                ERROR("weight not found");
                return T();
            }
        };

	/* RandomChance */
        class RandomChance
        {
            size_t chance;
            size_t index;
            std::bitset<100> seeds;

            void fill(void)
            {
                seeds.reset();
                const int min = 0;
                const int max = seeds.size() - 1;

                while(seeds.count() < chance)
                {
                    int rnd = rand(min, max);
                    seeds.set(rnd);
                }
            }

        public:
            RandomChance(size_t v/* 1 .. 99 chance */) : chance(v), index(0)
            {
                if(chance < 1 || chance > 99) chance = 50;

                fill();
            }

            int getChance(void) const
            {
                return chance;
            }

            bool check(void)
            {
                if(index < seeds.size())
                    return seeds.test(index++);
                else
                {
                    fill();
                    index = 0;
                    return check();
                }
            }

            bool last(void) const
            {
                return seeds.test(index);
            }

            std::string toString(void) const
            {
                return seeds.to_string();
            }
        };

        template<typename T>
        std::list<T> AdvancedSplit(const T & buf, const T & sep)
        {
            std::list<T> list;
            auto itbeg = buf.begin();

            for(;;)
            {
                auto itend = std::search(itbeg, buf.end(), sep.begin(), sep.end());
                list.push_back(T(itbeg, itend));

                if(itend >= buf.end()) break;

                itbeg = itend;
                std::advance(itbeg, sep.size());
            }

            return list;
        }
    }

    /* Timer */
    class Timer
    {
	struct TimerDeleter
	{
	    void operator() (SDL_TimerID* ptr)
	    {
        	SDL_RemoveTimer(*ptr);
        	delete ptr;
    	    };
	};

    protected:
        std::unique_ptr<SDL_TimerID, TimerDeleter> ptr;
        Timer(const SDL_TimerID &);

    public:
	Timer() {}

        static Timer		create(u32 interval /* ms */, u32(*)(u32, void*), void* param = nullptr);
        void			destroy(void);
        bool			isValid(void) const;
    };

    /* TickTrigger */
    struct TickTrigger
    {
	mutable u32 latest;

	TickTrigger();

	bool			check(u32 ms, u32 period) const;
	void			reset(void);
	void			disabled(bool f);
    };

    /* KeyValue */
    template<typename T>
    struct KeyValue : std::pair<std::string, T>
    {
        KeyValue() {}
        KeyValue(const std::string & str, const T & val) : std::pair<std::string, T>(str, val) {}

        const std::string &	key(void) const
        {
            return this->first;
        }
        const T &		value(void) const
        {
            return this->second;
        }
    };

} // SWE
#endif
