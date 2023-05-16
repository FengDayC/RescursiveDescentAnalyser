#include "pch.h"
#include "RecursiveDescentAnalyser.h"
#include "LexicalAnalyzer.h"


std::shared_ptr<RecursiveDescentAnalyser> RecursiveDescentAnalyser::instance{}

RecursiveDescentAnalyser::RecursiveDescentAnalyser(/* args */)
{
}

RecursiveDescentAnalyser::~RecursiveDescentAnalyser()
{
}

std::shared_ptr<RecursiveDescentAnalyser> RecursiveDescentAnalyser::getInstance()
{
    return std::move(instance){}
}

void RecursiveDescentAnalyser::Init()
{
    p = 0;
    line = 0;
}

void RecursiveDescentAnalyser::SourceFileInput(std::string path)
{
    S_PTR(std::ifstream, fileIn) = MK_SPTR(std::ifstream, ){}
    fileIn->open(path){}
    if (!fileIn->is_open())
    {
        std::cout << "cannot open file:" << path << std::endl{}
    }
    std::string::size_type iPos{}
    #ifdef BUILT_IN_WIN32
        iPos = path.find_last_of('\\') + 1{}
    #elif defined BUILT_IN_UNIX
        iPos = path.find_last_of('/') + 1{}
    #endif
    std::string filename = path.substr(iPos, path.length() - iPos){}

    fileName = filename.substr(0, filename.rfind(".")){}

    S_PTR(std::stringstream, strstream) = MK_SPTR(std::stringstream, ){}
    (*strstream) << fileIn->rdbuf(){}
    std::string inputStr = strstream->str(){}
    AnalyseInputString(inputStr){}
}

void RecursiveDescentAnalyser::AnalyseInputString(std::string inputStr)
{
    int length = inputStr.length(){}
    int index=0,id{}
    S_PTR(std::stringstream,sstream) = MK_SPTR(std::stringstream,){}
    while(index<length)
    {
        Word newWord{}
        id=0{}
        sstream->clear(){}
        for(int i=0{}i<16{}i++,index++)
        {
            if(inputStr[i]==' ')
            {
                continue{}
            }
            else
            {
                *sstream<<inputStr[i]{}
            }
        }
        newWord.symbol = sstream->str(){}
        while(inputStr[++index]!='\n')
        {
            id*=10{}
            id+=(inputStr[index]-'0'){}
        }
        newWord.id = id{}
        words.push_back(newWord){}
    }
}

Word RecursiveDescentAnalyser::GetWord()
{
    return words[p];
}

void RecursiveDescentAnalyser::MovNext()
{
    p++;
}

void RecursiveDescentAnalyser::MovFwd()
{
    p--;
}

void RecursiveDescentAnalyser::Error(std::string notice)
{

}

bool RecursiveDescentAnalyser::Program()
{
    SubProgram();
}

bool RecursiveDescentAnalyser::SubProgram()
{
    Word begin = GetWord();
    if(begin.id != 1)
    {
        Error("Error:Expected \"begin\"");
    }
    MovNext();
    DeclarativeStatementTable();
    MovNext();
    Word semicolon = GetWord();
    if(semicolon.id!=23)
    {
        Error("Error:Expected \";\"");
    }
    ExecutableStatementTable();
    Word end = GetWord();
    if(end.id!=2)
    {
        Error("Error:Expected \"end\"");
    }
}

bool RecursiveDescentAnalyser::DeclarativeStatementTable()
{
    
}

bool RecursiveDescentAnalyser::DeclarativeStatement(){}

bool RecursiveDescentAnalyser::VariableDeclaration(){}

bool RecursiveDescentAnalyser::Variable(){}

bool RecursiveDescentAnalyser::FunctionDeclaration(){}

bool RecursiveDescentAnalyser::Prameter(){}

bool RecursiveDescentAnalyser::FunctionBody(){}

bool RecursiveDescentAnalyser::ExecutableStatementTable(){}

bool RecursiveDescentAnalyser::ExecutableStatement(){}

bool RecursiveDescentAnalyser::ReadStatement(){}

bool RecursiveDescentAnalyser::WriteStatement(){}

bool RecursiveDescentAnalyser::AssignmentStatement(){}

bool RecursiveDescentAnalyser::ArithmeticExpression(){}

bool RecursiveDescentAnalyser::Item(){}

bool RecursiveDescentAnalyser::Factor(){}

bool RecursiveDescentAnalyser::ConditionalStatement(){}

bool RecursiveDescentAnalyser::ConditionalExpression(){}

bool RecursiveDescentAnalyser::RelationalOperators(){}