#pragma once
#include <lexer/Lexer.hpp>

class ParserUtils {
public:

    static map<string, int> operatorsPrecedence;
    static int getBinOpPrecedence(Lexer& lexer);

    static void skipSemicolons(Lexer &lexer);

    static bool isModifier(shared_ptr<Token> token);

    static void skipModifiers(Lexer &lexer);

    static long double parseDouble(string token);
    static int64_t parseLong(string token);

    class QualifiedName {
        vector<shared_ptr<Token>> list = vector<shared_ptr<Token>>();

    public:
        QualifiedName(Lexer &lexer);

        string getText();

        int size();

        void add(shared_ptr<Token> token);

        shared_ptr<Token> get(int index);

        // void add(int index, Token element) { list.insert(std::next(list.begin(),
        // index), element); }

        vector<shared_ptr<Token>> getList();

        void setList(vector<shared_ptr<Token>> list);

        vector<string> getTextList();
    };
};
