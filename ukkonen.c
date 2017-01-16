#include<stdio.h>
#include<stdlib.h>



typedef struct Node Node;
struct Node {
   int  first;
   int  *last;
   int nodeCount;
   Node **list;
   Node *suffixLink;
}; 

Node *activeNode;
int activeEdge=-1;
int activeLength=0;
int remaining=0;
long done;

Node *root=NULL;
Node *lastNode=NULL;
int jLast=0;
long end=0;

char *str;
FILE *f;

int *rootEnd = NULL;
int *splitEnd = NULL;


Node *newNode(int first, int *last)
{
    Node *node = (Node*) malloc(sizeof(Node));
    int i;
    node->list = (Node**) malloc((2)*sizeof(Node*));;
    node->nodeCount=0;
 
    node->suffixLink = root;
    node->first = first;
    node->last = last;
 
    return node;
}

int edgeLength(Node *n) {
    return *(n->last) - (n->first) + 1;
}

int walkDown(Node *currNode)
{
    
    if (activeLength >= edgeLength(currNode))
    {
        activeEdge += edgeLength(currNode);
        activeLength -= edgeLength(currNode);
        activeNode = currNode; 
        
        return 1;
    }
    return 0;
}

int findEdge(Node *n,int edge){
	int i;
	for (i=1;i<=n->nodeCount;i++){
		if(*(str+n->list[i]->first)==*(str+edge))
			return i;
	}
	return -1;
}
int findEdgeinSample(Node *n,char edge){
	int i;
	for (i=1;i<=n->nodeCount;i++){
		if(*(str+n->list[i]->first)==edge)
			return i;
	}
	return -1;
}
void addEdge(Node *parent, Node *child){
	
	parent->nodeCount++;
	parent->list=(Node**) realloc(parent->list,(parent->nodeCount+2)*sizeof(Node*));
	parent->list[parent->nodeCount]=child;
			
}

void extendSuffixTree()
{
	
    
    int positionNext;
    remaining++;
    lastNode = NULL;
    
    while(remaining > 0) {
 		
        if (activeLength == 0)
            activeEdge = end; 
 
        
        positionNext=findEdge(activeNode, activeEdge);
        if (positionNext == -1)
        {
           
            Node *temp=newNode(end, &end);
            addEdge(activeNode, temp);
            
 
           
            if (lastNode != NULL)
            {
                lastNode->suffixLink = activeNode;
                lastNode = NULL;
            }
        }
       
        
        else
        {
            
            Node *next = activeNode->list[positionNext];
            if (walkDown(next))
            {
               
                continue;
            }
            
            if (*(str+next->first+activeLength) == *(str+end))
            {
            
                if(lastNode != NULL && activeNode != root)
                {
                    lastNode->suffixLink = activeNode;
                    lastNode = NULL;
                }
 
                
                activeLength++;
                
                break;
            }
 
        
            
            splitEnd = (int*) malloc(sizeof(int));
            *splitEnd = next->first + activeLength -1;
 
            
            Node *split = newNode(next->first, splitEnd);
            activeNode->list[positionNext]=split;
            
            addEdge(split, newNode(end, &end));
            
            next->first += activeLength;
            addEdge(split, next);
            
            
            if (lastNode != NULL)
            {
            
                lastNode->suffixLink = split;
            }
 
          
            lastNode = split;
        }
 		
       
        remaining--;
        
        if (activeNode == root && activeLength > 0) 
        {
            activeLength--;
            activeEdge = end - remaining + 1;
        }
        else if (activeNode != root) 
        {
            activeNode = activeNode->suffixLink;
        }
        
    }


}


void buildSuffixTree()
{
    
    int i;
    rootEnd = (int*) malloc(sizeof(int));
    *rootEnd = - 1;
    root = newNode(-1, rootEnd);
 
    activeNode = root; 
    
    do{
    	end++;
		if(done>100)
    	if(end%(done/100)==0 )
    		printf("%d percent done\n",end/(done/100));
    	
    	
    	extendSuffixTree();
    	
    	
    	
	}while(*(str+end)!='$');
        	printf("\n");
   
    
    
}
void print(int depth, Node *current){
	int i;
	for(i=0;i<depth;i++){
		printf("-");
	}
	
	for(i=current->first;i<=*current->last;i++){
		printf("%c",*(str+i));
	}
	printf("\n");
	
	for(i=1;i<=current->nodeCount;i++){
		print(depth+1,current->list[i]);
	}
	
	
}
void nicePrint(int depth, Node *current){
	int i;
	int count =0;
	for(i=0;i<depth;i++){
		printf("-");
	}
	
	for(i=current->first;i<=*current->last;i++){
		printf("%c",*(str+i));
		count++;
		
	}
	
	printf("\n");
	
	for(i=1;i<=current->nodeCount;i++){
		nicePrint(depth+count,current->list[i]);
	}
	return;
	
	
}

void nicePrintToFile(int depth, Node *current){
	
	int i;
	int count =0;
	for(i=0;i<depth;i++){
		putc('-',f);
	
	}
	
	for(i=current->first;i<=*current->last;i++){
		char c =*(str+i);
		count++;
		putc(c,f);
	}
	putc('\n',f);
	
	for(i=1;i<=current->nodeCount;i++){
		nicePrintToFile(depth+count,current->list[i]);
	}
	
	
}


void printToFile(int depth, Node *current){
	
	int i;
	
	for(i=0;i<depth;i++){
		putc('-',f);
	
	}
	
	for(i=current->first;i<=*current->last;i++){
		char c =*(str+i);
		
		putc(c,f);
	}
	putc('\n',f);
	
	for(i=1;i<=current->nodeCount;i++){
		nicePrintToFile(depth+1,current->list[i]);
	}
	
	
}

int main(){
	
	FILE *fp;
	fp = fopen("in.txt", "r");
	char character;

	fseek (fp, 0, SEEK_END);
 	 end = ftell (fp);
 	 fseek (fp, 0, SEEK_SET);
	  str = malloc (end);
	
	 character= fgetc(fp);
	 	end=0;
		while(character!=EOF){
	
		if(character!='\n'){
				end++;

	*(str+end)=character;
		}

	character= fgetc(fp);
}
	
	
	str=realloc(str, (end+1)*sizeof(char));
	*(str+end+1)='$';
	done=end;
	
	end=0;
	
	buildSuffixTree();
	//print(0,root);
	//nicePrint(0,root);
	f = fopen("out.txt", "w+");
	//printToFile(0,root);
	nicePrintToFile(0,root);
	fclose(f);
	fclose(fp);
	
	
	
	int endTest=0;
	char*strTest;
		FILE *fpTest;
	fpTest = fopen("test.txt", "r");
	if (fpTest) {
	char characterTest;

	fseek (fpTest, 0, SEEK_END);
 	 endTest = ftell (fpTest);
 	 fseek (fpTest, 0, SEEK_SET);
	  strTest = malloc (endTest);
	
	 characterTest= fgetc(fpTest);
	 	endTest=0;
		while(characterTest!=EOF){
		
		if(characterTest!='\n'){
				endTest++;
	
	*(strTest+endTest)=characterTest;
		}

	characterTest= fgetc(fp);
}
	
	
	strTest=realloc(strTest, (endTest+1)*sizeof(char));
	*(strTest+endTest)='$';
	
	if(findSample(root,strTest)==1) {
		printf("sample was found");
	}
	else if(strTest[0]!='$'){
		printf("sample was not found");
	}
	
	
	
	}	
	
	
}

int findSample(Node* node, char* sample){
	int i=1;
	int lenghtInNode=0;
	Node *current;
	current=node;
	printf("\n");
	do{
	
		if(lenghtInNode==0){
			
			int path=findEdgeinSample(current,*(sample+i));
			
			if(path==-1){
				
				return -1;
			}
			else{
				current=current->list[path];
				lenghtInNode=edgeLength(current);
				continue;
			}
		}
		else{
			
			if(*(sample+i)!=*(str+*current->last-lenghtInNode+1)){
				return -1;
			}
			else{
				lenghtInNode--;
			}
		}
		i++;
	}while(*(sample+i)!='$');
	return 1;
}

