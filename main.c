
// you will receive two file names from the command line
//bir tane product listi yap
//bir tane manufacturer listi yap
//receives the manufacturer (e.g, FIAT) (the manufacturer file) and it prints-out all products (and
//  relative prices) produced by it.

//* optional: in case a third parameter is provided, consider it a “budget”
//  value and print a (sorted) list of products that can be bought with that

//bir tane manufacturer listi: name id next(pointer to the next node) product_ptr(pointer to the manufacturer node);
//bir tane product listi: id name_product price
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// productları producera bağlayacağız
typedef struct product
{
    char *id;
    char *name;
    int price;
    struct product *next;
} product;

typedef struct manufacturer{
    char *id;
    char *name;
    struct manufacturer *next;
    product *product_ptr;
} manufacturer;



product *make_products(char *input);
manufacturer *manufacture_list(char *input, product *head_p);
manufacturer *new_manufacture();
product *new_product();
void display_by_name(char *name, manufacturer *head);

manufacturer *search_link(manufacturer *m, product *head_p);
void free_manufacturers(manufacturer *head);
void free_products(product *head);



manufacturer *new_manufacturer()
{
    manufacturer *new;
    new = (manufacturer*)malloc(sizeof(manufacturer));
    new->id = NULL;
    new->name = NULL;
    new->next = NULL;
    new->product_ptr = NULL;
    return new;
}
product *new_product()
{
    product *new;
    new = (product*)malloc(sizeof(product));
    new->id = NULL;
    new->name = NULL;
    new-> price = (int) NULL;
    new->next = NULL;

    return new;
}

int main(void)
{

    product *head_product = new_product(); //product listi
    if (head_product == NULL)
    {
        fprintf(stderr, "memory allocation error");
        exit(EXIT_FAILURE);
    }
    char file_products[100];
    printf("please introduce the file name for products: ");
    scanf("%s", file_products);
    head_product = make_products(file_products);


    manufacturer *head_manufacturer = new_manufacturer(); //manufacture listi
    if (head_manufacturer == NULL)
    {
        fprintf(stderr, "memory allocation error");
        exit(EXIT_FAILURE);
    }
    char file_manufacturer[100];
    printf("please introduce the file name for producers: ");
    scanf("%s", file_manufacturer);
    head_manufacturer = manufacture_list(file_manufacturer, head_product);


    display_by_name("MERCEDES", head_manufacturer);
    free_products(head_product);
    free_manufacturers(head_manufacturer);
}


//CREATING THE PRODUCT'S LIST
product *make_products(char *input)
{
    product *head, *temp, *new_node;
    head = NULL;
    temp = NULL;
    new_node = NULL;

    FILE *fp;
    fp = fopen(input, "r" );
    if(fp == NULL)
    {
        fprintf(stderr, "file opening error");
        exit(EXIT_FAILURE);
    }
    char id[10];
    char name[100];
    int price;

    while(fscanf(fp, "%s %s %d", id, name, &price)==3)
    {
        new_node = new_product();
        new_node->id = (char*)malloc((strlen(id)*sizeof(char))+1);
        if(new_node->id==NULL)
        {
            fprintf(stderr, "memory allocation error");
            exit(EXIT_FAILURE);
        }
        new_node->name = (char*)malloc((strlen(name)*sizeof(char))+1);
        if(new_node->name==NULL)
        {
            fprintf(stderr, "memory allocation error");
            exit(EXIT_FAILURE);
        }

        strcpy(new_node->id, id);
        strcpy(new_node->name, name);
        new_node->price = price;
        new_node -> next = NULL;

        if(head==NULL)
        {
            head = new_node;
            temp = head;
        }
        else {
            temp->next = new_node;
            temp = temp->next;
        }

    }
    return head;
}



//CREATING THE PRODUCER'S LIST
manufacturer *manufacture_list(char *input, product *head_p) {

    char id[10];
    char name[10];
    FILE *fp;
    fp = fopen(input, "r");
    if (fp == NULL) {
        fprintf(stderr, "file opening error");
        exit(EXIT_FAILURE);
    }

    manufacturer *head = NULL;
    manufacturer *temp = NULL;
    manufacturer *new_node = NULL;


    while (fscanf(fp, "%s %s", name, id) == 2)
    {
        new_node=new_manufacturer();
        new_node->id = (char *) malloc((strlen(id) * sizeof(char)) + 1);
        if (new_node->id == NULL) {
            fprintf(stderr, "memory allocation error");
            exit(EXIT_FAILURE);
        }

        new_node->name = (char *) malloc((strlen(name) * sizeof(char)) + 1);
        if (new_node->name == NULL) {
            fprintf(stderr, "memory allocation error");
            exit(EXIT_FAILURE);
        }

        strcpy(new_node->name, name);
        strcpy(new_node->id, id);
        new_node->next = NULL;
        new_node->product_ptr = NULL;

        if (head == NULL)
        {
            head = new_node;
            temp = head;
        }
        else {
            temp->next = new_node;
            temp = temp->next;
        }
    }
    search_link(head,head_p);
    return head;
}



//CREATING THE LIST OF LISTS
manufacturer *search_link(manufacturer *m, product *head_p) {
    product *p = head_p;
    if (m == NULL || head_p == NULL) {
        return head_p;
    }

    while (p != NULL)
    {
        if (strcmp(m->id, p->id) == 0) {
            product *newp = new_product();
            newp->name= p->name;
            newp->id = p->id;
            newp->price = p->price;
            newp->next= NULL;
            if (m->product_ptr == NULL) {
                m->product_ptr = newp;
            } else {
                product *temp = m->product_ptr;
                while (temp->next != NULL) {
                    temp = temp->next;
                }
                temp->next = newp;
            }
        }
        p = p->next;
    }
    search_link(m->next, head_p);
}



//DISPLAY THE PRODUCTS OF THE GIVEN PRODUCER
void display_by_name(char *name, manufacturer *head)
{
    manufacturer *p = head;
    while(p!= NULL && strcmp(p->name, name)!=0)
    {
        p=p->next;
    }
    product *temp= p->product_ptr;
    while(temp!=NULL)
    {
        printf("%s", temp->name);
        printf("\n");
        temp=temp->next;
    }

}



//FREE
void free_products(product *head) {
    product *current = head;
    while (current != NULL)
    {
        product *temp = current;
        current = current->next;

        free(temp);
    }
}

void free_manufacturers(manufacturer *head) {
    manufacturer *current = head;
    while (current != NULL) {
        manufacturer *temp = current;
        current = current->next;
        free(temp);
    }
}
