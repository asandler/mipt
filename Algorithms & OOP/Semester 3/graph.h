#include <iostream>
#include "vertex_iterator.h"
#include "edge_iterator.h"

class TGraph {
public:
        TGraph() {}
    	virtual ~TGraph() {}
    	virtual TEdgeIterator ListNeighbours(const std::string &s) = 0;
        virtual TVertexIterator ListVertices() = 0;
        virtual void AddVertex(const std::string &s) = 0;
        virtual void AddEdge(const std::string &s, const std::string &t, double w) = 0;
        virtual void DeleteVertex(const std::string &s) = 0;
        virtual void DeleteEdge(const std::string &s, const std::string &t) = 0;
        virtual bool ExistEdge(const std::string &s, const std::string &t) = 0;
        virtual void SetWeight(const std::string &s, const std::string &t, double w) = 0;
        virtual int VertexCount() const = 0;
};

class TMapGraph : public TGraph {
public:
	virtual TEdgeIterator ListNeighbours(const std::string &s) {
		return TEdgeIterator(new TMapEdgeIteratorImpl(Edge, s));
	}
	virtual TVertexIterator ListVertices() {
		return TVertexIterator(Vertex);
	}
        virtual void AddVertex(const std::string &s) {
                if (Vertex.find(s) == Vertex.end()) {
                        Vertex[s] = 1;
                        vertex_count++;
                }
        }
        virtual void AddEdge(const std::string &s, const std::string &t, double w) {
                AddVertex(s);
                AddVertex(t);
                Edge[std::make_pair(s, t)] = w;
        }
        virtual void DeleteVertex(const std::string &s) {
                Vertex[s] = inf;
        }
        virtual void DeleteEdge(const std::string &s, const std::string &t) {
                Edge[make_pair(s, t)] = inf;
        }
        virtual bool ExistEdge(const std::string &s, const std::string &t) {
                return Edge.find(make_pair(s, t)) != Edge.end() && Edge[make_pair(s, t)] != inf;
        }
        virtual void SetWeight(const std::string &s, const std::string &t, double w) {
                Edge[make_pair(s, t)] = w;
        }
        virtual int VertexCount() const {
                return vertex_count;
        }
private:
	AdjustMatrixMap Edge;
	VertexMap Vertex;
        int vertex_count;
};

class TVectorGraph : public TGraph {
public:
        virtual TEdgeIterator ListNeighbours(std::string s) {
                return TEdgeIterator(new TVectorEdgeIteratorImpl(Edge, Vertex, ReVertex, s));
        }
	virtual TVertexIterator ListVertices() {
		return TVertexIterator(Vertex);
	}
        virtual void AddVertex(const std::string &s) {
                if (!Vertex[s]) {
                        vertex_count++;
                        ReVertex[vertex_count] = s;
                        Vertex[s] = vertex_count;
                        Edge.resize(vertex_count);
                        for (AdjustMatrixVector::iterator it = Edge.begin(); it != Edge.end(); ++it)
                                while ((*it).size() < vertex_count)
                                        (*it).push_back(0);
                }
        }
        virtual void AddEdge(const std::string &s, const std::string &t, double w) {
                AddVertex(s);
                AddVertex(t);
                Edge[Vertex[s] - 1][Vertex[t] - 1] = w;
        }
        virtual void DeleteVertex(const std::string &s) {
                //is not implemented because it's such a strange method...
                //I use it only in MapGraph
        }
        virtual void DeleteEdge(const std::string &s, const std::string &t) {
                //the same reason not to implement this method
        }
        virtual bool ExistEdge(const std::string &s, const std::string &t) {
        }
        virtual void SetWeight(const std::string &s, const std::string &t, double w) {
        }
        virtual int VertexCount() const {
                return vertex_count;
        }
private:
        AdjustMatrixVector Edge;
        VertexMap Vertex;
        ReVertexMap ReVertex;
        int vertex_count;
};

class TListGraph : public TGraph {
public:
        virtual TEdgeIterator ListNeighbours(const std::string &s) {
                return TEdgeIterator(new TListEdgeIteratorImpl(Edge, s));
        }
	virtual TVertexIterator ListVertices() {
		return TVertexIterator(Vertex);
	}
        virtual void AddVertex(const std::string &s) {
                Vertex[s] = 1;
        }
        virtual void AddEdge(const std::string &s, const std::string &t, double w) {
                AddVertex(s);
                AddVertex(t);
                Edge[s].push_back(std::make_pair(t, w));
        }
        virtual void DeleteVertex(const std::string &s) {
                //is not implemented because it's such a strange method...
                //I use it only in MapGraph
        }
        virtual void DeleteEdge(const std::string &s, const std::string &t) {
                //is not implemented because it's such a strange method...
                //I use it only in MapGraph
        }
        virtual bool ExistEdge(const std::string &s, const std::string &t) {
        }
        virtual void SetWeight(const std::string &s, const std::string &t, double w) {
        }
        virtual int VertexCount() const {
                return Vertex.size();
        }
private:
        MapList Edge;
        VertexMap Vertex;
};
