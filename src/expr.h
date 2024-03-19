#ifndef Expr_H
#define Expr_H
#include "token.h"
class Visitor;

class Expr{public:
	virtual void accept(Visitor &visitor) {}
};

class Binary   ;
class Grouping ;
class Literal  ;
class Unary    ;
class Visitor{
public:
	virtual void visitBinary   (const Binary    &Expr) = 0;
	virtual void visitGrouping (const Grouping  &Expr) = 0;
	virtual void visitLiteral  (const Literal   &Expr) = 0;
	virtual void visitUnary    (const Unary     &Expr) = 0;
	virtual ~Visitor() = default;
};

class Binary    : public Expr{
public:
	Binary   (Expr left, Token op, Expr right): _left{std::move(left)}
, _op{std::move(op)}
, _right{std::move(right)}
 {}
void accept(Visitor &visitor) override {
	visitor.visitBinary   (*this);
}
	Expr _left;
	Token _op;
	Expr _right;

};

class Grouping  : public Expr{
public:
	Grouping (Expr expression): _expression{std::move(expression)}
 {}
void accept(Visitor &visitor) override {
	visitor.visitGrouping (*this);
}
	Expr _expression;

};

class Literal   : public Expr{
public:
	Literal  (literal_type value): _value{std::move(value)}
 {}
void accept(Visitor &visitor) override {
	visitor.visitLiteral  (*this);
}
	literal_type _value;

};

class Unary     : public Expr{
public:
	Unary    (Token op, Expr right): _op{std::move(op)}
, _right{std::move(right)}
 {}
void accept(Visitor &visitor) override {
	visitor.visitUnary    (*this);
}
	Token _op;
	Expr _right;

};

#endif
