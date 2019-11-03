/* File: contact_manager.c
   Autor: Ricardo Andrade 93750
   Descricao: Contact management system */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOME 1024
#define EMAIL 512
#define MAX_INPUT 1600
#define key_name(a) (a->info->name)
#define key_domain(a) (a->domain)


typedef unsigned long big_int;

typedef struct Contact{
    char *name;
    char *email;
    char *phone;
} *p_contact;

typedef struct node {
    struct node *next, *previous;
    p_contact info;
} *link;

typedef struct Domain {
    char *domain;
    int count;
    struct Domain *next;
} *p_domain;

typedef struct {
    struct node *head, *last;
} list;


static big_int contacts_cap;     /* capacity of contacts table */
static big_int domains_cap;      /* capacity of domains table */
static big_int contacts_total;   /* number of saved contacts */
static big_int domains_total;    /* number of saved domains */
static p_domain head;


p_contact create_contact(link *contacts);
void free_contact(p_contact c);
void print_contact(p_contact c);
void command_a(list *l, p_domain *domains, link *contacts);
void command_l(list *l);
void command_p(link *contacts);
void command_e(p_domain *domains, link *contacts);
void command_r(list *l, p_domain *domains, link *contacts);
void command_c(p_domain *domains);
void command_x(list *l, link *contacts, p_domain *domains);
void error(char c);
list *init_list();
link *expand_contacts(link *contacts);
void add_last(list *l, link new);
link create_link(p_contact c);
void free_list(list *l);
void free_link(link node, list *l);
p_domain *init_domains(big_int M);
link *init_contacts(big_int M);
link remove_contact(link *contacts, char *name);
void insert_contact(link new, link *contacts);
link search_contact(link *contacts, char *v);
int hash(char *v, big_int M);
char *get_domain(char *email);
p_domain search_domain(p_domain *domains, char *v);
p_domain create_domain(char *domain);
void insert_domain(p_domain new, p_domain *domains);
p_domain *expand_domains(p_domain *domains);
void free_domains();
void read_line(char s[]);
int compare_pref(char *pref, char *string);
void command_P(list *l);
char *pertence(char *name);


int main() {
    char command;
    link *contacts = init_contacts(1031);  /* table of pointers to contacts */
    p_domain *domains = init_domains(1031); /* table of pointers to domains */
    list *l = init_list();

    while (1) {
        command = getchar();
        getchar();
        switch (command) {
            case 'a':
                command_a(l, domains, contacts);
                if (contacts_total > contacts_cap / 2)
                    contacts = expand_contacts(contacts); 
                if (domains_total > domains_cap / 2)
                    domains = expand_domains(domains);
                break;
        
            case 'l':
                command_l(l);
                break;

            case 'p':
                command_p(contacts);
                break;
        
            case 'r':
                command_r(l, domains, contacts);
                break;

            case 'e':
                command_e(domains, contacts);
                break;

            case 'c':
                command_c(domains);
                break;
            
            case 'P':
                command_P(l);
                break;

            case 'x':
                command_x(l, contacts, domains);
                return 0;
        }
    }

    return 0;
}

/* auxiliary function to print the error message associated to
the given error*/
void error(char c) {
    if (c == 'a') 
        printf("Nome existente.\n");

    else
        printf("Nome inexistente.\n");
    
}

/* auxiliary function to read the input given by the user */
void read_line(char s[]) {
    char c;
    int i = 0;

    while ((c = getchar()) != '\n') {
        s[i++] = c;
    }
    s[i] = '\0';
}


/* ------------------------------------------------------ */
/* command_a: adds a contact to the linked list and to    */
/*                     the hashtable                        */
/* ------------------------------------------------------ */
void command_a(list *l, p_domain *domains, link *contacts) {
    p_contact c = create_contact(contacts);

    /* if the contact can be created, continue*/
    char *domain;
    p_domain temp;

        /* creates a node for the contact to be inserted in
        the linked list and the name hashtable */
        link new = create_link(c);
        add_last(l, new);
        insert_contact(new, contacts);

        /* gets the domain of the email to be inserted in the 
        domain hashtable*/
        domain = get_domain(c->email);

        /* searches for the obtained domain in the existing domains.
        If it exists, adds to the count of that given domain*/
        if ((temp = search_domain(domains, domain)) != NULL) {
            temp->count++;
        }
        /* If it doesn't exist, creates that domain and inserts it into 
        the hashtable*/
        else {
            temp = create_domain(domain);
            insert_domain(temp, domains);
        }
}


/* ------------------------------------------------------ */
/* command_l: prints the contacts by order of insertion   */
/* ------------------------------------------------------ */
void command_l(list *l) {
    link t = l->head;
    char *temp;
    char *name;

    /* while the list still has a next contact, prints it*/
    while (t != NULL) {
        if ((temp = pertence(t->info->name)) != NULL) {
            name = t->info->name - temp;
            printf("%s (%s) %s %s\n", t->info->name, name, t->info->email, t->info->phone);
        }
        else
            print_contact(t->info);

        t = t->next;
    }
}


/* ------------------------------------------------------ */
/* command_p: searches for a certain contact, given its   */
/*                         name                           */
/* ------------------------------------------------------ */
void command_p(link *contacts) {
    char buffer[NOME];
    link temp;

    read_line(buffer);

    /* if the contact with the given name is found, prints its info*/
    if ((temp = search_contact(contacts, buffer)) != NULL) {
        print_contact(temp->info);
    }

    /* if the contact is not found, prints an error message using the 
    error() function */
    else {
        error('p');
    }
}


/* ------------------------------------------------------ */
/* command_e: changes the email of a certain contact to   */
/*                an inserted email                       */
/* ------------------------------------------------------ */
void command_e(p_domain *domains, link *contacts) {
    link temp;
    char buffer[NOME + EMAIL];
    char *name, *email;

    /* gets the name to search for and the new email*/
    read_line(buffer);
    name = strtok(buffer, " ");
    email = strtok(NULL, " ");

    /* if the contact with the given name is found, continue*/
    if ((temp = search_contact(contacts, name)) != NULL) {
        /* gets the domain associated with the given email, and decreases its count*/
        char *domain = get_domain(temp->info->email);
        p_domain aux = search_domain(domains, domain);
        aux->count--;
        free(temp->info->email);

        temp->info->email = malloc(sizeof(char) * (strlen(email) + 1));
        strcpy(temp->info->email, email);
        domain = get_domain(temp->info->email);

        /* if the domain already exists, increases its count */
        if ((aux = search_domain(domains, domain)) != NULL) {
            aux->count++;
        }
        /* if it doesn't already exist, creates it */
        else {
            aux = create_domain(domain);
            insert_domain(aux, domains);
        }

    }

    /* if the contact is not found, prints an error message via the 
    error() function */
    else
        error('e');

}

/* ------------------------------------------------------ */
/*           command_r: removes a certain contact         */
/* ------------------------------------------------------ */
void command_r(list *l, p_domain *domains, link *contacts) {
    char buffer[NOME];
    link temp;
    read_line(buffer);

    /* if the contact exists, removes it*/
    if ((temp = remove_contact(contacts, buffer)) != NULL) {
        /* gets the domain for the given contact */
        char *domain = get_domain(temp->info->email);
        p_domain aux;
        /* searches for the domain and then reduces its count */
        if ((aux = search_domain(domains, domain)) != NULL) {
            aux->count--;
        }
        free_link(temp, l);
    }

    /* if not, prints an error message indicating that the
    contact does not exist*/
    else
        error('r');
}

/* ------------------------------------------------------ */
/*   command_c: prints the ammount of emails in a given   */
/*                        domain                          */
/* ------------------------------------------------------ */
void command_c(p_domain *domains) {
    char buffer[511];
    p_domain temp;
    int cont = 0;

    /* searches for the given domain in the domains hashtable*/
    read_line(buffer);
    temp = search_domain(domains, buffer);

    /* if the domain exists, returns its count, if else 
    is zero, as initialized */
    if (temp != NULL) {
        cont = temp->count;
    }

    /* prints the number of domains */
    printf("%s:%d\n", buffer, cont);
}


/* ------------------------------------------------------ */
/*   command_x: frees the linked list and the hashtables    */
/* ------------------------------------------------------ */
void command_x(list *l, link *contacts, p_domain *domains) {
    free_list(l);
    free(contacts);
    free_domains();
    free(domains);
}
void print_contact(p_contact c) {
    printf("%s %s %s\n", c->name, c->email, c->phone);
}

/* ------------------------------------------------------ */
/*   create_contact: creates a contact with input name,   */
/*                email and phone number                  */
/* ------------------------------------------------------ */
p_contact create_contact(link *contacts) {
    char buffer[MAX_INPUT];
    p_contact c = NULL;
    char *token;

    read_line(buffer);


    /* if it doesn't find any contacts with the given name,
    creates one (allocates memory for each component and then
    adds the input info */
    c = malloc(sizeof(struct Contact));
    /* searches for a contact with the same name, and if found
    returns a NULL pointer */
    token = strtok(buffer, " ");
    if (search_contact(contacts, token) != NULL) {
        c->name = malloc(sizeof(char) * (strlen(token) + 1 + 2));
        strcpy(c->name, token);
        strcat(c->name, "_1");
        while (search_contact(contacts, c->name) != NULL) {
            c->name = realloc(c->name, (strlen(c->name) + 2) * sizeof(char));
            strcat(c->name, "_1");
        }
    }

    else {
        c->name = malloc(sizeof(char) * (strlen(token) + 1));
        strcpy(c->name, token); }

    token = strtok(NULL, " ");
    c->email = malloc(sizeof(char) * (strlen(token) + 1));
    strcpy(c->email, token);

    token = strtok(NULL, " ");
    c->phone = malloc(sizeof(char) * (strlen(token) + 1));
    strcpy(c->phone, token);

    /* returns the contact */
    return c;
}


void free_contact(p_contact c) {
    free(c->email);
    free(c->name);
    free(c->phone);
    free(c);
}

/* ------------------------------------------------------ */
/*          init_list: initializes the linked list        */
/* ------------------------------------------------------ */
list *init_list() {
    /* allocates space for a list type and initializes
    the head and the last to a NULL value */
    list *l = malloc(sizeof(list));
    l->head = NULL;
    l->last = NULL;

    return l;
}

/* ------------------------------------------------------ */
/*      expand_contacts: expands the contacts hashtable     */
/* ------------------------------------------------------ */
link *expand_contacts(link *contacts) {
    big_int i;
    link *t = contacts;
    link *new_hash = init_contacts(contacts_cap * 2);

    for (i = 0; i < contacts_cap / 2; i++)
        if (t[i] != NULL)
            insert_contact(t[i], new_hash);

    free(t);
    return new_hash;
}

/* ------------------------------------------------------ */
/*           add_last: adds to the end of the list        */
/* ------------------------------------------------------ */
void add_last(list *l, link new) {

    /* if there is no head, there aren't any
    elements, therefore adds to head of list */
    if (l->head == NULL) {
        l->head = new;
        new->previous = NULL;
        new->next = NULL;
    }

    /* if there isn't any last, the new element
    will be the last of the list */
    else if(l->last == NULL) {
        l->last = new;
        l->head->next = l->last;
        l->last->previous = l->head;
        l->last->next = NULL;
    }
    
    /* if neither, adds to the end of list*/
    else {
        l->last->next = new;
        new->previous = l->last;
        new->next = NULL;
        l->last = new;
    }
}

/* ------------------------------------------------------ */
/*              create_link: allocates a new node         */
/*            and adds its info the allocated node        */
/* ------------------------------------------------------ */
link create_link(p_contact c) {
    link new = malloc(sizeof(struct node));
    new->info = c;

    return new;
}

/* ------------------------------------------------------ */
/*          free_list: frees the whole linked list        */
/* ------------------------------------------------------ */
void free_list(list *l) {
    link aux = l->head;
    /* while there is still a next node, frees the node */
    while (aux != NULL) {
        aux = l->head->next;
        free_link(l->head, l);
    }
    free(l);
}

/* ------------------------------------------------------ */
/*               free_link: frees a given node            */
/* ------------------------------------------------------ */
void free_link(link node, list *l) {
    link previous = node->previous;
    link next = node->next;

    /* if the node is the head */
    if (node == l->head) {
        /* the head is changed into the next node and then
        frees the head contact and node */
        l->head = node->next;
        free_contact(node->info);
        free(node);
        /* if there are no contacts, initializes the last
        and head nodes to NULL */
        if (contacts_total == 0) {
            l->last = NULL;
            l->head = NULL;
        }
        return;
    }

    /* else if the node is the last, the last node will be the one
    previous to the node. Then frees the last contact and node */
    else if (node == l->last) {
        l->last = node->previous;
        l->last->next = NULL;
        free_contact(node->info);
        free(node);
    }
    /* if none of these conditions are satisfied, frees current node
    and changes previous nodes' next pointer to the one that was next
    to the node to free*/
    else {
        free_contact(node->info);
        free(node);
    
        previous->next = next;
        next->previous = previous;
    }

}

/* ------------------------------------------------------ */
/*       hash: hashes the given name to a position        */
/* ------------------------------------------------------ */
int hash(char *v, big_int M) {
    big_int h, a = 31415, b = 27183;

    /* iterates through the letters of the name and uses 
    a hashing algorithm to find a position for contact */
    for (h = 0; *v != '\0'; v++, a = a*b % (M-1))
        h = (a*h + *v) % M;

    return h;
}


/* ------------------------------------------------------ */
/*          free_domains: frees the domain hashtable        */
/* ------------------------------------------------------ */
void free_domains() {
    p_domain aux;

    /* iterates through all of the nodes and frees each of
    the domains, and then the correspondent node */
    while (head != NULL) {
        aux = head->next;
        free(head->domain);
        free(head);
        head = aux;
    }
}

/* ------------------------------------------------------ */
/*          expand_domains: expands the size of the       */
/*                    domain hashtable                      */
/* ------------------------------------------------------ */
p_domain *expand_domains(p_domain *domains) {
    big_int i;
    p_domain *t = domains;

    /* doubles the size of the domains */
    p_domain *new_hash = init_domains(domains_cap * 2);

    /* iterates through the current domains and rehashes them */
    for (i = 0; i < domains_cap / 2; i++)
        if (t[i] != NULL)
            insert_domain(t[i], new_hash);

    free(t);

    /* returns the expanded hashtable */
    return new_hash;
}


/* ------------------------------------------------------ */
/*      create_domain: creates a new domain node          */
/* ------------------------------------------------------ */
p_domain create_domain(char *domain) {
    p_domain new = malloc(sizeof(struct Domain));

    new->domain = malloc(sizeof(char) * (strlen(domain) + 1));

    /* after allocating space for the new domain node, inserts the 
    domain name into the domain element, and makes it the head of
    the list */
    strcpy(new->domain, domain);
    new->count = 1;
    new->next = head;
    head = new;

    return new;
}

/* ------------------------------------------------------ */
/*          insert_domain: inserts a new domain           */
/* ------------------------------------------------------ */
void insert_domain(p_domain new, p_domain *domains) {
    int i = hash(key_domain(new), domains_cap);

    /* after finding the position, if it has any information, 
    inserts it in another location which is free */
    while (domains[i] != NULL)
        i = (i+1) % domains_cap;
    domains[i] = new;

    domains_total++;
}


/* ------------------------------------------------------ */
/*      get_domain: gets the domain of a given email      */
/* ------------------------------------------------------ */
char *get_domain(char *email) {
    char *ptr = email;

    /* while a @ is not found, disregard */
    while(*(ptr++) != '@');

    /* return the pointer to the position directly after the @ */
    return ptr++;
}


/* ------------------------------------------------------ */
/*       search_domain: searches for a given domain       */
/* ------------------------------------------------------ */
p_domain search_domain(p_domain *domains, char *v) {
    int i = hash(v, domains_cap);

    /* after finding the position, if it is has any info and 
    if the domain is equal to the one given, return that node */
    while (domains[i] != NULL) {
        if (strcmp(v, key_domain(domains[i])) == 0)
            return domains[i];

        else
            i = (i+1) % domains_cap;
    }

    /* if not, returns a NULL pointer */
    return NULL;
}


/* ------------------------------------------------------ */
/*     search_contact: searches for a certain contact     */
/* ------------------------------------------------------ */
link search_contact(link *contacts, char *v) {
    int i = hash(v, contacts_cap);

    /* finds the position for the given contact, and if 
    it is has any info, compare it to the given name */
    while (contacts[i] != NULL) {
        /* if it finds the given name, returns the contact node */
        if (strcmp(v, key_name(contacts[i])) == 0)
            return contacts[i];

        /* if not, continues searching */
        else
            i = (i+1) % contacts_cap;
    }

    return NULL;
}


/* ------------------------------------------------------ */
/*  insert_contact: inserts a certain contact into the    */
/*                   contact map                          */
/* ------------------------------------------------------ */
void insert_contact(link new, link *contacts) {
    big_int i = hash(key_name(new), contacts_cap);

    /* if the position has any info, continue searching for
    a new position to put the contact */
    while (contacts[i] != NULL)
        i = (i+1) % contacts_cap;
    /* when an adequate position is found, inserts a new
    node with the given contact details and increases
    total contact map size */
    contacts[i] = new;

    contacts_total++;
}


/* ------------------------------------------------------ */
/*        init_domains: initializes the domain map        */
/* ------------------------------------------------------ */
p_domain *init_domains(big_int M) {
    big_int i;

    p_domain *domains = malloc(M*sizeof(struct Domain));

    /* initializes the ammount of domains to 0, and the
    size to the given M */
    domains_total = 0;
    domains_cap = M;

    /* initializes every position to a NULL pointer */
    for (i = 0; i < M; i++)
        domains[i] = NULL;

    return domains;
}


/* ------------------------------------------------------ */
/*        init_contacts: initializes the contacts map     */
/* ------------------------------------------------------ */
link *init_contacts(big_int M) {
    big_int i;
    
    link *contacts = malloc(M*sizeof(struct node));

    /* initializes the ammount of contacts to 0, and the
    size to the given M */
    contacts_total = 0;
    contacts_cap = M;

    /* initializes every position to a NULL pointer */
    for (i = 0; i < M; i++)
        contacts[i] = NULL;

    return contacts;
}


/* ------------------------------------------------------ */
/*   remove_contact: removes the contact with the given   */
/*               name from the contacts map               */
/* ------------------------------------------------------ */
link remove_contact(link *contacts, char *name) {
    int j, i = hash(name, contacts_cap); 
    link temp, aux;

    /* while there is a contact in the given position */
    while (contacts[i] != NULL)
        /* if it finds the contact with the given name, stops */
        if (strcmp(name, key_name(contacts[i])) == 0) 
            break;

        /* else, continues searching for a position */
        else 
            i = (i+1) % contacts_cap;

    /* if there are no contacts with that name, returns
    a NULL pointer */
    if (contacts[i] == NULL) return NULL;

    /* saves the item to be removed in temp and decreases
    contact total counter (size)*/
    temp = contacts[i];       
    contacts[i] = NULL;
    contacts_total--;

    /* removes the contact from the contacts map */ 
    for (j = (i+1) % contacts_cap; contacts[j] != NULL; j = (j+1) % contacts_cap, contacts_total--) {
        aux = contacts[j];
        contacts[j] = NULL;
        insert_contact(aux, contacts);
    }
    
    /* returns the removed contact */
    return temp;
}


void command_P(list *l) {
    char buffer[100];
    link aux = l->head;
    int count_pref = 0;

    scanf("%s", buffer);
    while(aux != NULL) {
        if (compare_pref(buffer, aux->info->name))
            count_pref++;
        aux = aux->next;
    }
    printf("%d\n", count_pref);
}


int compare_pref(char *pref, char *string) {
    char *aux = pref;
    while (*aux != '\0' && *string != '\0') {
        if (*aux != *string)
            return 0;
        aux++;
        string++;
    }
    return 1;
}


char *pertence(char *name) {
    
    while (*name != '\0') {
        if (strcmp(name, "_1") == 0) {
            return name;
        }
        name++;
    }

    return NULL;
}
