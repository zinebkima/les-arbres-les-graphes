#include <stdio.h>
#include <stdlib.h>

#define N 8
#define MAX_QUEUE 100
#define INF 1000000



typedef struct {
    int items[MAX_QUEUE];
    int front;
    int rear;
} Queue;

void initQueue(Queue* q) {
    q->front = -1;
    q->rear = -1;
}

int isEmpty(Queue* q) {
    return (q->front == -1);
}

int isFull(Queue* q) {
    return (q->rear == MAX_QUEUE - 1);
}

void enqueue(Queue* q, int value) {
    if (isFull(q)) {
        printf("La file est pleine!\n");
        return;
    }
    if (q->front == -1) {
        q->front = 0;
    }
    q->rear++;
    q->items[q->rear] = value;
}

int dequeue(Queue* q) {
    if (isEmpty(q)) {
        printf("La file est vide!\n");
        return -1;
    }
    int item = q->items[q->front];
    q->front++;
    if (q->front > q->rear) {
        q->front = q->rear = -1;
    }
    return item;
}


void BFS(int graph[N][N], int start) {
    int visited[N] = {0};
    Queue q;
    initQueue(&q);

    visited[start] = 1;
    enqueue(&q, start);

    printf("Parcours en largeur a partir de %c : ", start + 'A');

    while (!isEmpty(&q)) {
        int u = dequeue(&q);
        printf("%c ", u + 'A');

        for (int v = 0; v < N; v++) {
            if (graph[u][v] == 1 && !visited[v]) {
                visited[v] = 1;
                enqueue(&q, v);
            }
        }
    }
    printf("\n");
}


void DFS(int graph[N][N], int visited[], int start) {
    visited[start] = 1;
    printf("%c ", start + 'A');

    for (int v = 0; v < N; v++) {
        if (graph[start][v] == 1 && !visited[v]) {
            DFS(graph, visited, v);
        }
    }
}


int minDistance(int dist[], int visited[], int size) {
    int min = INF, min_index = -1;

    for (int v = 0; v < size; v++) {
        if (!visited[v] && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

void dijkstra(int graph[N][N], int source, int size) {
    int dist[N], visited[N];


    for (int i = 0; i < size; i++) {
        dist[i] = INF;
        visited[i] = 0;
    }
    dist[source] = 0;


    for (int count = 0; count < size - 1; count++) {
        int u = minDistance(dist, visited, size);
        if (u == -1) break;

        visited[u] = 1;

        for (int v = 0; v < size; v++) {
            if (!visited[v] && graph[u][v] && dist[u] != INF
                && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }


    printf("\nAlgorithme de Dijkstra - Plus courts chemins depuis %c :\n", source + 'A');
    printf("===============================================\n");
    printf("Sommet\t\tDistance\n");
    printf("-------\t\t--------\n");

    char nodes[N] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
    for (int i = 0; i < size; i++) {
        printf("%c\t\t", nodes[i]);
        if (dist[i] == INF)
            printf("INF\n");
        else
            printf("%d\n", dist[i]);
    }
}


void afficherGraphe(int graph[N][N], int size) {
    printf("\nMatrice d'adjacence :\n");
    printf("   ");
    for (int i = 0; i < size; i++) {
        printf("%c  ", 'A' + i);
    }
    printf("\n");

    for (int i = 0; i < size; i++) {
        printf("%c  ", 'A' + i);
        for (int j = 0; j < size; j++) {
            printf("%d  ", graph[i][j]);
        }
        printf("\n");
    }
}


int main() {

    printf("                  TESTS DES ALGORITHMES DE GRAPHES\n");
    printf("\n\n");


    printf("*****************************TEST 1: BFS ET DFS *********************************\n");
    int graph1[N][N] = {0};


    int edges[][2] = {
        {0, 1}, {0, 2}, {1, 4}, {2, 4}, {2, 3},
        {4, 5}, {3, 5}, {3, 6}, {5, 6}, {5, 7}, {6, 7}
    };
    int nbEdges = sizeof(edges) / sizeof(edges[0]);


    for (int i = 0; i < nbEdges; i++) {
        int a = edges[i][0], b = edges[i][1];
        graph1[a][b] = 1;
        graph1[b][a] = 1;
    }

    afficherGraphe(graph1, N);


    int visited[N] = {0};
    printf("\nParcours en profondeur (DFS) a partir de A : ");
    DFS(graph1, visited, 0);
    printf("\n");


    BFS(graph1, 0);


    int visited2[N] = {0};
    printf("Parcours en profondeur (DFS)  a partir de C : ");
    DFS(graph1, visited2, 2);
    printf("\n");

    BFS(graph1, 2);


    printf("\n\n*********************** TEST 2: ALGORITHME DE DIJKSTRA ************************\n");

    int graph2[N][N] = {
        {0, 1, 2, 0, 0, 0, 0, 0},
        {1, 0, 0, 2, 0, 3, 0, 0},
        {2, 0, 0, 3, 4, 0, 0, 0},
        {0, 2, 3, 0, 2, 3, 3, 0},
        {0, 0, 4, 2, 0, 0, 5, 0},
        {0, 3, 0, 3, 0, 0, 4, 0},
        {0, 0, 0, 3, 5, 4, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}
    };

    afficherGraphe(graph2, 7);
    dijkstra(graph2, 0, 7);


    printf("\n\n******************************** TEST 3: DIJKSTRA (AUTRE GRAPHE) *********************************\n");

    int graph3[N][N] = {
        {0, 4, 0, 0, 0, 0, 0, 8},
        {4, 0, 8, 0, 0, 0, 0, 11},
        {0, 8, 0, 7, 0, 4, 0, 0},
        {0, 0, 7, 0, 9, 14, 0, 0},
        {0, 0, 0, 9, 0, 10, 0, 0},
        {0, 0, 4, 14, 10, 0, 2, 0},
        {0, 0, 0, 0, 0, 2, 0, 1},
        {8, 11, 0, 0, 0, 0, 1, 0}
    };

    afficherGraphe(graph3, 8);
    dijkstra(graph3, 0, 8);


    dijkstra(graph3, 4, 8);


    printf("\n\n****************************TEST 4: COMPARAISON BFS VS DFS *************************\n");
    printf("Graphe en forme d'arbre :\n");

    int tree[N][N] = {0};
    int treeEdges[][2] = {
        {0, 1}, {0, 2}, {1, 3}, {1, 4}, {2, 5}, {2, 6}
    };
    int nbTreeEdges = sizeof(treeEdges) / sizeof(treeEdges[0]);

    for (int i = 0; i < nbTreeEdges; i++) {
        int a = treeEdges[i][0], b = treeEdges[i][1];
        tree[a][b] = 1;
        tree[b][a] = 1;
    }

    afficherGraphe(tree, 7);

    int visitedTree[N] = {0};
    printf("\nDFS : ");
    DFS(tree, visitedTree, 0);
    printf("\n");

    BFS(tree, 0);


    return 0;
}
