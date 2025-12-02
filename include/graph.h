#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>

#define MAXN 100
#define NAME_LEN 64
#define DESC_LEN 256
#define INF 1e18


// 边类型
typedef enum {
  EDGE_PEDESTRIAN = 0,
  EDGE_VEHICLE = 1
  } EdgeType;

// 位掩码，用于选择允许的道路类型
#define ALLOW_PEDESTRIAN  (1 << 0)  // 0001
#define ALLOW_VEHICLE     (1 << 1)  // 0010
#define ALLOW_ALL        (ALLOW_PEDESTRIAN | ALLOW_VEHICLE)

// 节点结构
typedef struct {
  int id;
  char name[NAME_LEN];
  char desc[DESC_LEN];
  bool exists;
} Node;

// 边结构
typedef struct {
  double w;
  EdgeType type;
  bool exists;
} Edge;

// 图结构 (邻接矩阵)
typedef struct {
  Node nodes[MAXN];
  Edge adj[MAXN][MAXN];
  int n;
} Graph;

// === 函数声明 ===

// 初始化图
void init_graph(Graph *g);

// 加载真实校园数据
void load_real_campus(Graph *g);

void add_node(Graph *g, const char *name, const char *desc);
void del_node(Graph *g, int id);
void update_node(Graph *g, int id, const char *name, const char *desc);


void add_edge(Graph *g, int u, int v, double w, EdgeType type);
void del_edge(Graph *g, int u, int v);
void update_edge(Graph *g, int u, int v, double w, EdgeType type);


void print_node(Graph *g, int id);

// Dijkstra
bool dijkstra(Graph *g,
              int src,
              int dst,
              int allowed_mask,
              int seq[],
              int *out_m,
              double *out_len);
void enumerate_all_paths(Graph *g, int src, int dst, int maxDepth, int allowed_mask);


bool compute_tour(Graph *g, int *spots, int m, int start, int *out_seq, double *out_len, int allowed_mask, bool return_to_start);

// 保存与加载
bool save_graph(Graph *g, const char *filename);
bool load_graph(Graph *g, const char *filename);


#endif