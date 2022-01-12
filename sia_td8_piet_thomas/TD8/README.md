# <center> Rendu Synthèse d'Image Avancée <br> TD8 - Système masse-ressort

 ## 1. Implémentation des méthodes addForces

    Comme demandé dans le sujet, on complète les formules GravityForce, DragForce et SpringForce, en utilisant les formules du cours.


## 2. Implémentation des méthodes getDimensions, get/setState et getDerivative

    Pour ces fonctions, nous nous sommes aidés de la méthode Eigen::segment


## 3. Implémentation de la fonction explicitEulerStep

    Dans cette partie, il s'agissait de faire une fonction de pas d'Euleur, qui déplace les particules le long de leur vitesse selon un deltaT.

## 4. Test de l'affichage et jeu avec les paramètres

    On affiche donc notre tissu, en jouant avec les paramètres : 

![tissu](Img_results/tissu.png)  
![tissu avec point d'accroche](Img_results/tissu_force_accroche.png) 

    Le programme semble avoir un problème sous Windows (je n'ai pas pu le lancer sous Linux), car parfois il crash quand je change les paramètre dans le fichier scn. De plus parfois il fonctionne avec les paramètres par défaut et parfois non. Je n'ai pas trouvé d'où celà pourrais venir.

## 5. Augmenter la stabilité du tissu

    Pour cette partie on ajoute des ressort le long de la diagonale des cellules de la grille. On complètes donc la fonction makeGrid.

![tissu rigide](Img_results/tissu_rigidité.png)

## 6. Implémentation du point du milieu

    On implémente donc la méthode midPointStep.

## 7. Création du classes Collider et PlaneCollider

    Pour gérer la collision il faut donc pour chaque particules, lancer un rayon dans la direction de sa vitesse, trouver une intersection avec le Mesh, vérifier que la particule est sur la face de l'intersection et calculer la réponse à cette collision.
    Malheureusement je n'ai pas réussi à implémenter cette partie.