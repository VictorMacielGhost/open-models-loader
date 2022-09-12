<<<<<<< HEAD
/*
    @Version 0.1
    @Date 11/09/2022 (BR)
    @Last Edit 20:08 (BR - RJ)
    @Author Victor Ghost

*/

#if defined _openmodels_included
    #endinput
#else 
    #define _openmodels_included
    #define _openmodels_version 0.1
#endif

#if !defined _samp_included
    #tryinclude <a_samp>
    #if !defined _samp_included
        #error Couldn't Locate a_samp Include! Please Insert a_samp before openmodels to load properly.
    #endif
#endif

#if !defined mysql_included
    #tryinclude <a_mysql>
    #if !defined mysql_included
        #error Couldn't Locate a_mysql Include! Please Insert a_mysql before openmodels to load properly.
    #endif
#endif

#define OPENDATABASE "openmodels"
#define OPENUSERNAME "root"
#define OPENPASSWORD ""
#define OPENHOSTNAME "127.0.0.1"
#define OPENTABLE "models"
#define MAX_MODELS (30000)
#define MIN_MODELS (20000)
#define INVALID_OMODEL_ID (0xFFFF)

new bool:ModelUsed[MAX_MODELS];

stock InitOpenModels()
{
    printf("open.models by Victor Ghost Loaded! Version %d Started. Check for new versions in https://github.com/VictorMacielGhost/open-models-loader/releases/", _openmodels_version);

    new MySQL:openmodels_connection = mysql_connect(OPENDATABASE, OPENUSERNAME, OPENPASSWORD, OPENHOSTNAME);
    new query[144];
    mysql_format(openmodels_connection, query, sizeof(query), "SELECT * FROM `%s`;", OPENTABLE);
    mysql_tquery(openmodels_connection, query, "OnOpenModelsInit");

    return 1;
}

static stock GetNewModelId()
{
    for(new i = MIN_MODELS; i < MAX_MODELS; i++)
    {
        if(ModelUsed[i] == true) continue;
        else
        {
            ModelUsed[i] = true;
            return i;
        }
    }
    return INVALID_OMODEL_ID;
}

forward OnOpenModelsInit();
public OnOpenModelsInit()
{
    if(cache_num_rows())
    {
        new rows = cache_num_rows();
        new baseid, newid, txd[32], dff[32];
        for(new i = 0; i < rows; i++)
        {
            newid = GetNewModelId();
            cache_get_value_int(i, "baseid", baseid);
            cache_get_value_name(i, "txd", txd);
            cache_get_value_name(i, "dff", dff);
            AddCharModel(baseid, newid, dff, txd);
            printf("baseid : %d, newid : %d, txd : %s, dff : %s", baseid, newid, txd, dff);
        }
    }
    else return 0;
    return 1;
=======
/*
    @Version 0.1
    @Date 11/09/2022 (BR)
    @Last Edit 20:08 (BR - RJ)
    @Author Victor Ghost

*/

#if defined _openmodels_included
    #endinput
#else 
    #define _openmodels_included
    #define _openmodels_version 0.1
#endif

#if !defined _samp_included
    #tryinclude <a_samp>
    #if !defined _samp_included
        #error Couldn't Locate a_samp Include! Please Insert a_samp before openmodels to load properly.
    #endif
#endif

#if !defined mysql_included
    #tryinclude <a_mysql>
    #if !defined mysql_included
        #error Couldn't Locate a_mysql Include! Please Insert a_mysql before openmodels to load properly.
    #endif
#endif

#define OPENDATABASE "openmodels"
#define OPENUSERNAME "root"
#define OPENPASSWORD ""
#define OPENHOSTNAME "127.0.0.1"
#define OPENTABLE "models"
#define MAX_MODELS (30000)
#define MIN_MODELS (20000)
#define INVALID_OMODEL_ID (0xFFFF)

new bool:ModelUsed[MAX_MODELS];

stock InitOpenModels()
{
    printf("open.models by Victor Ghost Loaded! Version %d Started. Check for new versions in https://github.com/VictorMacielGhost/open-models-loader/releases/", _openmodels_version);

    new MySQL:openmodels_connection = mysql_connect(OPENDATABASE, OPENUSERNAME, OPENPASSWORD, OPENHOSTNAME);
    new query[144];
    mysql_format(openmodels_connection, query, sizeof(query), "SELECT * FROM `%s`;", OPENTABLE);
    mysql_tquery(openmodels_connection, query, "OnOpenModelsInit");

    return 1;
}

static stock GetNewModelId()
{
    for(new i = MIN_MODELS; i < MAX_MODELS; i++)
    {
        if(ModelUsed[i] == true) continue;
        else
        {
            ModelUsed[i] = true;
            return i;
        }
    }
    return INVALID_OMODEL_ID;
}

forward OnOpenModelsInit();
public OnOpenModelsInit()
{
    if(cache_num_rows())
    {
        new rows = cache_num_rows();
        new baseid, newid, txd[32], dff[32];
        for(new i = 0; i < rows; i++)
        {
            newid = GetNewModelId();
            cache_get_value_int(i, "baseid", baseid);
            cache_get_value_name(i, "txd", txd);
            cache_get_value_name(i, "dff", dff);
            AddCharModel(baseid, newid, dff, txd);
            printf("baseid : %d, newid : %d, txd : %s, dff : %s", baseid, newid, txd, dff);
        }
    }
    else return 0;
    return 1;
>>>>>>> f8fa68d3c9de48f1648aced680b04476dcd86e94
}