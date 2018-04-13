#ifndef _GRAPH_DB_FILE_READER_H_
#define _GRAPH_DB_FILE_READER_H_

#include <fstream>
#include "Input_Reader.h"

/**
 *
 * @class: Graph_DB_File_Reader
 *
 * Implements File Reader for Graph database file
 *
 */

class Graph_DB_File_Reader : public Input_Reader
{
public:
 /// Constructor 
 Graph_DB_File_Reader (std::string& filename);

 /// Opens database file
 void open ();

 /// Close database file
 void close (); 

/**
 * Reads a line from the file and return it to the caller
 *
 * @param [in]: Line currently read from the graph database file
 * 
 * @return true  If successfully reads a line
 * @return false In case of read failure
 *
 */
 bool get_line (std::string &line);

private:
 /// File stream variable
 std::fstream fs_;

 /// Graph database file to read
 std::string graph_db_file_name_;
};

#endif /// !defined _GRAPH_DB_FILE_READER_H
