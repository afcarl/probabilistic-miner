#ifndef _DB_GRAPH_H_
#define _DB_GRAPH_H_


#include <boost/graph/adjacency_list.hpp>
#include <boost/unordered_map.hpp>



struct vertex_id_t {
    typedef boost::vertex_property_tag kind;
};
struct vertex_label_t {
    typedef boost::vertex_property_tag kind;
};

typedef boost::property<vertex_id_t,unsigned short int> id_property;  
typedef  boost::property <vertex_label_t,short int,id_property > vertex_property;

/// Type definition of original graph
typedef boost::adjacency_list<boost::setS, 
			       boost::setS,
			       boost::undirectedS,
			       vertex_property,
			       boost::no_property>  
			       DB_Graph_Type;
  
  

class DB_Graph
{
public:
 
    /// Default constructor
    DB_Graph (void);
 
    /// Destructor
    ~DB_Graph ();
 
    /// Insert a vertex
    bool insert_vertex (unsigned short int id, short int vertex_label);

    /// Insert an edge
    bool insert_edge (unsigned short int st, unsigned short int end);
 
    // Get edge list
    void get_edge_pair_list (std::list<std::pair<unsigned short int,unsigned short int> > &edge_list);
 
    /// Get vertex id list
    void get_vertex_id_list (std::list<unsigned short int> &list);

    /// Get neighbor list
    void get_neighbor_id_list (unsigned short int id, unsigned short int start_vertex_id,
            std::list<unsigned short int> &list);
 
    /// Returns a subgraph 
    boost::tuple<DB_Graph*,std::string> get_subgraph (const 
            boost::unordered_map<unsigned short int,short int> &vertex_id_map);

    /// Gets the graph id
    unsigned short int get_id ();
 
    // Get graph
    DB_Graph_Type& get_graph ();
 
    /// Sets graph id
    void set_id (unsigned short int graph_id);
 
    // Get the degree of the vertex
    unsigned short int get_degree (unsigned short int node_id);
 
    // Get the label of the vertex
    short int get_label (unsigned short int node_id);
 
    // populate the label list
    void get_out_edges_label_list (unsigned short int vertex_id, std::list<short int> &labellist);
 
    /// Get number of vertices
    int get_number_of_vertices ();
 
    /// Get number of edges
    int get_number_of_edges ();
 
    /// Print vertex list 
    void print_vertex_information (std::ostream &out);

    /// Print edge list 
    void print_edge_information (std::ostream &out);

    /// Print adjacency list
    void print_adjacency_information(std::ostream &out);

    // Label id list
    boost::unordered_map <short int, std::list<unsigned short int> > label_id_list;

private:
 
    /// Graph id
    unsigned short int id_;
   
    /// Define a variable for graph
    DB_Graph_Type graph_; 
 
    /// Type definition of vertex descriptor
    typedef typename boost::graph_traits <DB_Graph_Type>:: vertex_descriptor vdescriptor_;

    /// Type definition of vertex iterator
    typedef  typename boost::graph_traits <DB_Graph_Type>:: vertex_iterator vi;
    vi vi_, vi_end_;

    /// Type definition of edge descriptor 
    typedef typename boost::graph_traits <DB_Graph_Type>:: edge_descriptor edescriptor_;

    /// Type definition of edge iterator 
    typedef  typename boost::graph_traits <DB_Graph_Type>:: edge_iterator ei; 
    ei ei_, ei_end_;

    /// Type definition of adjacency iterator
    typedef  typename boost::graph_traits <DB_Graph_Type>:: adjacency_iterator ai;
    ai ai_,ai_end_;

    typedef boost::unordered_map<unsigned short int,vdescriptor_> vertex_map; 
    vertex_map map_of_vertex_desc_;
 
    /// Inserted variable for edge insertion
    bool inserted_;

};



#endif /// !defined _DB_GRAPH_H_
