#pragma once

#include <set>
#include <map>
#include <iostream>

class Tree 
{
public:
    Tree();
    bool create_node(int id, int parent);
    bool delete_node(int id);
    std::set <int> get_node_list();
    void print_tree();
    bool node_exists(int id) const;
private:
    std::map <int, std::set <int>> tree;
};