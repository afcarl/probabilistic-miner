#include "Graph_DB_File_Reader.h"

/// Constructor
Graph_DB_File_Reader :: Graph_DB_File_Reader (std::string& filename)
: graph_db_file_name_(filename)
{
}


/// Opens the file
void Graph_DB_File_Reader :: open ()
{
 fs_.open(graph_db_file_name_.c_str(),std::fstream::in);
}


/// Get a line from the file
bool Graph_DB_File_Reader:: get_line (std::string &line)
{
 if(!std::getline (fs_,line))
 {
  return false;
 }
 else{
  return true;
 }
}

/// Closes the file if it is open
void Graph_DB_File_Reader :: close ()
{
 if (fs_.is_open())
 {
  fs_.close();
 }
}

