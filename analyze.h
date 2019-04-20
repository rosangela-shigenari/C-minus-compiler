
#ifndef _ANALYZE_H_
#define _ANALYZE_H_

int main_loc;

/* Function buildSymtab constructs the symbol
 * table by preorder traversal of the syntax tree
 */

void buildSymtab(TreeNode *);
TreeNode * rebuldingParameters(TreeNode* funCallNode, TreeNode* recoveredFunKNode);

/* Procedure typeCheck performs type checking
 * by a postorder syntax tree traversal
 */
void typeCheck(TreeNode *);
TreeNode* FuncAddress (char* name);
#endif
