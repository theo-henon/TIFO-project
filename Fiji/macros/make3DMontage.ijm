ni = nImages;
getSelectionBounds(selectionX, selectionY, selectionWidth, selectionHeight);

// Calcul du nombre de colonnes et lignes pour un carré ou quasi-carré
cols = floor(sqrt(ni));
rows = Math.ceil(ni / cols);

titArr = newArray(ni);
idArr = newArray(ni);

// Sauvegarde des ID des images
for (i = 0; i < ni; i++) {
    selectImage(i + 1);
    idArr[i] = getImageID();
}

// Dupliquer la sélection de chaque image
for (i = 0; i < ni; i++) {
    selectImage(idArr[i]);
    run("Select None");
    makeRectangle(selectionX, selectionY, selectionWidth, selectionHeight);
    run("Duplicate...", "duplicate");
    titArr[i] = getTitle();
}

getDimensions(width, height, channels, slices, frames);

// Créer l'image de destination avec dimensions ajustées
montageWidth = cols * selectionWidth;
montageHeight = rows * selectionHeight;

newImage("3Dmontage", "32-bit black", montageWidth, montageHeight, slices);
OID = getImageID();

// Coller chaque image dans la bonne position (x, y)
for (i = 0; i < ni; i++) {
    selectImage(titArr[i]);
    TID = getImageID();
    
    col = i % cols;
    row = floor(i / cols);
    pasteX = col * selectionWidth;
    pasteY = row * selectionHeight;

    for (s = 1; s <= slices; s++) {
        selectImage(TID);
        Stack.setSlice(s);
        run("Select None");
        run("Select All");
        run("Copy");

        selectImage(OID);
        Stack.setSlice(s);
        makeRectangle(pasteX, pasteY, selectionWidth, selectionHeight);
        run("Paste");
        run("Select None");
    }

    selectImage(TID);
    close();
}
