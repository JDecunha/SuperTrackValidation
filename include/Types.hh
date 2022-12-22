#pragma once

// STL //
#include <memory>
#include <vector>


struct Sphere
{
    double radius;

    double x;
    double y;
    double z;
};


typedef std::vector<Sphere> Spheres;


struct Cell
{
    int id;
    Sphere body;
    Sphere nucleus;
};


typedef std::vector<Cell> Cells;


/*
class InputDataFile
{
  public:
    InputDataFile(std::string filename)
    {
        std::fstream stream;
        stream.open (filename, std::fstream::in);
    };

    
  public:
    static Cells ReadFile(std::fstream file)
    {
        Cells cells;

        std::string line;
        while (std::getline(file, line))
        {
            auto cell = ReadLine(line);
            cells.push_back(cell);
        }

        return cells;
    }

    
    static std::shared_ptr<Cell> ReadLine(std::string line)
    {
        auto cell = std::make_shared<Cell>();

        std::string token;
        std::vector<std::string> tokens;

        while (std::cin >> )
        {
            tokens.push_back(token);
        }

        // We expect exactly 7 tokens.

        return cell;
    }
};
*/

