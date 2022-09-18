#pragma once
#include <Defs.hpp>

class Out {
public: 
    static int messages;

    static void errorMessage(string msg);
    static void printMessage(string msg);

    /*static void errorMessage(Lexer lexer, String msg){
        if (messages <= 50) {
            System.err.println("\n"+lexer.getFilePath().toString()+": "+msg);
            messages++;
        } else {
            System.err.println("\n Too many errors! Exit!");
            System.exit(-1);
        }
    }*/
};