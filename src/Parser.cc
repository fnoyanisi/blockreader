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

// this is the implementation of the usual match() 
// function found in the recursive descent parsers
// it advances the token by incrementing an index
// counter
void Parser::match (TokenType t) {
        if (lookahead.type == t) {
                // advance the token
                index++;
                lookahead = tokens.at(index);
        } else {
                throw Exception("Parse error: expecting " + t.str() + " but got " + lookahead.type.str() + ".");
        }
}

// implementation of the production defining
// a program block
// block -> '{' decls stmst '}'
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

// implementation of decls
// decls -> decl decls | E
// checks whether the current token is an identifier
// type name otherwise skips
void Parser::decls() {
        if (lookahead.type == TokenType::TypeName){
                decl();
                decls();
        }
}

// decl -> type id ';'
// the way the values are retrieved, i.e. using index-1
// can be improved. It is assumed that if the parser is 
// past a correspanding match() statement, then it is
// safe the expect the previous token (remember, match() 
// advanced the index) includes a certain type of token
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

// factor -> id
// the code for this method is mostly symbol table related
// the only semantic action is to match the current token type
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

        // like we did with the TokenType class, the 
        // IdentifierType enum class could be converted to a normal
        // class which would then allow us to define a method such 
        // as str() to return the string representation of each
        // enumaration valiue
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

// stmt -> block | factor ';'
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

// stmts -> stmt stmts | E
void Parser::stmts(){
        TokenType t = lookahead.type;
        if (t == TokenType::LeftP || t == TokenType::Identifier) {
                stmt();
                stmts();
        }
}