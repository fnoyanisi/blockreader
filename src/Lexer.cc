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

#include "Type.h"
#include "Lexer.h"
#include "Exception.h"

std::vector<Token> 
Lexer::scan() {
        unsigned line_number = 1;

        while((lookahead = is.get()) != std::char_traits<char>::eof()) {
                // skip whitespaces     
                while (std::isspace(lookahead)) {
                        if (lookahead == '\n')
                                line_number++;
                        continue;
                }
        
                if (lookahead == '{') {
                        tokens.push_back(Token(TokenType::LeftP, "{"));
                } else if (lookahead == '}') {
                        tokens.push_back(Token(TokenType::RightP, "}"));
                } else if (lookahead == ';') {
                        tokens.push_back(Token(TokenType::Semicolon, ";"));
                } else if (std::isalpha(lookahead)){

                        // get the whole word
                        std::string word;
                        while(!std::isspace(lookahead)) {
                                word += lookahead;
                                lookahead = is.get();
                        }

                        TokenType t;
                        auto search = identifierTypes.find(word);
                        if (search != identifierTypes.end()) {
                                t = TokenType::TypeName;
                        } else {
                                t = TokenType::Identifier;
                        }
                        tokens.push_back(Token(t, word));
                } else {
                        throw Exception("Syntax error in line " + std::to_string(line_number));
                }
        }

        tokens.push_back(Token(TokenType::End,""));
        return tokens;
}

void 
Lexer::write(std::ostream& os) const {
        for(auto t: tokens)
                if (t.type != TokenType::End)
                        os << t.lexeme;
}