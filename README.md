# OceanDepth - Underwater Exploration RPG

## Description

**OceanDepth** est un jeu d'exploration sous-marine en mode texte développé en C. Plongez dans les profondeurs 
mystérieuses de l'océan, affrontez des créatures marines redoutables, gérez votre oxygène et découvrez des trésors 
engloutis.

### Caractéristiques principales

- **Système de combat** tactique contre diverses créatures marines
- **Gestion de l'oxygène** : ressource vitale qui diminue à chaque action
- **Inventaire** avec objets consommables et équipements
- **Exploration de zones** à différentes profondeurs
- **Sauvegarde/Chargement** de votre progression
- **Boutique** pour acheter équipements et consommables
- **Effets spéciaux** des créatures (paralysie, poison, etc.)

---

## 🚀 Installation et Compilation

### Compilation avec CLion en utilisant des CMakeLists

---

## Comment Jouer

### Démarrage du jeu

Au lancement, vous pouvez :
1. **Nouvelle partie** : Commencer une nouvelle aventure sous-marine
2. **Charger une partie** : Reprendre votre progression sauvegardée
3. **Quitter** : Fermer le jeu

### Navigation dans le jeu

Le jeu utilise une interface en ligne de commande avec des menus numérotés. Entrez le numéro correspondant à votre 
choix et appuyez sur Entrée.

### Ressources à gérer

- **Points de Vie (PV)** : Si vous atteignez 0, c'est la fin
- **Oxygène** : Diminue à chaque action. Si 0, vous perdez des PV
- **Fatigue** : Limite vos attaques par tour (0-5)
- **Perles** : Monnaie pour acheter équipements et objets

### Combat

- Sélectionnez vos cibles en utilisant leur numéro
- Choisissez entre attaque, utilisation d'objet
- Les créatures attaquent par ordre de vitesse
- Certaines créatures ont des effets spéciaux (poison, boost d'attaque)

### Exploration

- Naviguez entre différentes zones aquatiques
- Chaque zone a ses propres créatures et défis
- Plus vous descendez, plus les créatures sont dangereuses
- Trouvez des grottes pour vous reposer et sauvegarder

---

## 📂 Structure du Projet

```
OceanDepth/
├── CMakeLists.txt              # Configuration CMake
├── identifiants.txt            # Identifiants des étudiants
├── README.md                   # Ce fichier
├── PROGRESSION.md              # Progression et captures d'écran
├── saves/                       # Fichiers de sauvegarde de partie
├── code/                       # Code source principal
│   ├── includes/               # Fichiers d'en-tête (.h)
│   │   ├── core/               # Logique du jeu
│   │   │   ├── action.h
│   │   │   ├── combat_loop.h
│   │   │   ├── creature.h
│   │   │   ├── creature_data.h
│   │   │   ├── effect.h
│   │   │   ├── entity.h
│   │   │   ├── error_codes.h
│   │   │   ├── game_init.h
│   │   │   ├── inventory.h
│   │   │   ├── inventory_data.h
│   │   │   ├── item.h
│   │   │   ├── item_data.h
│   │   │   ├── item_pool.h
│   │   │   ├── map.h
│   │   │   ├── player.h
│   │   │   ├── reward_system.h
│   │   │   ├── save_system.h
│   │   │   └── shop.h
│   │   ├── helper/             # Fonctions utilitaires
│   │   │   └── input_validator.h
│   │   └── interface/          # Interface utilisateur
│   │       ├── interface_api.h
│   │       ├── interface_cli.h
│   │       └── interface_table.h
│   │
│   ├── src/                    # Implémentations (.c)
│   │   ├── main.c              # Point d'entrée
│   │   ├── core/               # Implémentations logique (identique aux includes)
│   │   ├── helper/             # Implémentations utilitaires (identique aux includes)
│   │   └── interface/          # Implémentations interface (identique aux includes)
│   │
│   └── tests/                  # Tests unitaires
│       ├── combat_loop_test.c
│       ├── creature_test.c
│       ├── effect_test.c
│       ├── map_test.c
│       ├── player_test.c
│       ├── rewards_test.c
│       ├── save_system_test.c
│       └── shop_test.c
│
├── documentation/              # Documentation technique
│   ├── guidelines.md           # Guide de développement
│   └── Keypoints.md            # Points clés du projet
│
├── images/                     # Captures d'écran
│
├── cmake-build-debug/          # Fichiers de build (généré)
│
└── tests/                      # Configuration tests
    └── CMakeLists.txt
```

---

## 🎯 Fonctionnalités Implémentées

Voir [PROGRESSION.md](PROGRESSION.md) pour les détails et captures d'écran.

---

## 🧪 Tests Unitaires

Le projet inclut une suite de tests unitaires complète.

### Tests disponibles

- `All CTest` : Exécute tous les tests
- `combat_loop_test` : Tests du système de combat
- `creature_test` : Tests des créatures et leurs actions
- `effect_test` : Tests des effets de statut
- `map_test` : Tests de la navigation
- `player_test` : Tests du joueur et ses ressources
- `rewards_test` : Tests du système de récompenses
- `save_system_test` : Tests de sauvegarde/chargement
- `shop_test` : Tests de la boutique

---

## 🛠️ Architecture Technique

### Modules Principaux

#### Core (Logique du jeu)
- **creature** : Gestion des créatures marines
- **player** : État et actions du joueur
- **combat_loop** : Boucle de combat principale
- **effect** : Système d'effets de statut
- **inventory** : Gestion de l'inventaire
- **item** : Items et équipements
- **map** : Exploration et navigation
- **save_system** : Persistance des données
- **shop** : Commerce et achats
- **action** : Actions qui ont un effet définit

#### Interface
- **interface_cli** : Interface en ligne de commande
- **interface_table** : Struct interface pour que la logique puisse appeler l'interface
- **interface_api** : API pour découpler la logique de l'interface plus facilement

#### Helper
- **input_validator** : Validation des entrées utilisateur

### Gestion Mémoire

- ✅ Allocation dynamique pour flexibilité
- ✅ Libération systématique des ressources
- ✅ Aucune fuite mémoire (vérifié avec tests)

---

## 📋 Contraintes Respectées

### Techniques
- ✅ Langage C (norme C99)
- ✅ Compilation sans warnings
- ✅ Gestion mémoire rigoureuse
- ✅ Validation des entrées utilisateur
- ✅ Code modulaire

### Fonctionnelles
- ✅ Interface en français
- ✅ Terminologie marine française
- ✅ Toutes les étapes implémentées
- ✅ Fonctionnalités bonus ajoutées

---

## 🐛 Problèmes Connus

Aucun bug majeur identifié. Voir [PROGRESSION.md](PROGRESSION.md) pour les difficultés rencontrées et résolues.

---

## 👥 Auteurs

Voir le fichier [identifiants.txt](identifiants.txt) pour la liste des contributeurs.

---

## 📸 Aperçu

---

![Combat](images/combat_image.png)
*Système de combat avec créatures marines*

---

![Carte](images/map_img.png)
*Exploration des zones océaniques*

---

![Boutique](images/shop.png)
*Commerce sous-marin avec le marchand pieuvre*

---

Pour plus de captures d'écran, consultez [PROGRESSION.md](PROGRESSION.md).

---