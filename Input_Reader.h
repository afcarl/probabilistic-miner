#ifndef	_INPUT_READER_
#define	_INPUT_READER_

/**
 *
 * @class: Input_Reader
 *
 * Defines the interface for Reading Input
 *
 */

class	Input_Reader
{
public:

 /// Open the input stream
 virtual void open() = 0 ;

 /// Close the input stream
 virtual void close() = 0 ;

/**
 * Reads a line from the streams and return it to the caller
 *
 * @param[in]: Line currently read from the stream
 *
 * @return true If successfully reads the file
 * @return false In case of read failure
 */
 virtual bool get_line (std::string &line) = 0;

};


#endif  ///!defined _INPUT_READER_H_

