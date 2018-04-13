#include "Result_Writer.h"


Result_Writer::Result_Writer(std::string& filename) 
:rating_file_name_to_write_(filename)
{

}

void Result_Writer :: open ()
{
 fs_.open(rating_file_name_to_write_.c_str(),std::fstream::out);
}

bool Result_Writer ::write_a_line(std::string& line)
{
 try{
  fs_.write(line.c_str(),line.length());
 }catch(std::exception &ex)
 {
  return false;  
 }
 return true;
}

/// Closes the file if it is open
void Result_Writer :: close ()
{
 if (fs_.is_open())
 {
  fs_.close();
 }
}