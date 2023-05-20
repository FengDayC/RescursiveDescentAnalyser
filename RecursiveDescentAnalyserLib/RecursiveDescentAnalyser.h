
struct Word;

struct Variable
{

};

struct Procedure
{

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

    static std::shared_ptr<RecursiveDescentAnalyser> instance;

private:

    void AnalyseInputString(std::string inputStr);

    Word GetWord();

    Word GetWord(int p);

    void MovNext();

    void MovFwd();

    void Error(std::string notice);

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
};
