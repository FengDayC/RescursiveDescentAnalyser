
struct Word;

class RecursiveDescentAnalyser
{
public:

    void Init();

    void SourceFileInput(std::string path);

    void Analyse();

    void Output(std::string path);
public:
    RecursiveDescentAnalyser(/* args */);
    ~RecursiveDescentAnalyser();

    std::shared_ptr<RecursiveDescentAnalyser> getInstance();
private:
    
    int p,line;

    std::vector<Word> words;

    std::string fileName;

    static std::shared_ptr<RecursiveDescentAnalyser> instance;

private:

    void AnalyseInputString(std::string inputStr);

    Word GetWord();

    void MovNext();

    void MovFwd();

    void Error(std::string notice);

private:

    bool Program();

    bool SubProgram();

    bool DeclarativeStatementTable();

    bool DeclarativeStatement();

    bool VariableDeclaration();

    bool Variable();

    bool FunctionDeclaration();

    bool Prameter();

    bool FunctionBody();

    bool ExecutableStatementTable();

    bool ExecutableStatement();

    bool ReadStatement();

    bool WriteStatement();

    bool AssignmentStatement();

    bool ArithmeticExpression();

    bool Item();

    bool Factor();

    bool ConditionalStatement();

    bool ConditionalExpression();

    bool RelationalOperators();
};
