<?php

// GENERATION DU PHP VERS HTML

ob_start();

// CODE PRINCIPAL

$directory = './';  // Chemin du répertoire à parcourir
// * : pour changer de dossier, il faudra changer $directory
$files = glob($directory . '*.c');
$f_config = file('config');

// On récupère les données après le "="
$n_config = array();
$l_config = 0;
foreach($f_config as $config){
    $config = explode('=',$config);
    $n_config[$l_config] = $config[1];
    $l_config++;
}
// $n_config contient les configurations
// on récupère chaque numéro de version
$versions = explode('.',$n_config[2]);
// [0] = MAJOR
// [1] = MINOR
// [2] = BUILD
$major = $versions[0];
$minor = $versions[1];
$build = $versions[2];

if(isset($argv[1])){ // si un argument est passé en paramètre lors de l'exécution du script !
    $parametre = $argv[1];

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

// Réécrire le fichier config
file_put_contents('config', implode('', $f_config));

$now = new DateTime();


// pour les commentaires
$debut = "/**";
$brief = "\brief";
$detail = "\detail";
$param = "\param";
$return = "\return";
$fin = "*/";

// pour savoir où les placer dans le HTML
$defconst = "/#define/";
$deb_struct = "/^struct/"; // les / sont utiles pour preg_match();
$fin_struct = "/typedef struct/";
$globales = "VARIABLES GLOBALES";
$fonction = "FONCTIONS";

?>
<!DOCTYPE html>
<html>
    <head>
        <title>Documentation Technique</title>
        <meta charset="utf-8">
        <link rel="stylesheet" href="style.css?">
    </head>
    <body>
        <main>
            <header>
                <h1 id="test">Documentation technique</h1>
                <h2><?php echo $n_config[0] ?></h2>
                <h2>Produit : <?php echo $n_config[1] ?></h2>
                <h3>Version : <?php echo $versions[0].".".$versions[1].".".$versions[2] ?> </h3>
                <h4>Date : <?php echo $now->format('d M Y H:i:s'); ?></h4>
            </header>
            <?php
            $i = 0;
            foreach($files as $file){
                $previousMatch = false;
                $dansStruct = false;
                $i_ligne = 0;
                $lines = file($file);
                while(strchr($file,('/'))){ // pour ne récupérer que le nom du fichier
                    $file = substr($file,1);
                }
                echo "
                <section>
                    <article class=\"boxes-art\">
                        <header>
                            <h1>".$file."</h1>
                        </header>
                        <dl>
                            <dt>Table des matières :</dt>
                            <dd><a href=\"#defines-".$i."\">.   DEFINES</a></dd>
                            <dd><a href=\"#structures-".$i."\">.   STRUCTURES</a></dd>
                            <dd><a href=\"#globales-".$i."\">.   GLOBALES</a></dd>
                            <dd><a href=\"#fonctions-".$i."\">.   FONCTIONS</a></dd>
                        </dl>
                        <article class=\"boxes-art\">
                            <span id=\"defines-".$i."\">
                                <hr>
                                <h2>DEFINES</h2>
                                <hr>
                            </span>";
                foreach($lines as $line){
                    if(preg_match($defconst,$line)){
                        $define = explode("/",$line);
                        if(isset($define[1])){
                            $define[1] = trim($define[1],'*');
                            $define[1] = "<p>".$define[1]."</p>";
                        } else {
                            $define[1] = "";
                        }
                        echo "
                            <article class=\"boxes-content\">
                                <span>
                                    <p>".$define[0]."</p>
                                    <p>".$define[1]."</p>
                                </span>
                            </article>
                        "; // le / apparaissant uniquement dans le commentaire
                        $previousMatch = true;
                    }
                    $i_ligne++;
                }
                $i_ligne = 0;
                echo "
                </article>
                <article class=\"boxes-art\">
                    <span id=\"structures-".$i."\">
                        <hr>
                        <h2>STRUCTURES</h2>
                        <hr>
                    </span>
                    <article class=\"boxes-content\">";
                    foreach ($lines as $line) {
                        if (preg_match($deb_struct, $line)) {
                            echo "<span>";
                            $i_temp = $i_ligne;
                            $dansStruct = true;
                            echo "<p>"; // Balise pour $structure[0]
                            echo $line."<br>";
                            while ($dansStruct) {
                                $structure = explode("/", $lines[$i_temp+1]);
                                if (preg_match($fin_struct, $lines[$i_temp+1])) {
                                    $dansStruct = false;
                                } else {
                                    if (isset($structure[0])) {
                                        echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;". $structure[0] . "<br>";
                                    }
                                }
                                $i_temp++;
                            }
                            echo $structure[0];
                            echo "</p>";
                            $i_temp = $i_ligne;
                            echo "<p>"; // Balise pour $structure[1]
                            $dansStruct = true;
                            while ($dansStruct) {
                                $structure = explode("/", $lines[$i_temp]);
                                if (isset($structure[1])) {
                                    $structure[1] = trim($structure[1],'*');
                                    echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" . $structure[1] . "<br>";
                                } else {
                                    echo "<br>";
                                }
                                if (preg_match($fin_struct, $lines[$i_temp])) {
                                    $dansStruct = false;
                                }
                                $i_temp++;
                            }
                            echo "</p>";
                            echo "</span>";
                        }
                        $i_ligne++;
                    }
                    $i_ligne = 0;
                    echo "</article>
                    </article>";
                    echo "<article class=\"boxes-art\">
                    <span id=\"globales-".$i."\">
                        <hr>
                        <h2>GLOBALES</h2>
                        <hr>
                    </span>
                    <article class=\"boxes-content\">";
                    foreach($lines as $line){
                        if(strpos($line,$globales)){
                            echo "<span>";
                            echo "<p>";
                            $i_temp = $i_ligne+1;
                            // si une ligne contient "VARIABLES GLOABLES" ...
                            while(!empty(trim($lines[$i_temp]))){
                                $globale = explode('/',$lines[$i_temp]);
                                echo $globale[0]."<br>";
                                $i_temp++;
                            }
                            echo "</p>";
                            echo "<p>";
                            $i_temp = $i_ligne+1;
                            while(!empty(trim($lines[$i_temp]))){
                                $globale = explode('/',$lines[$i_temp]);
                                $globale[1] = trim($globale[1],'*');
                                echo $globale[1]."<br>";
                                $i_temp++;
                            }
                            echo "</p>";
                            echo "</span>";
                            $i_temp = 0;
                        }
                        $i_ligne++;
                    }
                    $i_ligne = 0;
                    echo "</article>
                    </article>";
                    echo "<article class=\"boxes-art\">
                    <span id=\"fonctions-".$i."\">
                        <hr>
                        <h2>FONCTIONS</h2>
                        <hr>
                    </span>
                    <article class=\"boxes-content\">";
                foreach($lines as $line){
                    if(strpos($line,$fonction)){
                        $commentaire = array();
                        $comm_ind = 0;
                        $i_temp = $i_ligne+1;
                        while(!strstr($lines[$i_temp],$fin)){ // la boucle prend uniquement le commentaire
                            $lines[$i_temp] = trim($lines[$i_temp]);
                            $commentaire[$comm_ind] = $lines[$i_temp];
                            $comm_ind++;
                            $i_temp++;
                        }
                        $entete = str_replace('{','',$lines[$i_temp+1]);
                        $i_temp = 0;
                        echo "<span>";
                        echo "<p>".$entete."</p>";
                        echo "<p>";
                        foreach($commentaire as $comm){
                            $comm = trim($comm,'*');
                            echo $comm."<br>";
                        }
                        echo "</p>";
                        echo "</span>";
                    }
                    $i_ligne++;
                }
                $i_ligne = 0;
                echo "</article></article>";
                echo "</section>";
                $i++;
            }
            ?>
            <section>
                <article class="boxes-art">
                    <header>
                        <h1></h1>
                    </header>
                </article>
            </section>
        </main>
    </body>
</html>
<?php

// Fin de la mise en mémoire, on récupère le contenu
$htmlContent = ob_get_clean();

$n_html = "doc-tech-$versions[0]-$versions[1]-$versions[2].html";

file_put_contents($n_html,$htmlContent);

?>