those are from the documentation with the project.

```c++
typedef struct {
int id; // identifiant unique pour cibler
char nom[30];
int points_de_vie_max;
int points_de_vie_actuels;
int attaque_minimale;
int attaque_maximale;
int defense;
int vitesse;
char effet_special[20]; // "paralysie", "poison", "aucun"
int est_vivant;
} CreatureMarine;
```

```c++
typedef struct {
int points_de_vie;
int points_de_vie_max;
int niveau_oxygene;
int niveau_oxygene_max;
int niveau_fatigue; // 0 à 5
int perles; // monnaie du jeu
} Plongeur;
```
