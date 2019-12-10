//xjshi 2019/12/06
/*
1)����������ڶ�ʱ��
2����ֲ��nginx
2)c���Է�񣬻����Ĵ�������ӣ���ȡ
����1���ڵ�ʱ��ɫ���ߺ�ɫ
����2�����ڵ��Ǻ�ɫ
����3������Ҷ�ӽڵ㶼�Ǻ�ɫ��Ҷ�ӽڵ���NULL)
����4��ÿ����ɫ�ڵ�������ֽڵ㶼�Ǻ�ɫ��ÿ��Ҷ�ӽڵ㵽���ڵ��
����·���ϲ��������������ĺ�ɫ�ڵ㣩��
����5������һ�ڵ㵽��ÿ��Ҷ�ӽڵ�����м�·����������ͬ��Ŀ�ĺ�ɫ�ڵ�
*/
#pragma once
#include <stddef.h>
typedef struct Se_rbtree_node_s Se_rbtree_node_t;

struct Se_rbtree_node_s{
    unsigned int        key;        //�ؼ���
    Se_rbtree_node_t    *left;      //������
    Se_rbtree_node_t    *right;     //������
    Se_rbtree_node_t    *parent;    //���ڵ�
    unsigned char       color;      //��ɫ
    unsigned char       data;       //һ���ֽڵĽڵ�����
};

typedef struct Se_rbtree_s Se_rbtree_t;
typedef void (*SE_rbtree_insert_pt) (Se_rbtree_node_t*root,
    Se_rbtree_node_t*node,Se_rbtree_node_t*sentinel);

struct Se_rbtree_s{
    Se_rbtree_node_t    *root;      //ָ����ڵ㣬���ڵ�Ҳ������Ԫ��
    Se_rbtree_node_t    *sentinel;  //ָ���ڱ��ڵ�
    SE_rbtree_insert_pt insert;
};

/*
SE_rbtree_init ��ʼ�����������ʼ���ڱ��ڵ㣬���ڵ㣬SE_rbtree_insert_pt����
tree:�����������ָ��
s:�ڱ��ڵ��ָ��
i:SE_rbtree_insert_pt ���͵Ľڵ���ӷ���
*/
#define SE_rbtree_init(tree,s,i)                \
    SE_rbtree_sentinel_init(s);                 \
    (tree)->root = s;                           \
    (tree)->sentinel = s;                       \
    (tree)->insert = i;
/*
SE_rbtree_insert��������ݽڵ㣬�÷�������ת�������������ƽ��
tree�������������ָ��
node����Ҫ��ӵ�������Ľڵ�ָ��
*/
void SE_rbtree_insert(Se_rbtree_t *tree, Se_rbtree_node_t *node);
/*
SE_rbtree_delete��ɾ��������ڵ㣬�÷����� ͨ����ת���������������ƽ��
*/
void SE_rbtree_delete(Se_rbtree_t *tree, Se_rbtree_node_t *node);

/*
SE_rbtree_insert_value ��������������ݽڵ㣬�ؼ���Ψһ��
root:��ڱ�����ָ��
node:�����Ԫ�أ�Se_rbtree_node_t����ָ��
sentinel: �������ʼ��ʱ�ڱ��ڵ�ָ��
*/
void SE_rbtree_insert_value(Se_rbtree_node_t *root,Se_rbtree_node_t *node,
    Se_rbtree_node_t *sentinel);
/*
SE_rbtree_insert_timer_value��������������ݽڵ㣬�ؼ��ֱ�ʾʱ�����ʱ���
root: ���������ָ��
node:�����Ԫ�أ��ؼ�����ʱ�����ʱ�������Ǹ���
sentinel:�ڱ�ָ��
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
��ʼ���ڱ��ڵ㣬����ɫ��Ϊ��
*/
#define SE_rbtree_sentinel_init(node)   SE_rbt_black(node)


/*
    �ҵ���ǰ�ڵ㼰����������С�ڵ㣨�Թؼ���key�жϣ�
    node: ��ǰ�ڵ�
    sentinel�� �ڱ��ڵ�
*/
static inline Se_rbtree_node_t *SE_rbtree_min(Se_rbtree_node_t *node,Se_rbtree_node_t *sentinel)
{
    while(node->left != sentinel)
    {
        node = node->left;
    }
    return node;
}