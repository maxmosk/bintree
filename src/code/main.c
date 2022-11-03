#include "bintree.h"


int main(void)
{
    tree_t tree = {0};
    treeCtor(&tree);

    treeInsert(&tree, DUMMY_CHILD, "123");
    treeInsert(&tree, LEFT_CHILD, "456");
    treeNext(&tree, LEFT_CHILD);
    treeInsert(&tree, LEFT_CHILD, "789");
    treeNext(&tree, LEFT_CHILD);

    treeDtor(&tree);

    return 0;
}

