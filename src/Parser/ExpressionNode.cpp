//
// Created by Harley on 3/26/2017.
//

#include "ExpressionNode.h"

ExpressionNode::ExpressionNode(SymbolRecord *record) {
    left_tree_ = NULL;
    right_tree_ = NULL;
    parent_tree_ = NULL;
    record_ = record;
}

ExpressionNode::ExpressionNode() {
    record_ = NULL;
    left_tree_ = NULL;
    right_tree_ = NULL;
    parent_tree_ = NULL;
}
