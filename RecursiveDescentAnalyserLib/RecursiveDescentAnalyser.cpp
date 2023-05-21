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
    PostProcess();
}

bool RecursiveDescentAnalyser::Output()
{
    S_PTR(std::ofstream, varFile) = MK_SPTR(std::ofstream,fileName+".var");
    S_PTR(std::ofstream, proFile) = MK_SPTR(std::ofstream,fileName+".pro");
    S_PTR(std::ofstream, errFile) = MK_SPTR(std::ofstream,fileName+".err");

    if(errors.empty())
    {
        std::cout<<"未发现语法错误,分析完成"<<std::endl;
    }
    else
    {
        std::cout<<"语法分析发现如下错误:"<<std::endl;
        for(auto errLine : errors)
        {
            ParseError error = errLine.second[0];
            if(error.type==ErrorType::Expected)
            {
                std::cout<<"***"<<error.line<<":Expected"<<" \""<<error.symbol<<"\" after "<<error.preSymbol<<std::endl;
                *errFile<<"***"<<error.line<<":Expected"<<" \""<<error.symbol<<"\" after "<<error.preSymbol<<std::endl;
            }
            else if(error.type == ErrorType::Undefined)
            {
                std::cout<<"***"<<error.line<<":Symbol Undefined"<<" \""<<error.symbol<<"\""<<std::endl;
                *errFile<<"***"<<error.line<<":Symbol Undefined"<<" \""<<error.symbol<<"\""<<std::endl;
            }
            else if(error.type == ErrorType::Redefined)
            {
                std::cout<<"***"<<error.line<<":Symbol Redefined"<<" \""<<error.symbol<<"\""<<std::endl;
                *errFile<<"***"<<error.line<<":Symbol Redefined"<<" \""<<error.symbol<<"\""<<std::endl;
            }
        }
        return false;
    }
    for(auto procedure : procedureTable)
    {
        /*std::cout<<"Procedure:"<<procedure.pname<<std::endl;
        std::cout<<"type:"<<(procedure.ptype == Type::Void ? "Void" : "Integer")<<std::endl;
        std::cout<<"plev:"<<procedure.plev<<std::endl;
        std::cout<<"fadr:"<<procedure.fadr<<std::endl;
        std::cout<<"ladr:"<<procedure.ladr<<std::endl<<std::endl;*/
        
        *proFile<<"Procedure:"<<procedure.pname<<std::endl;
        *proFile<<"type:"<<(procedure.ptype == Type::Void ? "Void" : "Integer")<<std::endl;
        *proFile<<"plev:"<<procedure.plev<<std::endl;
        *proFile<<"fadr:"<<procedure.fadr<<std::endl;
        *proFile<<"ladr:"<<procedure.ladr<<std::endl<<std::endl;
    }

    for(auto var : variableTable)
    {
        /*std::cout<<"vname:"<<var.vname<<std::endl;
        std::cout<<"vproc:"<<var.vproc<<std::endl;
        std::cout<<"kind:"<<(var.vkind==VariableKind::Parameter?"Parameter":"Variable")<<std::endl;
        std::cout<<"vtype:"<<(var.vtype==Type::Void?"Void":"Integer")<<std::endl;
        std::cout<<"vlev:"<<var.vlev<<std::endl;
        std::cout<<"vadr:"<<var.vadr<<std::endl<<std::endl;*/

        *varFile<<"vname:"<<var.vname<<std::endl;
        *varFile<<"vproc:"<<var.vproc<<std::endl;
        *varFile<<"kind:"<<(var.vkind==VariableKind::Parameter?"Parameter":"Variable")<<std::endl;
        *varFile<<"vtype:"<<(var.vtype==Type::Void?"Void":"Integer")<<std::endl;
        *varFile<<"vlev:"<<var.vlev<<std::endl;
        *varFile<<"vadr:"<<var.vadr<<std::endl<<std::endl;
    }
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

void RecursiveDescentAnalyser::ExpectedError(std::string expected)
{
    int t = p;
    Word pre = GetWord(t-1);
    while(pre.id==24)
    {
        t--;
        pre = GetWord(t);
    }
    //std::cout<<"***LINE:"<<line<<" p="<<p<<" "<<notice<<" after \""<<pre.symbol<<"\""<<std::endl;
    errors[line].push_back({line,p,ErrorType::Expected,pre.symbol,expected});
}

void RecursiveDescentAnalyser::UndefinedError(std::string symbol)
{
    errors[line].push_back({line,p,ErrorType::Undefined,"",symbol});
}

void RecursiveDescentAnalyser::RedefinedError(std::string symbol)
{
    errors[line].push_back({line,p,ErrorType::Redefined,"",symbol});
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
            ExpectedError("end");
            return false;
        }
    }
    else
    {
        ExpectedError("begin");
        return false;
    }
    return true;
}

bool RecursiveDescentAnalyser::DeclarativeStatementTable()
{
    if(GetWord()==3)//integer
    {
        MovNext();
        DeclarativeStatement2(Type::Integer);
        if(GetWord() == 23)
        {
            MovNext();
            while(GetWord() == 3)//integer
            {
                MovNext();
                DeclarativeStatement2(Type::Integer);
                if(GetWord() == 23)
                {
                    MovNext();
                }
                else
                {
                    ExpectedError(";");
                    return false;
                }
            }
        }
        else
        {
            ExpectedError(";");
            return false;
        }
    }
    else
    {
        ExpectedError("integer");
        return false;        
    }
}

bool RecursiveDescentAnalyser::DeclarativeStatement2(Type type)
{
    if(GetWord()==7)//function
    {
        MovNext();
        if(GetWord()==10)//identifier
        {
            std::string identifier = GetWord().symbol;
            MovNext();
            if(GetWord()==21)//(
            {
                MovNext();
                Prameter();
                MovFwd();
                Word functionName = GetWord();
                MovNext();
                if(GetWord()==22)//)
                {
                    MovNext();
                    if(GetWord()==23)//;
                    {
                        BeginFunction(identifier,type);
                        MovNext();
                        FunctionBody();
                    }
                    else
                    {
                        ExpectedError(";");
                        return false;
                    }
                }
                else
                {
                    ExpectedError(")");
                    return false;
                }
            }
            else
            {
                ExpectedError("(");
                return false;
            }
        }
        else
        {
            ExpectedError("identifier");
            return false;
        }
    }
    else if(GetWord()==10)//identifier
    {
        Word identifier = GetWord();
        DeclareVariable(identifier.symbol,type);
        MovNext();
    }
    else
    {
        ExpectedError("identifier or function");
        return false;
    }
    return true;
}

bool RecursiveDescentAnalyser::Prameter()
{
    if(GetWord()==10)//identifier
    {
        Word identifier = GetWord();
        parameters.push(identifier.symbol);
        MovNext();
    }
    else
    {
        ExpectedError("identifier");
        return false;
    }
    return true;
}

bool RecursiveDescentAnalyser::VariableReduce()
{
    if(GetWord()==10)//identifier
    {
        Word identifier = GetWord();
        if(procedureStack.top().pname!=identifier.symbol)
        {
            if(!CheckVariableTable(identifier.symbol))
            {
                return false;
            }
        }
        MovNext();
    }
    else
    {
        ExpectedError("identifier");
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
            EndFunction();
            MovNext();
        }
        else
        {
            ExpectedError("end");
            return false;
        }
    }
    else
    {
        ExpectedError("begin");
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
            if(GetWord() == 10)//identifier
            {
                Word identifier = GetWord();
                DeclareVariable(identifier.symbol,Type::Integer);
                MovNext();
                if(GetWord() == 22)//)
                {
                    MovNext();
                }
                else
                {
                    ExpectedError(")");
                    return false;
                }
            }
            else
            {
                ExpectedError("identifier");
                return false;
            }
        }
        else
        {
            ExpectedError("(");
            return false;
        }
    }
    else if(GetWord() == 9)//write
    {
        MovNext();
        if(GetWord()==21)//(
        {
            MovNext();
            VariableReduce();
            if(GetWord() == 22)//)
            {
                MovNext();
            }
            else
            {
                ExpectedError(")");
                return false;
            }
        }
        else
        {
            ExpectedError("(");
            return false;
        }
    }
    else if(GetWord() == 10)//identifier
    {
        VariableReduce();
        if(GetWord() == 20)//:=
        {
            MovNext();
            ArithmeticExpression();
        }
        else
        {
            ExpectedError(":=");
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
                ExpectedError("else");
                return false;
            }
        }
        else
        {
            ExpectedError("then");
            return false;
        }
    }
    else
    {
        ExpectedError("read or write or if or identifier");
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
        Word identifier = GetWord();
        MovNext();
        Factor2(identifier.symbol);
    }
    else if(GetWord() == 11)//constant number
    {
        MovNext();
    }
    else
    {
        ExpectedError("identifier or constant number");
        return false;
    }
    return true;
}

bool RecursiveDescentAnalyser::Factor2(std::string symbol)
{
    if(GetWord() == 21)//(
    {
        CheckProcedureTable(symbol);
        MovNext();
        ArithmeticExpression();
        if(GetWord() == 22)//)
        {
            MovNext();
        }
        else
        {
            ExpectedError(")");
            return false;
        }
    }
    else
    {
        CheckVariableTable(symbol);
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
        ExpectedError("= or <> or <= or < or >= or >");
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
    bool defined=false;
    for(auto procedure : procedureTable)
    {
        if(procedure.pname==procedureName)
        {
            defined=true;
            break;
        }
    }
    std::stack<Procedure> tmp;
    while(!procedureStack.empty())
    {
        tmp.push(procedureStack.top());
        if(procedureStack.top().pname==procedureName)
        {
            defined=true;
        }
        procedureStack.pop();
    }
    while(!tmp.empty())
    {
        procedureStack.push(tmp.top());
        tmp.pop();
    }
    if(defined)
    {
        RedefinedError(procedureName);
    }
    procedureStack.push({procedureName,procedureType,(int)procedureStack.size(),-1,-1});
}

void RecursiveDescentAnalyser::EndFunction()
{
    Procedure procedure = procedureStack.top();
    procedureStack.pop();
    procedure.ladr = variableTable.size() - 1;
    procedureTable.push_back(procedure);
    parameters.pop();
}

void RecursiveDescentAnalyser::DeclareVariable(std::string vName,Type type)
{
    Procedure &procedure = procedureStack.top();
    bool defined=false;
    if(procedure.fadr!=-1)
    {
        for(int i=procedure.fadr;i<variableTable.size();i++)
        {
            Variable var = variableTable[i];
            if(var.vname==vName&&var.vlev==procedure.plev)
            {
                defined=true;
                break;
            }
        }
    }
    if(defined)
    {
        RedefinedError(vName);
        return;
    }
    VariableKind kind = VariableKind::Variable;
    if(parameters.size()>0)
    {
        kind = parameters.top()==vName?VariableKind::Parameter:VariableKind::Variable;
    }
    variableTable.push_back({vName,procedure.pname,kind,type,procedure.plev,(int)variableTable.size()});
    if(procedure.fadr==-1)
    {
        procedure.fadr = variableTable.size()-1;
    }
}

bool RecursiveDescentAnalyser::CheckVariableTable(std::string symbol)
{
    for(const auto var : variableTable)
    {
        if(symbol == var.vname)
        {
            return true;
        }
    }
    UndefinedError(symbol);
    return false;
}

bool RecursiveDescentAnalyser::CheckProcedureTable(std::string symbol)
{
    for(const auto proc : procedureTable)
    {
        if(symbol == proc.pname)
        {
            return true;
        }
    }
    if(procedureStack.top().pname == symbol)
    {
        return true;
    }
    UndefinedError(symbol);
    return false;
}

void RecursiveDescentAnalyser::PostProcess()
{
    for(int i=0;i<variableTable.size();i++)
    {
        variableTable[i].vadr = i;
    }
}