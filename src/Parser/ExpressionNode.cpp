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

vector<SymbolRecord *> ExpressionNode::all_identifiers_in_expression() {
    vector<SymbolRecord*>* identifiers = new vector<SymbolRecord*>;
    all_identifiers_in_expression(identifiers, this);
    return (*identifiers);
}


void ExpressionNode::all_identifiers_in_expression(vector<SymbolRecord*>* identifiers, ExpressionNode *node) {
    if (node == NULL)
        return;

    all_identifiers_in_expression(identifiers, node->left_tree_);
    all_identifiers_in_expression(identifiers, node->right_tree_);

    if (node->record_->kind_ == "variable" || node->record_->kind_ == "function")
        identifiers->push_back(node->record_);

}