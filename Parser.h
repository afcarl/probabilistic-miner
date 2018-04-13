#ifndef _PARSER_H
#define _PARSER_H

#include <string>
#include <vector>
#include <boost/unordered/unordered_map.hpp>

class Parser
{
public:
 ///
 virtual void parse_a_line  (std::string line_to_parse, 
         char delim, 
         std::vector<std::string>& token_vector);
 
 ///
 virtual void parse_program_argument (int argc, char* argv[], boost::unordered_map <std::string, std::string>& token_map);
 
private:
 ///
 bool is_even (int number);
};



#endif  /// !defined _PARSER_H_
