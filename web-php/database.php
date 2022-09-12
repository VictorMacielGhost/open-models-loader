<?php
const WEBSERVER_VERSION = "0.4";
const DATABASE = "openmodels";
const HOSTNAME = "127.0.0.1";
const USERNAME = "root";
const PASSWORD = "";

$connection = mysqli_connect(HOSTNAME, USERNAME, PASSWORD);
mysqli_query($connection, "CREATE DATABASE IF NOT EXISTS `openmodels`");
mysqli_select_db($connection, DATABASE);
mysqli_query($connection, "CREATE TABLE IF NOT EXISTS `models` (
    `name` TEXT NOT NULL , 
    `baseid` INT NOT NULL ,
     PRIMARY KEY (`name`(32))) ENGINE = InnoDB;
");
if(mysqli_errno($connection)) printf("ERROR! Contact admin to more informations. Error Number <b>%d</b>", mysqli_errno($connection));

?>