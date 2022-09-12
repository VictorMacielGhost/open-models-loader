<?php

?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>open.models Loader</title>
</head>
<body>

    <?php
        include "./database.php";
        if(isset($_GET['e']) && $_GET['e'] == "upload")
        {
            $numeroArquivos = 10;
            for($i = $numeroArquivos; $i > 0; $i--)
            {
                $tmp = $_FILES['files']['tmp_name'][$i];
                $name = $_FILES['files']['name'][$i];
                $path = "models/" . $name;
                $baseid = $_POST['baseid'][$i];
                $newid = $_POST['newid'][$i];
                move_uploaded_file($tmp, $path);
                mysqli_query($connection, "INSERT INTO `models` (unique_id, baseid, newid, name, path) VALUES (DEFAULT, '$baseid', '$newid', '$name', '$path');");
                printf("Model %s Inserted in database as url %s.", $name, $path);
            }
        }
    ?>

    <form method='post' action='index.php?e=upload' enctype="multipart/form-data">
        <b>Select file(s) to be load</b><input type="file" multiple name="files">
        <b>Baseid</b><input type="number" name='baseid'>
        <b>newid</b><input type="number" name="newid">
        <input type="submit" value="Insert in database">
    </form>
</body>
</html>