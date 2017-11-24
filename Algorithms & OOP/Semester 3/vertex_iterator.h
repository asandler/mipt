#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <map>

typedef std::map<std::string, size_t> VertexMap;

int _inf = 1000000000;

class TVertexIterator {
public:
        TVertexIterator(VertexMap &v) : V(v), cur(v.begin()) {
                while (cur != V.end() && cur->second == _inf)
                        ++cur;
        }
        virtual const std::string& Get() const {
                return cur->first;
        }
        virtual bool IsValid() const {
                return cur != V.end();
        }
        virtual void GetNext() {
                do
                        ++cur;
                while (cur != V.end() && cur->second == _inf);
        }
private:
        VertexMap &V;
        VertexMap::const_iterator cur;
};
