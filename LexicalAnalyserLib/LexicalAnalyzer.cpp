#include "pch.h"
#include "LexicalAnalyzer.h"

bool Word::operator==(Word& word)
{
    return id==word.id&&symbol==word.symbol;
}

bool Word::operator==(int id)
{
    return this->id == id;
}

S_PTR(LexicalAnalyzer, LexicalAnalyzer::instance) = MK_SPTR(LexicalAnalyzer, );

void LexicalAnalyzer::moveFwd()
{
    pointer--;
}

void LexicalAnalyzer::moveNxt()
{
    pointer++;
}

void LexicalAnalyzer::Init()
{
    pointer = -1;
    inputStr.reset();
    outputResult = MK_SPTR(std::vector<Word>, );
    outputErr = MK_SPTR(std::vector<LexError>, );
}

void LexicalAnalyzer::SourceFileInput(std::string path)
{
    S_PTR(std::ifstream, fileIn) = MK_SPTR(std::ifstream, );
    fileIn->open(path);
    if (!fileIn->is_open())
    {
        std::cout << "Error:Cannot open file:" << path << std::endl;
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
    inputStr = MK_SPTR(std::string, strstream->str());
}

void LexicalAnalyzer::Analyse()
{
    int line = 1;

    while (true)
    {
        Word word;
        S_PTR(std::stringstream, buf) = MK_SPTR(std::stringstream, );
        bool err = false;

#define JUDGE(x,index) else if(get()==x){ *buf<<x; word.id = index; }
#define RESERVE(x,index) if(word.symbol == x){ word.id=index; }

        moveNxt();
        if (SPACE(get()))
        {
            continue;
        }
        else if (LETTER(get()))
        {
            int cnt = 0;
            do 
            {
                if (cnt++ > 16)
                {
                    outputErr->push_back(LexError{ line,"标识符长度溢出" });
                    err = true;
                    break;
                }
                *buf << get();
                moveNxt();
            } while (LETTER(get()) || DIGIT(get()));
            if (!err)
            {
                moveFwd();
                word.id = 10;
            }
        }
        else if (DIGIT(get()))
        {
            int cnt = 0;
            do 
            {
                if (cnt++ > 16)
                {
                    outputErr->push_back(LexError{ line,"常量长度溢出" });
                    err = true;
                    break;
                }
                *buf << get();
                moveNxt();
            } while (DIGIT(get()));
            if (!err)
            {
                moveFwd();
                word.id = 11;
            }
        }
        JUDGE('=',12)
        JUDGE('-',18)
        JUDGE('*',19)
        JUDGE('(',21)
        JUDGE(')',22)
        else if (get() == '<')
        {
            *buf << '<';
            moveNxt();
            if (get() == '=')
            {
                *buf << '=';
                word.id = 14;
            }
            else if (get() == '>')
            {
                *buf << '>';
                word.id = 13;
            }
            else
            {
                word.id = 15;
                moveFwd();
            }
        }
        else if (get() == '>')
        {
            *buf << '>';
            moveNxt();
            if (get() == '=')
            {
                *buf << '=';
                word.id = 16;
            }
            else
            {
                word.id = 17;
                moveFwd();
            }
        }
        else if (get() == ':')
        {
            *buf << ':';
            moveNxt();
            if (get() == '=')
            {
                *buf << '=';
                word.id = 20;
            }
            else
            {
                outputErr->push_back(LexError{ line,"冒号不匹配" });
                continue;
            }
        }
        else if (get() == ';')
        {
            *buf << ';';
            word.id = 23;
        }
        else if (get() == '\n')
        {
            line++;
            *buf << "EOLN";
            word.id = 24;
        }
        else if (get()==EOF)
        {
            *buf << "EOF";
            word.id = 25;
        }
        else
        {
            outputErr->push_back(LexError{ line,"非法字符" });
        }

        if (err)
        {
            continue;
        }

        word.symbol = buf->str();

        //若识别结果为标识符，保留保留字
        if (word.id == 10)
        {
            RESERVE("begin", 1);
            RESERVE("end", 2);
            RESERVE("integer", 3);
            RESERVE("if", 4);
            RESERVE("then", 5);
            RESERVE("else", 6);
            RESERVE("function", 7);
            RESERVE("read", 8);
            RESERVE("write", 9);
        }

        outputResult->push_back(word);
        if (word.id == 25)
        {
            break;
        }
    }

    std::cout << "词法分析完成" << std::endl;
}

bool LexicalAnalyzer::Output()
{
    S_PTR(std::ofstream, fileErr) = MK_SPTR(std::ofstream, );
    S_PTR(std::ofstream, fileDyd) = MK_SPTR(std::ofstream, );

    fileErr->open(fileName + ".err", std::ios_base::out);
    if (!fileErr->is_open())
    {
        std::cout << "无法创建err文件" << std::endl;
        return false;
    }

    fileDyd->open(fileName + ".dyd", std::ios_base::out);
    if (!fileDyd->is_open())
    {
        std::cout << "无法创建dyd文件" << std::endl;
        return false;
    }

    for (auto it = outputResult->begin(); it != outputResult->end(); it++)
    {
        std::string item = it->ToString();
        //std::cout << item << std::endl;
        *fileDyd << item << std::endl;
    }
    if (!outputErr->size())
    {
        std::cout << "未出现错误，词法分析完成" << std::endl;
        return true;
    }
    else
    {
        std::cout << "分析过程中出现如下错误:" << std::endl;
    }
    for (auto it = outputErr->begin(); it != outputErr->end(); it++)
    {
        std::cout << "***LINE:" << it->line << "  " << it->text << std::endl;
        *fileErr << "***LINE:" << it->line << "  " << it->text << std::endl;
    }
    return false;
}

std::string LexicalAnalyzer::GetFileName() const
{
    return fileName;
}
