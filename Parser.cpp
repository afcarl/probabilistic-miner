
#include <sstream>/// for istringstream
#include "Parser.h"
 
/// Parse a line using delimiter and put the token in token list
void Parser :: parse_a_line (std::string line_to_parse, 
        char delim, 
        std::vector<std::string>& token_vector)
{
 std::istringstream iss (line_to_parse);
 std::string token;

 // Clears the list
 token_vector.clear();

 while (std::getline (iss,token, delim))
 {
  token_vector.push_back (token);
 }

}

bool Parser :: is_even (int number)
{
 return (number % 2 == 0);
}


/// Parse argument and put it into token map
void Parser :: parse_program_argument (int argc, char* argv[], boost::unordered_map <std::string, std::string>& token_map)
{
 /// Number of argument must be odd
 if (is_even (argc))
 {
  return ; 
 }

 // Clears the map before use
 token_map.clear();
 
 for (int i=1; i <argc; i=i+2)
 {
  token_map[argv[i]]= argv[i+1];
 }
}

