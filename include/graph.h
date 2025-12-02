#ifndef GRAPH_H


#define MAXN 100
#define NAME_LEN 64
#define DESC_LEN 256
#define INF 1e18


// 边类型
typedef enum {
  EDGE_PEDESTRIAN = 0,
  EDGE_VEHICLE = 1
  } EdgeType;


typedef struct {
  int id;
  char name[NAME_LEN];
  char desc[DESC_LEN];
  bool exists;
} Node;


typedef struct {
  double w;
  EdgeType type;
  bool exists;
} Edge;


typedef struct {
  Node nodes[MAXN];
  Edge adj[MAXN][MAXN];
  int n;
} Graph;


void init_graph(Graph *g);
void load_real_campus(Graph *g);


void add_node(Graph *g, const char *name, const char *desc);
void del_node(Graph *g, int id);
void update_node(Graph *g, int id, const char *name, const char *desc);


void add_edge(Graph *g, int u, int v, double w, EdgeType type);
void del_edge(Graph *g, int u, int v);
void update_edge(Graph *g, int u, int v, double w, EdgeType type);


void print_node(Graph *g, int id);


void dijkstra(Graph *g, int src, int dst, int allowed_mask);
void enumerate_all_paths(Graph *g, int src, int dst, int maxDepth, int allowed_mask);


bool compute_tour(Graph *g, int *spots, int m, int start, int *out_seq, double *out_len, int allowed_mask, bool return_to_start);


bool save_graph(Graph *g, const char *filename);
bool load_graph(Graph *g, const char *filename);


#endif