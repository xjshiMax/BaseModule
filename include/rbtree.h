//xjshi 2019/12/06
/*
1)红黑树，用于定时器
2）移植于nginx
2)c语言风格，基本的创建，添加，获取
特性1：节点时红色或者黑色
特性2：根节点是黑色
特性3：所有叶子节点都是黑色（叶子节点是NULL)
特性4：每个红色节点的两个字节点都是黑色（每个叶子节点到根节点的
所有路径上不能有两个连续的红色节点）。
特性5：从任一节点到其每个叶子节点的所有简单路径都包含相同数目的黑色节点
*/
#pragma once
#include <stddef.h>
typedef struct Se_rbtree_node_s Se_rbtree_node_t;

struct Se_rbtree_node_s{
    unsigned int        key;        //关键字
    Se_rbtree_node_t    *left;      //左子树
    Se_rbtree_node_t    *right;     //右子树
    Se_rbtree_node_t    *parent;    //父节点
    unsigned char       color;      //颜色
    unsigned char       data;       //一个字节的节点数据
};

typedef struct Se_rbtree_s Se_rbtree_t;
typedef void (*SE_rbtree_insert_pt) (Se_rbtree_node_t*root,
    Se_rbtree_node_t*node,Se_rbtree_node_t*sentinel);

struct Se_rbtree_s{
    Se_rbtree_node_t    *root;      //指向根节点，根节点也是数据元素
    Se_rbtree_node_t    *sentinel;  //指向哨兵节点
    SE_rbtree_insert_pt insert;
};

/*
SE_rbtree_init 初始化红黑树，初始化哨兵节点，根节点，SE_rbtree_insert_pt方法
tree:红黑树容器的指针
s:哨兵节点的指针
i:SE_rbtree_insert_pt 类型的节点添加方法
*/
#define SE_rbtree_init(tree,s,i)                \
    SE_rbtree_sentinel_init(s);                 \
    (tree)->root = s;                           \
    (tree)->sentinel = s;                       \
    (tree)->insert = i;
/*
SE_rbtree_insert：添加数据节点，该方法会旋转红黑树保持树的平衡
tree：红黑树容器的指针
node：需要添加到红黑树的节点指针
*/
void SE_rbtree_insert(Se_rbtree_t *tree, Se_rbtree_node_t *node);
/*
SE_rbtree_delete：删除红黑树节点，该方法会 通过旋转红黑树来保持树的平衡
*/
void SE_rbtree_delete(Se_rbtree_t *tree, Se_rbtree_node_t *node);

/*
SE_rbtree_insert_value ：向红黑树添加数据节点，关键字唯一。
root:红黑表容器指针
node:待添加元素，Se_rbtree_node_t类型指针
sentinel: 这棵树初始化时哨兵节点指针
*/
void SE_rbtree_insert_value(Se_rbtree_node_t *root,Se_rbtree_node_t *node,
    Se_rbtree_node_t *sentinel);
/*
SE_rbtree_insert_timer_value：向红黑树添加数据节点，关键字表示时间或者时间差
root: 红黑树容器指针
node:待添加元素，关键字是时间或者时间差，可能是负数
sentinel:哨兵指针
*/
void SE_rbtree_insert_timer_value(Se_rbtree_node_t *root,Se_rbtree_node_t *node,
    Se_rbtree_node_t *sentinel);

Se_rbtree_node_t *SE_rbtree_next(Se_rbtree_t *tree,Se_rbtree_node_t *node);

#define SE_rbt_red(node)                ((node)->color = 1)
#define SE_rbt_black(node)              ((node)->color = 0)
#define SE_rbt_is_red(node)             ((node)->color)
#define SE_rbt_is_black(node)           (!SE_rbt_is_red(node))
#define SE_rbt_copy_color(n1,n2)        (n1->color = n2->color)

/*
初始化哨兵节点，将颜色置为黑
*/
#define SE_rbtree_sentinel_init(node)   SE_rbt_black(node)


/*
    找到当前节点及其子树的最小节点（以关键字key判断）
    node: 当前节点
    sentinel： 哨兵节点
*/
static inline Se_rbtree_node_t *SE_rbtree_min(Se_rbtree_node_t *node,Se_rbtree_node_t *sentinel)
{
    while(node->left != sentinel)
    {
        node = node->left;
    }
    return node;
}