#include "Esu.h"
#include <stdlib.h>
#include <boost/lexical_cast.hpp>
#include <boost/range/algorithm/sort.hpp>
#include "Result_Writer.h"






Esu :: Esu (unsigned short int subgraph_size)
:g_(0),subgraph_size_(subgraph_size),munkres_ (subgraph_size)
{ 
}

Esu :: Esu (DB_Graph  *g,unsigned short int subgraph_size,unsigned short int graph_id)
: g_(g),subgraph_size_(subgraph_size),graph_id_(graph_id), munkres_ (subgraph_size)
{
}


Esu :: ~Esu ()
{
}

void Esu:: set_graph_id (unsigned short int graph_id)
{
    this->graph_id_= graph_id;
}
 
void Esu:: set_graph (DB_Graph *g)
{
    this->g_=g;
}

bool Esu :: has_elements (bool is_empty)
{
    if (is_empty == false) return true;
    else return false;
}


// insert into sequence list
void Esu:: insert_into_mined_sequence (boost::tuple<DB_Graph*,std::string> graph_and_sequence )
{

    
    boost::unordered_map<std::string,all_required_data*>:: iterator itmine = 
          mined_sequences.find (graph_and_sequence.get<1>());
 
    //graph_and_sequence.get<0>()->print_adjacency_information(std::cout);
            
    if(itmine != mined_sequences.end())
    {
    }else
    {
        all_required_data *datas = new all_required_data;
        datas->stored_graph = graph_and_sequence.get<0>();
        mined_sequences[graph_and_sequence.get<1>()]=datas;
        itmine = mined_sequences.find (graph_and_sequence.get<1>());
    }
   
    boost::unordered_map <unsigned short int, unsigned short int> id_mappings;
    munkres_.init();
    munkres_.generate_cost_matrix (graph_and_sequence.get<0>(),(*itmine).second->stored_graph);
    munkres_.run_munkres ();
    munkres_.get_mappings (id_mappings);
    
    // Now populate edge list dictionary
    std::list<std::pair<unsigned short int, unsigned short int> > edge_list_pair;
    graph_and_sequence.get<0>()->get_edge_pair_list (edge_list_pair);
    
    
    
    std::list<std::pair<unsigned short int, unsigned short int> > ::iterator  
            edge_list_iterator = edge_list_pair.begin();
  
    for   (; edge_list_iterator != edge_list_pair.end(); ++edge_list_iterator)
    {
        unsigned short int st, end,temp;
     
     
        st = id_mappings[(*edge_list_iterator).first];
        end =id_mappings[(*edge_list_iterator).second]; 
        if (st> end){
            temp= st;
            st=end;
            end=temp;
        }
    
        std::pair<unsigned short int, unsigned short int> edge_pair = std::make_pair (st, end);
        if( (*itmine).second->edge_pair_dictionary.find (edge_pair)
                != (*itmine).second->edge_pair_dictionary.end())
        { 
            //std::cout<<st<<"   "<<end<<"  "<<this->graph_id_<<std::endl;
            (*((*itmine).second->edge_pair_dictionary.find(edge_pair))).second.insert(this->graph_id_);
        }else{
            boost::unordered_set<unsigned short int> graph_id_set;
            graph_id_set.insert(this->graph_id_);
            (*itmine).second->stored_graph->insert_edge (st,end);
            (*itmine).second->edge_pair_dictionary[edge_pair]  = graph_id_set;
        }
    }
}

void Esu :: print_mined_sequences ()
{
    std::cout<<"Printing mined sequences :"<<std::endl;
    boost::unordered_map<std::string,all_required_data*> :: iterator mined_seq_iterator = mined_sequences.begin();
    std::ofstream fout("logger.txt");
    for ( ; mined_seq_iterator != mined_sequences.end(); ++mined_seq_iterator)
    {
        std::cout<< (*mined_seq_iterator).first <<std::endl;
        boost::unordered_map<std::pair<unsigned short int, unsigned short int> ,
             boost::unordered_set<unsigned short int> > :: iterator edge_pair_dict_iterator;
        
        edge_pair_dict_iterator= (*mined_seq_iterator).second->edge_pair_dictionary.begin(); 
                                
        //std::vector< std::pair<unsigned short int, unsigned short int> > v;
        std::vector<std::pair<std::pair<unsigned short int, unsigned short int>, unsigned short int> > v1;
        
        for (; edge_pair_dict_iterator != (*mined_seq_iterator).second->edge_pair_dictionary.end(); ++edge_pair_dict_iterator)
        {
            //std::cout<< (*edge_pair_dict_iterator).first.first <<","<<(*edge_pair_dict_iterator).first.second;
            //v.push_back(std::make_pair((*edge_pair_dict_iterator).first.first,(*edge_pair_dict_iterator).first.second));
            v1.push_back(std::make_pair(std::make_pair((*edge_pair_dict_iterator).first.first,
                    (*edge_pair_dict_iterator).first.second),(*edge_pair_dict_iterator).second.size()));
        }
        
        
        
        // Need also the result written to logger
        
        std::string seq = (*mined_seq_iterator).first ;
        fout<<seq<<std::endl;
        // Print the vertices id and label
        (*mined_seq_iterator).second->stored_graph->print_vertex_information (fout);
        std::sort (v1.begin(),v1.end(), sort_pred());
        // now write the edges
        std::vector< std::pair<std::pair<unsigned short int, unsigned short int>,unsigned short int>
            >::iterator edge_pair_iterator= v1.begin();
     
        for (; edge_pair_iterator != v1.end(); ++edge_pair_iterator)
        {
          fout<< (*edge_pair_iterator).first.first <<","<<(*edge_pair_iterator).first.second<<std::endl;
          std::cout<< (*edge_pair_iterator).first.first <<","<<(*edge_pair_iterator).first.second;
          std::cout<<"->>> "<<(*edge_pair_iterator).second<<std::endl;
        }
        
    }
}


void Esu:: run_esu ()
{
    std::list<unsigned short int> vertex_list;
    g_->get_vertex_id_list (vertex_list);

    while (has_elements(vertex_list.empty()))
    {
        unsigned short int v_id = vertex_list.front();
        vertex_list.pop_front();
        
        std::list<unsigned short int> extension_vertex_id_list;
        g_->get_neighbor_id_list(v_id,v_id, extension_vertex_id_list);
        
        boost::unordered_map<unsigned short int,short int> vsubgraph_vertex_map;
        boost::unordered_map<unsigned short int,short int> vsubgraph_vertex_extension_map;

        vsubgraph_vertex_map.reserve(this->subgraph_size_);
        vsubgraph_vertex_map [v_id] =1;

        std::list<unsigned short int>::iterator it;

        for (it= extension_vertex_id_list.begin(); it!= extension_vertex_id_list.end(); ++it)
        {
            vsubgraph_vertex_extension_map [*it]= 1;
        }  
        extend_subgraph (vsubgraph_vertex_map,extension_vertex_id_list,vsubgraph_vertex_extension_map,v_id);
    }
}



void Esu :: extend_subgraph (const boost::unordered_map <unsigned short int, short int> 
                &vsubgraph_vertex_map,
                std::list<unsigned short int> &vextension_vertex_list, 
		const boost::unordered_map<unsigned short int,short int> &vsubgraph_vertex_extension_map,
		unsigned short int start_vertex)
{
    if(vsubgraph_vertex_map.size() == this->subgraph_size_) 
    {
        insert_into_mined_sequence(g_->get_subgraph(vsubgraph_vertex_map));
        return ;
    }
    while (has_elements(vextension_vertex_list.empty())) 
    {
        unsigned short int v_id_to_add = vextension_vertex_list.front();
        vextension_vertex_list.pop_front();

        // New map and list
        boost::unordered_map <unsigned short int, short int> vsubgraph_vertex_map_new (vsubgraph_vertex_map);
        std::list <unsigned short int> vextension_vertex_list_new (vextension_vertex_list);
        boost::unordered_map <unsigned short int, short int> vsubgraph_vertex_extension_map_new (vsubgraph_vertex_extension_map);


        vsubgraph_vertex_map_new [v_id_to_add] = 1;
        std::list<unsigned short int> extension_vertex_id_list; 
        g_->get_neighbor_id_list(v_id_to_add, start_vertex, extension_vertex_id_list);
  
        unsigned short int v_id; 
  
        while (has_elements(extension_vertex_id_list.empty()))
        {
            v_id = extension_vertex_id_list.front();
            extension_vertex_id_list.pop_front();
            if (v_id > start_vertex) 
            {
                if(vsubgraph_vertex_extension_map.find (v_id) == vsubgraph_vertex_extension_map.end())
                {
                    vextension_vertex_list_new.push_back (v_id);  
                    vsubgraph_vertex_extension_map_new[v_id]= 1;
                }
            }
        }
        extend_subgraph (vsubgraph_vertex_map_new,vextension_vertex_list_new,
            vsubgraph_vertex_extension_map_new,start_vertex);
  
    }
}
