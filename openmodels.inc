/*
    @Version 0.4
    @Date 12/09/2022 (BR)
    @Last Edit 19:01 (BR - RJ)
    @Author Victor Ghost

*/

#if defined _openmodels_included
    #endinput
#else 
    #define _openmodels_included
    #define _openmodels_version "0.5"
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
#define MIN_MODELS (20001)

new bool:ModelUsed[MAX_MODELS];

stock InitOpenModels()
{
    printf("open.models by Victor Ghost Loaded! Version %s Started. Check for new versions in https://github.com/VictorMacielGhost/open-models-loader/releases/", _openmodels_version);

    new MySQL:openmodels_connection = mysql_connect(OPENHOSTNAME, OPENUSERNAME, OPENPASSWORD, OPENDATABASE);
    if(mysql_errno()) printf("[MySQL - open.models loader]: ERROR To Connect to the database! Please Check Tutorials or contact administrator to possible errors. Error Code %d", mysql_errno());
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

static stock RemoveExtension(string[])
{
    new dot = strfind(string, ".");
    if(dot == -1) return 0;
    strdel(string, dot, strlen(string));
    return 1;
}

forward OnOpenModelsInit();
public OnOpenModelsInit()
{
    if(cache_num_rows())
    {
        new rows = cache_num_rows();
        new baseid, newid, name[32], dff[32], txd[32];
        for(new i = 0; i < rows; i++)
        {
            newid = GetNewModelId();
            cache_get_value_int(i, "baseid", baseid);
            cache_get_value_name(i, "name", name);
            RemoveExtension(name);
            format(dff, sizeof(dff), "%s.dff", name);
            format(txd, sizeof(txd), "%s.txd", name);
            AddCharModel(baseid, newid, dff, txd);
            printf("[Debug]: Skin %d Added. Baseid : %d, TXD : %s,\n\
            DFF : %s, skiname (without extension) : %s", newid, baseid, txd, dff, name);
        }
    }
    else return 0;
    return 1;
}