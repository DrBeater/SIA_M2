# <center> Rendu Synthèse d'Image Avancée <br> TD5 - BRDF de Ward

 ## 1. Éclairage direct

    Avant de commencer, j'ai eu quelques soucis. Codant sous linux j'ai eu des problèmes d'include pour glah.h dans common.cpp et khrplatform.h dans glad.h. Je n'avais jamasi eu ce problème dans les autres TDs pourtant. La seule solution que j'ai trouvé est donc de modifier le chemin et mettre le chemin entier. Il faudra donc peut être commenter ces lignes et remettre celles d'origine pour pouvoir éxécuter.
    
    Pour cette partie, il suffisait de coder la formule donnée.
    J'ai du inverser m_alphaX et m_alphaY dans les calcul de hx et hy, car le résultat semblait inversé.

    J'obtiens donc ces résultats : 

$\alpha$<sub>x</sub> = $\alpha$<sub>y</sub> = 0.01  |   $\alpha$<sub>x</sub> = $\alpha$<sub>y</sub> = 0.1  | $\alpha$<sub>x</sub> = $\alpha$<sub>y</sub> = 0.25   |   $\alpha$<sub>x</sub> = $\alpha$<sub>y</sub> = 0.5
:-------------------------:|:-------------------------:|:-------------------------:|:-------------------------:
![sphereWard](Img_results/sphereWard_0.01.png)   |  ![sphereWard](Img_results/sphereWard_0.1.png)    |  ![sphereWard](Img_results/sphereWard_0.25.png)  |  ![sphereWard](Img_results/sphereWard_0.5.png)

![sphereWard](Img_results/sphereWard_5.png)   |  ![sphereWard](Img_results/sphereWard_6.png)    |  ![sphereWard](Img_results/sphereWard_7.png)  |  ![sphereWard](Img_results/sphereWard_8.png)
:-------------------------:|:-------------------------:|:-------------------------:|:-------------------------:
$\alpha$<sub>x</sub> = 0.2  $\alpha$<sub>y</sub> = 0.1  |   $\alpha$<sub>x</sub> = 0.5  $\alpha$<sub>y</sub> = 0.1  | $\alpha$<sub>x</sub> = 0.1  $\alpha$<sub>y</sub> = 0.2    |   $\alpha$<sub>x</sub> = 0.1  $\alpha$<sub>y</sub> = 0.5


## 2. Échantillonnage uniforme de la BRDF

    Pour cette partie j'ai commencé par faire le fichier directs_mats.cpp. Pour implémentez l'intégrateur DirectMats, on s'inspire de Whitted en replançant la réflexion miroir par un échantillonnage uniforme de la BRDF.
    J'ai ensuite complété le fichier warp.cpp.

$\alpha$<sub>x</sub> = $\alpha$<sub>y</sub> = 0.01  |   $\alpha$<sub>x</sub> = $\alpha$<sub>y</sub> = 0.1
:-------------------------:|:-------------------------:
![sphereWard](Img_results/sphereWardEnvMap_1.png)   |  ![sphereWard](Img_results/sphereWardEnvMap_2.png)

![sphereWard](Img_results/sphereWardEnvMap_3.png)   |  ![sphereWard](Img_results/sphereWardEnvMap_4.png)
:-------------------------:|:-------------------------:
$\alpha$<sub>x</sub> = 0.5  $\alpha$<sub>y</sub> = 0.1  |   $\alpha$<sub>x</sub> = 0.1  $\alpha$<sub>y</sub> = 0.5


## 3. Échantillonnage préférentiel

    


