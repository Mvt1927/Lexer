#include "iostream"
#include "fstream"
#include "vector"
#include "Token.cpp"
#include "Source.h"
using namespace std;

bool isWhiteSpace(char c)
{
    if (c == ' ' || c == '\n' || c == '\t')
    {
        return true;
    }
    return false;
}

bool isIdentifierStart(char c)
{
    return std::isalpha(c) || c == '_';
}

bool isIdentifierPart(char ch)
{
    return std::isalnum(ch) || ch == '_';
}

bool isNot(char c)
{
    if (c == '!')
    {
        return true;
    }
    return false;
}

bool isAndOrOrStart(char c)
{
    if (c == '&' || c == '|')
    {
        return true;
    }
    return false;
}

bool isAndOrOrPart(char pc, char c)
{
    if (pc == c)
    {
        return true;
    }
    return false;
}
bool isAddop(char c)
{
    if (c == '+' || c == '-')
    {
        return true;
    }
    return false;
}

bool isMulop(char c)
{
    if (c == '*' || c == '/')
    {
        return true;
    }
    return false;
}

bool isRelop(char c)
{
    if (c == '<' || c == '>')
    {
        return true;
    }
    return false;
}

bool isEqual(char c)
{
    if (c == '=')
    {
        return true;
    }
    return false;
}

bool isBracket(char bracket)
{
    char brackets[] = {'{', '}', '(', ')', '[', ']', ';', ','};
    for (char key : brackets)
    {
        if (bracket == key)
        {
            return true;
        }
    }
    return false;
}

bool isKeyword(string keywork)
{
    string keyworks[] = {"while", "if", "else", "return", "break", "continue", "int", "float", "void"};
    for (string key : keyworks)
    {
        if (keywork == key)
        {
            return true;
        }
    }
    return false;
}

class Lexer
{
private:
    bool atEOF = false;
    char ch;
    vector<Token> ListTokens;
    Source source;

public:
    Lexer(string sourcePath);

    int getSourceSize()
    {
        return source.getSize();
    }
    bool isEOF()
    {
        return atEOF;
    }

    pair<string, string> newIdToken(string id)
    {
        return {"identifier", id};
    }
    pair<string, string> newKeyworkToken(string id)
    {
        return {"keywork", id};
    }
    pair<string, string> newNumberToken(string number)
    {
        return {"num", number};
    }

    pair<string, string> newErrorToken(string error)
    {
        return {"Error", error};
    }
    pair<string, string> makeOpToken(string op)
    {
        return {op, op};
    }

    pair<string, string> nextToken()
    {
        if (atEOF)
        {
            return {"", ""};
        }
        try
        {
            while (isWhiteSpace(ch))
            { // scan past whitespace
                ch = source.read();
            }
        }
        catch (const std::exception &e)
        {
            atEOF = true;
            return nextToken();
        }
        if (isIdentifierStart(ch))
        {
            // return tokens for ids and reserved words
            string id = "";
            try
            {
                do
                {
                    id += ch;
                    ch = source.read();
                } while (isIdentifierPart(ch));
            }
            catch (const std::exception &e)
            {
                atEOF = true;
            }
            if (isKeyword(id))
            {
                return newKeyworkToken(id);
            }
            return newIdToken(id);
        }
        if (isdigit(ch))
        {
            try
            {
                string number = "";
                number += ch;
                ch = source.read();
                if (isdigit(ch))
                {
                    do
                    {
                        number += ch;
                        ch = source.read();
                    } while (isdigit(ch));
                    if (ch == '.')
                    {
                        do
                        {
                            number += ch;
                            ch = source.read();
                        } while (isdigit(ch));
                        // return newNumberToken(number);
                    }
                    if (ch == 'e' || ch == 'E')
                    {
                        number += ch;
                        ch = source.read();
                        if (isAddop(ch))
                        {
                            number += ch;
                            ch = source.read();
                            if (!isdigit(ch))
                            {
                                return newErrorToken(number);
                            }
                        }
                        do
                        {
                            number += ch;
                            ch = source.read();
                        } while (isdigit(ch));
                        return newNumberToken(number);
                    }

                    else
                    {
                        return newNumberToken(number);
                    }
                }
                else if (ch == '.')
                {
                    do
                    {
                        number += ch;
                        ch = source.read();
                    } while (isdigit(ch));
                    if (ch == 'E' || ch == 'e')
                    {
                        number += ch;
                        ch = source.read();
                        if (ch == '+' || ch == '-')
                        {
                            number += ch;
                            ch = source.read();
                            if (isdigit(ch))
                            {
                                do
                                {
                                    number += ch;
                                    ch = source.read();
                                } while (isdigit(ch));

                                return newNumberToken(number);
                            }
                            else
                            {
                                return newErrorToken(number);
                            }
                        }
                        else if (isdigit(ch))
                        {
                            do
                            {
                                number += ch;
                                ch = source.read();
                            } while (isdigit(ch));
                            return newNumberToken(number);
                        }
                        else
                        {
                            return newErrorToken(number);
                        }
                    }

                    else
                    {
                        return newNumberToken(number);
                    }
                }

                else
                {
                    return newNumberToken(number);
                }
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }
        }
        // if (ch == '.')
        // {
        //     try
        //     {
        //         string number = "";
        //         number += ch;
        //         ch = source.read();
        //         if (isdigit(ch))
        //         {
        //             do
        //             {
        //                 number += ch;
        //                 ch = source.read();
        //             } while (isdigit(ch));

        //             return newNumberToken(number);
        //         }
        //         else
        //         {
        //             return newErrorToken(number);
        //         }
        //     }
        //     catch (const std::exception &e)
        //     {
        //         std::cerr << e.what() << '\n';
        //     }
        // }
        if (isAddop(ch) || isMulop(ch))
        {
            string op = "";
            op.push_back(ch);
            ch = source.read();
            return makeOpToken(op);
        }

        if (isNot(ch))
        {
            string op = "";
            op.push_back(ch);
            ch = source.read();
            if (isEqual(ch))
            {
                op.push_back(ch);
                ch = source.read();
            }
            return makeOpToken(op);
        }
        if (isEqual(ch))
        {
            string op = "";
            op.push_back(ch);
            ch = source.read();
            if (isEqual(ch))
            {
                op.push_back(ch);
                ch = source.read();
            }
            return makeOpToken(op);
        }
        if (isRelop(ch))
        {
            string op = "";
            op.push_back(ch);
            ch = source.read();
            if (isEqual(ch))
            {
                op.push_back(ch);
                ch = source.read();
            }
            return makeOpToken(op);
        }
        if (isBracket(ch))
        {
            string op = "";
            op.push_back(ch);
            ch = source.read();
            return makeOpToken(op);
        }

        string error = "";
        error.push_back(ch);

        if (error == "")
        {
            atEOF = true;
            return {"",""};
        }
        
        if (source.isEOF())
        {
            atEOF = true;
        }
        else
        {
            ch = source.read();
        }
        return newErrorToken(error);
    }
};

Lexer::Lexer(string sourcePath)
{
    cout << "Lexer" << endl;
    this->source = Source(sourcePath);
    cout << source.getSource() << endl;
    ch = source.read();
}

int main(int argc, char const *argv[])
{
    pair<string, string> token;
    try
    {
        Lexer lexer = Lexer("test1.txt");
        while (!lexer.isEOF())
        {
            token = lexer.nextToken();
            if (token.first != "null" && token.first != ""&& token.second != "")
            {
                cout << token.first << ":'" << token.second << "'" << endl;
            }
        }
        // for (int i = 0; i < lexer.getSourceSize() || lexer.isEOF(); i++)
        // {
        //     token = lexer.nextToken();
        //     if (token.first != "null" && token.first != "")
        //     {
        //         cout << token.first << ":'" << token.second << "'" << endl;
        //     }
        // }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    string n;
    cin >> n;
    return 0;
}