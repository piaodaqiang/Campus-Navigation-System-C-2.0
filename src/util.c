#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include "util.h"
#include "graph.h"


void input_line(char *buf, int n) {
  fgets(buf, n, stdin);
  size_t L = strlen(buf);
  if (L>0 && buf[L-1]=='\n') buf[L-1]=0;
}


int read_int(const char *prompt) {
  char s[128];
  printf("%s", prompt);
  input_line(s, sizeof(s));
  return atoi(s);
}


double read_double(const char *prompt) {
  char s[128];
  printf("%s", prompt);
  input_line(s, sizeof(s));
  return atof(s);
}

// ------ 增加、删除、更新节点 ------

/* 辅助：安全读取一行（去掉末尾换行） */
static void read_line(char *buf, int len) {
    if (!fgets(buf, len, stdin)) {
        buf[0] = '\0';
        return;
    }
    size_t L = strlen(buf);
    if (L > 0 && buf[L-1] == '\n') buf[L-1] = '\0';
}

/* 按原型实现：添加节点（通过传入 name, desc） */
void add_node(Graph *g, const char *name, const char *desc) {
    if (!g || !name) return;

    /* 找一个空闲 id：优先使用小的空位 */
    int id = -1;
    for (int i = 0; i < MAXN; ++i) {
        if (!g->nodes[i].exists) { id = i; break; }
    }
    if (id == -1) {
        printf("错误：节点数量已达上限（%d），无法添加。\n", MAXN);
        return;
    }

    g->nodes[id].exists = true;
    g->nodes[id].id = id;
    strncpy(g->nodes[id].name, name, NAME_LEN - 1);
    g->nodes[id].name[NAME_LEN - 1] = '\0';
    if (desc) {
        strncpy(g->nodes[id].desc, desc, DESC_LEN - 1);
        g->nodes[id].desc[DESC_LEN - 1] = '\0';
    } else {
        g->nodes[id].desc[0] = '\0';
    }
    if (id + 1 > g->n) g->n = id + 1;

    printf("已添加地点：%d. %s\n", id, g->nodes[id].name);
}

/* 按原型实现：删除节点（按 id），并清除所有相关边 */
void del_node(Graph *g, int id) {
    if (!g) return;
    if (id < 0 || id >= MAXN || !g->nodes[id].exists) {
        printf("删除失败：无效节点编号 %d\n", id);
        return;
    }

    /* 清除节点数据 */
    g->nodes[id].exists = false;
    g->nodes[id].name[0] = '\0';
    g->nodes[id].desc[0] = '\0';

    /* 清除与之相关的边（邻接矩阵）    */
    for (int i = 0; i < g->n; ++i) {
        g->adj[id][i].exists = false;
        g->adj[i][id].exists = false;
        g->adj[id][i].w = 0;
        g->adj[i][id].w = 0;
    }

    /* 可选：如果删除的是最高编号节点，调整 g->n（保持 n 为最大存在 id +1） */
    while (g->n > 0 && !g->nodes[g->n - 1].exists) g->n--;

    printf("已删除节点 %d\n", id);
}

/* 按原型实现：更新节点信息（按 id） */
void update_node(Graph *g, int id, const char *name, const char *desc) {
    if (!g) return;
    if (id < 0 || id >= MAXN || !g->nodes[id].exists) {
        printf("更新失败：无效节点编号 %d\n", id);
        return;
    }

    if (name) {
        strncpy(g->nodes[id].name, name, NAME_LEN - 1);
        g->nodes[id].name[NAME_LEN - 1] = '\0';
    }
    if (desc) {
        strncpy(g->nodes[id].desc, desc, DESC_LEN - 1);
        g->nodes[id].desc[DESC_LEN - 1] = '\0';
    }

    printf("已更新节点 %d 信息：%s\n", id, g->nodes[id].name);
}

/* 交互式管理（可由 main 中的 case 7 调用） */
void manage_nodes(Graph *g) {
    if (!g) return;
    int op;
    char bufName[NAME_LEN];
    char bufDesc[DESC_LEN];
    int id;

    while (1) {
        printf("\n==== 地点管理 ====\n");
        printf("1. 添加地点\n");
        printf("2. 删除地点\n");
        printf("3. 修改地点\n");
        printf("0. 返回主菜单\n");
        printf("请选择：");
        if (scanf("%d", &op) != 1) {
            /* 读取失败，清空 stdin 并返回 */
            int c; while ((c = getchar()) != EOF && c != '\n') {}
            return;
        }
        int c = getchar(); /* 吃掉换行 */

        if (op == 0) return;

        switch (op) {
            case 1:
                printf("输入地点名称：");
                read_line(bufName, sizeof(bufName));
                printf("输入地点简介：");
                read_line(bufDesc, sizeof(bufDesc));
                add_node(g, bufName, bufDesc);
                break;

            case 2:
                printf("请输入要删除的地点编号：");
                if (scanf("%d", &id) != 1) { int d; while ((d=getchar())!=EOF && d!='\n'); break; }
                del_node(g, id);
                break;

            case 3:
                printf("请输入要更新的地点编号：");
                if (scanf("%d", &id) != 1) { int d; while ((d=getchar())!=EOF && d!='\n'); break; }
                c = getchar(); /* 吃换行 */
                if (id < 0 || id >= MAXN || !g->nodes[id].exists) {
                    printf("无效编号或节点不存在。\n");
                    break;
                }
                printf("新的地点名称（回车表示不修改，直接回车跳过）：");
                read_line(bufName, sizeof(bufName));
                printf("新的地点简介（回车表示不修改）：");
                read_line(bufDesc, sizeof(bufDesc));
                /* 只有当输入非空才替换 */
                if (bufName[0] == '\0') {
                    update_node(g, id, NULL, (bufDesc[0] ? bufDesc : NULL));
                } else {
                    update_node(g, id, bufName, (bufDesc[0] ? bufDesc : NULL));
                }
                break;

            default:
                printf("无效选项，请重试。\n");
        }
    }
}