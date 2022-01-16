#include "InterpreterLambda.h"
#include "Interpreter.h"
//#include "Error.cpp"

InterpreterLambda* bind(const InterpreterLambda* lambda, const InterpreterLambda* arg, Interpreter* interpreter)
{
	if (lambda == nullptr) return nullptr;

	if (lambda->type == LambdaType::Declaration) {
		const DeclarationLambda* decl = static_cast<const DeclarationLambda*>(lambda);
		InterpreterLambda* result = bind(decl->contents, arg, decl->varID, interpreter);

		return result;
	}
	else if (lambda->type == LambdaType::Variable || lambda->type == LambdaType::Implementation) {
		return new ImplementationLambda(lambda->copy(), arg->copy());
	}
	else if (lambda->type == LambdaType::Builtin) {
		const BuiltinLambda* builtin = static_cast<const BuiltinLambda*>(lambda);
		std::vector<InterpreterLambda*> args = builtin->args;
		args.push_back(arg->copy());

		if (builtin->builtinType == BuiltinType::PRINTN && args.size() == 2) {
			std::cout << lambdaToNum(args[0]);
			
			return args[1]->copy();
		} 
		else if(builtin->builtinType == BuiltinType::PRINTC && args.size() == 2) {
			std::cout << ((char)lambdaToNum(args[0]));

			return args[1]->copy();
		}
		else if (builtin->builtinType == BuiltinType::READN && args.size() == 1) {
			int num;
			*interpreter->input >> num;
			InterpreterLambda* numLambda = numToLambda(num, interpreter);
			InterpreterLambda* mapped = bind(args[0], numLambda, interpreter);
			delete numLambda;

			return mapped;
		}
		else {
			return new BuiltinLambda(builtin->builtinType, args);
		}
	}
	else {
		return lambda->copy();
	}
}

InterpreterLambda* bind(const InterpreterLambda* lambda, const InterpreterLambda* arg, int varID, Interpreter* interpreter)
{
	if (lambda == nullptr) return nullptr;

	/*std::cout << "======CALLING=====" << std::endl;
	lambda->print(0, std::cout);
	std::cout << "======WITH=====" << std::endl;
	arg->print(0, std::cout);
	std::cout << std::endl << std::endl;*/

	if (lambda->type == LambdaType::Variable) {
		const VariableLambda* var = static_cast<const VariableLambda*>(lambda);

		if (var->varID == varID) return arg->copy();
		else return lambda->copy();
	}
	else if (lambda->type == LambdaType::Declaration) {
		const DeclarationLambda* decl = static_cast<const DeclarationLambda*>(lambda);
		//if (decl->varID == varID) throw new RuntimeError("Bad tree structure!");

		return new DeclarationLambda(decl->varID, bind(decl->contents, arg, varID, interpreter));
	}
	else if (lambda->type == LambdaType::Implementation) {
		const ImplementationLambda* impl = static_cast<const ImplementationLambda*>(lambda);
		auto* nfunc = bind(impl->func, arg, varID, interpreter);
		auto* narg = bind(impl->arg, arg, varID, interpreter);

		InterpreterLambda* result;
		if (nfunc->type == LambdaType::Declaration || nfunc->type == LambdaType::Builtin) {
			result = bind(nfunc, narg, interpreter);
			delete nfunc;
			delete narg;
		}
		else {
			result = new ImplementationLambda(nfunc, narg);
		}

		return result;
	}
	else if (lambda->type == LambdaType::Builtin) {
		const BuiltinLambda* builtin = static_cast<const BuiltinLambda*>(lambda);
		std::vector<InterpreterLambda*> args;
		for (int i = 0; i < builtin->args.size(); i++) {
			InterpreterLambda* oldArg = builtin->args[i];
			args.push_back(bind(oldArg, arg, interpreter));
		}

		return new BuiltinLambda(builtin->builtinType, args);
	}
	else {
		return lambda->copy();
	}
}

int lambdaToNum(const InterpreterLambda* lam)
{
	if (lam->type == LambdaType::Declaration) {
		const DeclarationLambda* funcDecl = static_cast<const DeclarationLambda*>(lam);

		if (funcDecl->contents->type == LambdaType::Declaration) {
			const DeclarationLambda* varDecl = static_cast<const DeclarationLambda*>(funcDecl->contents);

			const InterpreterLambda* curr = varDecl->contents;
			int depth = 0;
			while (true) {
				if (curr->type == LambdaType::Implementation) {
					const ImplementationLambda* impl = static_cast<const ImplementationLambda*>(curr);

					if (impl->func->type == LambdaType::Variable) {
						const VariableLambda* implFuncVar = static_cast<const VariableLambda*>(impl->func);

						if (implFuncVar->varID != funcDecl->varID) {
							return -1;
						}
					}
					else {
						return -1;
					}

					curr = impl->arg;
					depth++;
				}
				else if (curr->type == LambdaType::Variable) {
					const VariableLambda* startVar = static_cast<const VariableLambda*>(curr);

					if (startVar->varID != varDecl->varID) {
						return -1;
					}
					else {
						return depth;
					}
				}
				else {
					return -1;
				}
			}
		}
	}

	return -1;
}

InterpreterLambda* numToLambda(int num, Interpreter* interpreter)
{
	int funcID = interpreter->get_new_symbol();
	int varID = interpreter->get_new_symbol();

	InterpreterLambda* content = new VariableLambda(varID);
	for (int i = 0; i < num; i++) {
		content = new ImplementationLambda(new VariableLambda(funcID), content);
	}

	return new DeclarationLambda(funcID, new DeclarationLambda(varID, content));
}

DeclarationLambda::DeclarationLambda(int varID, InterpreterLambda* contents) {
	this->type = LambdaType::Declaration;
	this->varID = varID;
	this->contents = contents;
}

DeclarationLambda::~DeclarationLambda()
{
	delete this->contents;
}

void DeclarationLambda::print(int depth, std::ostream& out) const
{
	InterpreterLambda::print(depth, out);
	out << "decl %" << this->varID << std::endl;
	this->contents->print(depth + 1, out);
}

InterpreterLambda* DeclarationLambda::copy() const
{
	return new DeclarationLambda(this->varID, this->contents->copy());
}

ImplementationLambda::ImplementationLambda(InterpreterLambda* func, InterpreterLambda* arg) {
	this->type = LambdaType::Implementation;
	this->func = func;
	this->arg = arg;
}

ImplementationLambda::~ImplementationLambda()
{
	delete this->arg;
	delete this->func;
}

void ImplementationLambda::print(int depth, std::ostream& out) const
{
	InterpreterLambda::print(depth, out);
	out << "call" << std::endl;

	this->func->print(depth + 1, out);
	this->arg->print(depth + 1, out);
}

InterpreterLambda* ImplementationLambda::copy() const
{
	return new ImplementationLambda(this->func->copy(), this->arg->copy());
}

VariableLambda::VariableLambda(int varID) {
	this->type = LambdaType::Variable;
	this->varID = varID;
}

VariableLambda::~VariableLambda()
{

}

void VariableLambda::print(int depth, std::ostream& out) const
{
	InterpreterLambda::print(depth, out);
	out << "%" << this->varID << std::endl;
}

InterpreterLambda* VariableLambda::copy() const
{
	return new VariableLambda(this->varID);
}

void InterpreterLambda::print(int depth, std::ostream& out) const
{
	for (int i = 0; i < depth; i++) out << "\t";
}

BuiltinLambda::BuiltinLambda(BuiltinType builtinType, std::vector<InterpreterLambda*> args)
{
	this->builtinType = builtinType;
	this->type = LambdaType::Builtin;
	this->args = args;
}

BuiltinLambda::~BuiltinLambda()
{
	for (InterpreterLambda* lam : args) {
		delete lam;
	}
}

void BuiltinLambda::print(int depth, std::ostream& out) const
{
	InterpreterLambda::print(depth, out);
	std::cout << builtinTypeToName.at(this->builtinType) << std::endl;
}

InterpreterLambda* BuiltinLambda::copy() const
{
	return new BuiltinLambda(this->builtinType, this->args);
}
