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
bool ExpressionTree::add_bracket_tree(ExpressionTree *tree) {
    tree->set_all_nodes_in_para(true);
    add_bracket_tree(root_node_, tree);
}

bool ExpressionTree::add_bracket_tree(ExpressionNode* node, ExpressionTree *tree) {

    if (node->left_tree_ == NULL) {
        node->left_tree_ = tree->root_node_;
        node->parent_tree_ = tree->root_node_;
    } else if (node->right_tree_ == NULL){
        node->right_tree_ = tree->root_node_;
        node->parent_tree_ = tree->root_node_;
    } else {
        add_bracket_tree(node->right_tree_, tree);
    }
    post_order_print();
    return true;
}

bool ExpressionTree::add_new_record(SymbolRecord *record, ExpressionNode* node) {
    if (node->record_ == NULL)
        node->record_ = record;
    else if (node->left_tree_ == NULL) {
        node->left_tree_ = new ExpressionNode(node->record_, root_node_->in_para_);
        node->left_tree_->parent_tree_ = node;
        node->record_ = record;
    }
    else if (node->right_tree_ == NULL) {
        node->right_tree_ = new ExpressionNode(record, root_node_->in_para_);
        node->right_tree_->parent_tree_ = node;
        post_order_print();
        check_tree_order(node);
        post_order_print();
    } else {
        add_new_record(record, node->right_tree_);
    }
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
    } else if (node->parent_tree_->in_para_) {
        ExpressionNode* top_para_node = node->parent_tree_;
        while (top_para_node->parent_tree_ != NULL && top_para_node->in_para_ ) {
            top_para_node = top_para_node->parent_tree_;
        }
        node->parent_tree_->right_tree_ = node->left_tree_;
        node->left_tree_ = node->parent_tree_;
        node->parent_tree_->parent_tree_ = node;

        if (top_para_node->in_para_) {
            node->parent_tree_ = NULL;
            root_node_ = node;
        } else {
            node->parent_tree_ = top_para_node;

        }
        check_tree_order(node);

    }
}

void ExpressionTree::set_all_nodes_in_para(bool in_para) {
    set_all_nodes_in_para(in_para, root_node_);
}

void ExpressionTree::set_all_nodes_in_para(bool in_para, ExpressionNode *node) {
    if (node == NULL)
        return;
    set_all_nodes_in_para(in_para, node->left_tree_);
    set_all_nodes_in_para(in_para, node->right_tree_);

    node->in_para_ = in_para;
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

bool ExpressionTree::add_bracket_tree(ExpressionNode *node) {
    return false;
}

