#ifndef _MINER_H_
#define _MINER_H_ 

#include "DB_Graph.h"
#include "Parser.h"
#include "Timer.h"


class Miner
{
public:
 /// Constructor 
 Miner ();
 
 // Number of db graphs
 unsigned short int get_no_of_db_graphs ();
 
 // Print statistics
 void print_db_graph_statistics (std::ostream &out);
 
 // Parse arguments
 void parse_argument (int argc,char* argv[]);
 
 // Read all the database graphs
 void read_db_graphs ();
 
 /// Miner
 void mine_db_graphs ();

private:

 
 /// Subgraph Size
unsigned short int  subgraph_size_;
 
 /// File name (input file)
 std::string filename_;
 
 /// Parser
 Parser parser_;
 
 /// Vector of db graphs
 std::vector<DB_Graph*> vector_of_db_graphs_;
 
 /// Timer
 Timer clock_;
 
};



#endif /// !defined _MINER_H_
