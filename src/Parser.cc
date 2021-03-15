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
#include <optional>

#include "Parser.h"
#include "Token.h"
#include "Exception.h"
#include "Symbol.h"
#include "Type.h"

void Parser::match (TokenType t) {
        if (lookahead.type == t) {
                // advance the token
                index++;
                lookahead = tokens.at(index);
        } else {
                throw Exception("Parse error: expecting " + t.str() + " but got " + lookahead.type.str() + ".");
        }
}

void Parser::block() {
        // symbol table related code
        Environment *saved = top;
        top = new Environment(top);
        std::cout << "{ ";
        blockId++;

        // parser code
        match(TokenType::LeftP);
        decls();
        stmts();
        match(TokenType::RightP);

        // symbol table related code
        top = saved;
        std::cout << "} ";
        blockId--;
}

void Parser::decls() {
        if (lookahead.type == TokenType::TypeName){
                decl();
                decls();
        }
}

void Parser::decl() {
        // the match() calls here are for the parser. The rest of the code
        // aims to gather information for the symbol table entry
        match(TokenType::TypeName);
        IdentifierType idType = tokens.at(index-1).idType;

        match(TokenType::Identifier);
        std::string id = tokens.at(index-1).lexeme;

        match(TokenType::Semicolon);
        top->put(id, Symbol(idType, blockId));
}

void Parser::factor() {
        match(TokenType::Identifier);
        
        // symbol table code
        std::string id = tokens.at(index-1).lexeme;
        std::optional<Symbol> symbol = top->get(id);
        std::cout << id + ":" ;
        
        IdentifierType identifierType;
        try {
                identifierType = symbol.value().type;
        } catch (std::bad_optional_access e) {
                std::cout << "Undeclared identifier" << std::endl;
                exit(EXIT_FAILURE);
        }

        switch(identifierType) {
                case IdentifierType::Int:
                        std::cout << "Int";
                        break;
                case IdentifierType::Char:
                        std::cout << "Char";
                        break;
                case IdentifierType::Bool:
                        std::cout << "Bool";
                        break;
                default:
                        std::cout << "Invalid";
                        break; 
        }
}

void Parser::stmt() {
        if (lookahead.type == TokenType::LeftP) {
                block();
        } else if(lookahead.type == TokenType::Identifier) {
                factor();
                match(TokenType::Semicolon);
                std::cout << "; ";
        } else {
                throw Exception("Parser error: expecting a statement but got " + lookahead.type.str());
        }
}

void Parser::stmts(){
        TokenType t = lookahead.type;
        if (t == TokenType::LeftP || t == TokenType::Identifier) {
                stmt();
                stmts();
        }
}