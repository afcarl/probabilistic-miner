#include <iostream>
#include "Timer.h"
#include "Miner.h"


int main(int argc,char *argv[])
{

 
 Timer clock;
 Miner  graph_miner;
 graph_miner.parse_argument (argc, &argv[0]);
 
 clock.start_timer();
 graph_miner.read_db_graphs();
 clock.end_timer();
 
 std::cout<<"File Reading Time:"<<clock.get_time_in_secs()<<std::endl;
 //graph_miner.print_db_graph_statistics(std::cout);
 
 clock.start_timer();
 graph_miner.mine_db_graphs ();
 clock.end_timer();
 std::cout<<"Mining Time:"<<clock.get_time_in_secs()<<std::endl; 
 

 /*DB_Graph *g1,*g2;
 g1= new DB_Graph ();
 g2= new DB_Graph ();
 
 
 g1->insert_vertex (0, 1);
 g1->insert_vertex (1, 1);
 g1->insert_vertex (2, 2);
 g1->insert_vertex (3, 2);
 g1->insert_edge (0,1);
 g1->insert_edge (1,2);
 g1->insert_edge (1,3);
 g1->insert_edge (2,3);
 g1->insert_edge (3,2);
 g1->insert_edge (3,1);
 
 g1->print_edge_information (std::cout);*/
 
 /*
 g2->insert_vertex (2, 1);
 g2->insert_vertex (0, 1);
 g2->insert_vertex (1, 2);
 g2->insert_vertex (3, 2);
 g2->insert_edge (2,0);
 g2->insert_edge (0,1);
 g2->insert_edge (0,3);
 g2->insert_edge (1,3);*/
    
 //Munkres mkr (4);
 //mkr.generate_cost_matrix (g1,g2);
 //mkr.run_munkres(); */  

}
