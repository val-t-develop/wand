/*  SPL - Simple Programming Language compiler
*  Copyright (C) 2022  Valentyn Tymchyshyn
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
 *  Valentyn Tymchyshyn (val.t.develop) (val.t.develo@gmail.com)
 *
 *  IR tree part for if/else.
 */

#pragma once
#include "IRStatement.hpp"
#include "expression/IRExpression.hpp"

class IRIfElse : public IRStatement {
public:
    shared_ptr<IRExpression> cond;
    shared_ptr<IRStatement> thenSt;
    shared_ptr<IRStatement> elseSt;

    IRIfElse(shared_ptr<IRExpression> _cond, shared_ptr<IRStatement> _thenSt, shared_ptr<IRStatement> _elseSt);
};
