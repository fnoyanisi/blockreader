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

#include "Parser.h"
#include "Token.h"
#include "Exception.h"
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
        match(TokenType::LeftP);
        decls();
        stmts();
        match(TokenType::RightP);
}

void Parser::decls() {
        if (lookahead.type == TokenType::TypeName){
                decl();
                decls();
        }
}

void Parser::decl() {
        match(TokenType::TypeName);
        match(TokenType::Identifier);
        match(TokenType::Semicolon);
}

void Parser::stmt() {
        if (lookahead.type == TokenType::LeftP) {
                block();
        } else if(lookahead.type == TokenType::Identifier) {
                match(TokenType::Identifier);
                match(TokenType::Semicolon);
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