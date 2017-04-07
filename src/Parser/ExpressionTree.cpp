//
// Created by Harley on 3/25/2017.
//

#include "ExpressionTree.h"

ExpressionTree::ExpressionTree() {
    root_node_ = new ExpressionNode();
}

bool ExpressionTree::add_new_record(SymbolRecord *record) {
    add_new_record(record, root_node_);
    return true;
}
bool ExpressionTree::add_bracket_tree(ExpressionTree *tree) {
    tree->set_all_nodes_in_para(true);
    add_bracket_tree(root_node_, tree);
    return true;
}

bool ExpressionTree::split_tree_with_rel_operator(ExpressionTree *tree, SymbolRecord *rel_record) {
    ExpressionNode* node = new ExpressionNode(rel_record, false);
    node->left_tree_ = root_node_;
    root_node_->parent_tree_ = node;
    root_node_ = node;
    root_node_->right_tree_ = tree->root_node_;
    tree->root_node_->parent_tree_ = root_node_;

    return true;
}

bool ExpressionTree::add_bracket_tree(ExpressionNode* node, ExpressionTree *tree) {

    if (node->record_ == NULL) {
        node->record_ = tree->root_node_->record_;
        node->left_tree_ = tree->root_node_->left_tree_;
        node->right_tree_ = tree->root_node_->right_tree_;
    }
    else if (node->left_tree_ == NULL) {
        node->left_tree_ = tree->root_node_;
        node->left_tree_->parent_tree_ = node;
    } else if (node->right_tree_ == NULL){
        node->right_tree_ = tree->root_node_;
        node->right_tree_->parent_tree_ = node;
        check_tree_order(node);
    } else {
        add_bracket_tree(node->right_tree_, tree);
    }
    return true;
}

bool ExpressionTree::add_new_record(SymbolRecord *record, ExpressionNode* node) {
    if (node->record_ == NULL)
        node->record_ = record;
    else if (node->left_tree_ == NULL) {
        node->in_para_ = false;
        node->left_tree_ = new ExpressionNode(node->record_, false);
        node->left_tree_->parent_tree_ = node;
        node->record_ = record;
    }
    else if (node->right_tree_ == NULL) {
        node->right_tree_ = new ExpressionNode(record, false);
        node->right_tree_->parent_tree_ = node;
        check_tree_order(node);
    } else {
        add_new_record(record, node->right_tree_);
    }
    return true;
}


void ExpressionTree::check_tree_order(ExpressionNode *node) {
    if (node->parent_tree_ == NULL)
        return;

    if ((node->record_->kind_ == "ADDOP" && (node->parent_tree_->record_->kind_ == "MULTOP" || node->parent_tree_->record_->type_ == "SUB" ) ||
            (node->record_ ->kind_ == "MULTOP" && (node->parent_tree_->record_->type_ == "DIV")))) {
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
        node->left_tree_ = top_para_node->right_tree_;

        if (top_para_node->in_para_) {
            node->parent_tree_ = NULL;
            top_para_node->parent_tree_ = node;
            root_node_ = node;
        } else {
            top_para_node->right_tree_->parent_tree_ = node;
            top_para_node->right_tree_ = node;
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

int ExpressionTree::calculate_total() {
    stack<SymbolRecord*>* tmp_post_fix_queue = new stack<SymbolRecord*>;
    calculate_total(root_node_, tmp_post_fix_queue);
    stack<SymbolRecord*>* post_fix_queue = new stack<SymbolRecord*>;;

    while (!tmp_post_fix_queue->empty()) {
        SymbolRecord* record = tmp_post_fix_queue->top();
        post_fix_queue->push(record);
        tmp_post_fix_queue->pop();
    }

    while (post_fix_queue->size() > 0) {
        SymbolRecord* first_record = post_fix_queue->top();
        post_fix_queue->pop();
        if (first_record->kind_ == "MULTOP" || first_record->kind_ == "ADDOP") {
            string operator_type = first_record->type_;
            first_record = tmp_post_fix_queue->top();
            tmp_post_fix_queue->pop();
            int second = first_record->integer_value_;
            int first = tmp_post_fix_queue->top()->integer_value_;
            tmp_post_fix_queue->pop();

            if (operator_type == "MULTI") {
                first_record->integer_value_ = first * second;
            } else if (operator_type == "DIV") {
                first_record->integer_value_ = first / second;
            } else if (operator_type == "AND") {
                first_record->integer_value_ = first && second;
            } else if (operator_type == "ADD") {
                first_record->integer_value_ = first + second;
            } else if (operator_type == "SUB") {
                first_record->integer_value_ = first - second;
            } else if (operator_type == "OR") {
                first_record->integer_value_ = first || second;
            }
        }
        tmp_post_fix_queue->push(first_record);

    }
    SymbolRecord* return_record = tmp_post_fix_queue->top();
    delete tmp_post_fix_queue;
    delete post_fix_queue;
    return return_record->integer_value_;

}

void ExpressionTree::calculate_total(ExpressionNode *node, stack<SymbolRecord*>* queue) {
    if (node == NULL)
        return;

    calculate_total(node->left_tree_, queue);
    calculate_total(node->right_tree_, queue);

    if (node->record_ == NULL )
        return;
    queue->push(node->record_);

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


