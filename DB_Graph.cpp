#include "DB_Graph.h"
#include <boost/lexical_cast.hpp>


DB_Graph :: DB_Graph (void)
:graph_()
{
}


DB_Graph :: ~DB_Graph ()
{
}

void DB_Graph :: set_id (unsigned short int graph_id)
{
    this->id_ = graph_id;
}


unsigned short int DB_Graph :: get_id ()
{
    return this->id_;
}


int DB_Graph ::get_number_of_vertices() 
{
    return boost::num_vertices(this->graph_);
}


int DB_Graph ::get_number_of_edges() 
{
    return boost::num_edges(this->graph_);
}


short int DB_Graph ::  get_label (unsigned short int nid)
{
    return boost::get(vertex_label_t(), this->graph_,(*(map_of_vertex_desc_.find(nid))).second);
}

void DB_Graph :: get_out_edges_label_list (unsigned short int vertex_id, std::list<short int> &labellist)
{
    typename vertex_map::iterator itst = map_of_vertex_desc_.find (vertex_id);
    if (itst==map_of_vertex_desc_.end())
    {}
    else{
        vdescriptor_ vd = (*itst).second;
        unsigned short int vid; 
  
        boost::tie (ai_,ai_end_) = boost::adjacent_vertices (vd, this->graph_);
        for (; ai_ != ai_end_ ; ++ai_)
        {
            vid = boost::get(vertex_label_t(), this->graph_, *ai_);
            labellist.push_back(vid);
        } 
    } 
    labellist.sort();
}

bool DB_Graph :: insert_vertex ( unsigned short int id, short int vertex_label)
{
    vdescriptor_  vadded = boost::add_vertex (graph_);
    map_of_vertex_desc_ [id] = vadded;
    boost::put(vertex_id_t(), this->graph_, vadded, id );
    boost::put(vertex_label_t(), this->graph_, vadded, vertex_label);
    if (label_id_list.find(vertex_label)!= label_id_list.end())
    {
        (*label_id_list.find(vertex_label)).second.push_back(id);
    }else{
        std::list<unsigned short int> vertex_id_list;
        vertex_id_list.push_back(id);
        label_id_list[vertex_label]=vertex_id_list;  
    }
}


bool  DB_Graph:: insert_edge (unsigned short int st, unsigned short int end)
{
    // Get the vertex descriptor of both starting and ending vertex 
    unsigned short int temp;
    if (st > end)
    {
        temp= st;
        st=end;
        end=temp;
    }
    
    typename vertex_map::iterator itst = map_of_vertex_desc_.find (st);
    typename vertex_map::iterator itend = map_of_vertex_desc_.find (end);

    if (itst == map_of_vertex_desc_.end() || itend == map_of_vertex_desc_.end())
    {
        return false;
    }
    else{
        edescriptor_ eadded;
        boost::tie (eadded,inserted_) = boost::add_edge (itst->second,itend->second,this->graph_);
    }
 
    if (inserted_ == false) return false;
    return true;
}

void DB_Graph:: get_edge_pair_list (std::list<std::pair<unsigned short int,unsigned short int> > &edge_list)
{
    boost::tie (ei_,ei_end_) = boost::edges (this->graph_);
    unsigned short int src,target;
    for (; ei_ != ei_end_; ++ei_)
    {
        vdescriptor_ src_ = boost::source (*ei_,this->graph_);
        vdescriptor_ target_ = boost::target (*ei_,this->graph_);

        src = boost::get(vertex_id_t(), this->graph_, src_);    
        target = boost::get(vertex_id_t(), this->graph_, target_);
        edge_list.push_back(std::make_pair(src,target));
    } 
}

void DB_Graph :: get_vertex_id_list(std::list <unsigned short int> &list)
{
    boost::tie(vi_,vi_end_) = boost::vertices (this->graph_);
    for ( ; vi_ != vi_end_ ; ++vi_)
    {
        unsigned short int vid = boost::get(vertex_id_t(), this->graph_, *vi_);
        list.push_back(vid);
    }
}


void DB_Graph ::get_neighbor_id_list (unsigned short int vertex_id, unsigned 
            short int start_vertex_id, std::list<unsigned short int> &v_id_list)
{
 
    typename vertex_map::iterator itst = map_of_vertex_desc_.find (vertex_id);
    if (itst==map_of_vertex_desc_.end())
    {}
    else{
        vdescriptor_ vd = (*itst).second;
        unsigned short int vid; 
  
        boost::tie (ai_,ai_end_) = boost::adjacent_vertices (vd, this->graph_);
        for (; ai_ != ai_end_ ; ++ai_)
        {
            vid = boost::get(vertex_id_t(), this->graph_, *ai_);
            if (vid > start_vertex_id)
            {
                v_id_list.push_back(vid);
            }
        } 
    }
}

unsigned short int DB_Graph :: get_degree (unsigned short int node_id)
{
    typename vertex_map::iterator itst = map_of_vertex_desc_.find (node_id);
    return boost::out_degree ((*itst).second, this->graph_);
}

DB_Graph_Type& DB_Graph:: get_graph ()
{
    return this->graph_;
}

boost::tuple<DB_Graph*,std::string> DB_Graph :: get_subgraph (
         const boost::unordered_map<unsigned short int,short int> &vertex_id_map) 
{
    
    DB_Graph *g = new DB_Graph();
    std::list<short int> label_list;
 
    std::string s("");
    boost::unordered_map<unsigned short int,short int> ::const_iterator it=vertex_id_map.begin();

    boost::unordered_map<unsigned short int,short int> id_mappings;
    
    unsigned short int vidorg,vid;
    short int vlabel;
 
    typename vertex_map::iterator itst;
    unsigned short int idcount =0;

    // add vertices
    for (;it != vertex_id_map.end();++it)
    {
        itst = map_of_vertex_desc_.find((*it).first);
        vdescriptor_ vd = (*itst).second;
        vidorg = boost::get(vertex_id_t(), this->graph_, vd);
        vid = idcount;
        vlabel = boost::get(vertex_label_t(), this->graph_, vd);
        id_mappings[vidorg]=vid;
        g->insert_vertex(vid,vlabel);
        label_list.push_back(vlabel);
        idcount++;
    }
 
    // default sort use less than operator for sorting
    label_list.sort();
 
    // Create the label sequence string
    while (!label_list.empty())
    {
        if (s.empty())
        {
            s = s+ boost::lexical_cast<std::string>(label_list.front()); 
        }
        else{
            s = s+","+boost::lexical_cast<std::string>(label_list.front()); 
        }
        label_list.pop_front();
    }
 
    // add edges
    boost::tie (ei_,ei_end_)= boost::edges (this->graph_);
    for (; ei_ != ei_end_; ++ei_)
    {
        vdescriptor_ src_ = boost::source (*ei_,this->graph_);
        vdescriptor_ target_ = boost::target (*ei_,this->graph_);

        unsigned short int src, target;
  
        src = boost::get(vertex_id_t(), this->graph_, src_);
        target = boost::get(vertex_id_t(), this->graph_, target_);
  
        if (vertex_id_map.find(src) != vertex_id_map.end() && 
            vertex_id_map.find(target) != vertex_id_map.end()  )
        {   
            g->insert_edge(id_mappings[src],id_mappings[target]);
        }
    }
 
    return boost::make_tuple(g,s);
}



void DB_Graph :: print_vertex_information (std::ostream &out)
{
    // vi_ for vertex iterator
    boost::tie(vi_,vi_end_) = boost::vertices (this->graph_);
    unsigned short int vid; 
    short int vlabel;
 
    for ( ; vi_ != vi_end_ ; ++vi_)
    {
        vid = boost::get(vertex_id_t(), this->graph_, *vi_);
        vlabel = boost::get(vertex_label_t(), this->graph_, *vi_);
        out<<vid<<"("<<vlabel<<")"<<std::endl;
    }
}


void DB_Graph:: print_edge_information (std::ostream &out)
{
    boost::tie (ei_,ei_end_) = boost::edges (this->graph_);
    for (; ei_ != ei_end_; ++ei_)
    {
        vdescriptor_ src_ = boost::source (*ei_,this->graph_);
        vdescriptor_ target_ = boost::target (*ei_,this->graph_);

        unsigned short int src,target;
        src = boost::get(vertex_id_t(), this->graph_, src_);
        target = boost::get(vertex_id_t(), this->graph_, target_);
        out<<src<<"-->"<<target<<std::endl;
    }
}


void DB_Graph :: print_adjacency_information (std::ostream &out)
{
    // vi_ for vertex iterator
    out<<"Printing adjacency information"<<std::endl;

    boost::tie(vi_,vi_end_) = boost::vertices (this->graph_);
    unsigned short int vid,vid_adj;
 
    for ( ; vi_ != vi_end_ ; ++vi_)
    {
        vid = boost::get(vertex_id_t(), this->graph_, *vi_);
        out<<vid<<":"; 
        boost::tie (ai_,ai_end_) = boost::adjacent_vertices (*vi_, this->graph_);
 
        for (; ai_ != ai_end_ ; ++ai_)
        {
            vid_adj = boost::get(vertex_id_t(), this->graph_, *ai_);
            out<<vid_adj<<",";
        }
        out<<std::endl;
    }
}
