
struct Word;

enum class VariableKind
{
    Variable = 0,
    Parameter = 1
};

enum class Type
{
    Void = 0,
    Integer = 1
};

struct Variable
{
    std::string vname;
    std::string vproc;
    VariableKind vkind;
    Type vtype;
    int vlev;
    int vadr;
};

struct Procedure
{
    std::string pname;
    Type ptype;
    int plev;
    int fadr;
    int ladr;
};

struct ParseError
{
    int line;
    int p;
    std::string preSymbol;
    std::string symbol;
};

class RecursiveDescentAnalyser
{
public:

    void Init();

    void SourceFileInput(std::string path);

    void Analyse();

    bool Output();
public:
    RecursiveDescentAnalyser(/* args */);
    ~RecursiveDescentAnalyser();

    static std::shared_ptr<RecursiveDescentAnalyser> getInstance();
private:
    
    int p,line;

    std::vector<Word> words;

    std::string fileName;

    std::stack<Procedure> procedureStack;

    std::map<int,std::vector<ParseError>> errors;

    std::vector<Variable> variableTable;

    std::vector<Procedure> procedureTable;

    static std::shared_ptr<RecursiveDescentAnalyser> instance;

private:

    void AnalyseInputString(std::string inputStr);

    Word GetWord();

    Word GetWord(int p);

    void MovNext();

    void MovFwd();

    void Error(std::string expected);

private:

    bool Program();

    bool SubProgram();

    bool DeclarativeStatementTable();

    bool DeclarativeStatement2();

    bool FunctionDeclaration();

    bool Prameter();

    bool FunctionBody();

    bool ExecutableStatementTable();

    bool ExecutableStatement();

    bool ArithmeticExpression();

    bool Item();

    bool Factor();

    bool Factor2();

    bool ConditionalExpression();

private:

    void BeginProgram();

    void EndProgram();

    void BeginFunction(std::string procedureName, Type procedureType);

    void EndFunction();

    void DeclareVariable();

    bool CheckVariableTable(std::string symbol);
};
