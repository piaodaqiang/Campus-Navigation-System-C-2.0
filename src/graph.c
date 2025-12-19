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

/* ---------- Dijkstra 最短路 (返回 true 表示存在路径) ---------- */
#include <stdlib.h>

bool dijkstra(Graph *g,
              int src,
              int dst,
              int allowed_mask,
              int seq[],
              int *out_m,
              double *out_len)
{
    if (!g || src < 0 || dst < 0 || src >= g->n || dst >= g->n) return false;

    double dist[MAXN];
    int prev[MAXN];
    bool vis[MAXN];

    for (int i = 0; i < MAXN; ++i) {
        dist[i] = INF;
        prev[i] = -1;
        vis[i] = false;
    }

    dist[src] = 0.0;

    for (int step = 0; step < g->n; ++step) {
        int u = -1;
        double best = INF;
        for (int i = 0; i < g->n; ++i) {
            if (!vis[i] && dist[i] < best) {
                best = dist[i];
                u = i;
            }
        }
        if (u == -1) break;
        if (u == dst) break;
        vis[u] = true;

        for (int v = 0; v < g->n; ++v) {
            if (!g->adj[u][v].exists) continue;
            /* 按类型过滤：edge.type 是 0 或 1，使用位掩码判断 */
            if (!(allowed_mask & (1 << g->adj[u][v].type))) continue;
            double w = g->adj[u][v].w;
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                prev[v] = u;
            }
        }
    }

    if (dist[dst] >= INF) return false;

    /* 回溯路径 */
    int tmp[MAXN];
    int cnt = 0;
    for (int cur = dst; cur != -1 && cnt < MAXN; cur = prev[cur]) {
        tmp[cnt++] = cur;
    }
    /* 逆序写入 seq */
    *out_m = cnt;
    for (int i = 0; i < cnt; ++i) seq[i] = tmp[cnt - 1 - i];
    *out_len = dist[dst];
    return true;
}

/* ---------- 全路径枚举（逐个打印） ---------- */
static void dfs_all_paths(Graph *g, int u, int dst, int depth, int maxDepth,
                          int allowed_mask, bool *seen, int *path, int plen)
{
    if (depth > maxDepth) return;
    if (u == dst) {
        /* 打印路径 */
        for (int i = 0; i < plen; ++i) {
            printf("%s", g->nodes[path[i]].name);
            if (i + 1 < plen) printf(" -> ");
        }
        printf("\n");
        return;
    }

    for (int v = 0; v < g->n; ++v) {
        if (!g->adj[u][v].exists) continue;
        if (seen[v]) continue;
        if (!(allowed_mask & (1 << g->adj[u][v].type))) continue;
        seen[v] = true;
        path[plen] = v;
        dfs_all_paths(g, v, dst, depth + 1, maxDepth, allowed_mask, seen, path, plen + 1);
        seen[v] = false;
    }
}

void enumerate_all_paths(Graph *g, int src, int dst, int maxDepth, int allowed_mask)
{
    if (!g) return;
    if (src < 0 || src >= g->n || dst < 0 || dst >= g->n) {
        printf("起点或终点编号无效\n"); return;
    }
    bool seen[MAXN] = {0};
    int path[MAXN];
    seen[src] = true;
    path[0] = src;
    dfs_all_paths(g, src, dst, 0, maxDepth, allowed_mask, seen, path, 1);
}

/* ---------- 保存/加载（简洁文本格式） ---------- */
bool save_graph(Graph *g, const char *filename)
{
    if (!g || !filename) return false;
    FILE *f = fopen(filename, "w");
    if (!f) return false;

    /* 写入节点数量和节点信息 */
    fprintf(f, "%d\n", g->n);
    for (int i = 0; i < g->n; ++i) {
        if (!g->nodes[i].exists) {
            fprintf(f, "%d|%s|%s|0\n", i, g->nodes[i].name, g->nodes[i].desc);
        } else {
            fprintf(f, "%d|%s|%s|1\n", i, g->nodes[i].name, g->nodes[i].desc);
        }
    }

    /* 写入边数量及边信息 */
    int edgeCount = 0;
    for (int i = 0; i < g->n; ++i)
        for (int j = i+1; j < g->n; ++j)
            if (g->adj[i][j].exists) edgeCount++;

    fprintf(f, "%d\n", edgeCount);
    for (int i = 0; i < g->n; ++i) {
        for (int j = i+1; j < g->n; ++j) {
            if (g->adj[i][j].exists) {
                fprintf(f, "%d %d %.6f %d\n", i, j, g->adj[i][j].w, (int)g->adj[i][j].type);
            }
        }
    }

    fclose(f);
    return true;
}

bool load_graph(Graph *g, const char *filename)
{
    if (!g || !filename) return false;
    FILE *f = fopen(filename, "r");
    if (!f) return false;

    init_graph(g);

    int n;
    if (fscanf(f, "%d\n", &n) != 1) { fclose(f); return false; }
    /* 读节点 */
    for (int i = 0; i < n; ++i) {
        int id, exists;
        char line[512];
        if (!fgets(line, sizeof(line), f)) break;
        /* 格式： id|name|desc|exists\n */
        char *p = strtok(line, "|");
        if (!p) continue;
        id = atoi(p);
        p = strtok(NULL, "|");
        const char *name = p ? p : "";
        p = strtok(NULL, "|");
        const char *desc = p ? p : "";
        p = strtok(NULL, "|");
        exists = p ? atoi(p) : 0;

        if (id >= 0 && id < MAXN) {
            strncpy(g->nodes[id].name, name, NAME_LEN-1);
            g->nodes[id].name[NAME_LEN-1] = '\0';
            strncpy(g->nodes[id].desc, desc, DESC_LEN-1);
            g->nodes[id].desc[DESC_LEN-1] = '\0';
            g->nodes[id].exists = (exists != 0);
            if (id+1 > g->n) g->n = id+1;
        }
    }

    /* 读边 */
    int m;
    if (fscanf(f, "%d\n", &m) != 1) { fclose(f); return true; }
    for (int i = 0; i < m; ++i) {
        int u, v, type;
        double w;
        if (fscanf(f, "%d %d %lf %d\n", &u, &v, &w, &type) != 4) break;
        if (u >= 0 && u < MAXN && v >= 0 && v < MAXN) {
            g->adj[u][v].exists = g->adj[v][u].exists = true;
            g->adj[u][v].w = g->adj[v][u].w = w;
            g->adj[u][v].type = g->adj[v][u].type = (EdgeType)type;
            if (u+1 > g->n) g->n = u+1;
            if (v+1 > g->n) g->n = v+1;
        }
    }

    fclose(f);
    return true;
}

