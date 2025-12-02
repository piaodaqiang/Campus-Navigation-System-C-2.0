#include <stdio.h>
{11, "国际微电子学院", "国际微电子学院所在地"},
{12, "化学工程与能源学院", "化学工程与能源学院所在地"},
{13, "计算机学院", "计算机学院所在地"},
{14, "电子工程与智能化学院", "电子工程与智能化学院所在地"},
{15, "6栋教学楼", "各学科课程可教学地点"},
{16, "图书馆", "看书的地方"},
{17, "第二食堂", "吃饭的地方"},
{18, "幼儿园", "东莞理工学院附属幼儿园"},
{19, "教师村", "老师集中地"},
{20, "真草运动场", "较大运动场，可跑步打球"}
};


// TODO: 补上实际距离，我可为你生成示例


typedef struct { int u,v; double w; EdgeType t; } CampusEdge;


CampusEdge campus_edges[] = {
  {0,1,120,EDGE_PEDESTRIAN},
  {1,2,80,EDGE_PEDESTRIAN},
  {2,3,150,EDGE_PEDESTRIAN},
  {3,4,200,EDGE_PEDESTRIAN},
  {4,5,160,EDGE_PEDESTRIAN},
  {5,6,100,EDGE_VEHICLE},
  {6,7,180,EDGE_VEHICLE},
  {7,8,140,EDGE_PEDESTRIAN},
  {8,9,130,EDGE_PEDESTRIAN},
  {9,13,90,EDGE_PEDESTRIAN},
  {13,10,190,EDGE_VEHICLE},
  {10,11,110,EDGE_PEDESTRIAN},
  {11,12,160,EDGE_PEDESTRIAN},
  {12,14,130,EDGE_PEDESTRIAN},
  {14,15,170,EDGE_VEHICLE},
  {15,16,140,EDGE_PEDESTRIAN},
  {16,17,120,EDGE_PEDESTRIAN},
  {17,18,100,EDGE_PEDESTRIAN},
  {18,19,150,EDGE_PEDESTRIAN},
  {19,20,200,EDGE_VEHICLE}
};


void load_real_campus(Graph *g) {
  init_graph(g);
  int nodeCount = sizeof(campus_nodes)/sizeof(campus_nodes[0]);
  int edgeCount = sizeof(campus_edges)/sizeof(campus_edges[0]);


  for (int i=0;i<nodeCount;i++) {
    int id = campus_nodes[i].id;
    g->nodes[id].exists = true;
    g->n = id+1;
    strncpy(g->nodes[id].name, campus_nodes[i].name, NAME_LEN);
    strncpy(g->nodes[id].desc, campus_nodes[i].desc, DESC_LEN);
  }


  for (int i=0;i<edgeCount;i++) {
    int u = campus_edges[i].u;
    int v = campus_edges[i].v;
    g->adj[u][v].exists = g->adj[v][u].exists = true;
    g->adj[u][v].w = g->adj[v][u].w = campus_edges[i].w;
    g->adj[u][v].type = g->adj[v][u].type = campus_edges[i].t;
  }


  printf("已加载东莞理工学院真实地点数据，共 %d 个点，%d 条边\n", nodeCount, edgeCount);
}