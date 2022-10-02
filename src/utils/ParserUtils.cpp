#include "ParserUtils.hpp"

void ParserUtils::skipSemicolons(Lexer &lexer) {
    while(true) {
        if(lexer.getCurrent()->kind == Token::Kind::SEMICOLON) {
            lexer.goForward();
        } else {
            break;
        }
    }
}

bool ParserUtils::isModifier(shared_ptr<Token> token) {
    return token->kind == Token::Kind::PUBLIC ||
           token->kind == Token::Kind::PRIVATE ||
           token->kind == Token::Kind::PROTECTED ||
           token->kind == Token::Kind::STATIC ||
           token->kind == Token::Kind::FINAL ||
           token->kind == Token::Kind::VOLATILE ||
           token->kind == Token::Kind::TRANSIENT ||
           token->kind == Token::Kind::SYNCHRONIZED ||
           token->kind == Token::Kind::ABSTRACT ||
           token->kind == Token::Kind::NATIVE ||
           token->kind == Token::Kind::STRICTFP ||
           token->kind == Token::Kind::CONST;
}

void ParserUtils::skipModifiers(Lexer &lexer) {
    while(true) {
        if(isModifier(lexer.getCurrent())) {
            lexer.goForward();
        } else {
            break;
        }
    }
}

ParserUtils::QualifiedName::QualifiedName(Lexer &lexer) {
    while(true) {
        if(lexer.getNext()->kind == Token::Kind::DOT) {
            list.push_back(lexer.getCurrent());
            lexer.goForward();
            lexer.goForward();
        } else {
            break;
        }
    }
    list.push_back(lexer.getCurrent());
    lexer.goForward();
}

string ParserUtils::QualifiedName::getText() {
    string str = "";
    for(shared_ptr<Token> t : list) {
        str.append(t->str).append(".");
    }
    if(!str.empty()) {
        str.pop_back();
    }
    return str;
}

int ParserUtils::QualifiedName::size() {
    return list.size();
}

void ParserUtils::QualifiedName::add(shared_ptr<Token> token) {
    list.push_back(token);
}

shared_ptr<Token> ParserUtils::QualifiedName::get(int index) {
    return list[index];
}

vector<shared_ptr<Token>> ParserUtils::QualifiedName::getList() {
    return list;
}

void ParserUtils::QualifiedName::setList(vector<shared_ptr<Token>> list) {
    this->list = list;
}
