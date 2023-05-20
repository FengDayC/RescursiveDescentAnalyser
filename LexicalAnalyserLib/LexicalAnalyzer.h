#pragma once
struct Word
{
    std::string symbol;
    int id;

    std::string ToString()
    {
        S_PTR(std::stringstream, sstream) = MK_SPTR(std::stringstream, );
        for (int i = 0; i < 16 - symbol.length(); i++)
        {
            *sstream << " ";
        }
        *sstream << symbol;
        *sstream << "  " << id;
        return sstream->str();
    }

    bool operator==(Word& word);

    bool operator==(int id);
};

struct Error
{
    int line;
    std::string text;
};

class LexicalAnalyzer
{
private:
    std::string fileName;

private:
    std::shared_ptr<std::string> inputStr;

    S_PTR(std::vector<Word>, outputResult);
    S_PTR(std::vector<Error>, outputErr);

    static std::shared_ptr<LexicalAnalyzer> instance;

    //字符指针
    int pointer;

    inline char get() const
    {
        if (pointer > inputStr->length() - 1)
        {
            return EOF;
        }
        else
        {
            return (*inputStr)[pointer];
        }
    }

    void moveNxt();

    void moveFwd();

public:
    static std::shared_ptr<LexicalAnalyzer> getInstance() { return instance; };

    void Init();

    void SourceFileInput(std::string path);

    void Analyse();

    bool Output();

    std::string GetFileName() const;
    
};