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

#ifndef _SWE_SHAREDLIST_
#define _SWE_SHAREDLIST_

#include <list>
#include <iterator>
#include <memory>

class StreamBase;

template<typename T>
class SharedList
{
    typedef std::list<T> basetype_t;
    std::shared_ptr<basetype_t> ptr;

    friend StreamBase & operator<< (StreamBase & sb, const SharedList<T> & sv)
    {
	return sb << (*sv.ptr);
    }

    friend StreamBase & operator>> (StreamBase & sb, SharedList<T> & sv)
    {
	return sb >> (*sv.ptr);
    }

public:
    typedef typename basetype_t::reverse_iterator reverse_iterator;
    typedef typename basetype_t::const_reverse_iterator const_reverse_iterator;
    typedef typename basetype_t::iterator iterator;
    typedef typename basetype_t::const_iterator const_iterator;

    SharedList() : ptr(std::make_shared<basetype_t>()) {}
    SharedList(size_t len, const T & t = T()) : ptr(std::make_shared<basetype_t>(len, t)) {}
    template<class InputIterator>
    SharedList(InputIterator it1, InputIterator it2) : ptr(std::make_shared<basetype_t>(it1, it2)) {}
    SharedList(const std::list<T> & v) : ptr(std::make_shared<basetype_t>(v.begin(), v.end())) {}

    size_t              size(void) const { return ptr->size(); }
    bool                empty(void) const { return ptr->empty(); }
    bool                unique(void) const { return ptr.unique(); }

    void                reset(void) { ptr = std::make_shared<basetype_t>(); }
    void                clear(void) { ptr->clear(); }
    void                resize(size_t sz, const T & t = T()) { ptr->resize(sz, t); }
    void		swap(SharedList & v) { ptr.swap(v.ptr); }
    void		reverse(void) { ptr->reverse(); }

    void		remove(const T & t) { ptr->remove(t); }
    template <class Predicate>
    void		remove_if(Predicate pred) { ptr->remove_if(pred); }

    iterator		erase(iterator position) { return ptr->erase(position); }
    iterator		erase(iterator first, iterator last) { return ptr->erase(first, last); }

    void		sort(void) { ptr->sort(); }
    template <class Compare>
    void		sort(Compare comp) { ptr->sort(comp); }

    void		unique(void) { ptr->unique(); }
    template <class BinaryPredicate>
    void		unique(BinaryPredicate pred) { ptr->unique(pred); }

    SharedList<T> &   	push_front(const T & t) { ptr->push_front(t); return *this; }
    SharedList<T> &   	pop_front(void) { ptr->pop_front(); return *this; }

    SharedList<T> &   	push_back(const T & t) { ptr->push_back(t); return *this; }
    SharedList<T> &   	pop_back(void) { ptr->pop_back(); return *this; }

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

    iterator		index(size_t pos) { iterator it = begin(); std::advance(it, pos); return it; }
    const_iterator	index(size_t pos) const { iterator it = begin(); std::advance(it, pos); return it; }

    SharedList<T> &	operator= (const std::list<T> & v) { ptr->assign(v.begin(), v.end()); return *this; }
    SharedList<T>	copy(void) const { return SharedList(begin(), end()); }
    std::list<T>        toList(void) const { return *ptr; }

    SharedList<T> &	operator<< (const T & t) { return push_back(t); }
};

#endif
