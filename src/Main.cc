/*-
 * Copyright (c) 2021 Fehmi Noyan Isi
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <iostream>
#include <sstream>
#include <fstream>
#include <memory>

#include "Lexer.h"
#include "Parser.h"

using namespace std;

void usage() {
        cout << "usage : blockreader [input_file_name]" << endl;
}

int main(int argc, char **argv) {
        unique_ptr<istream> is;

        if (argc == 1) {
                cout << "enter an expression : ";
                string exp;
                getline(cin, exp);
                istringstream iss(exp);
                is.reset(new istringstream(exp));
        } else if (argc == 2) {
                is.reset(new ifstream(argv[1]));
        } else {
                usage();
                exit(EXIT_FAILURE);
        }

        

        try {        
                Lexer lexer(*is);
                vector<Token> tokens;
                tokens = lexer.scan();

                Parser parser(tokens);
                parser.parse();
        } catch (exception& e) {
                cerr << e.what() << endl;
                exit(EXIT_FAILURE);
        }

        return 0;
}