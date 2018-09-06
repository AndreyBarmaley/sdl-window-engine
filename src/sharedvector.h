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

#ifndef _SWE_SHAREDVECTOR_
#define _SWE_SHAREDVECTOR_

#include <vector>
#include <iterator>
#include <memory>

class StreamBase;

template<typename T>
class SharedVector
{
    typedef std::vector<T> basetype_t;
    std::shared_ptr<basetype_t> ptr;

    friend StreamBase & operator<< (StreamBase & sb, const SharedVector<T> & sv)
    {
	return sb << (*sv.ptr);
    }

    friend StreamBase & operator>> (StreamBase & sb, SharedVector<T> & sv)
    {
	return sb >> (*sv.ptr);
    }

public:
    typedef typename basetype_t::reverse_iterator reverse_iterator;
    typedef typename basetype_t::const_reverse_iterator const_reverse_iterator;
    typedef typename basetype_t::iterator iterator;
    typedef typename basetype_t::const_iterator const_iterator;

    SharedVector() : ptr(std::make_shared<basetype_t>()) {}
    SharedVector(size_t len, const T & t = T()) : ptr(std::make_shared<basetype_t>(len, t)) {}
    template<class InputIterator>
    SharedVector(InputIterator it1, InputIterator it2) : ptr(std::make_shared<basetype_t>(it1, it2)) {}
    SharedVector(const std::vector<T> & v) : ptr(std::make_shared<basetype_t>(v.begin(), v.end())) {}

    size_t              size(void) const { return ptr->size(); }
    bool                empty(void) const { return ptr->empty(); }
    bool                unique(void) const { return ptr.unique(); }

    void                reset(void) { ptr = std::make_shared<basetype_t>(); }
    void                clear(void) { ptr->clear(); }
    void                resize(size_t sz, const T & t = T()) { ptr->resize(sz, t); }
    void                reserve(size_t sz) { if(sz) ptr->reserve(sz); }
    void		swap(SharedVector & v) { ptr.swap(v.ptr); }

    SharedVector<T> &   push_back(const T & t) { ptr->push_back(t); return *this; }
    SharedVector<T> &   pop_back(void) { ptr->pop_back(); return *this; }

    template<class InputIterator>
    void		assign(InputIterator first, InputIterator last) { ptr->assign(first, last); }
    void		assign(size_t sz, const T & t) { ptr->assign(sz, t); }

    iterator		insert(iterator position, const T & t) { return ptr->insert(position, t); }
    template<class InputIterator>
    void		insert(iterator position, InputIterator first, InputIterator last) { ptr->insert(position, first, last); }

    const_reverse_iterator
                        rbegin(void) const { return ptr->rbegin(); }
    const_reverse_iterator
                        rend(void) const { return ptr->rend(); }

    reverse_iterator    rbegin(void) { return ptr->rbegin(); }
    reverse_iterator    rend(void) { return ptr->rend(); }

    const_iterator      begin(void) const { return ptr->begin(); }
    const_iterator      end(void) const { return ptr->end(); }

    iterator            begin(void) { return ptr->begin(); }
    iterator            end(void) { return ptr->end(); }

    T &                 front(void) { return ptr->front(); }
    const T &           front(void) const { return ptr->front(); }

    T &                 back(void) { return ptr->back(); }
    const T &           back(void) const { return ptr->back(); }

    T &                 at(size_t pos) { return ptr->at(pos); }
    const T &           at(size_t pos) const { return ptr->at(pos); }

    T*                  data(void) { return ptr->data(); }
    const T*            data(void) const { return ptr->data(); }

    T &                 operator[] (size_t pos) { return ptr->operator[](pos); }
    const T &           operator[] (size_t pos) const { return ptr->operator[](pos); }

    SharedVector<T> &	operator= (const std::vector<T> & v) { ptr->assign(v.begin(), v.end()); return *this; }
    SharedVector<T>	copy(void) const { return SharedVector(begin(), end()); }
    std::vector<T>	toVector(void) const { return *ptr; }

    SharedVector<T> &	operator<< (const T & t) { return push_back(t); }
};

#endif
