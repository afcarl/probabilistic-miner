
#include <limits>
#include <iostream>
#include "Munkres.h"
#include <vector>
#include <boost/unordered_map.hpp>

Munkres :: Munkres (unsigned short int subgraph_size)
:subgraph_size_(subgraph_size)
{
    nrow_ = this->subgraph_size_; 
    ncol_ = this->subgraph_size_;
 
    for (int r=0;r<nrow_; ++r)
    {
        row_cover_[r]=0;
        col_cover_[r]=0;
        for (int c=0; c<ncol_; ++c)
        {
            matching_matrix_[r][c]=0;
         }   
    }
 
    for (int r=0;r<2*nrow_+1; ++r)
    {
        for (int c=0; c<2; ++c)
        {
            path_matrix_[r][c]=0;
        }   
    }
}

Munkres :: ~Munkres ()
{
}

void Munkres :: init ()
{
    for (int r=0;r<nrow_; ++r)
    {
        row_cover_[r]=0;
        col_cover_[r]=0;
        for (int c=0; c<ncol_; ++c)
        {
            matching_matrix_[r][c]=0;
         }   
    }
 
    for (int r=0;r<2*nrow_+1; ++r)
    {
        for (int c=0; c<2; ++c)
        {
            path_matrix_[r][c]=0;
        }   
    }  
}

void Munkres :: run_munkres ()
{   
    //generate_worst_cost_matrix();
    #ifdef _DEBUG_MUNKRES_
        print_cost_matrix ();
        step7();
    #endif
    step1 ();   
}


void Munkres :: generate_worst_cost_matrix ()
{
    for (int r=0; r<nrow_; ++r)
    {
        for (int c=0; c<ncol_; ++c)
        {
            cost_matrix_ [r][c]= (r+1)*(c+1);    
        }
    }
}

int Munkres  :: calculate_edit_distance (const std::list <short int> &list1,
            const std::list <short int> &list2)
{
    const int len1 = list1.size(), len2 = list2.size();
    std::vector<std::vector<int> > cost(len1 + 1, std::vector<int>(len2 + 1));
 
    cost[0][0] = 0;
    for(unsigned int i = 1; i <= len1; ++i)
    {
       cost[i][0] = i;
    }
    for(unsigned int i = 1; i <= len2; ++i)
    {
       cost[0][i] = i;
    }
    std::list <short int> ::const_iterator it,it1;
    it= list1.begin();
    it1= list2.begin();
    
    unsigned int i,j;
    i = j =1;
    
    for( ;it!=list1.end(); ++it)
    {
        j=1;
        it1= list2.begin();
        for(;it1!= list2.end(); ++it1)
        {
            // No substitution
            // Check here ???
            cost[i][j] = std::min( std::min(cost[i - 1][j] + 1,cost[i][j - 1] + 1),
                                          cost[i - 1][j - 1] + (*it == *it1 ? 0 : 100) );
            ++j;
        }
        ++i;
    }
    return cost[len1][len2];
}

void Munkres :: generate_cost_matrix (DB_Graph *g1, DB_Graph *g2)
{
    // Mapping of Graph g2 to g1
    std::list <unsigned short int> list1, list2;
    
    // Try to save it as it will be severely used
    g1->get_vertex_id_list (list1);
    g2->get_vertex_id_list (list2);
    
    std::list <unsigned short int> ::const_iterator it,it1;
    it= list1.begin();
    it1= list2.begin();
    
    
    int cost ;
   
    // Incorporating the node id cost was Dr. Xia's idea
    // But, I later modified it so that it works
    for ( ; it != list1.end();++it)
    {
        it1= list2.begin();
        for ( ; it1 != list2.end();++it1)
        { 
            if (g2->get_label (*it1) != g1->get_label (*it))
            {
                cost_matrix_ [*it][*it1]= (*it1)+ (100* (this->subgraph_size_ + 2));
            }
            else
            {
                // Find the edit distance 
                std::list <short int> labellist1, labellist2;
                g1->get_out_edges_label_list (*it,labellist1);
                g2->get_out_edges_label_list (*it1,labellist2);
                cost = calculate_edit_distance (labellist1,labellist2);
                cost_matrix_ [*it][*it1]= (*it1)+ (100*cost);  
            }
        }
    }
    
    for (int i=0 ; i != list1.size();++i)
    {
        int row_min= 100; 
        int index_min=0;
        bool flag=true;
        for (int j=0 ; j!= list2.size();++j)
        { 
          if (cost_matrix_ [i][j]  >99)
          {
              flag=false;
          }
          if (flag)
          {
              if (row_min>cost_matrix_[i][j])
              {
                  row_min = cost_matrix_[i][j];
                  index_min= j;
              }
          }
        }
        
        if (flag)
        {
           for (int j=0 ; j!= list2.size();++j)
           {
               if (j!=index_min)
               {
                   cost_matrix_[i][j]=cost_matrix_[i][j]+50;
               }
           }
        }
    }
    
    #ifdef _DEBUG_MUNKRES_ 
    print_cost_matrix();
    #endif
}
/*
 * For each row of the matrix, find the smallest element 
 * and subtract it from every element in its row
 */
void Munkres :: step1()
{
    int min_in_row;
 
    for (int r=0;r<nrow_; ++r)
    {
        min_in_row =cost_matrix_[r][0];
  
        // Calculate the minimum of a single row
        for (int c=0;c<ncol_;++c)
        {
            if(cost_matrix_[r][c] <min_in_row)
            {
                min_in_row = cost_matrix_[r][c];
            }
        }
  
        // Subtract the minimum value from every column
        for (int c=0;c<ncol_;++c)
        {
            cost_matrix_[r][c]= cost_matrix_[r][c]- min_in_row; 
        }
    }    

    #ifdef _DEBUG_MUNKRES_ 
    // Go to Step 2
    std::cout << "Going to Step 2"<<std::endl;
    print_cost_matrix();
    step7 ();
    #endif 
         
    step2();
}
/**
 * Find a zero in the resulting matrix, if there is no starred zero
 * in its row or column, star Z. Repeat for each element in the matrix
 */
void Munkres :: step2()
{
    for (int r = 0; r < nrow_; ++r)
    {
        for (int c = 0; c < ncol_; ++c)
        {
            if (cost_matrix_[r][c] == 0 && row_cover_[r] == 0 && col_cover_[c] == 0)
            {
                // 1 represents star
                matching_matrix_[r][c] = 1;
                row_cover_[r] = 1;
                col_cover_[c] = 1;
            }
        }
    }
    
    for (int r = 0; r < nrow_; r++)
    {
        row_cover_[r] = 0;
    }
  
    for (int c = 0; c < ncol_; c++)
    {
        col_cover_[c] = 0;
    }
 
    #ifdef _DEBUG_MUNKRES_
    std::cout << "Going to Step 3"<<std::endl;
    print_cost_matrix();
    step7();
    #endif 
 
    step3();
}
/**
 * Cover each column containing a starred zero. if k columns are covered
 * , the starred zeros describe a complete set of unique assignments. 
 * In this case go to Done, otherwise go to step 4.
 */
void Munkres :: step3 ()
{
    int colcount;
    for (int r = 0; r < nrow_; r++)
    {
        for (int c = 0; c < ncol_; c++)
        {
            if (matching_matrix_[r][c] == 1)
            {
                col_cover_[c] = 1;
            }
        }
    }
    
    colcount = 0;
    for (int c = 0; c < ncol_; c++)
    {
        if (col_cover_[c] == 1)
        {
            colcount += 1;
        }
    }
    if (colcount >= ncol_ || colcount >=nrow_)
    {
        #ifdef _DEBUG_MUNKRES_ 
        std::cout << "Going to Step 7"<<std::endl;
        print_cost_matrix();
        step7();
        #endif
        step7();
    }
    else
    {
        #ifdef _DEBUG_MUNKRES_ 
        std::cout << "Going to Step 4"<<std::endl;
        print_cost_matrix();
        step7();
        #endif

        step4 ();
    }
  
}


/*
 * Find a non-covered zero and prime it. If there is no starred zero in the row 
 * containing this primed zero, go to step 5. Otherwise cover this row and uncover
 * the column containing the starred zero. Continue in this manner until there are
 * uncovered zero left. Save the smallest uncovered value and go to step 6
 */
void Munkres :: step4 ()
{
    int row = -1;
    int col = -1;
    bool done;

 
    done = false;
    while (!done)
    { 
        find_a_zero(row, col);
  
        if (row == -1)
        {
            done = true;
            #ifdef _DEBUG_MUNKRES_ 
            std::cout << "Going to Step 6"<<std::endl;
            print_cost_matrix();
            step7();
            #endif
            step6();
        }
        else
        {
            // Find a non-covered zero and prime it
            // 2 indicates prime
            matching_matrix_[row][col] = 2;
            if (star_in_row(row))
            {
                // Cover this row and uncover the column containing 
                // the starred zero, continue in this manner until there is no un
                // covered zero left.  
                find_star_in_row(row, col);
                row_cover_[row] = 1;
                col_cover_[col] = 0;
            }
            else
            {
                // If there is no starred zero in the row containing this 
                // primed zero, go to step 5. 
                done = true;
                path_row_0_ = row;
                path_col_0_ = col;
     
                #ifdef _DEBUG_MUNKRES_
                std::cout << "Going to Step 5"<<std::endl;
                print_cost_matrix();
                step7();
                #endif

                step5 ();
            }
        }
    }    
}

/*
 * Construct a series of alternating primed and starred zero as follows.
 *  (1) Let Z_0 represent uncovered primed zero found in step 4
 *  (2) Let Z_1 denote the starred zero in the column Z_0. Let Z_2 denote
 * the primed zero in the row of Z_1. Continue until the series terminates
 * at a primed zero of the series, star each primed zero of the series, erase
 * all primes and uncover every line in the matrix. Return to step 3.
 */
void Munkres :: step5 ()
{
    bool done;
    int r = -1;
    int c = -1;

    path_count_ = 1;
    path_matrix_[path_count_ - 1] [0] = path_row_0_;
    path_matrix_[path_count_ - 1] [1] = path_col_0_;
    done = false;

    while (!done)
    {
        // Find star in col
        find_star_in_col(path_matrix_[path_count_ - 1][1], r);
        if (r > -1)
        {
            path_count_ += 1;
            path_matrix_[path_count_ - 1] [0] = r;
            path_matrix_[path_count_ - 1] [1] = path_matrix_[path_count_ - 2] [1];
        }
        else
        {
            done = true;
        }
        if (!done)
        {
            // Continue until 
            find_prime_in_row(path_matrix_[path_count_ - 1] [0], c);
            path_count_ += 1;
            path_matrix_[path_count_ - 1][0] = path_matrix_[path_count_ - 2] [0];
            path_matrix_[path_count_ - 1][1] = c;
        }
    }
    
    augment_path();
    clear_covers();
    erase_primes();
 
    #ifdef _DEBUG_MUNKRES_
    std::cout << "Going to Step 3"<<std::endl;
    print_cost_matrix();
    step7();
    #endif
 
    step3();
}

/**
 * Add the value found in Step 4 to every element of each covered row, and 
 * subtract it from every element of each uncovered column. Return to Step 4
 * without altering stars, primes or covered lines
 */

void Munkres :: step6 ()
{
    int minval = std::numeric_limits<int>::max();
    find_smallest(minval);
 
    #ifdef _DEBUG_MUNKRES_
    std::cout<<minval<<std::endl;
    #endif
 
    for (int r = 0; r < nrow_; ++r)
    {
        for (int c = 0; c < ncol_; ++c)
        {
            if (row_cover_[r] == 1)
            {
                cost_matrix_[r][c] += minval;
            }
            if (col_cover_[c] == 0)
            {
                cost_matrix_[r][c] -= minval;
            }
        }
    }
 
    #ifdef _DEBUG_MUNKRES_
    std::cout << "Going to Step 4"<<std::endl;
    print_cost_matrix();
    step7();
    #endif
 
    step4();
}

/**
 * Assignment pairs are indicated by the positions of the starred zeros in the 
 * cost matrix. If c(i,j) is a starred zero then the element associated with row i
 * and with the element in column j.
 */
void Munkres :: step7 ()
{
#ifdef _DEBUG_MUNKRES_
    std::cout<<"----------Matching matrix-------------"<<std::endl;
    for (int r = 0; r < nrow_; r++)
    {
        for (int c = 0; c < ncol_; c++)
        {
            std::cout<< matching_matrix_[r][c]<<"\t";
        }
        std::cout<< std::endl;
    }
    std::cout<<"-----------------------"<<std::endl;
#endif
}


void Munkres :: get_mappings (boost::unordered_map <unsigned short int, unsigned short int> &id_mappings)
{
    id_mappings.clear ();
    for (int r = 0; r < nrow_; r++)
    {
        for (int c = 0; c < ncol_; c++)
        {
            if(matching_matrix_[r][c]==1) id_mappings[r]=c;
        }
    }
}


//methods to support step 4
void Munkres::find_a_zero(int &row, int &col)
{
    int r = 0;
    int c;
    bool done;
    row = -1;
    col = -1;
    done = false;
    while (!done)
    {
        c = 0;
        while (true)
        {
            if (cost_matrix_[r][c] == 0 && row_cover_[r] == 0 && col_cover_[c] == 0)
            {
                row = r;
                col = c;
                done = true;
            }
            c += 1;
            if (c >= ncol_ || done)
            {
                break;
            }
        }
        r += 1;
        if (r >= nrow_)
        {
            done = true;
        }
    }
}

bool Munkres::  star_in_row(int row)
{
    bool tmp = false;
    for (int c = 0; c < ncol_; ++c)
    {
        if (matching_matrix_[row][c] == 1)
        {
            tmp = true;
        }
    }
    return tmp;
}

void Munkres:: find_star_in_row(int row, int &col)
{
    col = -1;
    for (int c = 0; c < ncol_; c++)
    {
        if (matching_matrix_[row][c] == 1)
        {
            col = c;
        }
    }
}



// methods to support step 5
void Munkres ::  find_star_in_col(int c,int& r)
{
    r = -1;
    for (int i = 0; i < nrow_; i++)
    {
        // 1 for star
        if (matching_matrix_[i][c] == 1)
        {
            r = i;
        }
    }
}

void Munkres:: find_prime_in_row(int r, int& c)
{
    for (int j = 0; j < ncol_; ++j)
    {
        // 2 for prime
        if (matching_matrix_[r][j] == 2)
        {
            c = j;
        }
    }
}

void Munkres::  augment_path()
{
    for (int p = 0; p < path_count_; ++p)
    {
        if (matching_matrix_[path_matrix_[p] [0]][path_matrix_[p] [1]] == 1)
        {
            matching_matrix_[path_matrix_[p][0]][ path_matrix_[p][1]] = 0;
        }
        else
        {
            matching_matrix_[path_matrix_[p][0]] [path_matrix_[p][1]] = 1;
        }
    }
}

void Munkres::  clear_covers()
{
    for (int r = 0; r < nrow_; ++r)
    {
        row_cover_[r] = 0;
    }
    for (int c = 0; c < ncol_; ++c)
    {
        col_cover_[c] = 0;
    }
}

void Munkres :: erase_primes()
{
    for (int r = 0; r < nrow_; ++r)
    {
        for (int c = 0; c < ncol_; ++c)
        {
            if (matching_matrix_[r] [c] == 2)
            {
                matching_matrix_[r][c] = 0;
            }
        }
    }
}


//methods to support step 6
void Munkres:: find_smallest(int &minval)
{
    for (int r = 0; r < nrow_; ++r)
    {
        for (int c = 0; c < ncol_; ++c)
        {
            if (row_cover_[r] == 0 && col_cover_[c] == 0)
            {
                if (minval > cost_matrix_[r][c])
                {
                    minval = cost_matrix_[r] [c];
                }
            }
        }
    }
}


void Munkres :: print_cost_matrix ()
{
    std::cout<<"----------Cost Matrix-------------"<<std::endl;  
    for (int r = 0; r < nrow_; ++r)
    {
        for (int c = 0; c < ncol_; ++c)
        {
            std::cout<< cost_matrix_[r][c]<<"\t";
        }
        std::cout<< std::endl;
    }
    std::cout<<"-----------------------"<<std::endl;
}
