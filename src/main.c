#include <stdio.h>
#include "graph.h"
#include "util.h"

void show_menu() {
    printf("\n====== 东莞理工学院 导游系统 ======\n");
    printf("1. 查看所有校园地点\n");
    printf("2. 查询地点信息\n");
    printf("3. 查询两点最短路径（Dijkstra）\n");
    printf("4. 显示所有路径（DFS 枚举）\n");
    printf("5. 保存地图到文件\n");
    printf("6. 从文件加载地图\n");
    printf("0. 退出系统\n");
    printf("请选择：");
}

void list_all_nodes(Graph *g) {
    printf("\n====== 校园地点列表（共 %d 个） ======\n", g->n);
    for (int i = 0; i < g->n; i++) {
        if (g->nodes[i].exists) {
            printf("%2d. %-20s  ——  %s\n",
                   g->nodes[i].id,
                   g->nodes[i].name,
                   g->nodes[i].desc);
        }
    }
}

void query_node(Graph *g) {
    int id;
    printf("请输入地点编号：");
    scanf("%d", &id);
    if (id < 0 || id >= g->n || !g->nodes[id].exists) {
        printf("无效地点编号！\n");
        return;
    }
    printf("\n编号：%d\n名称：%s\n简介：%s\n",
           g->nodes[id].id,
           g->nodes[id].name,
           g->nodes[id].desc);
}

void shortest_path(Graph *g) {
    int s, t;
    printf("请输入起点编号：");
    scanf("%d", &s);
    printf("请输入终点编号：");
    scanf("%d", &t);

    double out_len;
    int seq[MAXN], m;

    if (!dijkstra(g, s, t, ALLOW_ALL, seq, &m, &out_len)) {
        printf("路径不存在！\n");
        return;
    }

    printf("\n最短路径长度：%.2f 米\n路径：\n", out_len);
    for (int i = 0; i < m; i++) {
        printf("%s", g->nodes[seq[i]].name);
        if (i != m - 1) printf(" -> ");
    }
    printf("\n");
}

void show_all_paths(Graph *g) {
    int s, t, depth;
    printf("请输入起点编号：");
    scanf("%d", &s);
    printf("请输入终点编号：");
    scanf("%d", &t);
    printf("最大路径深度（建议 < 10）：");
    scanf("%d", &depth);

    enumerate_all_paths(g, s, t, depth, ALLOW_ALL);
}

int main() {
    setbuf(stdout, NULL);
    Graph g;
    load_real_campus(&g);   // 载入真实校园数据

    int choice;

    while (1) {
        show_menu();
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                list_all_nodes(&g);
                break;
            case 2:
                query_node(&g);
                break;
            case 3:
                shortest_path(&g);
                break;
            case 4:
                show_all_paths(&g);
                break;
            case 5:
                save_graph(&g, "campus.map");
                printf("已保存到 campus.map\n");
                break;
            case 6:
                if (load_graph(&g, "campus.map"))
                    printf("成功从 campus.map 加载！\n");
                else
                    printf("加载失败！\n");
                break;
            case 0:
                printf("感谢使用，再见！\n");
                return 0;
            default:
                printf("无效选项，请重新输入！\n");
        }
    }

    return 0;
}
