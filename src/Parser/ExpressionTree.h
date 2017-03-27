//
// Created by Harley on 3/25/2017.
//
#include "SymbolRecord.h"
#include "ExpressionNode.h"
#include <stack>
#ifndef COMPILER_ABSTRACTEXPRESSIONTREE_H
#define COMPILER_ABSTRACTEXPRESSIONTREE_H


class ExpressionTree {
public:
    ExpressionNode* root_node_;
    ExpressionTree();
    bool add_new_record(SymbolRecord *record);
    bool add_new_record(SymbolRecord *record, ExpressionNode* node);
    void check_tree_order(ExpressionNode *node);
    void set_all_nodes_in_para(bool in_para);
    void set_all_nodes_in_para(bool in_para, ExpressionNode *node);
    string post_order_print();

    string post_order_print(ExpressionNode *node);

    bool add_bracket_tree(ExpressionTree *tree);

    bool add_bracket_tree(ExpressionNode *node);

    bool add_bracket_tree(ExpressionNode *node, ExpressionTree *tree);

    int calculate_total();

    int calculate_total(ExpressionNode *node);
    void calculate_total(ExpressionNode *node, stack<SymbolRecord*>* stack);

};


#endif //COMPILER_ABSTRACTEXPRESSIONTREE_H
