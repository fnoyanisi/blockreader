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

        while((ch = is.get()) != std::char_traits<char>::eof()) {
                // skip whitespaces     
                while (std::isspace(ch)) {
                        if (ch == '\n')
                                line_number++;
                        ch = is.get();
                        if (ch == std::char_traits<char>::eof()) {
                                tokens.push_back(Token(TokenType::End, ""));
                                return tokens;
                        }
                        continue;
                }
        
                if (ch == '{') {
                        tokens.push_back(Token(TokenType::LeftP, "{"));
                } else if (ch == '}') {
                        tokens.push_back(Token(TokenType::RightP, "}"));
                } else if (ch == ';') {
                        tokens.push_back(Token(TokenType::Semicolon, ";"));
                } else if (std::isalpha(ch)){

                        // get the whole word
                        // read till either a space char or a reserved char
                        // is found
                        std::string word;
                        while(!std::isspace(ch)) {
                                if (reserved.contains(ch)) {
                                        is.unget();
                                        break;
                                }
                                word += ch;
                                ch = is.get();
                                if (ch == std::char_traits<char>::eof()) {
                                        tokens.push_back(Token(TokenType::End, ""));
                                        return tokens;
                                }
                        }

                        auto search = identifierTypes.find(word);
                        if (search != identifierTypes.end()) {
                                Token token(TokenType::TypeName, word);
                                IdentifierType idType;
                                if (word == "Int") 
                                        idType = IdentifierType::Int;
                                else if (word == "Char")
                                        idType = IdentifierType::Char;
                                else 
                                        idType = IdentifierType::Bool;
                                token.idType = idType;
                                tokens.push_back(token);
                        } else {
                                tokens.push_back(Token(TokenType::Identifier, word));
                        }
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