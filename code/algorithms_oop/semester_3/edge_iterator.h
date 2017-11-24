#pragma once

#include <iostream>
#include <list>
#include <vector>
#include <memory>
#include <string>
#include <map>

typedef std::map<std::pair<std::string, std::string>, double> AdjustMatrixMap;

typedef std::map<std::string, size_t> VertexMap;
typedef std::map<size_t, std::string> ReVertexMap;
typedef std::vector<double> VectorMatrixColumn;
typedef std::vector<VectorMatrixColumn> AdjustMatrixVector;

typedef std::list<std::pair<std::string, double>> VertexList;
typedef std::map<std::string, VertexList> MapList;

int inf = 1000000000;

class TEdgeIteratorImpl {
public:
    TEdgeIteratorImpl() {
    }
    virtual TEdgeIteratorImpl* Clone() const = 0;
    virtual const std::string& From() const = 0;
    virtual const std::string& To() const = 0;
    virtual double Weight() const = 0;
    virtual bool IsValid() const = 0;
    virtual void GetNext() = 0;
    virtual ~TEdgeIteratorImpl() {
    }
};

class TMapEdgeIteratorImpl: public TEdgeIteratorImpl {
public:
    TMapEdgeIteratorImpl(AdjustMatrixMap& e, const std::string& s)
        : E(e)
        , from(s)
    {
        cur = E.lower_bound(std::make_pair(from, ""));
        while (cur != E.end() && cur->second == inf)
            ++cur;
    }
    virtual TEdgeIteratorImpl* Clone() const {
        return new TMapEdgeIteratorImpl(*this);
    }
    virtual const std::string& From() const {
        return from;
    }
    virtual const std::string& To() const {
        return cur->first.second;
    }
    virtual double Weight() const {
        return cur->second;
    }
    virtual bool IsValid() const {
        return cur != E.end() && cur->first.first == from;
    }
    virtual void GetNext() {
        do
            ++cur;
        while (cur != E.end() && cur->second == inf);
    }

private:
    std::string from;
    AdjustMatrixMap& E;
    AdjustMatrixMap::const_iterator cur;
};

class TVectorEdgeIteratorImpl: public TEdgeIteratorImpl {
public:
    TVectorEdgeIteratorImpl(AdjustMatrixVector& e, VertexMap& v, ReVertexMap& r, const std::string& s)
        : E(e)
        , V(v)
        , R(r)
        , from(s)
    {
        cur = E[V[from] - 1].begin();
        Skip();
    }
    virtual TEdgeIteratorImpl* Clone() const {
        return new TVectorEdgeIteratorImpl(*this);
    }
    virtual const std::string& From() const {
        return from;
    }
    virtual const std::string& To() const {
        return R[cur - E[V[from] - 1].begin() + 1];
    }
    virtual double Weight() const {
        return *cur;
    }
    virtual bool IsValid() const {
        return cur != E[V[from] - 1].end();
    }
    virtual void GetNext() {
        ++cur;
        Skip();
    }

private:
    void Skip() {
        while (IsValid() && *cur == 0)
            ++cur;
    }
    AdjustMatrixVector& E;
    VertexMap& V;
    ReVertexMap& R;
    VectorMatrixColumn::const_iterator cur;
    std::string from;
};

class TListEdgeIteratorImpl: public TEdgeIteratorImpl {
public:
    TListEdgeIteratorImpl(MapList& e, const std::string& s)
        : E(e)
        , from(s)
    {
        cur = E[from].begin();
    }
    virtual TEdgeIteratorImpl* Clone() const {
        return new TListEdgeIteratorImpl(*this);
    }
    virtual const std::string& From() const {
        return from;
    }
    virtual const std::string& To() const {
        return (*cur).first;
    }
    virtual double Weight() const {
        return (*cur).second;
    }
    virtual bool IsValid() const {
        return cur != E[from].end();
    }
    virtual void GetNext() {
        cur++;
    }

private:
    MapList& E;
    VertexList::const_iterator cur;
    std::string from;
};

class TEdgeIterator {
public:
    TEdgeIterator(TEdgeIteratorImpl* a)
        : Impl(a)
    {
    }
    TEdgeIterator(const TEdgeIterator& a)
        : Impl(a.Impl->Clone())
    {
    }
    const std::string& From() const {
        return Impl->From();
    }
    const std::string& To() const {
        return Impl->To();
    }
    int Weight() const {
        return Impl->Weight();
    }
    bool IsValid() const {
        return Impl->IsValid();
    }
    void GetNext() {
        Impl->GetNext();
    }

private:
    std::auto_ptr<TEdgeIteratorImpl> Impl;
};
