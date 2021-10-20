#pragma once
#include <unordered_map>
#include <string>
#include <memory>

struct SymbolNode {
	SymbolNode* parent = NULL;
	std::vector<SymbolNode*> children;

	int id = -1;
	std::string symbol;

	~SymbolNode();
};