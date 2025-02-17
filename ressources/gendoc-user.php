
<?php
    //tableaux pour stocker (aux mêmes indices dans les différents tableaux) les balises a retrouver dans le fichier md et a mettre dans le html
    ob_start();

    $varMd = ['```', '- ', '|', '#### ', '### ', '## ', '# '];
    $varHtml = ['<pre> ', '<ul><li>','<table><th>', '<h4>','<h3>','<h2>','<h1 id = "titre">'];
    $varHtmlFin = [' ', '</li>','</th></table>', '</h4>', '</h3>', '</h2>', '</h1>'];

    // récupération du fichier de configuration
    $f_config = file('config');
    $n_config = array();
    $l_config = 0;
    foreach($f_config as $config){
        $config = explode('=',$config);
        $n_config[$l_config] = $config[1];
        $l_config++;
    }
    // $n_config contient les valeurs de chaque options
    $versions = explode('.',$n_config[2]); // on récupère chaque numéro de version
    // [0] = MAJOR
    // [1] = MINOR
    // [2] = BUILD
    $major = $versions[0];
    $minor = $versions[1];
    $build = $versions[2];

    if(isset($argv[1])){ // le paramètre qui donne un fichier .md
        if(strpos($argv[1],'.md')){
            $lignesMd = file($argv[1]);
        } else {
            echo "Erreur ! Aucun fichier .md n'a été détecté en paramètre !";
            echo "Veuillez entrer un fichier .md en premier paramètre !";
        }
    }

    if(isset($argv[2])){ // [2] et non [1] car le premier paramètre DOIT être le fichier .md
        $parametre = $argv[2];
    
        switch($parametre){
            case '--major':
                $major++;
                $minor = 0;
                $build = 0;
                $versions[0] = $major;
                $versions[1] = $minor;
                $versions[2] = $build;
                break;
            case '--minor':
                $minor++;
                $build = 0;
                $versions[1] = $minor;
                $versions[2] = $build;
                break;
            case '--build':
                $build++;
                $versions[2] = $build;
                break;
        }
    }
    $f_config[2] = "VERSION=$major.$minor.$build";

    // Réécriture du fichier 'config'
    file_put_contents('config', implode('', $f_config));
    
    //variables pour aider a mettre en place les balise html de type bloc
    $tabOuvert = false;
    $listOuverte = false;
    $preOuvert = false;
    $balise = false;


    ?>

    <!DOCTYPE html>
    <html lang="fr">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <link rel="stylesheet" href="style_doc_user.css">
        <title>Documentation utilisateur</title>
    </head>
    <body>

    <?php


    //on passe dans une boucle pour chaque ligne du fichier
    foreach ($lignesMd as $numLig => $lig) 
    {
        //remplacement des balises de lien mardown par des balises html
        if (strpos($lig, "[") !== false && strpos($lig, "(") !== false)
        {
            $lig = str_replace("[", "<a href = '#titre'>", $lig);
            $lig = str_replace("]", "</a>", $lig);
            while(strpos($lig, "(") !== false)
            {
                $debutBalise = strpos($lig, '(');
                $finBalise = strpos($lig, ')') - $debutBalise +1;
                $lig = substr_replace($lig, '', $debutBalise, $finBalise);
            }
        }

        //remplacement markdown des balises de fonte a chasse fixe par les balises html correspondant

        $lig = str_replace(" `", " <code> ", $lig);
        $lig = str_replace("` ", " </code> ", $lig);



        if ($lig != "\n") 
        {
            //si une liste est actuellement ouverte, on la continue
            if ($listOuverte == true) 
            {
                // on vérifie qu'qu'on est pas arrivé à la fin de la liste
                if(strpos($lig, "- ") !== false)
                {
                    //test du nombre de tabulations afin de créer des listes imbriquées
                    $nvTabListe = 0;
                    if(strpos($lig, "    ") !== false)
                    {
                        $nvTabListe = 1;
                        if(strpos($lig, "        ") !== false)
                        {
                            $nvTabListe = 2;
                            if(strpos($lig, "            ") !== false)
                            {
                                $nvTabListe = 3;
                            }
                        }
                    }

                    // ouvertures et fermetures des listes imbriquées
                    if ($nvTabListe > $tabListe) 
                    {
                        echo "<ul>\n";
                    }
                    while ($nvTabListe < $tabListe) 
                    {
                        echo "</ul>\n";
                        $tabListe --;
                    }

                    // on retire la balise mardown indiquand une liste puis on ajoute au fichier une nouvelle ligne a la liste
                    $lig = str_replace("- ", " ", $lig);
                    echo "<li>\n" . $lig  . "</li>\n";
                    $tabListe = $nvTabListe;
                }
                else
                {
                    // si la liste est terminée on la ferme (en fermant toutes les listes imbriquées ouvertes également)
                    while ($tabListe >= 0) 
                    {
                        echo "</ul>\n";
                        $tabListe--;
                    }
                    $listOuverte = false;
                }
            }
            

            //si un tableau est actuellement ouvert, on vérifie si il se poursuit 
            elseif ($tabOuvert == true) 
            {
                if(strpos($lig, "|") !== false)
                {                        
                    // on s'assure que l'on ne traite pas une ligne de délimitation du tableau (sinon on l'ignore)
                    if (strpos($lig, "|-|") === false) 
                    {
                        echo "<tr>\n";
                        $tableau = explode("|", $lig);
                        foreach ($tableau as $numCase => $case) 
                        {
                            if($numCase != "0" and $numCase != count($tableau)-1)
                            {
                                echo "<td>" . $case . "</td>\n";
                            }
                        }
                        echo "</tr>";
                    }
                }
                else
                {
                    echo "</table>\n";
                    $tabOuvert = false;
                }
            }

            //si un bloc de texte préformaté est actuellement ouvert, on vérifie qu'il continue (si oui il continue sinon il est fermée)
            if ($preOuvert == true) 
            {
                if(strpos($lig, "```") !== false)
                {
                    echo "</pre>\n";
                    $preOuvert = false;
                }
                else
                {
                    echo $lig;
                }
            }
            elseif($tabOuvert == false && $listOuverte == false)
            {
                $balise = false;
                for ($idx=0; $idx < 7; $idx++) 
                {
                    if (strpos($lig, $varMd[$idx]) !== false && $balise == false) 
                    {
                        //si une balise est rouvée, on passe la variable vraie, car si aucune n'est reouvée sue la ligne, ce sera un simple paragraphe
                        $balise = true;

                        //ligne particulière si une balise de texte préformatée est retrouvée (car la ligne du fichier md ne contient que ```)
                        if ($idx == 0) 
                        {
                            $preOuvert = true;
                            echo $varHtml[$idx];
                        }
                        elseif ($idx == 2) 
                        {
                            $tabOuvert = true;
                            echo "<table>\n<tr>\n";
                            $tableau = explode("|", $lig);
                            foreach ($tableau as $numCase => $case) 
                            {
                                if($case != "" and $case != "\n")
                                {
                                    echo "<th>" . $case . "</th>\n";
                                }
                            }
                            echo "</tr>";
                        }

                        //sinon, on place la balise html correspondant a celle trouvée, suivi de la ligne actuelle, puis la balise de fin 
                        else 
                        {
                            $lig = str_replace($varMd[$idx], " ", $lig);
                            echo $varHtml[$idx] . "\n" . $lig . $varHtmlFin[$idx] . "\n";
                        }

                        if ($idx == 1) 
                        {
                            $listOuverte = true;
                            $tabListe = 0;
                        }
                    }
                }
                if ($balise == false) 
                {
                    echo "<p>\n" . $lig . "</p>\n";
                }
            }
        }
        else 
        {
            echo "<br>\n";
        }
    }

    function read() {
        static $stdin = null;
        if ($stdin === null) {
        $stdin = fopen('php://stdin', 'r');
        }
        return rtrim(fgets($stdin));
    }
?>

</body>
</html>

<?php
    $htmlContent = ob_get_clean();
    $n_html = "doc-user-$versions[0]-$versions[1]-$versions[2].html";

    file_put_contents($n_html,$htmlContent);
?>
