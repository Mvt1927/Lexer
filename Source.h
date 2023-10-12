#if !defined(Source_h)
#define Source_h

#include "iostream"
#include "fstream"

class Source
{
private:
    bool eOF = false;
    int pos = 0;
    std::string source = "";

public:
    Source();
    Source(std::string sourcePath);
    std::string getSource();
    char read();
    bool isEOF();
    int getSize(){
        return source.length();
    }
    int getPos(){
        return pos;
    }
};

inline Source::Source()
{
}

inline Source::Source(std::string sourcePath)
{
    std::ifstream file(sourcePath);
    try
    {
        std::string line;
        while (getline(file, line))
        {
            this->source += line + "\n";
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    file.close();
}

inline std::string Source::getSource()
{
    return source;
}

inline char Source::read()
{
    if (this->pos >= this->source.length())
    {
        this->eOF = true;
    }
    return this->source[pos++];
}


inline bool Source::isEOF()
{
    return this->eOF;
}

#endif // Source