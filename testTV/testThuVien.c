#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "weightedGraphlib.h"

int setVertex( Graph g){
    FILE *fp = fopen( "/mnt/d/Toan_roi_rac/testTV/sgb-words.txt", "r");
    int size = 1;
    int len;
    char name[10];
    if( fp == NULL){
        printf("Error to open file!!\n");
        return 0;
    }    
    while( fgets( name, 10, fp) != NULL ){
        len = strlen( name);
        if( name[len -1] == '\n') name[len - 2] = '\0';
        addVertex( g, size, name);
        size++;
    }
    size--;
    return size;
}

int compare( char s1[], char s2[]){
    int len1, len2, i;
    int num = 0;
    len1 = strlen( s1);
    len2 = strlen( s2);
    for( i = 0; i < len1; i++){
        if( s1[i] != s2[i]) num++;
    }
    return num;
}

void setEdge( Graph g, int size){
    int i, j;
    char s1[10], s2[10];
    for( i = 1; i <= size - 1; i++){
        strcpy( s1, getVertex( g, i));
        for( j = i + 1; j <= size; j++){
            strcpy( s2, getVertex( g, j));    
            if( compare( s1, s2) == 1){
                addEdge( g, i, j, 1);
                addEdge( g, j, i, 1);
            }
        }
    }
}


int main(){
    Graph g = createGraph();
    int size, i;
    int rs;
    char s1[10], s2[10];
    int v1 = 0, v2 = 0;
    int length = 0;
    int *path;
    size = setVertex( g);
    setEdge( g, size);
    path = (int*) calloc( size, sizeof(int));
    // so_tp_lt
    rs = DFS_num( g, size);
    printf("Do thi co %d thanh phan lien thong!!\n", rs);
    //to mau 
    coloring( g, size);
    printf_dot( g, size, "dothi.dot");
    //cay bao trum
    Graph cay_bt = createGraph();
    cay_bt = MST(g,size);
    printf_dot(cay_bt,size,"/mnt/d/Toan_roi_rac/testTV/dothibt.dot");
    //Lien thong manh
    Graph h = revGraph( g, size);
    Graph LT = createGraph();
    printf("Co %d thanh phan lien thong manh\n", DFSmanh( g, h, LT, size));
    //printfLTManh( LT, g, 104);
     // dijkstra bfs dfs 
    printf("Nhap 2 diem:");
    scanf("%s", s1);
    scanf("%s", s2);
    for( i = 1; i <= size; i ++){
        
        if( compare( s1, getVertex( g, i)) == 0) v1 = i;
        if( compare( s2, getVertex( g, i)) == 0) v2 = i;
        if( v1 != 0 && v2 != 0) break;
    }
   
    if( v1 == 0 || v2 == 0){
        printf("Khong co dinh hop le!!\n");
    }else{
        printf("Duong di tu %s den %s la: ", s1, s2);
        Dijkstra( g, v1, v2, path, &length, size);
        for( i = length - 1; i >= 0; i--){
            printf("%s\t", getVertex( g, path[i]));
        }
        printf("\n");

        //BFS( g, v1, v2, size);
        //DFS(g,v1,v2,size);
    }
    dropGraph( g);
    free( path);
}