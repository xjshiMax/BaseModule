#include "../include/rbtree.h"
static inline void SE_rbtree_left_rotate(Se_rbtree_node_t **root,
    Se_rbtree_node_t *sentinel, Se_rbtree_node_t *node);
static inline void SE_rbtree_right_rotate(Se_rbtree_node_t **root,
    Se_rbtree_node_t *sentinel, Se_rbtree_node_t *node);

void SE_rbtree_insert(Se_rbtree_t *tree, Se_rbtree_node_t *node)
{
    Se_rbtree_node_t **root,*temp,*sentinel;
    root = &tree->root;
    sentinel = tree->sentinel;
    if(*root == sentinel)
    {
        node->parent = NULL;
        node->left = sentinel;
        node->right = sentinel;
        SE_rbt_black(node);
        *root = node;
        return;
    }
    tree->insert(*root,node,sentinel);
    /* re-balance tree */

    while (node != *root && SE_rbt_is_red(node->parent)) {

        if (node->parent == node->parent->parent->left) {
            temp = node->parent->parent->right;

            if (SE_rbt_is_red(temp)) {
                SE_rbt_black(node->parent);
                SE_rbt_black(temp);
                SE_rbt_red(node->parent->parent);
                node = node->parent->parent;

            } else {
                if (node == node->parent->right) {
                    node = node->parent;
                    SE_rbtree_left_rotate(root, sentinel, node);
                }

                SE_rbt_black(node->parent);
                SE_rbt_red(node->parent->parent);
                SE_rbtree_right_rotate(root, sentinel, node->parent->parent);
            }

        } else {
            temp = node->parent->parent->left;

            if (SE_rbt_is_red(temp)) {
                SE_rbt_black(node->parent);
                SE_rbt_black(temp);
                SE_rbt_red(node->parent->parent);
                node = node->parent->parent;

            } else {
                if (node == node->parent->left) {
                    node = node->parent;
                    SE_rbtree_right_rotate(root, sentinel, node);
                }

                SE_rbt_black(node->parent);
                SE_rbt_red(node->parent->parent);
                SE_rbtree_left_rotate(root, sentinel, node->parent->parent);
            }
        }
    }

    SE_rbt_black(*root);
}
/*
SE_rbtree_delete：删除红黑树节点，该方法会 通过旋转红黑树来保持树的平衡
*/
void SE_rbtree_delete(Se_rbtree_t *tree, Se_rbtree_node_t *node)
{
    unsigned int red;
    Se_rbtree_node_t **root,*sentinel,*subst, *temp, *w;
    root = &tree->root;
    sentinel = tree->sentinel;

    if (node->left == sentinel) {
        temp = node->right;
        subst = node;

    } else if (node->right == sentinel) {
        temp = node->left;
        subst = node;

    } else {
        subst = SE_rbtree_min(node->right, sentinel);

        if (subst->left != sentinel) {
            temp = subst->left;
        } else {
            temp = subst->right;
        }
    }

    if (subst == *root) {
        *root = temp;
        SE_rbt_black(temp);

        /* DEBUG stuff */
        node->left = NULL;
        node->right = NULL;
        node->parent = NULL;
        node->key = 0;

        return;
    }

    red = SE_rbt_is_red(subst);

    if (subst == subst->parent->left) {
        subst->parent->left = temp;

    } else {
        subst->parent->right = temp;
    }

    if (subst == node) {

        temp->parent = subst->parent;

    } else {

        if (subst->parent == node) {
            temp->parent = subst;

        } else {
            temp->parent = subst->parent;
        }

        subst->left = node->left;
        subst->right = node->right;
        subst->parent = node->parent;
        SE_rbt_copy_color(subst, node);

        if (node == *root) {
            *root = subst;

        } else {
            if (node == node->parent->left) {
                node->parent->left = subst;
            } else {
                node->parent->right = subst;
            }
        }

        if (subst->left != sentinel) {
            subst->left->parent = subst;
        }

        if (subst->right != sentinel) {
            subst->right->parent = subst;
        }
    }

    /* DEBUG stuff */
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->key = 0;

    if (red) {
        return;
    }

    /* a delete fixup */

    while (temp != *root && SE_rbt_is_black(temp)) {

        if (temp == temp->parent->left) {
            w = temp->parent->right;

            if (SE_rbt_is_red(w)) {
                SE_rbt_black(w);
                SE_rbt_red(temp->parent);
                SE_rbtree_left_rotate(root, sentinel, temp->parent);
                w = temp->parent->right;
            }

            if (SE_rbt_is_black(w->left) && SE_rbt_is_black(w->right)) {
                SE_rbt_red(w);
                temp = temp->parent;

            } else {
                if (SE_rbt_is_black(w->right)) {
                    SE_rbt_black(w->left);
                    SE_rbt_red(w);
                    SE_rbtree_right_rotate(root, sentinel, w);
                    w = temp->parent->right;
                }

                SE_rbt_copy_color(w, temp->parent);
                SE_rbt_black(temp->parent);
                SE_rbt_black(w->right);
                SE_rbtree_left_rotate(root, sentinel, temp->parent);
                temp = *root;
            }

        } else {
            w = temp->parent->left;

            if (SE_rbt_is_red(w)) {
                SE_rbt_black(w);
                SE_rbt_red(temp->parent);
                SE_rbtree_right_rotate(root, sentinel, temp->parent);
                w = temp->parent->left;
            }

            if (SE_rbt_is_black(w->left) && SE_rbt_is_black(w->right)) {
                SE_rbt_red(w);
                temp = temp->parent;

            } else {
                if (SE_rbt_is_black(w->left)) {
                    SE_rbt_black(w->right);
                    SE_rbt_red(w);
                    SE_rbtree_left_rotate(root, sentinel, w);
                    w = temp->parent->left;
                }

                SE_rbt_copy_color(w, temp->parent);
                SE_rbt_black(temp->parent);
                SE_rbt_black(w->left);
                SE_rbtree_right_rotate(root, sentinel, temp->parent);
                temp = *root;
            }
        }
    }

    SE_rbt_black(temp);
}

/*
SE_rbtree_insert_value ：向红黑树添加数据节点，关键字唯一。
root:红黑表容器指针
node:待添加元素，Se_rbtree_node_t类型指针
sentinel: 这棵树初始化时哨兵节点指针
*/
void SE_rbtree_insert_value(Se_rbtree_node_t *root,Se_rbtree_node_t *node,
    Se_rbtree_node_t *sentinel)
{
    Se_rbtree_node_t  **p;

    for ( ;; ) {

        p = (node->key < root->key) ? &root->left : &root->right;

        if (*p == sentinel) {
            break;
        }

        root = *p;
    }

    *p = node;
    node->parent = root;
    node->left = sentinel;
    node->right = sentinel;
    SE_rbt_red(node);
}
/*
SE_rbtree_insert_timer_value：向红黑树添加数据节点，关键字表示时间或者时间差
root: 红黑树容器指针
node:待添加元素，关键字是时间或者时间差，可能是负数
sentinel:哨兵指针
*/
void SE_rbtree_insert_timer_value(Se_rbtree_node_t *root,Se_rbtree_node_t *node,
    Se_rbtree_node_t *sentinel)
{
       Se_rbtree_node_t  **p;

    for ( ;; ) {

        /*
         * Timer values
         * 1) are spread in small range, usually several minutes,
         * 2) and overflow each 49 days, if milliseconds are stored in 32 bits.
         * The comparison takes into account that overflow.
         */

        /*  node->key < temp->key */

        p = ((unsigned int) (node->key - root->key) < 0)
            ? &root->left : &root->right;

        if (*p == sentinel) {
            break;
        }

        root = *p;
    }

    *p = node;
    node->parent = root;
    node->left = sentinel;
    node->right = sentinel;
    SE_rbt_red(node);
}
Se_rbtree_node_t *SE_rbtree_next(Se_rbtree_t *tree,Se_rbtree_node_t *node)
{
    Se_rbtree_node_t *root,*sentinel,*parent;
    sentinel = tree->sentinel;
    if(node->right != sentinel)
        return SE_rbtree_min(node->right,sentinel);
    root = tree->root;
    for(;;)
    {
        parent = node->parent;
        if(node == root)
        {
            return NULL;
        }
        if(node == parent->left)
            return parent;
        node = parent;
    }
}
static inline void SE_rbtree_right_rotate(Se_rbtree_node_t **root,
    Se_rbtree_node_t *sentinel, Se_rbtree_node_t *node)
{
    Se_rbtree_node_t *temp;
    temp = node->left;
    node->left = temp->right;
    if(temp->right != sentinel)
    {
        temp->right->parent = node;
    }
    temp->parent = node->parent;
    if(node == *root)
    {
        *root = temp;
    }
    else if(node == node->parent->right)
    {
        node->parent->left = temp;
    }
    else
    {
        node->parent->left = temp;
    }
    temp->right = node;
    node->parent = temp;
}
static inline void SE_rbtree_left_rotate(Se_rbtree_node_t **root,
    Se_rbtree_node_t *sentinel, Se_rbtree_node_t *node)
{
    Se_rbtree_node_t *temp;
    temp = node->right;
    node->right = temp->left;
    if(temp->left != sentinel)
    {
        temp->left->parent = node;
    }
    temp->parent = node->parent;
    if(node == *root)
    {
        *root = temp;
    }
    else if(node == node->parent->left)
    {
        node->parent->left = temp;
    }
    else
    {
        node->parent->right = temp;
    }
    temp->left = node;
    node->parent = temp; 



}