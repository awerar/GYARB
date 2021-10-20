#pragma once
#include <unordered_map>
#include <string>
#include <memory>

struct SymbolNode {
	SymbolNode* parent;
	std::vector<SymbolNode*> children;

	int id;
	std::string symbol;

	~SymbolNode();
};