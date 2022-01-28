#pragma once
#include <iostream>
#include <memory>
#include <map>
#include <vector>
#include <set>

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
	virtual Shared<InterpreterLambda> copy() const = 0;
	virtual bool isReferenceFree(Shared<std::multiset<int>> identifiers) const = 0;
};

struct DeclarationLambda : public InterpreterLambda {
	int varID;
	Shared<InterpreterLambda> contents;

	DeclarationLambda(int varID, Shared<InterpreterLambda> contents);

	// Inherited via InterpreterLambda
	virtual void print(int depth, std::ostream& out) const override;

	// Inherited via InterpreterLambda
	virtual Shared<InterpreterLambda> copy() const override;

	virtual bool isReferenceFree(Shared<std::multiset<int>> identifiers) const override;
};

struct ImplementationLambda : public InterpreterLambda {
	Shared<InterpreterLambda> func;
	Shared<InterpreterLambda> arg;

	ImplementationLambda(Shared<InterpreterLambda> func, Shared<InterpreterLambda> arg);

	// Inherited via InterpreterLambda
	virtual void print(int depth, std::ostream& out) const override;

	// Inherited via InterpreterLambda
	virtual Shared<InterpreterLambda> copy() const override;

	virtual bool isReferenceFree(Shared<std::multiset<int>> identifiers) const override;
};

struct VariableLambda : public InterpreterLambda {
	int varID;

	VariableLambda(int varID);

	// Inherited via InterpreterLambda
	virtual void print(int depth, std::ostream& out) const override;

	// Inherited via InterpreterLambda
	virtual Shared<InterpreterLambda> copy() const override;

	virtual bool isReferenceFree(Shared<std::multiset<int>> identifiers) const override;
};

struct BuiltinLambda : public InterpreterLambda {
	BuiltinType builtinType;
	std::vector<Shared<InterpreterLambda>> args;

	BuiltinLambda(BuiltinType builtinType, std::vector<Shared<InterpreterLambda>> args);

	// Inherited via InterpreterLambda
	virtual void print(int depth, std::ostream& out) const override;

	// Inherited via InterpreterLambda
	virtual Shared<InterpreterLambda> copy() const override;

	virtual bool isReferenceFree(Shared<std::multiset<int>> identifiers) const override;
};

template<typename T, std::enable_if_t<std::is_base_of_v<InterpreterLambda, T>, bool> = true>
inline T* As(const Shared<InterpreterLambda>& base)
{
	return reinterpret_cast<T*>(base.get());
}

Shared<InterpreterLambda> bind(const Shared<InterpreterLambda> lambda, const Shared<InterpreterLambda> arg, Interpreter* interpreter);
Shared<InterpreterLambda> bind(const Shared<InterpreterLambda> lambda, const Shared<InterpreterLambda> arg, int varID, Interpreter* interpreter);

int lambdaToNum(const Shared<InterpreterLambda> lam);
Shared<InterpreterLambda> numToLambda(int num, Interpreter* interpreter);