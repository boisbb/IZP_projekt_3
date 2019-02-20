/*!
  * @brief Dokumentace 3. projektu - Shlukova analyza.
  * @author Boris Burkalo.
/**
  * Struktura objektu, ktera v sobe obsahuje souradnice X, Y a ID bodu.
  */

struct obj_t {
    /** ID objektu */
    int id;
    /** umisteni objektu na ose X */
    float x;
    /** umisteni objektu na ose Y */
    float y;
};

/**
  * Struktura pole shluku, ktera urcuje velikost, kapacitu a objekty v shluku.
  */

struct cluster_t {
    /** velikost shluku */
    int size;
    /** kapacita shluku */
    int capacity;
    /** objekt, ktery se ve shluku nachazi */
    struct obj_t *obj;
};

/**
  * \defgroup firstgroup Prace se shluky
  * @{
  */
}
/**
  * @brief Funkce, ktera se postara o inicializaci shluku, v pripade, ze je kapacita rovna 0, tak nastavi objekt na NULL.
  * @param c shluk, ktery je potreba inicializovat
  * @param cap hodnota, na kterou se ma inicializovat kapacita clusteru
  *
*/
void init_cluster(struct cluster_t *c, int cap);

/**
  * @brief Funkce, ktera odstrani objetky clusteru a inicializuje jej na prazdny cluster
  * @param c cluster, ktery ma byt vycisten a znovu inicializovan
  * \pre c nesmi byt NULL
  * \pre kapacita musi byt vetsi nebo rovna 0
  */

void clear_cluster(struct cluster_t *c);

/**
  * @brief Hodnota, o kterou ma byt navysena aktualni kapacita shluku.
  *
  */

extern const int CLUSTER_CHUNK;

/**
  * @brief Zvetseni kapacity shluku ze stavajici kapacity na new_cap.
  * @param c Shluk, ktery ma byt zvetseny
  * @param new_cap nova kapacita
  * \return shluk c
  */

struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/**
  * @brief Prida objekt na konec shluku a v pripade, ze kapacita shluku mala, zvetsi ji.
  * @param c shluk, do ktereho bude pridan objekt
  * @param obj objekt, ktery bude pridan do shluku
  */

void append_cluster(struct cluster_t *c, struct obj_t obj);

/**
  * @brief Prida do c1 objekty z c2. Potom je seradi podle id.
  * @param c1 shluk, do ktereho budou prirazeny objekty
  * @param c2 shluk, ve kterem jsou objekty pro prirazeni
  */

void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);




/**
  * @brief Odstrani shluk na indexu 'idx' z pole shluku 'carr'.
  * @param carr pole shluku
  * @param idx index na kterem se nachazi shluk pro odstraneni
  * @param narr celkovy pocet shluku
  * \pre idx je mensi nez narr
  * \pre narr je vetsi nez 0
  * \return novy pocet shluku v poli
  */

int remove_cluster(struct cluster_t *carr, int narr, int idx);

/**@}*/

/**
  * \defgroup secondgroup Prace se vzdalenosti
  * @{
  */


/**
  * @brief Pocita Euklidovskou vzdalenost mezi dvema objekty.
  * @param o1 prvni objekt
  * @param o2 druhy objekt
  * \pre o1 neni NULL
  * \pre o2 neni NULL
  * \return nejmensi vzdalenost mezi objekty
  */


float obj_distance(struct obj_t *o1, struct obj_t *o2);

/**
  * @brief Pocita nejmensi vzdalenost dvou shluku od sebe.
  * @param c1 prvni shluk
  * @param c2 druhy shluk
  * \pre c1 a c2 nesmi byt NULL
  * \pre velikost c1 a c2 musi byt vetsi nez 0
  * \return nejmensi vzdalenost mezi shluky
  */

float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

/**
  * @brief Najde dva nejblizsi shluky a jejich indexy ulozi na adresu c1 a c2.
  * @param carr pole shluku
  * @param narr velikost pole shluku
  * @param c1 index prvniho shluku
  * @param c2 index druheho shluku
  * \pre narr musi byt vetsi nez 0
  */


void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/**@}*/

/** \addtogroup firstgroup
 *  @{
 */


/**
  * @brief Funkce, ktera seradi objekty ve shluku podle ID.
  * @param c shluk, ktery obsahuje objekty k serazeni
  */


void sort_cluster(struct cluster_t *c);

/**
  * @brief Tiskne shluk c na stdout.
  * @param c shluk, ktery ma byt vytisknuty
  */

void print_cluster(struct cluster_t *c);

/**
  * @brief Prekontroluje, zda se nahodou ve vstupnim souboru nevyskytuji dva objekty se stejnym ID.
  * @param c shluky, ktere maji byt prekontrolovane
  * @param Count pocet shluku ve vstupnim souboru
  * \return vraci 0 v pripade, ze se nevyskytuji dva objekty se stejnym ID
  * \return vraci 1 v pripade, ze se vyskytuji dva objekty se stejnym ID
  */

int id_check(struct cluster_t *c, int Count);

/**
  * @brief Funkce, ktera uvolni veskerou alokovanou pamet.
  * @param clusters pole shluku, ve kterem budou vsechny shluky uvolneny
  * @param Count pocet shluku, ktere maji byt uvoleny
  */

void free_all(struct cluster_t *clusters, int Count);

/**
  * @brief Nacte ze souboru 'filename' objekty. Pro kazdy objekt vytvori shluk a ulozi jej do pole shluku
  * @brief Alokuje prostor pro pole vsech shluku a ukazatel na prvni polozku pole ulozi do pametu, kam se
  * @brief odkazuje parametr arr.
  * @param filename vstupni soubor s objekty
  * @param arr pole shluku
  * \pre arr nesmi byt NULL
  * \return vraci hodnotu Count
  */

int load_clusters(char *filename, struct cluster_t **arr);

/**
  * @brief Funkce, ktera zpracovava vstupni data.
  * @param clusters pole shluku
  * @param Count pocet shluku v poli shluku
  * @param finalCount konecny pocet shluku
  * \post promenna Count nemuze byt zaporne cislo
  * \return vraci hodnotu Count
  */

int process_clusters(struct cluster_t *clusters, int Count, int finalCount);

/**
  * @brief Vytiskne pole shluku.
  * @param carr pole shluku
  * @param narr pocet shluku k vytisknuti
  */

void print_clusters(struct cluster_t *carr, int narr);

/** @}*/
