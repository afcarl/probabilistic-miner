/* 
 * File:   Ratings_Result_Writer.h
 * Author: tksaha
 *
 * Created on September 23, 2014, 6:04 PM
 */

#ifndef RATINGS_RESULT_WRITER_H
#define	RATINGS_RESULT_WRITER_H

#include <fstream>
#include "Output_Writer.h"


class Result_Writer : public Output_Writer
{
public:
    
/// Constructor 
Result_Writer (std::string& filename);

/// Open the output stream
void open() ;

/// Close the output stream
void close() ;

/**
 * 
 * @param[in]: Line to write
 *
 * @return true If successfully writes into the file
 * @return false In case of write failure
 */
bool write_a_line (std::string &line);   
    
private:
/// File stream variable
 std::fstream fs_;

/// User Rating file to write
 std::string rating_file_name_to_write_;
};


#endif	/* RATINGS_RESULT_WRITER_H */

