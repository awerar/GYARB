#include "InterpreterLambda.h"
#include "Interpreter.h"
#include "Error.cpp"

Shared<InterpreterLambda> bind(const Shared<InterpreterLambda> lambda, const Shared<InterpreterLambda> arg, Interpreter* interpreter)
{
	if (lambda == nullptr) return nullptr;

	if (lambda->type == LambdaType::Declaration) {
		const DeclarationLambda* decl = As<const DeclarationLambda>(lambda);
		Shared<InterpreterLambda> result = bind(decl->contents, arg, decl->varID, interpreter);

		return result;
	}
	else if (lambda->type == LambdaType::Variable || lambda->type == LambdaType::Implementation) {
		return MakeShared<ImplementationLambda>(lambda, arg->copy());
	}
	else if (lambda->type == LambdaType::Builtin) {
		if (!arg->isReferenceFree(MakeShared<std::multiset<int>>())) return MakeShared<ImplementationLambda>(lambda, arg);

		BuiltinLambda* builtin = As<BuiltinLambda>(lambda);
		auto& args = builtin->args;
		args.push_back(arg->copy());

		if (builtin->builtinType == BuiltinType::PRINTN && args.size() == 2) {
			std::cout << lambdaToNum(args[0]);
			
			return args[1];
		} 
		else if(builtin->builtinType == BuiltinType::PRINTC && args.size() == 2) {
			std::cout << ((char)lambdaToNum(args[0]));

			return args[1];
		}
		else if (builtin->builtinType == BuiltinType::READN && args.size() == 1) {
			int num;
			*interpreter->input >> num;
			Shared<InterpreterLambda> numLambda = numToLambda(num, interpreter);
			Shared<InterpreterLambda> mapped = bind(args[0], numLambda, interpreter);

			return mapped;
		}
		else if (builtin->builtinType == BuiltinType::READC && args.size() == 1) {
			char chr;
			*interpreter->input >> chr;
			Shared<InterpreterLambda> numLambda = numToLambda(chr, interpreter);
			Shared<InterpreterLambda> mapped = bind(args[0], numLambda, interpreter);

			return mapped;
		}
		else {
			return lambda;
		}
	}
	else {
		return lambda;
	}
}

Shared<InterpreterLambda> bind(Shared<InterpreterLambda> lambda, const Shared<InterpreterLambda> arg, int varID, Interpreter* interpreter)
{
	if (lambda == nullptr) return nullptr;

	/*std::cout << "======CALLING=====" << std::endl;
	lambda->print(0, std::cout);
	std::cout << "======WITH=====" << std::endl;
	arg->print(0, std::cout);
	std::cout << std::endl << std::endl;*/

	if (lambda->type == LambdaType::Variable) {
		const VariableLambda* var = As<const VariableLambda>(lambda);

		if (var->varID == varID) return arg->copy();
		else return lambda;
	}
	else if (lambda->type == LambdaType::Declaration) {
		DeclarationLambda* decl = As<DeclarationLambda>(lambda);
		if (decl->varID == varID) throw new RuntimeError("Bad tree structure!");

		decl->contents = bind(decl->contents, arg, varID, interpreter);
		return lambda;;
	}
	else if (lambda->type == LambdaType::Implementation) {
		ImplementationLambda* impl = As<ImplementationLambda>(lambda);
		impl->func = bind(impl->func, arg, varID, interpreter);
		impl->arg = bind(impl->arg, arg, varID, interpreter);

		Shared<InterpreterLambda> result = lambda;
		if (impl->func->type == LambdaType::Declaration || impl->func->type == LambdaType::Builtin) {
			result = bind(impl->func, impl->arg, interpreter);
		}

		return result;
	}
	else if (lambda->type == LambdaType::Builtin) {
		BuiltinLambda* builtin = As<BuiltinLambda>(lambda);
		std::vector<Shared<InterpreterLambda>>& args = builtin->args;
		for (int i = 0; i < builtin->args.size(); i++) {
			args[i] = bind(args[i], arg, interpreter);
		}

		return lambda;
	}
	else {
		return lambda;
	}
}

int lambdaToNum(const Shared<InterpreterLambda> lam)
{
	if (lam->type == LambdaType::Declaration) {
		const DeclarationLambda* funcDecl = As<const DeclarationLambda>(lam);

		if (funcDecl->contents->type == LambdaType::Declaration) {
			const DeclarationLambda* varDecl = As<const DeclarationLambda>(funcDecl->contents);

			Shared<InterpreterLambda> curr = varDecl->contents;
			int depth = 0;
			while (true) {
				if (curr->type == LambdaType::Implementation) {
					const ImplementationLambda* impl = As<const ImplementationLambda>(curr);

					if (impl->func->type == LambdaType::Variable) {
						const VariableLambda* implFuncVar = As<const VariableLambda>(impl->func);

						if (implFuncVar->varID != funcDecl->varID) {
							throw RuntimeError("Error in parsing number");
						}
					}
					else {
						throw RuntimeError("Error in parsing number");
					}

					curr = impl->arg;
					depth++;
				}
				else if (curr->type == LambdaType::Variable) {
					const VariableLambda* startVar = As<const VariableLambda>(curr);

					if (startVar->varID != varDecl->varID) {
						throw RuntimeError("Error in parsing number");
					}
					else {
						return depth;
					}
				}
				else {
					throw RuntimeError("Error in parsing number");
				}
			}
		}
	}

	throw RuntimeError("Error in parsing number");
}

Shared<InterpreterLambda> numToLambda(int num, Interpreter* interpreter)
{
	int funcID = interpreter->get_new_symbol();
	int varID = interpreter->get_new_symbol();

	Shared<InterpreterLambda> content = MakeShared<VariableLambda>(varID);
	for (int i = 0; i < num; i++) {
		content = MakeShared<ImplementationLambda>(MakeShared<VariableLambda>(funcID), content);
	}

	return MakeShared<DeclarationLambda>(funcID, MakeShared<DeclarationLambda>(varID, content));
}

DeclarationLambda::DeclarationLambda(int varID, Shared<InterpreterLambda> contents) {
	this->type = LambdaType::Declaration;
	this->varID = varID;
	this->contents = contents;
}

void DeclarationLambda::print(int depth, std::ostream& out) const
{
	InterpreterLambda::print(depth, out);
	out << "decl %" << this->varID << std::endl;
	this->contents->print(depth + 1, out);
}

Shared<InterpreterLambda> DeclarationLambda::copy() const
{
	return MakeShared<DeclarationLambda>(this->varID, this->contents->copy());
}

bool DeclarationLambda::isReferenceFree(Shared<std::multiset<int>> identifiers) const
{
	identifiers->insert(this->varID);
	bool result = this->contents->isReferenceFree(identifiers);
	identifiers->erase(this->varID);
	return result;
}


ImplementationLambda::ImplementationLambda(Shared<InterpreterLambda> func, Shared<InterpreterLambda> arg) {
	this->type = LambdaType::Implementation;
	this->func = func;
	this->arg = arg;
}

void ImplementationLambda::print(int depth, std::ostream& out) const
{
	InterpreterLambda::print(depth, out);
	out << "call" << std::endl;

	this->func->print(depth + 1, out);
	this->arg->print(depth + 1, out);
}

Shared<InterpreterLambda> ImplementationLambda::copy() const
{
	return MakeShared<ImplementationLambda>(this->func->copy(), this->arg->copy());
}

bool ImplementationLambda::isReferenceFree(Shared<std::multiset<int>> identifiers) const
{
	return this->func->isReferenceFree(identifiers) && this->arg->isReferenceFree(identifiers);
}

VariableLambda::VariableLambda(int varID) {
	this->type = LambdaType::Variable;
	this->varID = varID;
}

void VariableLambda::print(int depth, std::ostream& out) const
{
	InterpreterLambda::print(depth, out);
	out << "%" << this->varID << std::endl;
}

bool VariableLambda::isReferenceFree(Shared<std::multiset<int>> identifiers) const
{
	return identifiers->find(this->varID) != identifiers->end();
}

Shared<InterpreterLambda> VariableLambda::copy() const
{
	return MakeShared<VariableLambda>(this->varID);
}

void InterpreterLambda::print(int depth, std::ostream& out) const
{
	for (int i = 0; i < depth; i++) out << "\t";
}

BuiltinLambda::BuiltinLambda(BuiltinType builtinType, std::vector<Shared<InterpreterLambda>> args)
{
	this->builtinType = builtinType;
	this->type = LambdaType::Builtin;
	this->args = args;
}

void BuiltinLambda::print(int depth, std::ostream& out) const
{
	InterpreterLambda::print(depth, out);
	std::cout << builtinTypeToName.at(this->builtinType) << std::endl;
}

Shared<InterpreterLambda> BuiltinLambda::copy() const
{
	return MakeShared<BuiltinLambda>(this->builtinType, this->args);
}

bool BuiltinLambda::isReferenceFree(Shared<std::multiset<int>> identifiers) const
{
	return false;
}
