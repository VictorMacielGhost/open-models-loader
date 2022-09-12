<?php
    $GLOBAL_BASEID = 0; // Change the GLOBAL_BASEID to have one different baseid to global inserts
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
            for($i = 0; $i < count($_FILES['files']['name']); $i++)
            {
                $tmp = $_FILES['files']['tmp_name'][$i];
                $name = $_FILES['files']['name'][$i];
                $path = "../models/" . $name;
                $webpath = "./assets/" . $name;
                move_uploaded_file($tmp, $path);
                copy($path, $webpath);
                mysqli_query($connection, "INSERT INTO `models` (name, baseid) VALUES ('$name', '$GLOBAL_BASEID');");
                printf("Model <b>%s</b> Inserted in database as weburl <b>%s</b> and saved in <b>%s</b> to be loaded in the MP.<br>", $name, $webpath, $path);
            }
        }
    ?>

    <form method='post' action='index.php?e=upload' enctype="multipart/form-data">
        <b>Select file(s) to be load</b><input type="file" multiple name="files[]">
        <input type="submit" value="Insert in database">
        <b><a href='single.htm'>Or Send File By File</a> (Note: In This option you can set the baseid for each model)</b>
    </form>
</body>
</html>