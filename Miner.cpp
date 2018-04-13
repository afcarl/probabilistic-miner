#include "Miner.h"
#include "Esu.h"
#include "Graph_DB_File_Reader.h"
#include <boost/thread/thread.hpp>
#include <boost/lexical_cast.hpp>



Miner :: Miner ()
:filename_(),
 clock_ ()
{
}


void Miner :: parse_argument (int argc, char *argv[])
{
 boost::unordered_map<std::string, std::string> token_map;
 parser_.parse_program_argument(argc,&argv[0],token_map );
    
 if (token_map.find("-f")!= token_map.end())
 {
   filename_ = token_map["-f"];
 }
 if (token_map.find("-s")!= token_map.end())
 {
   subgraph_size_= boost::lexical_cast <unsigned short int> (token_map ["-s"]);
 }
}


void Miner  :: read_db_graphs ()
{
 if(filename_.empty()) return ;

 Graph_DB_File_Reader freader (filename_);
 freader.open();
 std::string line;
 
 DB_Graph  *g;
 std::vector<std::string> token_vector;
 unsigned short int graph_count=1;
 
 while (freader.get_line(line))
 {
     
  // Checks whether is it a new transaction or not ??
  std::size_t found= line.find ("t");
  // if t is in the line
  if (found != std::string::npos)
  {
   if(graph_count>1)
   {
    vector_of_db_graphs_.push_back(g);
   }
   g = new DB_Graph ();
   g->set_id(graph_count);
   graph_count++;
  }
  
  // Checks whether it is a new vertex
 found= line.find ("v");
  // if t is in the line
  if (found != std::string::npos)
  {
   parser_.parse_a_line(line,' ',token_vector);
   unsigned short int vid;
   short int  vlabel;
   
   if (token_vector.size()<3) return ;
   
   std::istringstream iss_vid (token_vector[1]);
   iss_vid>> vid;
   
   std::istringstream iss_vlabel (token_vector[2]);
   iss_vlabel>> vlabel;
   
   g->insert_vertex(vid,vlabel);
  }
  
  // Checks whether it is a new edge
  found= line.find ("e");
  // if t is in the line
  if (found != std::string::npos)
  {
   parser_.parse_a_line(line,' ',token_vector);
   size_t st,end;
   
   
   if (token_vector.size()<4) return ;
   std::istringstream iss_st (token_vector[1]);
   iss_st >> st;
   
   std::istringstream iss_end (token_vector[2]);
   iss_end >> end;
   g->insert_edge(st,end);
   
  }
 }
 if (g->get_number_of_vertices()>0)
 {
    vector_of_db_graphs_.push_back(g);
 }
}


unsigned short int Miner :: get_no_of_db_graphs ()
{
 return vector_of_db_graphs_.size();
}




void Miner :: print_db_graph_statistics (std::ostream &out)
{
 out<<"Number of graphs in the database: " <<get_no_of_db_graphs()<<std::endl;
 
 size_t graphs_above_subgraph_size_thrshold=0;
 
 for (size_t i=0;i<get_no_of_db_graphs();++i )
 {
  out<<"Graph ID: "<<vector_of_db_graphs_[i]->get_id()<<std::endl;
  out<<"No. of vertices: "<<vector_of_db_graphs_[i]->get_number_of_vertices()<<std::endl;
  if (vector_of_db_graphs_[i]->get_number_of_vertices()>= this->subgraph_size_)
  {
   graphs_above_subgraph_size_thrshold++;   
  }
  out<<"No. of edges: "<<vector_of_db_graphs_[i]->get_number_of_edges ()<<std::endl;
 }
 out<<"Total number of graphs above the subgraph size threshold "<<graphs_above_subgraph_size_thrshold<<std::endl;
}



void Miner :: mine_db_graphs ()
{
 
 //std::vector<boost::thread*> all_threads;
 Esu  esu(this->subgraph_size_);
    
 for(size_t i=0; i<get_no_of_db_graphs(); ++i)
 {  
  //std::cout<<i<<std::endl;
  
  esu.set_graph_id(i+1);
  esu.set_graph(vector_of_db_graphs_[i]);
  esu.run_esu();

   
  //vector_of_db_graphs_[i]->print_vertex_information(std::cout);
  //vector_of_db_graphs_[i]->print_edge_information(std::cout);
  //vector_of_db_graphs_[i]->print_adjacency_information(std::cout);


  //boost::thread *worker_thread = new boost::thread(esu);
  //all_threads.push_back(worker_thread);
 }
 /*for(size_t i=0; i<all_threads.size();++i)
 {
  all_threads[i]->join();
 }
 */   
 esu.print_mined_sequences();
 
}


