#include "pch.h"
#include "RecursiveDescentAnalyser.h"
#include "LexicalAnalyzer.h"


std::shared_ptr<RecursiveDescentAnalyser> RecursiveDescentAnalyser::instance;

RecursiveDescentAnalyser::RecursiveDescentAnalyser(/* args */)
{
}

RecursiveDescentAnalyser::~RecursiveDescentAnalyser()
{
}

std::shared_ptr<RecursiveDescentAnalyser> RecursiveDescentAnalyser::getInstance()
{
    return std::move(instance);
}

void RecursiveDescentAnalyser::SourceFileInput(std::string path)
{
    S_PTR(std::ifstream, fileIn) = MK_SPTR(std::ifstream, );
    fileIn->open(path);
    if (!fileIn->is_open())
    {
        std::cout << "cannot open file:" << path << std::endl;
    }
    std::string::size_type iPos;
    #ifdef BUILT_IN_WIN32
        iPos = path.find_last_of('\\') + 1;
    #elif defined BUILT_IN_UNIX
        iPos = path.find_last_of('/') + 1;
    #endif
    std::string filename = path.substr(iPos, path.length() - iPos);

    fileName = filename.substr(0, filename.rfind("."));

    S_PTR(std::stringstream, strstream) = MK_SPTR(std::stringstream, );
    (*strstream) << fileIn->rdbuf();
    std::string inputStr = strstream->str();
    AnalyseInputString(inputStr);
}

void RecursiveDescentAnalyser::AnalyseInputString(std::string inputStr)
{
    int length = inputStr.length();
    int index=0,id;
    S_PTR(std::stringstream,sstream) = MK_SPTR(std::stringstream,);
    while(index<length)
    {
        Word newWord;
        id=0;
        sstream->clear();
        for(int i=0;i<16;i++,index++)
        {
            if(inputStr[i]==' ')
            {
                continue;
            }
            else
            {
                *sstream<<inputStr[i];
            }
        }
        newWord.symbol = sstream->str();
        while(inputStr[++index]!='\n')
        {
            id*=10;
            id+=(inputStr[index]-'0');
        }
        newWord.id = id;
        words.push_back(newWord);
    }
}