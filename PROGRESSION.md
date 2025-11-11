# Progression OceanDepths
## Étapes réalisées
- [x] Étape 1 : Génération créatures
- [x] Étape 2 : Attaque joueur
- [X] Étape 3 : Attaque créatures
- [x] Étape 4 : Récompenses
- [x] Étape 5 : Sauvegarde/Chargement
- [x] Étape 6 : Compétences aquatiques
- [x] Étape 7 : Cartographie des océans
- [x] Bonus : Marchand
- [x] Bonus : Défis spéciaux : Boss
- [x] Bonus : Système de progression (le joueur débloque des parties de la carte, pas de niveaux)

## Captures d'écran :
- 1. Génération des creatures & Attaque des créatures :

![creature_image.png](images/creature_image.png)

---

### 2 & 3. Attaque du joueur : on peut voir la méduse perdre des points de vie après l'attaque du joueur.

![combat_image.png](images/combat_image.png)

![combat_image_2.png](images/combat_image_2.png)

---

### 4. Récompenses : 

![recompenses_img.png](images/recompenses_img.png)

---

###  5. Sauvegarde/Chargement : Sauvegarde les données en binaire à partir d'un struct et les charge ensuite de la même manière.

![Save.png](images/save_img.png)

![Load.png](images/load_img.png)

---

###  6. Compétences aquatiques (basé sur l'équipement pour le joueur, definit pour les créatures) :

![combat_image.png](images/combat_image.png)

---

### 7. Cartographie des océans :

![map_img.png](images/map_img.png)

---

### Bonus : Marchand

![shop_img.png](images/shop.png)

### Bonus : Défis spéciaux : Boss

![boss_img.png](images/boss_img.png)

### Bonus : Système de progression (le joueur débloque des parties de la carte, pas de niveaux)

![progression_img.png](images/progression_img.png)

Cette version de la carte est une version antérieure à la version finale, mais elle illustre le système de progression.

La seule différence est la partie visuelle, qui à été changée pour être compatible avec tout terminal qui supporte l'ASCII

## Difficultés rencontrées

- Séparer la logique de l'interface

- L'intégration du code entre les différentes versions de chaque membre du groupe

- Suivre strictement les demandes

- Prévoir les implémentations de manière modulaire
 
- Le projet en lui même

- Les effets (4 versions différentes avant d'arriver à un résultat satisfaisant)

- La mise en place des tests et de s'assurer qu'ils soient pertinents
