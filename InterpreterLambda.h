#pragma once
#include <iostream>
#include <memory>
#include <map>
#include <vector>

template<typename T> using Shared = std::shared_ptr<T>;

template<typename T, typename... Args, std::enable_if_t<std::is_constructible_v<T, Args...>, bool> = true>
Shared<T> MakeShared(Args&&... args)
{
	return std::shared_ptr<T>(new T(std::forward<Args>(args)...), std::default_delete<T>());
}

class Interpreter;

enum class BuiltinType {
	READC, READN, PRINTC, PRINTN
};

const std::map<BuiltinType, std::string> builtinTypeToName{
	{ BuiltinType::PRINTC, "PRINTC" },
	{ BuiltinType::PRINTN, "PRINTN" },
	{ BuiltinType::READC, "READC"   },
	{ BuiltinType::READN, "READN"   },
};

const std::map<std::string, BuiltinType> nameToBuiltinType {
	{ "PRINTC", BuiltinType::PRINTC },
	{ "PRINTN", BuiltinType::PRINTN },
	{ "READC", BuiltinType::READC   },
	{ "READN", BuiltinType::READN   },
};

enum class LambdaType {
	Declaration, Implementation, Variable, Builtin
};

struct InterpreterLambda{
	LambdaType type;

	virtual void print(int depth, std::ostream& out) const = 0;
	virtual InterpreterLambda* copy() const = 0;

	virtual ~InterpreterLambda() {}
};

struct DeclarationLambda : public InterpreterLambda {
	int varID;
	InterpreterLambda* contents;

	DeclarationLambda(int varID, InterpreterLambda* contents);
	virtual ~DeclarationLambda();

	// Inherited via InterpreterLambda
	virtual void print(int depth, std::ostream& out) const override;

	// Inherited via InterpreterLambda
	virtual InterpreterLambda* copy() const override;
};

struct ImplementationLambda : public InterpreterLambda {
	InterpreterLambda* func;
	InterpreterLambda* arg;

	ImplementationLambda(InterpreterLambda* func, InterpreterLambda* arg);
	virtual ~ImplementationLambda();

	// Inherited via InterpreterLambda
	virtual void print(int depth, std::ostream& out) const override;

	// Inherited via InterpreterLambda
	virtual InterpreterLambda* copy() const override;
};

struct VariableLambda : public InterpreterLambda {
	int varID;

	VariableLambda(int varID);
	virtual ~VariableLambda();

	// Inherited via InterpreterLambda
	virtual void print(int depth, std::ostream& out) const override;

	// Inherited via InterpreterLambda
	virtual InterpreterLambda* copy() const override;
};

struct BuiltinLambda : public InterpreterLambda {
	BuiltinType builtinType;
	std::vector<InterpreterLambda*> args;

	BuiltinLambda(BuiltinType builtinType, std::vector<InterpreterLambda*> args);
	virtual ~BuiltinLambda();

	// Inherited via InterpreterLambda
	virtual void print(int depth, std::ostream& out) const override;

	// Inherited via InterpreterLambda
	virtual InterpreterLambda* copy() const override;
};

template<typename T, std::enable_if_t<std::is_base_of_v<InterpreterLambda, T>, bool> = true>
inline T* As(const Shared<InterpreterLambda>& base)
{
	return reinterpret_cast<T*>(base.get());
}

InterpreterLambda* bind(const InterpreterLambda* lambda, const InterpreterLambda* arg, Interpreter* interpreter);
InterpreterLambda* bind(const InterpreterLambda* lambda, const InterpreterLambda* arg, int varID, Interpreter* interpreter);

int lambdaToNum(const InterpreterLambda* lam);
InterpreterLambda* numToLambda(int num, Interpreter* interpreter);