#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "weightedGraphlib.h"


#define INFINITIVE_VALUE 10000000


//Do thi co ban
Graph createGraph(){
  Graph g = (Graph)malloc(sizeof(struct _Graph));
  if(g==NULL) {
	  printf("NULL, can not allocate mem, abort...\n");
	  return NULL;
  }
  g->vertices = make_jrb();
  g->edges = make_jrb();
  return g;
}

void addVertex(Graph graph, int id, char* name)
{
	JRB vertices  = graph->vertices;
	JRB edges  = graph->edges;
	JRB tree;
	
	//update vertex
	detailVertex iver = (detailVertex)malloc(sizeof(struct _detailVertex));
	if(iver==NULL) {
	  printf("NULL, can not allocate mem, abort...\n");
	  return;
	}
	iver->name = strdup(name);
	iver->indegreeTree = make_jrb();
	iver->linethong = make_jrb();
    iver->color = 0;
    iver->parent = -1;
	iver->wei = INFINITIVE_VALUE;
	jrb_insert_int(vertices,id,new_jval_v(iver));
	
	//update edge
	tree = make_jrb();
	jrb_insert_int(edges,id,new_jval_v(tree));
}

char *getVertex(Graph graph, int id)
{
	JRB vnode=jrb_find_int(graph->vertices,id);
	
	if(vnode==NULL) return NULL;
	detailVertex iver = (detailVertex) vnode->val.v;
	
	return iver->name;
}

void addEdge(Graph graph, int v1, int v2, double wei)
{
	JRB enode = jrb_find_int(graph->edges,v1);
	JRB vnode = jrb_find_int(graph->vertices,v2);
	JRB tree;
	if((enode==NULL)||(vnode==NULL)) {
		printf("vertex not found\n");
		return;
    	}

	tree=(JRB)(enode->val).v;
	jrb_insert_int(tree,v2,new_jval_d(wei));
	
	detailVertex iver = (detailVertex) vnode->val.v;
	tree = iver->indegreeTree;
	jrb_insert_int(tree,v1,JNULL);
}

int hasEdge(Graph graph, int v1, int v2)
{
	JRB enode = jrb_find_int(graph->edges,v1);
	JRB tree;
	if(enode==NULL) {
		printf("vertex not found\n");
		return 0;
    	}
    	tree=(JRB)(enode->val).v;
	if(jrb_find_int(tree,v2)!=NULL)
		return 1;
	else return 0;
}

double getEdgeValue(Graph graph, int v1, int v2)
{
	JRB enode = jrb_find_int(graph->edges,v1);
	JRB tree;

	if(enode==NULL) {
		printf("vertex not found\n");
		return INFINITIVE_VALUE;
    	}
    
    	tree = jrb_find_int((JRB)(enode->val).v,v2);

	if(tree==NULL){
		return INFINITIVE_VALUE;
	}

    	return (tree->val).d;
}

int indegree(Graph graph, int v, int* output)
{
	JRB vnode = jrb_find_int(graph->vertices,v);
	JRB tree,node;
	int innum = 0;
	
	if(vnode==NULL) {
		printf("vertex not found\n");
		return 0;
    	}
    	detailVertex iver = (detailVertex) vnode->val.v;
	tree = iver->indegreeTree;
	//traverse tree
	jrb_traverse(node, tree){
		output[innum] = node->key.i;
		innum++;
		//printf("\nnumber innum:%d\n",output[innum-1]);
	}
	return innum;
}

int outdegree(Graph graph, int v, int* output)
{
	JRB enode = jrb_find_int(graph->edges,v);
	JRB tree,node;
	int outnum = 0;
	
	if(enode==NULL) {
		printf("vertex not found\n");
		return 0;
    	}

	tree =(JRB)(enode->val).v;
	//traverse tree
	jrb_traverse(node, tree){
		output[outnum] = node->key.i;
		outnum++;
		//printf("\nnumber innum:%d\n",output[outnum-1]);
	}
	return outnum;
}


//DFS
int DFS_num( Graph graph, int size){
    int *visited = malloc( (size + 1) * sizeof(int));
    int *out = malloc( (size) * sizeof(int));
    int i, j, k, v, num;
    Dllist queue = new_dllist();
    Dllist queue_list = new_dllist();
    Dllist now, tmp;
    int rs = 0;
    for( i = 1; i <= size; i++){
        visited[i] = 0;
        dll_append( queue_list, new_jval_i( i));
    }    
    //dll_append( queue, new_jval_i( start));
    
    while( !dll_empty( queue_list)){
        tmp = dll_last( queue_list);
        k = jval_i( tmp->val);
        //dll_delete_node( tmp);
        dll_append( queue, new_jval_i( k));

        while( !dll_empty( queue)){       
            now = dll_last( queue);      
            i = jval_i( now->val);
            dll_delete_node( now);       
            num = outdegree( graph, i, out);
            if( visited[i] != 1){
                dll_traverse( tmp, queue_list){
                    k = jval_i(tmp->val);
                    if ( k == i){
                        dll_delete_node( tmp);
                        break;
                    }
                }
                visited[i] = 1;
                for( j = 0; j < num; j++){
                    v = out[j];
                    if( visited[ v] == 0){
                        dll_append( queue, new_jval_i( v));
                    }
                }
            }
        }

        rs++;
    }
    free( out);
    free_dllist( queue);
    free_dllist( queue_list);
    free( visited);
    return rs;
}

void DFS( Graph graph, int start, int stop, int size){
    int *visited = malloc( (size + 1) * sizeof(int));
    int *out = malloc( (size) * sizeof(int));
    int i, j, v, num;
    Dllist now;
    Dllist queue = new_dllist();
    Graph check, list, tmp;

    for( i = 1; i <= size; i++){
        visited[i] = 0;
    }
    
    dll_append( queue, new_jval_i( start));
    //dll_append( queue, new_jval_i( start));
    

    while( !dll_empty( queue)){       
        now = dll_last( queue);      
        i = jval_i( now->val);
        dll_delete_node( now);
        if( i == stop){
            printf("%s\t", getVertex( graph, i));
            break;
        }       
        num = outdegree( graph, i, out);
        if( visited[i] != 1){
            printf("%s\t",getVertex( graph, i));
            visited[i] = 1;
            for( j = 0; j < num; j++){
                v = out[j];
                if( visited[ v] == 0){
                    dll_append( queue, new_jval_i( v));
                }
            }
        }
    }
    printf("\n");
    free_dllist( queue);
    free( out);
    free( visited);    
}

//BFS
void BFS( Graph graph, int start, int stop, int size){
    int *visited = malloc( (size + 1) * sizeof(int));
    int *out = malloc( (size) * sizeof(int));
    int i, j, v, num;
    Dllist now;
    Dllist queue = new_dllist();
    Graph check, list, tmp;

    for( i = 1; i <= size; i++){
        visited[i] = 0;
    }
    
    dll_append( queue, new_jval_i( start));
    //dll_append( queue, new_jval_i( start));
    

    while( !dll_empty( queue)){       
        now = dll_first( queue);      
        i = jval_i( now->val);
        dll_delete_node( now);
        if( i == stop){
            printf("%s\t", getVertex( graph, i));
            break;
        }       
        num = outdegree( graph, i, out);
        if( visited[i] != 1){
            printf("%s\t",getVertex( graph, i));
            visited[i] = 1;
            for( j = 0; j < num; j++){
                v = out[j];
                if( visited[ v] == 0){
                    dll_append( queue, new_jval_i( v));
                }
            }
        }
    }
    printf("\n");
    free_dllist( queue);
    free( out);
    free( visited);    
}

//Lien thong
int DFSmanh( Graph graph, Graph h, Graph LT, int size){
    int *visited = malloc( (size + 1) * sizeof(int));
    int *out = malloc( (size) * sizeof(int));
    int i, j, k, v, num;
    Dllist queue = new_dllist();
    Dllist queue_list = new_dllist();
    Dllist now, tmp;

    Dllist step1 = new_dllist();
    Dllist step2 = new_dllist();
    Dllist step3 = new_dllist();
    Dllist lst;
    int ts, ts1, slgLT;
    //int rs = 0;
    for( i = 1; i <= size; i++){
        visited[i] = 0;
        dll_append( queue_list, new_jval_i( i));
    }    
    //dll_append( queue, new_jval_i( start));
    
    while( !dll_empty( queue_list)){
        tmp = dll_last( queue_list);
        k = jval_i( tmp->val);
        //dll_delete_node( tmp);
        dll_append( queue, new_jval_i( k));

        while( !dll_empty( queue)){       
            now = dll_last( queue);      
            i = jval_i( now->val);
            dll_delete_node( now);       
            num = outdegree( graph, i, out);
            if( visited[i] != 1){
                dll_append( step1, new_jval_i( i));
                dll_traverse( tmp, queue_list){
                    k = jval_i(tmp->val);
                    if ( k == i){
                        dll_delete_node( tmp);
                        break;
                    }
                }
                visited[i] = 1;
                for( j = 0; j < num; j++){
                    v = out[j];
                    if( visited[ v] == 0){
                        dll_append( queue, new_jval_i( v));
                    }
                }
            }
        }
        while( !dll_empty( step1)){
            lst = dll_last( step1);
            ts = jval_i( lst->val);
            dll_delete_node( lst);
            dll_prepend( step2, new_jval_i( ts));
        }
       //rs++;
    }
    //return rs;
    for( i = 1; i <= size; i++){
        visited[i] = 0;
    }    
    slgLT = 1;
    while( !dll_empty( step2)){
        tmp = dll_first( step2);
        ts = jval_i( tmp->val);
        //dll_delete_node( tmp);
        dll_append( step3, new_jval_i( ts));
        addVertex( LT, slgLT, "");
        while( !dll_empty( step3)){       
            lst = dll_last( step3);      
            ts1 = jval_i( lst->val);
            dll_delete_node( lst);       
            num = outdegree( h, ts1, out);
            if( visited[ts1] != 1){
                dll_traverse( tmp, step2){
                    ts = jval_i(tmp->val);
                    if ( ts == ts1){
                        dll_delete_node( tmp);
                        break;
                    }
                }
                visited[ts1] = 1;
                addLienthong( LT, slgLT, ts1);
                for( j = 0; j < num; j++){
                    v = out[j];
                    if( visited[ v] == 0){
                        dll_append( step3, new_jval_i( v));
                    }
                }
            }
        }
    slgLT++;
    }

    free_dllist( queue);
    free_dllist( queue_list);
    free_dllist( step1);
    free_dllist( step2);
    free_dllist( step3);
    free( out);
    free( visited);
    return slgLT - 1;
}

JRB getLienthong( Graph graph, int v){
	JRB vnode = jrb_find_int(graph->vertices,v);
	if(vnode==NULL) {
		return NULL;
    }
	detailVertex iver = (detailVertex) vnode->val.v;
	return  iver->linethong;
}

void addLienthong( Graph graph, int v1, int v2){
	JRB vnode = jrb_find_int(graph->vertices,v1);
	JRB LTnode;
	if(vnode==NULL) {
		return;
    }
	detailVertex iver = (detailVertex) vnode->val.v;
	LTnode = iver->linethong;
	jrb_insert_int( LTnode, v2, JNULL);
}

void printfLTManh( Graph LT, Graph g, int id){
    JRB enode = LT->vertices;
    JRB lst, tmp, LTManh;
    int key;

    LTManh = getLienthong( LT, id);
    jrb_traverse( tmp, LTManh){
        key = jval_i( tmp->key);
        printf("%s\t", getVertex( g, key));
    }
    printf("\n");
}

Graph revGraph( Graph g, int size){
    JRB root, lst;
    Graph h = createGraph();
    int *out = calloc( size, sizeof( int));
    int num;
    int id, i;

    root = g->vertices;
    jrb_traverse( lst, root){
        id = jval_i( lst->key);
        addVertex( h, id, getVertex( g, id));
    }

    root = g->vertices;
    jrb_traverse( lst, root){
        id = jval_i( lst->key);
        num = outdegree( g, id, out);
        for( i = 0; i < num; i++){
            addEdge( h, out[i], id, getEdgeValue( g, id, out[i]));
        }
    }

    return h;
}

//Dijkstra
Dllist extractMin( Dllist priQ){
    Dllist result = dll_first( priQ);
    Dllist node;
    ADT tmp;
    double min = INFINITIVE_VALUE;
    dll_traverse( node, priQ){
        tmp = (ADT)(node->val).v;
        if( tmp->wei < min){
            result = node;
            min = tmp->wei;
        }
    }
    return result;
}

double Dijkstra(Graph graph, int s, int t, int *path, int *length, int size){
    ADT tmp;
    Dllist priQueue = new_dllist();
    Dllist node1, node2;
    JRB now, tree, lst, root;
    int i, id1, id2, id3;
    double wei = 0;
    double wei_line;
    
    root = graph->vertices;
    jrb_traverse( lst, root){
        i = jval_i( lst->key);
        setParentVertex( graph, i , -1);
        setWeiVertex( graph, i, INFINITIVE_VALUE);
    }
    
    setWeiVertex( graph, s, 0);
    root = graph->vertices;
    jrb_traverse( lst, root){
        i = jval_i( lst->key);
        tmp = (ADT)malloc( sizeof(ADT*));
        tmp->id = i;
        tmp->wei = getWeiVertex( graph, i);
        dll_insert_b( priQueue, new_jval_v( tmp));
    }

    
    while( !dll_empty( priQueue)){
        node1 = extractMin( priQueue);
        tmp = (ADT)(node1->val).v;
        wei_line = tmp->wei;
        dll_delete_node( node1);
        id1 = tmp->id;
        now = jrb_find_int( graph->edges, id1);
        tree = (JRB)(now->val).v;
        //relaxing
        jrb_traverse( lst, tree){
            id2 = jval_i( lst->key);
            if( getWeiVertex( graph, id2) > wei_line + getEdgeValue(graph, id1, id2)){
                setWeiVertex( graph, id2, wei_line + getEdgeValue(graph, id1, id2));
                setParentVertex( graph, id2, id1);
                dll_traverse( node2, priQueue){
                    tmp = (ADT)(node2->val).v;
                    if( tmp->id == id2 ){
                        tmp->wei = getWeiVertex( graph, id2);
                    }
                }
            }
        }
    }

    free_dllist( priQueue);
    id3 = t;
    *length = 0;
    while(1){
        path[ *length] = id3;
        id3 = getParentVertex( graph, id3);
        *length = *length + 1;
        if( id3 == s){
            path[ *length] = id3;
            *length = *length + 1;
            return getWeiVertex( graph, t);
        }
        if( id3 == -1) return INFINITIVE_VALUE;
    }

    
}

double getWeiVertex(Graph graph, int id){
	JRB vnode=jrb_find_int(graph->vertices,id);
	
	if(vnode==NULL) return INFINITIVE_VALUE + 1;
	detailVertex iver = (detailVertex) vnode->val.v;
	
	return iver->wei;
}

int getParentVertex(Graph graph, int id){
	JRB vnode=jrb_find_int(graph->vertices,id);
	
	if(vnode==NULL) return -1;
	detailVertex iver = (detailVertex) vnode->val.v;
	
	return iver->parent;
}

void setWeiVertex( Graph graph, int id, double wei){
	JRB vnode=jrb_find_int(graph->vertices,id);
	
	if(vnode==NULL){
		printf("Vertex not found!!! ( set wei)\n");
		exit(1);
	}
	detailVertex iver = (detailVertex) vnode->val.v;
	iver->wei = wei;
}

void setParentVertex( Graph graph, int id, int parent){
	JRB vnode=jrb_find_int(graph->vertices,id);
	
	if(vnode==NULL){
		printf("Vertex not found!!! ( set parent)\n");
		exit(1);
	}
	detailVertex iver = (detailVertex) vnode->val.v;
	iver->parent = parent;
}

//Coloring
void setColorVertex( Graph graph, int id, int color){
    JRB vnode=jrb_find_int(graph->vertices,id);
	
	if(vnode==NULL){
		printf("Vertex not found!!! ( set wei)\n");
		exit(1);
	}
	detailVertex iver = (detailVertex) vnode->val.v;
	iver->color = color;
}

int getColorVertex(Graph graph, int id){
	JRB vnode=jrb_find_int(graph->vertices,id);
	
	if(vnode==NULL) return -1;
	detailVertex iver = (detailVertex) vnode->val.v;
	
	return iver->color;
}

void coloring( Graph graph, int size){
    int *out = calloc( size, sizeof( int));
    int num, id, color1, color2, i;
    JRB node, lst;

    node = graph->vertices;
    jrb_traverse( lst, node){
        id = jval_i( lst->key);
        num = outdegree( graph, id, out);
        for( i = 0; i < num; i++){
            color1 = getColorVertex( graph, id);
            color2 = getColorVertex( graph, out[i]);
            if( id > out[i] && color1 == color2){
                color1 = color1 + 1;
                setColorVertex( graph, id, color1);
                i = -1;
            }
        }
    }
    free( out);
}

void RGB( int i, char *s){
    switch ( i)
    {
    case 0:
        strcpy( s, "White");
        break;
    case 1:
        strcpy( s, "Yellow");
        break;
    case 2:
        strcpy( s, "Blue");
        break;
    case 3:
        strcpy( s, "Red");
        break;
    case 4:
        strcpy( s, "Green");
        break;
    case 5:
        strcpy( s, "Black");
        break;
    case 6:
        strcpy( s, "Teal");
        break;
    case 7:
        strcpy( s, "Brown");
        break;
    case 8:
        strcpy( s, "Azure");
        break;
    case 9:
        strcpy( s, "Ivory");
        break;
    case 10:
        strcpy( s, "Silver");
        break;
    case 11:
        strcpy( s, "Purple");
        break;
    case 12:
        strcpy( s, "Navyblue");
        break;
    case 13:
        strcpy( s, "Peagreen");
        break;
    case 14:
        strcpy( s, "Gray");
        break;
    case 15:
        strcpy( s, "Orange");
        break;
    case 16:
        strcpy( s, "Maroon");
        break;
    case 17:
        strcpy( s, "Charcoal");
        break;
    case 18:
        strcpy( s, "Aquamarine");
        break;
    case 19:
        strcpy( s, "Coral");
        break;
    case 20:
        strcpy( s, "Fuchsia");
        break;
    case 21:
        strcpy( s, "Wheat");
        break;
    case 22:
        strcpy( s, "Lime");
        break;
    case 23:
        strcpy( s, "Crimson");
        break;
    case 24:
        strcpy( s, "Khaki");
        break;
    case 25:
        strcpy( s, "Hot pink");
        break;
    case 26:
        strcpy( s, "Magenta");
        break;
    case 27:
        strcpy( s, "Golden");
        break;
    case 28:
        strcpy( s, "Plum");
        break;
    case 29:
        strcpy( s, "Olive");
        break;
    case 30:
        strcpy( s, "Cyan");
        break;
    case 31:
        strcpy( s, "lightbrown");
        break;
    case 32:
        strcpy( s, "lightgreen");
        break;
    case 33:
        strcpy( s, "lightblue");
        break;
    case 34:
        strcpy( s, "darkbrown");
        break;
    case 35:
        strcpy( s, "darkgreen");
        break;
    case 36:
        strcpy( s, "darkblue");
        break;
    
    }
}

void printf_dot( Graph graph, int size, char name[]){
    FILE *fp;
    int *out = calloc( size, sizeof( int));
    int num, id, i;
    JRB node, lst;
    char s[20];
    fp = fopen( name, "w");
    if( fp == NULL){
        printf("Error\n");
        exit(1);
    }

    fprintf( fp, "graph dothi\n{\n");

    node = graph->vertices;
    jrb_traverse( lst, node){
        id = jval_i( lst->key);
        RGB( getColorVertex( graph, id), s);
        fprintf( fp, "\t%d [fillcolor=%s, style=filled];\n", id, s);
    }

    node = graph->vertices;
    jrb_traverse( lst, node){
        id = jval_i( lst->key);
        num = outdegree( graph, id, out);
        for( i = 0; i < num; i++){
            if( out[i] > id){
                fprintf( fp, "\t%d -- %d\n", id, out[i]);
            }
        }
    }
    fprintf( fp, "}");
    fclose( fp);
    free( out);
}

void dropGraph(Graph graph)
{
	JRB node,innode;
	detailVertex detailnode;
	
	jrb_traverse(node,graph->edges){
		jrb_free_tree((JRB)jval_v(node->val));
	}
	jrb_free_tree(graph->edges);
	
	jrb_traverse(node,graph->vertices){
		detailnode = (detailVertex) node->val.v;
		free(detailnode->name);
		jrb_free_tree((JRB)detailnode->indegreeTree);
	}
	jrb_free_tree(graph->vertices);
	
	free(graph);
}

Graph MST( Graph graph, int size){
    Graph gMST = createGraph();
    ADT tmp;
    JRB root;
    Dllist priQueue = new_dllist();
    Dllist node1, node2;
    JRB now, tree, lst;
    int i, id1, id2, id3;
    int s;
    double wei = 0;
    
    root = graph->vertices;
    jrb_traverse( lst, root){
        i = jval_i( lst->key);
        setParentVertex( graph, i , -1);
        setWeiVertex( graph, i, INFINITIVE_VALUE);
        addVertex( gMST, i, "");
    }

    root = graph->vertices;
    jrb_traverse( lst, root){
        i = jval_i( lst->key);
        setWeiVertex( graph, i, 0);
        break;
    }
    

    root = graph->vertices;
    jrb_traverse( lst, root){
        i = jval_i( lst->key);
        tmp = (ADT)malloc( sizeof(ADT*));
        tmp->id = i;
        tmp->wei = getWeiVertex( graph, i);
        dll_insert_b( priQueue, new_jval_v( tmp));
    }
    
    while( !dll_empty( priQueue)){
        node1 = extractMin( priQueue);
        tmp = (ADT)(node1->val).v;
        dll_delete_node( node1);
        id1 = tmp->id;
        now = jrb_find_int( graph->edges, id1);
        tree = (JRB)(now->val).v;
        //relaxing
        jrb_traverse( lst, tree){
            id2 = jval_i( lst->key);
            if( getWeiVertex( graph, id2) > getEdgeValue(graph, id1, id2)){
                setWeiVertex( graph, id2, getEdgeValue(graph, id1, id2));
                setParentVertex( graph, id2, id1);
                dll_traverse( node2, priQueue){
                    tmp = (ADT)(node2->val).v;
                    if( tmp->id == id2 ){
                        tmp->wei = getWeiVertex( graph, id2);
                        break;
                    }
                }
            }
        }
    }
    
    free_dllist( priQueue);

    root = graph->vertices;
    jrb_traverse( lst, root){
        id1 = jval_i( lst->key);
        id2 = getParentVertex( graph, id1);
        //printf("%d %d\n", id1, id2);
        if( id2 != -1){
            addEdge( gMST, id1, id2, 0);
            addEdge( gMST, id2, id1, 0);
        }
    }

    return gMST;
}