    #include <stdio.h>
    #include <stdlib.h>

    typedef struct {

        char titolo[50];
        char regista[50];
        int anno;
        float durata;
    }Film;

    // Funzione per verificare se il file esiste
    int file_esiste(const char *filename) {
        FILE *file = fopen(filename, "r");
        if (file) {
            fclose(file);
            return 1;
        }
        return 0;
    }

    // Funzione per leggere la libreria di canzoni da file
    Film *leggi_film(const char *filename, int *dim) {
        FILE *fileBinario = fopen(filename, "rb");
        if (fileBinario == NULL) {
            printf("Errore: impossibile aprire il file '%s'\n", filename);
            *dim = 0;
            return NULL;
        }

        // Leggiamo la dimensione della libreria
        fread(dim, sizeof(int), 1, fileBinario);

        // Allochiamo la libreria
        Film *libreria = malloc(sizeof(Film) * (*dim));
        if (libreria == NULL) {
            printf("Errore di allocazione.\n");
            fclose(fileBinario);
            *dim = 0;
            return NULL;
        }

        // Leggiamo le canzoni effettive
        fread(libreria, sizeof(Film), *dim, fileBinario);
        fclose(fileBinario);

        return libreria;
    }

    // Funzione per salvare la libreria su file
    void salva_libreria(const char *filename, Film *libreria, int dim) {
        FILE *fileBinarioWrite = fopen(filename, "wb");
        if (fileBinarioWrite == NULL) {
            printf("Errore: impossibile aprire il file '%s' per la scrittura\n", filename);
            return;
        }

        fwrite(&dim, sizeof(int), 1, fileBinarioWrite);
        fwrite(libreria, sizeof(Film), dim, fileBinarioWrite);
        fclose(fileBinarioWrite);
    }

    void Addfilm(Film *films, int index) {

        printf("Inserisci il titolo: ");
        fgets((films+index)->titolo, 50, stdin);
        printf("Inserisci il regista: ");
        gets((films+index)->regista);
        printf("Inserisci il anno: ");
        scanf("%d", &(films+index)->anno);
        printf("Inserisci il durata: ");
        scanf("%f", &(films+index)->durata);

        // %d - int
        /* %s - array i char */
        // %c - char
        // %f - float
        // %p - puntatore

    }

    void Visulizza(Film *films, int index) {

        for (int i = 0; i < index; i++) {
            printf("Titolo: %s - Regista %s - Anno %d - Durata %f \n", (films+i)->titolo,(films+i)->regista, (films+i)->anno, (films+i)->durata);
        }
    }

    void Riceca(Film *films, int index) {

        char titolo[50];

        printf("Inserisci il titolo: ");
        fgets(titolo, 50, stdin);

        for (int i = 0; i < index; i++) {

            if (films[i].titolo == titolo) {
                printf("Titolo: %s - Regista %s - Anno %d - Durata %f \n", (films+i)->titolo,(films+i)->regista, (films+i)->anno, (films+i)->durata);
            }
        }
    }

    void Riordina(Film *films, int index) {

        Film appoggio;
        int indextemp;
        for (int i = 0; i < index; i++) {
            indextemp = i;

            for (int j = i; j < index; j++) {
                if ((films+j)->anno > (films+indextemp)->anno) {
                    indextemp = j;
                }
            }

            // scambio
            appoggio = *(films + indextemp);
            *(films + indextemp) = *(films + i);
            *(films + i) = appoggio;

        }

    }

    int main(void) {

        char *filename = "../film.dat";

        int index = 0;
        Film *films;
        if (file_esiste(filename) == 0) {
            printf("il file di base non esiste ancora");
            films = malloc(sizeof(Film));
        } else {
            films = leggi_film(filename, &index);
        }

        int scelta = 0;

        do {
            printf("\n--- MENU ---\n");
            printf("1. Aggiungi film\n");
            printf("2. Visualizza catalogo\n");
            printf("3. Cerca film per titolo\n");
            printf("4. Ordina libreria per anno\n");
            printf("5. Salva ed esci\n");
            printf("0. Esci senza salvare\n");
            printf("Scelta: ");
            scanf("%d", &scelta);
            getchar(); // per consumare il '\n' rimasto in buffer

            switch(scelta) {
                case 1:
                    // Aggiungi film
                    // Se vuoi usare realloc, serve assegnare il ritorno a films
                    realloc(films, sizeof(Film) * (index + 1));
                    if (!films) {
                        printf("Errore allocazione memoria!\n");
                        exit(1);
                    }
                    Addfilm(films, index);
                    index++;
                    break;

                case 2:
                    // Visualizza catalogo
                    Visulizza(films, index);
                    break;

                case 3:
                    // Cerca film per titolo
                    Riceca(films, index);
                    break;

                case 4:
                    // Ordina per anno
                    Riordina(films, index);
                    printf("Libreria ordinata per anno.\n");
                    break;

                case 5:
                    // Salva e esci
                    salva_libreria(filename, films, index);
                    printf("Libreria salvata. Arrivederci!\n");
                    break;

                case 0:
                    printf("Uscita senza salvare.\n");
                    break;

                default:
                    printf("Scelta non valida, riprova.\n");
            }

        } while(scelta != 5 && scelta != 0);

        free(films);

        return 0;
    }