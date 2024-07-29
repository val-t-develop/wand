/*  SPL - Simple Programming Language compiler
*  Copyright (C) 2022-2024  Valentyn Tymchyshyn
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or any
 *  later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/*
 *  Valentyn Tymchyshyn (val.t.develop) (val.t.develop@gmail.com)
 *
 *  IR tree part for function.
 */

#pragma once
#include "IRNode.hpp"
#include "statement/IRBlock.hpp"
#include "statement/IRVarDecl.hpp"

class IRFunction : public IRNode {
public:
    string name;
    string type;
    vector<shared_ptr<IRVarDecl>> args;
    shared_ptr<IRBlock> body;

    IRFunction(string _name, string _type, vector<shared_ptr<IRVarDecl>> _args, shared_ptr<IRBlock> _body);
};
