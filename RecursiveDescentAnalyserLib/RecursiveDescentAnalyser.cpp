#include "pch.h"
#include "RecursiveDescentAnalyser.h"
#include "LexicalAnalyzer.h"


std::shared_ptr<RecursiveDescentAnalyser> RecursiveDescentAnalyser::instance;

RecursiveDescentAnalyser::RecursiveDescentAnalyser()
{
}

RecursiveDescentAnalyser::~RecursiveDescentAnalyser()
{
}

std::shared_ptr<RecursiveDescentAnalyser> RecursiveDescentAnalyser::getInstance()
{
    if(instance==nullptr)
    {
        instance = MK_SPTR(RecursiveDescentAnalyser,);
    }
    return instance;
}

void RecursiveDescentAnalyser::Init()
{
    p = 0;
    line = 0;
    while(!procedureStack.empty())
    {
        procedureStack.pop();
    }
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

void RecursiveDescentAnalyser::Analyse()
{
    Program();
}

bool RecursiveDescentAnalyser::Output()
{
    return true;
}

void RecursiveDescentAnalyser::AnalyseInputString(std::string inputStr)
{
    int length = inputStr.length();
    int index=0,id;
    S_PTR(std::stringstream,sstream) = MK_SPTR(std::stringstream,);
    Word newWord;
    while(index<length)
    {
        id=0;
        sstream->str("");
        for(int i=0;i<16;i++,index++)
        {
            if(inputStr[index]==' ')
            {
                continue;
            }
            else
            {
                *sstream<<inputStr[index];
            }
        }
        index++;
        newWord.symbol = sstream->str();
        while(inputStr[++index]!='\n')
        {
            id*=10;
            id+=(inputStr[index]-'0');
        }
        index++;
        newWord.id = id;
        words.push_back(newWord);
    }
}

Word RecursiveDescentAnalyser::GetWord()
{
    return words[p];
}

Word RecursiveDescentAnalyser::GetWord(int p)
{
    return words[p];
}

void RecursiveDescentAnalyser::MovNext()
{
    p++;
    while(GetWord()==24)
    {
        line++;
        p++;
    }
}

void RecursiveDescentAnalyser::MovFwd()
{
    p--;
}

void RecursiveDescentAnalyser::Error(std::string expected)
{
    int t = p;
    Word pre = GetWord(t-1);
    while(pre.id==24)
    {
        t--;
        pre = GetWord(t);
    }
    //std::cout<<"***LINE:"<<line<<" p="<<p<<" "<<notice<<" after \""<<pre.symbol<<"\""<<std::endl;
    errors[line].push_back({line,p,pre.symbol,expected});
}

bool RecursiveDescentAnalyser::Program()
{
    SubProgram();
    return true;
}

bool RecursiveDescentAnalyser::SubProgram()
{
    if(GetWord()==1)//begin
    {
        BeginProgram();
        MovNext();
        DeclarativeStatementTable();
        ExecutableStatementTable();
        if(GetWord()==2)//end
        {
            EndProgram();
            MovNext();
        }
        else
        {
            Error("end");
            return false;
        }
    }
    else
    {
        Error("begin");
        return false;
    }
    return true;
}

bool RecursiveDescentAnalyser::DeclarativeStatementTable()
{
    if(GetWord()==3)//integer
    {
        MovNext();
        DeclarativeStatement2();
        if(GetWord() == 23)
        {
            MovNext();
            while(GetWord() == 3)//integer
            {
                MovNext();
                DeclarativeStatement2();
                if(GetWord() == 23)
                {
                    MovNext();
                }
                else
                {
                    Error(";");
                    return false;
                }
            }
        }
        else
        {
            Error(";");
            return false;
        }
    }
    else
    {
        Error("integer");
        return false;        
    }
}

bool RecursiveDescentAnalyser::DeclarativeStatement2()
{
    if(GetWord()==7)//function
    {
        MovNext();
        if(GetWord()==10)//identifier
        {
            MovNext();
            if(GetWord()==21)//(
            {
                MovNext();
                Prameter();
                if(GetWord()==22)//)
                {
                    MovNext();
                    if(GetWord()==23)//;
                    {
                        MovNext();
                        FunctionBody();
                    }
                    else
                    {
                        Error(";");
                        return false;
                    }
                }
                else
                {
                    Error(")");
                    return false;
                }
            }
            else
            {
                Error("(");
                return false;
            }
        }
        else
        {
            Error("identifier");
            return false;
        }
    }
    else if(GetWord()==10)//identifier
    {
        MovNext();
    }
    else
    {
        Error("identifier or function");
        return false;
    }
    return true;
}

bool RecursiveDescentAnalyser::Prameter()
{
    if(GetWord()==10)//identifier
    {
        MovNext();
    }
    else
    {
        Error("identifier");
        return false;
    }
    return true;
}

bool RecursiveDescentAnalyser::FunctionBody()
{
    if(GetWord()==1)//begin
    {
        MovNext();
        DeclarativeStatementTable();
        ExecutableStatementTable();
        if(GetWord()==2)//end
        {
            MovNext();
        }
        else
        {
            Error("end");
            return false;
        }
    }
    else
    {
        Error("begin");
        return false;
    }
    return true;
}

bool RecursiveDescentAnalyser::ExecutableStatementTable()
{
    ExecutableStatement();
    while(GetWord()==23)//;
    {
        MovNext();
        ExecutableStatement();
    }
    return true;
}

bool RecursiveDescentAnalyser::ExecutableStatement()
{
    if(GetWord() == 8)//read
    {
        MovNext();
        if(GetWord()==21)//(
        {
            MovNext();
            if(GetWord()==10)//identifier
            {
                MovNext();
                if(GetWord() == 22)//)
                {
                    MovNext();
                }
                else
                {
                    Error(")");
                    return false;
                }
            }
            else
            {
                Error("identifier");
                return false;
            }
        }
        else
        {
            Error("(");
            return false;
        }
    }
    else if(GetWord() == 9)//write
    {
        MovNext();
        if(GetWord()==21)//(
        {
            MovNext();
            if(GetWord()==10)//identifier
            {
                MovNext();
                if(GetWord() == 22)//)
                {
                    MovNext();
                }
                else
                {
                    Error(")");
                    return false;
                }
            }
            else
            {
                Error("identifier");
                return false;
            }
        }
        else
        {
            Error("(");
            return false;
        }
    }
    else if(GetWord() == 10)//identifier
    {
        MovNext();
        if(GetWord() == 20)//:=
        {
            MovNext();
            ArithmeticExpression();
        }
        else
        {
            Error(":=");
            return false;
        }
    }
    else if(GetWord() == 4)//if
    {
        MovNext();
        ConditionalExpression();
        if(GetWord() == 5)//then
        {
            MovNext();
            ExecutableStatement();
            if(GetWord() == 6)//else
            {
                MovNext();
                ExecutableStatement();
            }
            else
            {
                Error("else");
                return false;
            }
        }
        else
        {
            Error("then");
            return false;
        }
    }
    else
    {
        Error("read or write or if or identifier");
        return false;
    }
    return true;
}

bool RecursiveDescentAnalyser::ArithmeticExpression()
{
    Item();
    while(GetWord()==18)
    {
        MovNext();
        Item();
    }
    return true;
}

bool RecursiveDescentAnalyser::Item()
{
    Factor();
    while(GetWord()==19)//*
    {
        MovNext();
        Factor();
    }
    return true;
}

bool RecursiveDescentAnalyser::Factor()
{
    if(GetWord() == 10)//identifier
    {
        MovNext();
        Factor2();
    }
    else if(GetWord() == 11)//constant number
    {
        MovNext();
    }
    else
    {
        Error("identifier or constant number");
        return false;
    }
    return true;
}

bool RecursiveDescentAnalyser::Factor2()
{
    if(GetWord() == 21)//(
    {
        MovNext();
        ArithmeticExpression();
        if(GetWord() == 22)//)
        {
            MovNext();
        }
        else
        {
            Error(")");
            return false;
        }
    }
    return true;
}

bool RecursiveDescentAnalyser::ConditionalExpression()
{
    ArithmeticExpression();
    Word conditionOperator = GetWord();
    if(conditionOperator == 12
        ||conditionOperator == 13
        ||conditionOperator == 14
        ||conditionOperator == 15
        ||conditionOperator == 16
        ||conditionOperator == 17)//condition operator
    {
        MovNext();
        ArithmeticExpression();
    }
    else
    {
        Error("= or <> or <= or < or >= or >");
        return false;
    }
    return true;
}

void RecursiveDescentAnalyser::BeginProgram()
{
    procedureStack.push({"Program",Type::Void,0,-1,-1});
}

void RecursiveDescentAnalyser::EndProgram()
{
    Procedure procedure = procedureStack.top();
    procedureStack.pop();
    procedure.ladr = variableTable.size() - 1;
    procedureTable.push_back(procedure);
}

void RecursiveDescentAnalyser::BeginFunction(std::string procedureName, Type procedureType)
{
    procedureStack.push({procedureName,procedureType,procedureStack.size(),-1,-1});
}

void RecursiveDescentAnalyser::EndFunction()
{
    Procedure procedure = procedureStack.top();
    procedureStack.pop();
    procedure.ladr = variableTable.size() - 1;
    procedureTable.push_back(procedure);
}

void RecursiveDescentAnalyser::DeclareVariable()
{

}

bool RecursiveDescentAnalyser::CheckVariableTable(std::string symbol)
{

}