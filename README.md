# Get Started

## Readme.md technical guide for using open.models loader

To use the open.models loader, you should include the following code in your script:
```c
#include <open.models>
```
### Requirements for using open.models

To use open.models, you need to have a MySQL database available. You can use a separate database or the same database that you use for your server, but with a specific table for open.models.


The use of open.mp is optional, as open.models was made with compatibility with old samp scripts in mind


### What does open.models loader do?

The open.models loader helps load models (skins and objects) from the DL version of samp/OMP more quickly.


### How does the web server work?

You will access the website (run by PHP, so make sure you have an Apache server that will process PHP). It will create a database if it does not exist, create a table, and show you the option to upload files to the system. What files are these? Models (.txd and .dff) used by samp.


After the files are uploaded to the database, two folders will have files: assets and models. The assets folder is used so that clients can download the files from the server, and the models folder is for the server to load the files itself.


After completing these steps, simply run your server and enjoy the power of open.models loader!


Check our wiki for more information.


###### open.models loader - Made by Victor Ghost and contributors 2022 ~ 2023