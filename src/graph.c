#include "graph.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

/* 节点数组类型（仅用于本文件） */
typedef struct { int id; const char *name; const char *desc; } CampusNode;

/* 真实校园节点：21 个 */
static CampusNode campus_nodes[] = {
    {0,  "东莞理工学院国际创新区", "中法学院主要地点，豪华装修，非常新"},
    {1,  "益禾堂", "为数不多入驻北街的奶茶品牌"},
    {2,  "学生活动中心", "主要的社团活动地点，包括舞台表演、讲座等"},
    {3,  "学生社区", "学生宿舍，可以睡觉的地方"},
    {4,  "第一食堂", "可以吃饭的地方"},
    {5,  "假草运动场", "较小的运动场，可以跑步、打篮球、踢足球等"},
    {6,  "机械园", "机械学院所在地，可以参观机械学院"},
    {7,  "莞星台", "东莞理工学院标志性建筑，可以进行天文工作"},
    {8,  "教育学院", "教育学院所在地"},
    {9,  "文科楼", "主要文学与传媒学院所在地"},
    {10, "机械工程学院", "机械工程学院所在地"},
    {11, "国际微电子学院", "国际微电子学院所在地"},
    {12, "化学工程与能源学院", "化学工程与能源学院所在地"},
    {13, "计算机学院", "计算机学院所在地"},
    {14, "电子工程与智能化学院", "电子工程与智能化学院所在地"},
    {15, "6栋教学楼", "各学科课程可教学地点"},
    {16, "图书馆", "看书的地方"},
    {17, "第二食堂", "吃饭的地方"},
    {18, "幼儿园", "东莞理工学院附属幼儿园"},
    {19, "教师村", "老师集中地"},
    {20, "真草运动场", "较大运动场，可以跑步、打篮球、踢足球等"}
};

/* 边的结构与数组 */
typedef struct { int u, v; double w; EdgeType t; } CampusEdge;

static CampusEdge campus_edges[] = {
    {0,1,120, EDGE_PEDESTRIAN},
    {1,2,80,  EDGE_PEDESTRIAN},
    {2,3,150, EDGE_PEDESTRIAN},
    {3,4,200, EDGE_PEDESTRIAN},
    {4,5,160, EDGE_PEDESTRIAN},
    {5,6,100, EDGE_VEHICLE},
    {6,7,180, EDGE_VEHICLE},
    {7,8,140, EDGE_PEDESTRIAN},
    {8,9,130, EDGE_PEDESTRIAN},
    {9,13,90, EDGE_PEDESTRIAN},
    {13,10,190, EDGE_VEHICLE},
    {10,11,110, EDGE_PEDESTRIAN},
    {11,12,160, EDGE_PEDESTRIAN},
    {12,14,130, EDGE_PEDESTRIAN},
    {14,15,170, EDGE_VEHICLE},
    {15,16,140, EDGE_PEDESTRIAN},
    {16,17,120, EDGE_PEDESTRIAN},
    {17,18,100, EDGE_PEDESTRIAN},
    {18,19,150, EDGE_PEDESTRIAN},
    {19,20,200, EDGE_VEHICLE}
};

/* 加载到 Graph 结构中的函数 */

/* 初始化图：清空所有节点与边 */
void init_graph(Graph *g) {
    g->n = 0;

    for (int i = 0; i < MAXN; i++) {
        g->nodes[i].exists = false;
        g->nodes[i].id = i;
        g->nodes[i].name[0] = '\0';
        g->nodes[i].desc[0] = '\0';

        for (int j = 0; j < MAXN; j++) {
            g->adj[i][j].exists = false;
            g->adj[i][j].w = 0;
            g->adj[i][j].type = EDGE_PEDESTRIAN; // 默认类型
        }
    }
}
void load_real_campus(Graph *g) {
    init_graph(g); // 先初始化图结构（清空）

    int nodeCount = (int)(sizeof(campus_nodes)/sizeof(campus_nodes[0]));
    int edgeCount = (int)(sizeof(campus_edges)/sizeof(campus_edges[0]));

    /* 将节点信息写入 g->nodes */
    for (int i = 0; i < nodeCount; ++i) {
        int id = campus_nodes[i].id;
        if (id < 0 || id >= MAXN) continue;
        g->nodes[id].exists = true;
        g->nodes[id].id = id;
        /* 使用 strncpy 防止越界，确保以 '\0' 结尾 */
        strncpy(g->nodes[id].name, campus_nodes[i].name, NAME_LEN - 1);
        g->nodes[id].name[NAME_LEN - 1] = '\0';
        strncpy(g->nodes[id].desc, campus_nodes[i].desc, DESC_LEN - 1);
        g->nodes[id].desc[DESC_LEN - 1] = '\0';
        if (id + 1 > g->n) g->n = id + 1;
    }

    /* 将边写入邻接矩阵 */
    for (int i = 0; i < edgeCount; ++i) {
        int u = campus_edges[i].u;
        int v = campus_edges[i].v;
        if (u < 0 || u >= MAXN || v < 0 || v >= MAXN) continue;
        g->adj[u][v].exists = g->adj[v][u].exists = true;
        g->adj[u][v].w = g->adj[v][u].w = campus_edges[i].w;
        g->adj[u][v].type = g->adj[v][u].type = campus_edges[i].t;
        if (u+1 > g->n) g->n = u+1;
        if (v+1 > g->n) g->n = v+1;
    }

    printf("已加载东莞理工学院真实地点数据，共 %d 个点，%d 条边\n", nodeCount, edgeCount);
}
