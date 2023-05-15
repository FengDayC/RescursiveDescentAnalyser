
struct Word;

class RecursiveDescentAnalyser
{
public:

    void SourceFileInput(std::string path);

    void Analyse();

    void Output(std::string path);
public:
    RecursiveDescentAnalyser(/* args */);
    ~RecursiveDescentAnalyser();

    std::shared_ptr<RecursiveDescentAnalyser> getInstance();
private:
    
    std::vector<Word> words;

    std::string fileName;

    static std::shared_ptr<RecursiveDescentAnalyser> instance;

private:

    void AnalyseInputString(std::string inputStr);
};
