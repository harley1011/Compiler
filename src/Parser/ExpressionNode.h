//
// Created by Harley on 3/26/2017.
//
#include "SymbolRecord.h"
#include <stack>
#ifndef COMPILER_NODEEXPRESSION_H
#define COMPILER_NODEEXPRESSION_H


class ExpressionNode {
public:
    ExpressionNode(SymbolRecord *pRecord, bool in_para);
    ExpressionNode();

    ExpressionNode* left_tree_;
    ExpressionNode* right_tree_;
    SymbolRecord* record_;
    ExpressionNode* parent_tree_;
    bool in_para_;

    vector<SymbolRecord *> all_identifiers_in_expression();
    void generate_queue(ExpressionNode *node, stack<SymbolRecord*>* queue)
    void all_identifiers_in_expression(vector<SymbolRecord*>* identifiers, ExpressionNode *node);

};


#endif //COMPILER_NODEEXPRESSION_H
