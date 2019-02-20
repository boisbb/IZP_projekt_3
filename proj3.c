/**
 * 3. projekt
 *
 * Jednoducha shlukova analyza: 2D nejblizsi soused.
 * Single linkage
 * Burkalo Boris
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX
#include <string.h>

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

void free_all(struct cluster_t *clusters, int Count); //funkce, ktera uvolni vsechny clustery;

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);

    c->capacity = cap;

    if(cap == 0){
      c->obj = NULL;
      return;
    }
    c->obj = malloc(cap * sizeof(struct obj_t));
    if(c->obj == NULL){
      fprintf(stderr, "Error: memory allocation was unsuccesful.\n");
      exit(1);
    }
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
    free(c->obj);
    c->size = 0;
    init_cluster(c, 0);
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = (struct obj_t*)arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */

 //v pripade, ze je velikost stejna jako kapacita clusteru, je nutno kapacitu zvetsit o CLUSTER_CHUNK
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
  if(c->size == c->capacity){
    resize_cluster(c, c->capacity + CLUSTER_CHUNK); //zvetsi cluster, v pripade, ze size==cap
  }
  c->obj[c->size] = obj; //nahraje na pozici c->size (jelikoz indexy se pocitaji od nuly)
  c->size += 1; //zvetsi velikost o jeden (pri nahrani objektu)
}


/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);

    int i = 0;

    while(i < c2->size){ //priradi do clusteru c1 vsechny objekty z c2
      append_cluster(c1, c2->obj[i]);
      i++;
    }
    sort_cluster(c1);
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);

    int i = idx;

    clear_cluster(&carr[idx]);
    while((i + 1) < narr){
      carr[i] = carr[i + 1]; //zmensi indexy u kazdeho nasledujiciho clusteru o jedno
      i++;
    }
    narr-=1; //zmensi celkovy pocet clusteru

    return narr;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

    float d = 0;
    float a = 0;
    float a1 = 0;
    float b = 0;

    a = ((o1->x) - (o2->x));
    a1 = ((o1->y) - (o2->y));
    b = pow(a, 2) + pow(a1, 2);
    d = sqrt(b);

    return d;
}

/*
 Pocita vzdalenost dvou shluku.
 bude pouzivat funkci obj_distance, pomoci ktere vypocita vsechny vzdalenosti vsech ostatnich bodu
 vybere tu nejmensi a s tim shlukem se potom spoji
 pouzije NEJMENSI vzdalenost ktera vyjde postupne pomoci pocitani obj_distance
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    int x = c1->size;
    int y = c2->size;
    int i = 0;
    float dst = 0.0;
    float dist = INFINITY;

    while(i < x){
      for(int j = 0; j < y; j++){
        dst = obj_distance(&c1->obj[i], &c2->obj[j]);
        if(dst < dist){ //pri prvnim, pokud je vzdalenost mensi nez infinity - coz bude, tak je podminka splnena a do konecne promenne dist, nahraje nejmensi vzdalenost
          dist = dst;
        }
      }
      i++;
    }
  return dist;
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);
    int i = 0;
    float distance = INFINITY;
    float dst = 0.0;
    int n = 1;

    while(i < (narr - 1)){ // posledni uz tam stoprocentne nepotrebujeme - vsechny mozne kombinace uz byly provedeny
      for(int j = i; j < narr; j++){
        if(i != j && j != (i - n)){
          dst = cluster_distance(&carr[i], &carr[j]);
          if(dst < distance){
            distance = dst;
            *c1 = i;
            *c2 = j;
          }
        }
      }
      i++;
    }
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = (const struct obj_t *)a;
    const struct obj_t *o2 = (const struct obj_t *)b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

int id_check(struct cluster_t *c, int Count) //zkontroluje, zda nahodou nejsou dva objekty o stejnem ID
{
  for(int i = 0; i < Count; i++){
    for(int j = 0; j < Count; j++){
      if(j != i && c[i].obj->id == c[j].obj->id){
        fprintf(stderr, "Error: two or more objects with the same ID.\n");
        free_all(c, Count);
        return 1;
      }
    }
  }
  return 0;
}

void free_all(struct cluster_t *clusters, int Count) //uvolni veskerou alokovanou pamet
{
  int i = 0;

  while(i != Count){
    clear_cluster(&clusters[i]);
    i++;
  }
  free(clusters);
}
/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);

    FILE *object = fopen(filename, "r");


    if(object == NULL){
      fprintf(stderr, "Error: unable to read file.\n");
      return -1;
    }
    else{
      int Count = 0;
      int i = 0;
      int controllCount = 0;
      int c = 0;
      int d = 0;
      char newline;

      d = fscanf(object, "count=%d%[^\n]", &Count, &newline);

      if(d != 1 || Count == 0){
        fprintf(stderr, "Error: mistake in the input text file.\n");
        fclose(object);
        return -1;
      }

      *arr = malloc(sizeof(struct cluster_t) * Count);
      if(arr == NULL){
        fprintf(stderr, "Error: memory allocation was unsuccesful.\n");
        fclose(object);
        return -1;
      }

      for(i = 0; i < Count; i++){
        init_cluster(&(*arr)[i], 1);
        (*arr)[i].size = 1;
        c = fscanf(object, "%d %f %f%[^\n]", &(*arr)[i].obj->id, &(*arr)[i].obj->x, &(*arr)[i].obj->y, &newline); //v pripade, ze posledni znak neni newline, tak se do c cislo vetsi nez 3, tudiz pozdejsi podminka nebude splnena
        controllCount++;

        if(c != 3 || (*arr)[i].obj->x < 0 || (*arr)[i].obj->x > 1000 || (*arr)[i].obj->y < 0 || (*arr)[i].obj->y > 1000){
          fprintf(stderr, "Error: mistake in the input text file.\n");
          free_all(*arr, controllCount);
          fclose(object);
          return -1;
        }
      }

      if(controllCount != Count){
        fprintf(stderr, "Error: number of clusters doesn't match or there's a mistake in the first line.\n");
        free_all(*arr, controllCount);
        fclose(object);
        return -1;
      }
      fclose(object);
      return Count;
    }
}

int process_clusters(struct cluster_t *clusters, int Count, int finalCount) //pomocna funkce pro zpracovani vstupnich dat
{
  int c2 = 0;
  int c1 = 0;

  while(Count != finalCount){
    find_neighbours(clusters, Count, &c1, &c2); //c1 a c2 je cislo dvou nejblizsich CLUSTERU --> CLUSTERY KTERE PUJDOU DO merge_clusters
    merge_clusters(&clusters[c1], &clusters[c2]);
    Count = remove_cluster(clusters, Count, c2); //objCount je prozatimni promenna;

  }
  return Count;
}
/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

int main(int argc, char *argv[])
{
    struct cluster_t *clusters;
    int Count = 0;
    int finalCount = 0;
    int num_of_args = argc;
    char *help;
    int idCheck = 0;

    if(num_of_args == 3){
      Count = load_clusters(argv[1], &clusters);
      finalCount = strtol(argv[2], &help, 10);

      if(Count == -1){
        return 1;
      }
      else if(strlen(help) != 0 || finalCount > Count){ //jestlize je delka pomocneho stringu vetsi nez 0, tak je format vstupu SPATNY
        fprintf(stderr, "Error: wrong format of the second argument or N is higher than Count.\n");
        free_all(clusters, Count);
        return 1;
      }
      idCheck = id_check(clusters, Count);
      if(idCheck == 1){
        return 1;
      }
      Count = process_clusters(clusters, Count, finalCount);
      print_clusters(clusters, Count);
      free_all(clusters, Count);
      return 0;
    }
    else if(num_of_args == 2){
      Count = load_clusters(argv[1], &clusters);
      finalCount = 1;

      if(Count == -1){
        return 1;
      }
      idCheck = id_check(clusters, Count);
      if(idCheck == 1){
        return 1;
      }
      Count = process_clusters(clusters, Count, finalCount);
      print_clusters(clusters, Count);
      free_all(clusters, Count);
      return 0;
    }
    else if(num_of_args <= 1 || num_of_args > 3){
      fprintf(stderr, "Error: wrong input format.\n");
      return 1;
    }
}
