#include "SymbolTree.h"
#include <memory>

SymbolNode::~SymbolNode()
{
	for (auto child : children) delete child;
}
