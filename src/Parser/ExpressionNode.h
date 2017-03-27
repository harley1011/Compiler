//
// Created by Harley on 3/26/2017.
//
#include "SymbolRecord.h"
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

    bool add_new_record(SymbolRecord *record);

    void check_tree_order();

    string post_order_print();

};


#endif //COMPILER_NODEEXPRESSION_H
