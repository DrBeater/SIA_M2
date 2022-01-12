# <center> Rendu Synthèse d'Image Avancée <br> TD4 - Echantillonnage

 ## 1. Intégration Monte Carlo

    Pour cette première partie j'ai donc commencé par implémenter la fonction int_mc. 
    On commence par implémenter la fonction int_mc().
    Puis on calcul la variance analytique : pour ce calcul j'ai choisis de mettre directement (16/9) / n, car je l'ai calculé à l'avance avec les a et b donné dans le code (a = 0 et b = 1).
    On ajoute ensuite la variance numérique (varest de la fonction int_mc).

    On affiche ensuite l'erreur d'estimation, la variance analytique et la variance numérique:
    
![Monte-Carlo](Img_results/Monte_carlo.PNG)


## 2. Anti-Aliasing

### 2.1 Echnatillonnage régulier

    Pour cette partie, on échantillonne le pixel en une grille de n * n, où n correspond à l'attribut sampleCount de la caméra.

    On obtient ça avec "tw.scn" en fixant le nombre d'échantillons à 8 dans le fichier scène :

![TW 2.1](Img_results/tw_2_1.png)

### 2.2 Echnatillonnage stratifié

    Pour cette partie on fait un échantillonnage de façon stratifié en ajoutant du jitter à chaque rayon. On obtiens donc ces résultats : 

![TW 2.2](Img_results/tw_2_2.png)
<img src="Img_results/tw_2_2_zoom.png" alt="TW 2.2 zoom" style="width:250px;"/>

2x2            |  8x8
:-------------------------:|:-------------------------:
![deuxSpheres 2x2](Img_results/deuxSpheres_2x2.png)   |  ![deuxSpheres 8x8](Img_results/deuxSpheres_8x8.png)

![deuxSpheres 32x32](Img_results/deuxSpheres_32x32.png)   |  ![deuxSpheres 128x128](Img_results/deuxSpheres_128x128.png)
:-------------------------:|:-------------------------:
32x32            |  128x128


## 3. Sources étendues

### 3.1 Echnatillonnage de Monte-Carlo

    Pour cette partie, on modifie whitted.cpp pour modifier le comportement des lumières dans AreaLight.
    On obtiens ce résultat :

![TW Area 3.1](Img_results/tw_area.png)


### 3.2 Source texturée

