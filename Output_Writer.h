/* 
 * File:   Output_Writer.h
 * Author: tksaha
 *
 * Created on September 23, 2014, 5:53 PM
 */

#ifndef OUTPUT_WRITER_H
#define	OUTPUT_WRITER_H

#include <string>

class	Output_Writer
{
public:

 /// Open the output stream
 virtual void open() = 0 ;

 /// Close the output stream
 virtual void close() = 0 ;

/**
 * 
 * @param[in]: Line to write
 *
 * @return true If successfully writes into the file
 * @return false In case of write failure
 */
 virtual bool write_a_line (std::string &line) = 0;

};


#endif	/* OUTPUT_WRITER_H */

