#pragma once

#include <iostream>

#include "transition.h"

typedef boost::shared_ptr<Transition> TransitionPtr;

class Graph {
public:
  friend std::ostream& operator<<(std::ostream& out, const Graph& g);

  typedef uint32 vertex;

private:
  enum FlagType {
    ZERO = 0,
    ONE  = 1,
    MANY = ONE << 1
  };
    
public:
#pragma pack(1)
  struct AdjacentList {
    uint32 What;
    byte Flags;

    AdjacentList(): Flags(ZERO) { What = 0xFFFFFFFF; }
  };

  struct Vertex {
    TransitionPtr Tran;
    AdjacentList In, Out;
  };
#pragma pack()

  Graph(uint32 numVs, uint32 reserveSize);
  Graph(uint32 numVs = 0);

  uint32 numVertices() const { return Vertices.size(); }
  uint32 capacity() const { return Vertices.capacity(); }

  vertex addVertex();

  bool edgeExists(const vertex source, const vertex target) const;

  void addEdge(const vertex source, const vertex target);

  vertex inVertex(vertex v, size_t i) const {
    return _adjacent(Vertices[v].In, i);
  }

  vertex outVertex(vertex v, size_t i) const {
    return _adjacent(Vertices[v].Out, i);
  }

  uint32 inDegree(const vertex v) const {
    return _degree(Vertices[v].In);
  }

  uint32 outDegree(const vertex v) const {
    return _degree(Vertices[v].Out);
  }

  const TransitionPtr operator[](vertex v) const {
    return Vertices[v].Tran; 
  }
  
  TransitionPtr& operator[](vertex v) { return Vertices[v].Tran; }
 
  void clear() {
    Vertices.clear();
    AdjLists.clear();
  }

private:
  void _add(AdjacentList& l, vertex v);

  uint32 _degree(const AdjacentList& l) const;

  vertex _adjacent(const AdjacentList& l, size_t i) const;
  vertex& _adjacent(AdjacentList& l, size_t i);

  std::vector< Vertex > Vertices;
  std::vector< std::vector< vertex > > AdjLists;
};

typedef boost::shared_ptr<Graph> GraphPtr;
