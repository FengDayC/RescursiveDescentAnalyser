#include "pch.h"
#include "LexicalAnalyzer.h"
#include "RecursiveDescentAnalyser.h"

int main(int argc, char** argv) 
{
    std::string path,fileName;
    path = std::filesystem::current_path().u8string();
    if(argc>0)
    {
        char* str = argv[1];
        fileName = std::string(str);
    }
    else
    {
        std::cout<<"Error:Expected file name!"<<std::endl;
        return 1;
    }
    LexicalAnalyzer::getInstance()->Init();
    std::string slash;
    #ifdef BUILT_IN_UNIX
        slash = "/";
    #elif defined(BUILT_IN_WINDOWS) 
        slash = "\\";
    #endif
    LexicalAnalyzer::getInstance()->SourceFileInput(path+slash+fileName+".pas");
    LexicalAnalyzer::getInstance()->Analyse();
    if(!LexicalAnalyzer::getInstance()->Output())
    {
        return 1;
    }
    RecursiveDescentAnalyser::getInstance()->Init();
    RecursiveDescentAnalyser::getInstance()->SourceFileInput(path+slash+fileName+".dyd");
    RecursiveDescentAnalyser::getInstance()->Analyse();
    RecursiveDescentAnalyser::getInstance()->Output();
    return 0;
}
