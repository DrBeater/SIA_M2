# <center> Rendu Synthèse d'Image Avancée <br> TD2 - Shadow Volumes

 ## 1. Construction des volumes d'ombres

    Pour cette première partie j'ai donc commencé par implémentez la fonction computeShadowVolume. J'ai eu quelques difficultés pour la création du quad et notamment pour le mettre dans le bon sens en fonction de quelle face était celle éclairée et quelle face était celel à l'ombre.
    J'ai ensuite appelé la fonction dans le Viewer::init(), et finalement j'ai dessinez le maillage dans le Viewer::display() en reprenant en partie le code de la partie du shader _blinnPrg.
    J'obtiens donc bien les résulats attendus.
    
![Shadow volumes visualisés en rendu fil de fer](Img_results/part1_end.png)
![Shadow volumes visualisés en rendu fil de fer](Img_results/part1_end2.png)


## 2. Comptage des entrées et sorties


