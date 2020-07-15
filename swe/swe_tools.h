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
#include <vector>
#include <memory>
#include <iterator>
#include <algorithm>
#include <bitset>

#include "swe_types.h"
#include "swe_binarybuf.h"
#include "swe_rect.h"
#include "swe_systems.h"
#include "swe_serialize.h"

namespace SWE
{
    namespace Tools
    {
        void		delay(unsigned int ms);
        int		countBits(unsigned long);

        int		rand(int min, int max);
        float		randf(float min, float max);

        u32		crc32b(const char*);
        u32		crc32b(const u8*, size_t);

        int		crc16b(const char*);
        int		crc16b(const u8*, size_t);

        u32		ticks(void);

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

        template<typename T>
        const T*	rand(const std::vector<T> & vec)
        {
	    if(vec.empty()) return NULL;
	    if(vec.size() == 1) return &vec.front();
            typename std::vector<T>::const_iterator it = vec.begin();
            std::advance(it, rand(0, vec.size() - 1));
            return it == vec.end() ? NULL : &(*it);
        }

        template<typename T>
        const T*	rand(const std::list<T> & list)
        {
	    if(list.empty()) return NULL;
	    if(list.size() == 1) return &list.front();
            typename std::list<T>::const_iterator it = list.begin();
            std::advance(it, rand(0, list.size() - 1));
            return it == list.end() ? NULL : &(*it);
        }

        template<typename T>
        class RandQueue : protected std::vector< std::pair<T, size_t> >
        {
	protected:
            size_t getMaxWeight(void) const
            {
                size_t max = 0;

                for(auto it = this->begin(); it != this->end(); ++it)
                    max += (*it).second;

                return max;
            }

        public:
            RandQueue(size_t size = 0)
            {
                if(size)
                    this->reserve(size);
            }

            void push(const T & value, size_t weight)
            {
                if(weight)
                    this->push_back(std::make_pair(value, weight));
            }

	    bool isValid(void) const
	    {
		return this->size();
	    }

            T	 get(void)
            {
                if(this->size())
                {
                    std::vector<size_t> percents;
                    percents.reserve(this->size());
                    // get max
                    size_t max = getMaxWeight();

                    // set weights
                    for(auto it = this->begin(); it != this->end(); ++it)
                        percents.push_back(100 * (*it).second / max);

                    // calc max
                    max = 0;

                    for(auto itf = percents.begin(); itf != percents.end(); ++itf)
                        max += *itf;

                    size_t rnd = rand(0, max);
                    size_t amount = 0;

                    // get rnd
                    for(auto itf = percents.begin(); itf != percents.end(); ++itf)
                    {
                        amount += *itf;

                        if(rnd <= amount)
                            return this->at(std::distance(percents.begin(), itf)).first;
                    }
                }

                ERROR("weight not found");
                return T();
            }
        };

        class RandomChance
        {
            unsigned int chance;
            unsigned int index;
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
            RandomChance(unsigned int v/* 1 .. 99 chance */) : chance(v), index(0)
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

    class Timer
    {
        std::shared_ptr<SDL_TimerID>
        ptr;

    public:
        Timer(const SDL_TimerID & id = 0);
        ~Timer();

        static Timer	create(u32 interval /* ms */, u32(*)(u32, void*), void* param = NULL);
        void		destroy(void);
        bool		isValid(void) const
        {
            return ptr && *ptr;
        }
    };

    struct TickTrigger
    {
	mutable u32 latest;

	TickTrigger();

	bool	check(u32 ms, u32 period) const;
	void	reset(void);
	void	disabled(bool f);
    };

    template<typename T>
    struct KeyValue : std::pair<std::string, T>
    {
        KeyValue() {}
        KeyValue(const std::string & str, const T & val) : std::pair<std::string, T>(str, val) {}

        const std::string &	key(void) const
        {
            return this->first;
        }
        const T 	&	value(void) const
        {
            return this->second;
        }
    };

} // SWE
#endif
