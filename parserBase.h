#ifndef PARSER_BASE_H
#define PARSER_BASE_H

#include <list>
#include <map>
#include <memory>

class Expr {
public:
  virtual ~Expr() {}
  virtual void print(std::string indent) = 0;
  virtual double compute(const std::map<std::string, double> &context) = 0;
  virtual bool operator==(const Expr &other) = 0;
  enum class VisitState { NotVisited, TempMark, PermMark };
  VisitState visited = VisitState::NotVisited;
};

enum class OpType : std::size_t { Add, Sub, Mul, Div, Exp };

std::string opTypeToStr(OpType type);

class BinOp : public Expr {
public:
  OpType type;
  std::shared_ptr<Expr> op1;
  std::shared_ptr<Expr> op2;
  BinOp(OpType type, std::shared_ptr<Expr> op1, std::shared_ptr<Expr> op2);
  void print(std::string indent);
  double compute(const std::map<std::string, double> &ctx);
  bool operator==(const Expr &other);
};

class Negate : public Expr {
public:
  std::shared_ptr<Expr> op;
  Negate(std::shared_ptr<Expr> op);
  void print(std::string indent);
  double compute(const std::map<std::string, double> &ctx);
  bool operator==(const Expr &other);
};

class Value : public Expr {
public:
  double val;
  Value(double val);
  void print(std::string indent);
  double compute(const std::map<std::string, double> &);
  bool operator==(const Expr &other);
};

class Constant : public Expr {
public:
  std::string name;
  Constant(std::string name);
  void print(std::string indent);
  double compute(const std::map<std::string, double> &ctx);
  bool operator==(const Expr &other);
};

class ParserBase {
  std::list<std::shared_ptr<Expr>> knownExprs;

protected:
  std::shared_ptr<Expr> makeExpr(OpType, std::shared_ptr<Expr>,
                                 std::shared_ptr<Expr>);
  std::shared_ptr<Expr> makeExpr(std::shared_ptr<Expr>);
  std::shared_ptr<Expr> makeExpr(double);
  std::shared_ptr<Expr> makeExpr(std::string);

public:
  ParserBase();
  virtual ~ParserBase();
};

using ResultType = std::shared_ptr<Expr>;
#endif