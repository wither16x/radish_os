#include <cash/ast.hpp>

namespace Cash
{
        void NodeAllocator::freeNode(this NodeAllocator &self, Node *node)
        {
                for (auto &nd : self.nodes) {
                        if (nd == node)
                                delete nd;
                }
        }

        void NodeAllocator::freeAll(this NodeAllocator &self)
        {
                self.nodes.clear();
        }
} // namespace Cash