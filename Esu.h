#ifndef _ESU_H_
#define _ESU_H_

#include "DB_Graph.h"
#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
#include "Munkres.h"
#include <algorithm>
#include <vector>
/**
 * 
 * @class Esu
 */

struct sort_pred {
    bool operator()(const std::pair<std::pair<unsigned short int, unsigned short int>, unsigned short int>&left, 
                const std::pair<std::pair<unsigned short int, unsigned short int>, unsigned short int>   &right) 
    {
        if (left.first.first == right.first.first)
        {
            return left.first.second <right.first.second;
        }else
        {
            return left.first.first < right.first.first; 
        }
    }
};

class Esu{
public:
    /// Constructor  
    Esu (unsigned short int subgraph_size);
    /// ESU
    Esu (DB_Graph *g, unsigned short int subgraph_size,unsigned short int graph_id);
    /// Destructor
    ~Esu ();
    ///
    void insert_into_mined_sequence (boost::tuple<DB_Graph*,std::string> graph_and_sequence);
 
    void set_graph_id (unsigned short int graph_id);
 
    void set_graph (DB_Graph *g);
    /// ESU Runner
    void run_esu ();
 
    bool operator ()()
    {
        run_esu();
    }
    void print_mined_sequences ();

private:
    /// 
    Munkres  munkres_;
    
    struct all_required_data{
        DB_Graph *stored_graph;
        boost::unordered_map<std::pair<unsigned short int, unsigned short int> ,
             boost::unordered_set<unsigned short int> > edge_pair_dictionary; 
    };
    /// 
    unsigned short int subgraph_size_;
 
    /// 
    DB_Graph  *g_; 
 
    unsigned short int graph_id_;

    /// Whether a list has more elements or not
    bool has_elements (bool is_empty);
 
    /// ESU extend subgraph
    void extend_subgraph (const boost::unordered_map<unsigned short int,short int> &vsubgraph_vertex_map, 
  		std::list<unsigned short int>&vextension_vertex_list, 
		const boost::unordered_map<unsigned short int,short int> &vsubgraph_vertex_extension_map, 
		unsigned short int start_vertex);

 
    /// all mined sequences
    boost::unordered_map<std::string,all_required_data*> mined_sequences;
};

#endif /// ! defined _ESU_H_
