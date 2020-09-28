// Header for graph coloring heuristics
// Plan - to have DSATUR & RLF heuristics

#include "heugcol.h"

template <class Graph>
class InclusionChecker
{
  private:
    typedef boost::graph_traits<Graph> GraphTraits;
    typedef typename GraphTraits::vertex_descriptor vertex_descriptor;

    
    std::function<bool(const vertex_descriptor&, const vertex_descriptor&, const Graph&)> property_invalidation_condition;

    static bool is_edge(const vertex_descriptor& v, const vertex_descriptor& candidate, const Graph& G)
    {
      return boost::edge(v, candidate, G).second;
    };

    static bool is_not_edge(const vertex_descriptor& v, const vertex_descriptor& candidate, const Graph& G)
    {
      return not(is_edge(v, candidate, G));
    };


    void get_condition(const std::string& property)
    {
      if (property == "stable-set")
      {
        property_invalidation_condition = is_edge;
      }
      else if (property == "clique")
      {
        property_invalidation_condition = is_not_edge;
      }
      
    };
    
  public:
    InclusionChecker(const Graph& G_in, const std::string& inclusion_type_in, const std::string& property_in)
    {
      // const Graph& G = G_in;
      // const std::string& inclusion_type = inclusion_type_in;
      // const std::string& property = property_in;
      get_condition(property_in);
    };

    bool check(const std::vector<vertex_descriptor>& stable_set, const vertex_descriptor& candidate, const Graph& G)
    {
      // check if the candidate vertex forms a stable set with stable_set
      
      for (auto& v : stable_set)
      {
        if (property_invalidation_condition(v, candidate, G))
        {
          return false;
        };
      }

      return true;
    }
};

namespace heuristics 
{
  template <class Graph, class Order>
  typename boost::graph_traits<Graph>::vertices_size_type
  Greedy(const Graph& G, const Order& order)
  {
    typedef boost::graph_traits<Graph> GraphTraits;
    typedef typename GraphTraits::vertex_descriptor vertex_descriptor;
    typedef typename GraphTraits::vertices_size_type size_type;
    // typedef typename boost::property_traits<Order>::value_type OrderType;

    // BOOST_STATIC_ASSERT((boost::is_same<OrderType, vertex_descriptor>::value));

    const size_type V = num_vertices(G);
    
    // We need a collection 'S' of color classes. Each color
    // class has to be an independent (aka stable) set.

    std::vector<std::vector<vertex_descriptor>> S;
        
    for (size_type i = 0; i < V; i++) 
    {
      size_type j = 0;
      vertex_descriptor current = order[i];
      for (j = 0; j < S.size(); j++)
      {
          // checking if current vertex can be added to the 
          // current class
           InclusionChecker<Graph> ic(G, "vertex", "stable-set");
           if (ic.check(S[j], current, G))
           {
             S[j].push_back(current);
             break;
           }
      }
      
      // if current vertex could not be added to any of the
      // existing color classes, create a new color class
      if (j >= S.size())
      {
        std::vector<vertex_descriptor> temp{current};
        S.push_back(temp);
      }
    }
    return S.size();
  }
}


namespace boost {
  template <class VertexListGraph, class Order, class Color>
  typename graph_traits<VertexListGraph>::vertices_size_type
  sequential_vertex_color_ting(const VertexListGraph& G,
    Order order, Color color)
  {
    typedef graph_traits<VertexListGraph> GraphTraits;
    typedef typename GraphTraits::vertex_descriptor vertex_descriptor;
    typedef typename GraphTraits::vertices_size_type size_type;
    typedef typename property_traits<Color>::value_type ColorType;
    typedef typename property_traits<Order>::value_type OrderType;

    BOOST_CONCEPT_ASSERT(( VertexListGraphConcept<VertexListGraph> ));
    BOOST_CONCEPT_ASSERT(( ReadWritePropertyMapConcept<Color, vertex_descriptor> ));
    BOOST_CONCEPT_ASSERT(( IntegerConcept<ColorType> ));
    BOOST_CONCEPT_ASSERT(( ReadablePropertyMapConcept<Order, size_type> ));
    BOOST_STATIC_ASSERT((is_same<OrderType, vertex_descriptor>::value));

    size_type max_color = 0;
    const size_type V = num_vertices(G);
    std::vector<size_type>
      mark(V, numeric_limits_max(max_color));

    typename GraphTraits::vertex_iterator v, vend;
    for (boost::tie(v, vend) = vertices(G); v != vend; ++v)
      color[*v] = V - 1; // which means "not colored"



    for (size_type i = 0; i < V; i++) {
      vertex_descriptor current = order[i];

      // mark all the colors of the adjacent vertices
      typename GraphTraits::adjacency_iterator ai, aend;
      for (boost::tie(ai, aend) = adjacent_vertices(current, G); ai != aend; ++ai)
        mark[color[*ai]] = i;

      // find the smallest color unused by the adjacent vertices
      size_type smallest_color = 0;
      while (smallest_color < max_color && mark[smallest_color] == i)
        ++smallest_color;

      // if all the colors are used up, increase the number of colors
      if (smallest_color == max_color)
        ++max_color;

      color[current] = smallest_color;
    }
    return max_color;
  }
} // namespace boost