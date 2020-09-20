#include<stdio.h>
#include<stdlib.h>
struct biheap{
    int key,degree;
    struct biheap *p,*child,*sibling;
};
struct list{
    struct biheap *node;
    struct list *next;
    struct list *prev;
};
struct biheap *makenode(int k){
    struct biheap *node;
    node = (struct biheap*)malloc(sizeof(struct biheap));
    node->key = k;
    node->degree = 0;
    node->p = node->child = node->sibling = NULL;
    return node;
}

struct list *makeheap(struct list *heap){
    struct list *l1 = heap;
    struct list *l2 = heap->next;
    while(l2){
        if(l1->node->degree==l2->node->degree){
            if(l1->node->key > l2->node->key){
                l1->node->sibling = l2->node->child;
                l2->node->child = l1->node;
                l1->node->p = l2->node;
                l2->node->degree++;
            }else{
                l2->node->sibling = l1->node->child;
                l1->node->child = l2->node;
                l2->node->p = l1->node;
                l1->node->degree++;
                l2->node = l1->node;
            }
        }else
            break;
        heap = l2;
        l1 = l1->next;
        l2 = l2->next;
    }
    heap->prev = NULL;
    return heap;
}
struct list *insert(struct list *heap,int k){
    struct biheap *node = makenode(k);
    struct list *l;
    l = (struct list*)malloc(sizeof(struct list));
    l->node = node;
    l->next=NULL;
    l->prev=NULL;
    if(heap==NULL){
        return l;
    }
    l->next = heap;
    heap->prev = l;
    heap = l;
    return makeheap(heap);
}
struct list *mergel(struct list *l1 , struct list *l2){
    struct list *newl;
    newl=NULL;
    struct list *p = l1;
    struct list *q = l2;
    while(p && q){
        struct list *l;
        l = (struct list*)malloc(sizeof(struct list));
        if(p->node->degree > q->node->degree){
            l->node = q->node;
            q=q->next;
        }else{
            l->node = p->node;
            p=p->next;
        }
        l->next=NULL;
        l->prev=NULL;
        if(newl==NULL)
            newl = l;
        else{
            newl->next = l;
            l->prev = newl;
            newl = l;
        }
    }
    while(p){
        struct list *l;
        l = (struct list*)malloc(sizeof(struct list));
        l->node = p->node;
        l->next=NULL;
        l->prev=NULL;
        p=p->next;
        if(newl==NULL)
            newl = l;
        else{
            newl->next = l;
            l->prev = newl;
            newl = l;
        }
    }
    while(q){
        struct list *l;
        l = (struct list*)malloc(sizeof(struct list));
        l->node = q->node;
        l->next=NULL;
        l->prev=NULL;
        q=q->next;
        if(newl==NULL)
            newl = l;
        else{
            newl->next = l;
            l->prev = newl;
            newl = l;
        }
    }
    while(newl->prev)
        newl = newl->prev;
    return newl;
}
struct list *Union(struct list *l1 , struct list *l2){
    if(l1==NULL)
        return l2;
    if(l2==NULL)
        return l1;
    struct list *heap;
    heap = mergel(l1,l2);
    struct list *p = heap;
    struct list *q = heap->next;
    while(q){
        if(p->node->degree==q->node->degree){
            if(q->next && q->node->degree==q->next->node->degree){
                p = p->next;
                q = q->next;
                continue;
            }
            if(p->node->key > q->node->key){
                p->node->sibling = q->node->child;
                q->node->child = p->node;
                p->node->p = q->node;
                q->node->degree++;
            }else{
                q->node->sibling = p->node->child;
                p->node->child = q->node;
                q->node->p = p->node;
                p->node->degree++;
                q->node = p->node;
            }
            q->prev = p->prev;
            if(p->prev){
                (p->prev)->next = q;
                p = p->next;
            }
            else{
                p = p->next;
                heap = p;
            }
            q = q->next;
            continue;
        }
        p = p->next;
        q = q->next;
    }
    return heap;
}

struct biheap *getMin(struct list *l){
    if(l==NULL)
        return NULL;
    struct biheap *mn = l->node;
    struct list *p = l;
    while(p){
        if(p->node->key < mn->key)
            mn = p->node;
        p = p->next;
    }
    return mn;
}
void printNode(struct biheap *n){
    if(n==NULL)
        return;
    printf("%d ",n->key);
    printf("\nsibling  : ");
    printNode(n->sibling);
    printf("\nchild  : ");
    printNode(n->child);
}
void display(struct list *heap){
    if(heap==NULL)
        return;
    struct list *l = heap;
    while(l){
        printf("\nroot : ");
        printNode(l->node);
        l = l->next;
    }
}
struct list *deleteminNode(struct list *heap){
    struct list *l = heap;
    struct biheap *n = getMin(heap);
    while(l){
        if(l->node->key==n->key)
            break;
        l = l->next;
    }
    if(l->prev && l->next){
        l->prev->next = l->next;
        l->next->prev = l->prev;
    }else if(l->prev){
        l->prev->next = NULL;
    }else{
        heap = l->next;
        l->next->prev = NULL;
    }
    struct list *l2 = NULL;
    n = n->child;
    while(n){
        struct list *temp;
        temp = (struct list*)malloc(sizeof(struct list));
        temp->node = n;
        n = n->sibling;
        temp->node->sibling=NULL;
        temp->node->p = NULL;
        temp->next = NULL;temp->prev=NULL;
        if(l2==NULL){
            l2 = temp;
        }else{
            l2->prev = temp;
            temp->next= l2;
            l2 = temp;
        }
    }
    return Union(heap,l2);
}
int main(){
    struct list *heap;
    struct list *heap2;
    heap=NULL;
    heap2=NULL;
    int k,n;
    printf("Binomial heap **** \n\n\n");
    printf("\n1.Create heap \n2.Insert \n3.Union \n4.GetMin \n5.DeleteMin \n6.display \n7.Exit \n");
    while(1){
        int c;
        printf("\nEnter Choice : ");
        scanf("%d",&c);
        switch(c){
        case 1:
            printf("Enter key : ");scanf("%d",&k);
            heap = insert(heap,k);
            printf("Heap created successfully! \n");
            break;
        case 2:
            if(heap==NULL)
                printf("First create heap...\n");
            else{
                printf("Enter key : ");scanf("%d",&k);
                heap = insert(heap,k);
                printf("key inserted successfully! \n");
            }
            break;
        case 3:
            if(heap==NULL)
                printf("First create heap...\n");
            else{
                printf("No of element in 2nd heap : ");
                scanf("%d",&n);
                printf("Enter elements : ");
                for(int i=0;i<n;i++){
                    scanf("%d",&k);
                    heap2 = insert(heap2,k);
                }
                heap = Union(heap,heap2);
                heap2=NULL;
                printf("Union successfully completed \n");
            }
            break;
        case 4:
            if(heap==NULL)
                printf("First create heap...\n");
            else
                printf("Minimum key = %d\n",getMin(heap)->key);
            break;
        case 5:
            if(heap==NULL)
                printf("First create heap...\n");
            else{
               heap = deleteminNode(heap);
               printf("Deletion successfully completed \n");
            }
            break;
        case 6:
            if(heap==NULL)
                printf("First create heap...\n");
            else
                display(heap);
            break;
        case 7:
            exit(0);
        default:
            printf("Invalid Choice !!!\n");
        }
    }
    return 0;
}
