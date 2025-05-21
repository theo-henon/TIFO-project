# Description
Registration d'images 3D (slices d'images 2D) à implémenter à la main (C++). 
    - Optimiseur (descente de gradient), pas de machine learning
    - Métrique (MI)
    - Transformation (Rigid ?) (SyN trop compliqué)
    - Images TIFF (utilisation d'une bibliothèque externe)
    - Parallèlisation des calculs
    - Interpolateur ? (soit on le fait soit on considère que les images font exactement la même taille)

# Détails

Toutes les valeurs numériques auront la précision d'un float.

## Métrique
Valeur numérique qui permet de comparer deux images, dans l'intervalle [0(min), 1(max)]. La métrique à utiliser est : Mutual Information (MI), qui est basée sur l'entropie croisée des deux images. Elle permet d'étudier la distribution statistique et spatiale.
[Plus d'informations](https://fr.wikipedia.org/wiki/Information_mutuelle)

Pour comparer des images de différentes tailles on utilise un interpolateur (bilinéaire, bicubique, etc.)

## Transformation
Une transformation géométrique que l'on veut appliquer à l'image mouvante. On se limitera à des transformations *Rigid (rotation + translation)*.
Donc on a ces paramètres :
    - Translations: *tx*, *ty*, *tz*
    - Rotations: *rx*, *ry*, *rz*

On pourra éventuellement améliorer cette transformation en rajoutant un alignement des centres de masses.

## Optimiseur
Algorithme de descente de gradient à pas constant (et plus si assez de temps) qui maximise la *métrique* (ou minimise l'erreur) entre l'image mouvante et l'image de référence.

L'optimiseur a ces paramètres :
    - La métrique de similarité
    - Pas de la descente de gradient *g*
    - Facteur de relaxation *r* (ralentissement de l'algorithme quand le gradient s'approche de 0)

# Structure (fortement inspiré d'ITK)
```
./
    - src/
        - optimizer/
            - Interface 'Optimizer' that contains:
                - a metric
                - a gradient step
                - a relaxation factor
            - Sub-class 'RegularGradientDescentOptimizer' of class 'Optimizer'
        - metric/
            - Interface 'Metric'
            - Sub-class 'MutualInformationMetric' of class 'Metric'
        - logger/
            - Class 'Logger', just a logger
        - transform/
            - Interface 'Transform'
            - Sub-class 'RigidTransform' of class 'Transform'
            - Transform file reader/writer
        - image/
            - Class 'Image'
            - TIFF image reader/writer (using a third-party library: TinyTIFF (https://jkriege2.github.io/TinyTIFF/))
        - cli/
            - Structure 'CLI' that contains input data of the Command Line Interface
        - registration/
            - Class 'Registration' that contains:
                - an optimizer,
                - a metric,
                - a transform,
                - a reference image,
                - a moving image
        - math/
            - Class 'Matrix4f'
            - Class 'Vector3f'
    - tests/
        - unit
            - Checks that the code works as expected
        - quality
            - Registration process quality check, by computing a metric (MI) between output image and reference image
        - benchmark
            - Measure the execution time, memory/cpu usage of the registration process
 ```

