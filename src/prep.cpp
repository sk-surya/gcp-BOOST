#include "prep.h"

// class to represent a Graph object
namespace Surya{
    std::vector<Edge> readInput(std::string &file_path, int &vertex_count)
    {
        std::ifstream myReadFile;
        std::string keyword = "edge";
        std::string cur_line = "c123";
        std::size_t i = 0;

        myReadFile.open(file_path);
        // std::cout << "status: " << myReadFile.good() << std::endl;
        // getline(myReadFile, cur_line);
        // std::cout << "firstline: " << cur_line << std::endl;

        // skip lines until the line that begins with 'p' is found
        while (!myReadFile.eof() && cur_line[0] != 'p' && i < 100)
        {
            // std::cout << i << ": ";
            // std::cout << cur_line << std::endl;
            
            getline(myReadFile, cur_line);
            // std::cout << cur_line << std::endl;
            i += 1;
        }

        std::vector<std::string> tokens;
        
        tokens = stringToToken(cur_line);
        //std::cout << tokens.size() << std::endl; 
        //for (int i = 0; i < tokens.size(); i++) std::cout << tokens[i] << std::endl;
        vertex_count = stoi(tokens[2]);
        std::size_t edge_count = stoi(tokens[3]);

        // std::cout << "vertex_count: " << vertex_count << std::endl
        //	<< "edge count: " << edge_count;

        std::vector<Edge> edges;
        edges.reserve(edge_count);
        i = 0;
        while (!myReadFile.eof() && i < edge_count)
        {
            // std::cout << cur_line << std::endl;
            // std::cout << i << std::endl;
            i += 1;

            getline(myReadFile, cur_line);
            // std::cout << i << " -> " << cur_line << std::endl;
            tokens = stringToToken(cur_line);

            Edge cur_edge = Edge(stoi(tokens[1]), stoi(tokens[2]));
            edges.push_back(cur_edge);

            // std::cout << cur_line << std::endl;

        }

        myReadFile.close();

        return edges;

    }

    std::vector<std::string> stringToToken(std::string &line)
    {
        std::istringstream iss (line);
        /* std::vector<std::string> tokens;
        copy(istream_iterator<std::string>(iss),
            istream_iterator<std::string>(),
            back_inserter(tokens)); */

        std::vector<std::string> tokens{ std::istream_iterator<std::string>{iss},
                        std::istream_iterator<std::string>{} };

        return tokens;
    }
}
