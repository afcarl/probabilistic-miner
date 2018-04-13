#ifndef MUNKRES_H
#define	MUNKRES_H

#include "DB_Graph.h"

#define _DEBUG_MUNKRES_
#undef  _DEBUG_MUNKRES_

class Munkres
{
public:
    // Constructor
    Munkres (unsigned short int subgraph_size);
  
    // Destructor 
    ~Munkres ();
    
    // Initializer
    void init ();
  
    // Run Munkres algorithm for optimal assignment
    void run_munkres();
    
    // This one is for graph mining
    void generate_cost_matrix (DB_Graph *g1, DB_Graph *g2);
    
    void get_mappings (boost::unordered_map <unsigned short int, unsigned short int> &id_mappings);
  
private:
    // variables
    unsigned short int subgraph_size_;
    unsigned short int nrow_;
    unsigned short int ncol_;
    int path_count_;
  
  
    int path_row_0_;
    int path_col_0_;
    int cost_matrix_[32][32];
    int matching_matrix_[32][32];
    int row_cover_ [32],col_cover_[32];
    int path_matrix_ [65][2];
  
    // Cost matrix generation  
    void generate_worst_cost_matrix ();
    
    void print_cost_matrix (); 
    
    
  
    // Perform all the steps in Munkres algorithm
    void step1();
    void step2();
    void step3();
    void step4();
    void step5();
    void step6();
    void step7();
  
  
    // Step 4 helper methods
    void find_a_zero (int &row, int &col);
    bool star_in_row(int row);
    void find_star_in_row(int row, int &col);
  
    // Step 5 helper methods
    void find_star_in_col(int c, int &r);
    void find_prime_in_row(int r, int& c);
    void augment_path();
    void clear_covers();
    void erase_primes();
  
    // Step 6 helper methods
    void find_smallest(int &minval);
    
    // some other helper methods
    int calculate_edit_distance (const std::list <short int> &list1,
            const std::list <short int> &list2);
  
};

#endif	/* MUNKRES_H */

