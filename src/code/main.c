#include "bintree.h"


int main(void)
{
    tree_t tree = {0};
    treeCtor(&tree);
    treeInsert(&tree, "a");
    treeInsert(&tree, "b");
    treeInsert(&tree, "c");
    treeInsert(&tree, "d");
    treeInsert(&tree, "e");
    treeInsert(&tree, "f");

    return 0;
}

