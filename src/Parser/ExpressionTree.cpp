//
// Created by Harley on 3/25/2017.
//

#include "ExpressionTree.h"

ExpressionTree::ExpressionTree() {
    root_node_ = new ExpressionNode();
}

bool ExpressionTree::add_new_record(SymbolRecord *record) {
    add_new_record(record, root_node_);
}

bool ExpressionTree::add_new_record(SymbolRecord *record, ExpressionNode* node) {
    if (node->record_ == NULL)
        node->record_ = record;
    else if (node->left_tree_ == NULL) {
        node->left_tree_ = new ExpressionNode(node->record_);
        node->left_tree_->parent_tree_ = node;
        node->record_ = record;
    }
    else if (node->right_tree_ == NULL) {
        node->right_tree_ = new ExpressionNode(record);
        node->right_tree_->parent_tree_ = node;
        check_tree_order(node);
    } else {
        add_new_record(record, node->right_tree_);
    }
    post_order_print();
    return true;
}


void ExpressionTree::check_tree_order(ExpressionNode *node) {
    if (node->parent_tree_ == NULL)
        return;

    if (node->record_->kind_ == "ADDOP" && node->parent_tree_->record_->kind_ == "MULTOP" ) {
        node->parent_tree_->right_tree_ = node->left_tree_;
        node->left_tree_ = node->parent_tree_;
        node->parent_tree_ = node->left_tree_->parent_tree_;
        node->left_tree_->parent_tree_ = node;
        if (node->parent_tree_ != NULL)
            node->parent_tree_->right_tree_ = node;
        else
            root_node_ = node;
        check_tree_order(node);

    }
}

string ExpressionTree::post_order_print() {
    string result = post_order_print(root_node_);
    cout << result << endl;
    return result;
}

string ExpressionTree::post_order_print(ExpressionNode *node) {
    if (node == NULL)
        return "";

    string result = post_order_print(node->left_tree_);
    result += post_order_print(node->right_tree_);

    if (node->record_->type_ == "int")
        return result + to_string(node->record_->integer_value_) + " ";
    else
        return result + node->record_->type_ + " ";
}

