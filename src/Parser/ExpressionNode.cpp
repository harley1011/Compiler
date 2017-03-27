//
// Created by Harley on 3/26/2017.
//

#include "ExpressionNode.h"

ExpressionNode::ExpressionNode(SymbolRecord *record, bool in_para) {
    left_tree_ = NULL;
    right_tree_ = NULL;
    parent_tree_ = NULL;
    record_ = record;
    in_para_ = in_para;
}

ExpressionNode::ExpressionNode() {
    record_ = NULL;
    left_tree_ = NULL;
    right_tree_ = NULL;
    parent_tree_ = NULL;
    in_para_ = false;
}
