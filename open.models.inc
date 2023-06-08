/*
    @Version 0.6
    @Date 12/09/2022 (BR)
    @Last Edit 07/06/2023 (BRAZIL)
    @Author Victor Ghost

*/

/*
    native InitOpenModels();
    native DisableOpenModelsWriteLogs();
    native UseOpenModelsCustomConnect(const hostname[], const username[], const password[], const database[]);
    forward OnModelLoad(ModelType:type, newid, baseid, const dff[], const txd[], const modelname[]);
*/

#if defined _openmodels_included
    #endinput
#else 
    #define _openmodels_included
    #define _openmodels_version "0.6"
#endif

#if !defined _INC_open_mp
    #tryinclude <open.mp>
    #if !defined _INC_open_mp
        #warning Couldn't Locate open.mp Include! Please Insert open.mp before openmodels to load properly. Trying samp version
    #endif
#endif

#if !defined _samp_included
    #tryinclude <a_samp>
    #if !defined _samp_included
        #error Couldn't Locate a_samp Include! Please Insert a_samp or open.mp before openmodels to load properly.
        #endinput
    #endif
#endif

#if !defined mysql_included
    #tryinclude <a_mysql>
    #if !defined mysql_included
        #error Couldn't Locate a_mysql Include! Please Insert a_mysql before openmodels to load properly.
        #endinput
    #endif
#endif

// Macros

#define     _openmodelsTable            "models"

#define     MAX_SKIN_MODELS_ID          (30000)
#define     MIN_SKIN_MODELS_ID          (20000)
#define     MAX_OBJECTS_MODELS_ID       (-30000)
#define     MIN_OBJECTS_MODELS_ID       (-10000)
#define     INVALID_OPEN_MODEL_ID       (0xFFFFF)

// Local data and tags

static enum ModelType:_openmodels_model_types
{
    ModelType:MODEL_TYPE_SKIN,
    ModelType:MODEL_TYPE_OBJECT
};

static enum LogType:_openmodels_log_types
{
    LogType:LOG_TYPE_WARNING,
    LogType:LOG_TYPE_ERROR,
    LogType:LOG_TYPE_INFO,
    LogType:LOG_TYPE_DEBUG
};

static enum _openmodels_custom_connection
{
    string:_hostname[64],
    string:_username[64],
    string:_password[64],
    string:_database[64]
};

stock static 
    gSkinsIdsUsed = MIN_SKIN_MODELS_ID;

stock static 
    gObjectsIdsUsed = MIN_OBJECTS_MODELS_ID;

stock static 
    CustomConnectionInfo[_openmodels_custom_connection];

stock static
    MySQL:gOpenModelsConnection;

stock static 
    bool:gUsingCustomConnection;
static 
    bool:gOpenModelsWriteLogsFile = true;

// Callbacks and special type returns functions

forward OpenModels_OnDataReceive(ModelType:type);
forward MySQL:CustomConnection();
forward OnModelLoad(ModelType:type, newid, baseid, const dff[], const txd[], const modelname[]);

// open.models procedures and functions

/**
 * Used to start OpenModels (if not used, nothing will happens, only the code will be generate in the AMX) (It was made because samp / omp let you load models whatever you want)
 */
stock InitOpenModels()
{
    #if defined _INC_open_mp
        if(GetConsoleVarAsInt("openmodels_Started") == 1)
        {
            OpenModels_Log("open.models started twice, ignoring second Init request.", LOG_TYPE_DEBUG);
            return 1;
        }
    #else
        if(GetServerVarAsInt("openmodels_Started") == 1)
        {
            OpenModels_Log("open.models started twice, ignoring second Init request.", LOG_TYPE_DEBUG);
            return 1;
        }
    #endif
    
    new string:_strlog_[148];
    format(_strlog_, sizeof(_strlog_), "open.models by Victor Ghost Loaded! Version %s Started. Check for new versions in https://github.com/VictorMacielGhost/open-models-loader/releases/", _openmodels_version);
    OpenModels_Log(_strlog_, LOG_TYPE_INFO);

    gOpenModelsConnection = (gUsingCustomConnection) ?  CustomConnection() : mysql_connect_file();

    if(mysql_errno())
    {
        format(_strlog_, sizeof(_strlog_), "Couldn't Connect to the database! Please Check Tutorials or contact an administrator to check for possible errors. MySQL Error Code %d", mysql_errno());
        OpenModels_Log(_strlog_, LOG_TYPE_ERROR);
        return 0;
    }

    #if defined _openmodelsDEBUG
        OpenModels_Log("Connected to the database.", LOG_TYPE_DEBUG);
    #endif

    new string:query[144];
    mysql_format(gOpenModelsConnection, query, sizeof(query), "SELECT * FROM `%e` WHERE `model_type` = '0' LIMIT %d;", _openmodelsTable, (MAX_SKIN_MODELS_ID - MIN_SKIN_MODELS_ID));
    #if defined _openmodelsDEBUG
        OpenModels_Log("Executing query to load skins...", LOG_TYPE_DEBUG);
    #endif
    mysql_tquery(gOpenModelsConnection, query, "OpenModels_OnDataReceive", "i", _:MODEL_TYPE_SKIN);

    return 1;
}

static stock GetNewModelId(ModelType:type = MODEL_TYPE_SKIN)
{
    if(type == MODEL_TYPE_SKIN)
    {
        #if defined _openmodelsDEBUG
            OpenModels_Log("Requesting model type skin ID...", LOG_TYPE_DEBUG);
        #endif
        if(gSkinsIdsUsed < MAX_SKIN_MODELS_ID)
        {
            #if defined _openmodelsDEBUG
            new string:_str_log[128];
            format(_str_log, sizeof(_str_log), "Model type skin ID found. Id: %d", gSkinsIdsUsed);
            OpenModels_Log(_str_log, LOG_TYPE_DEBUG);
            #endif
            return gSkinsIdsUsed++;
        }
        else
        {
            #if defined _openmodelsDEBUG
                OpenModels_Log("No model type skin ID found. Limit reachead.", LOG_TYPE_DEBUG);
            #endif
            return INVALID_OPEN_MODEL_ID;
        }
    }
    else if(type == MODEL_TYPE_OBJECT)
    {
        #if defined _openmodelsDEBUG
            OpenModels_Log("Requesting model type object ID...", LOG_TYPE_DEBUG);
        #endif
        return INVALID_OPEN_MODEL_ID;
    }
    else 
    {
        OpenModels_Log("Invalid model type passed to GetNewModelId", LOG_TYPE_ERROR);
    }
    return INVALID_OPEN_MODEL_ID;
}

static stock RemoveExtension(string[])
{
    new dot = strfind(string, ".");
    if(dot == -1) return 0;
    strdel(string, dot, strlen(string));
    return 1;
}

static stock abs(number)
{
    if(number > 0) return number;
    else return (number * -1);
}

static stock OpenModels_Log(const text[], LogType:type = LOG_TYPE_WARNING)
{
    new string:prefix[32];
    switch(type)
    {
        case LOG_TYPE_DEBUG:
        {
            format(prefix, sizeof(prefix), ">>> [open.models] [Debug]:");
        }
        case LOG_TYPE_ERROR:
        {
            format(prefix, sizeof(prefix), ">>> [open.models] [Error]:");
        }
        case LOG_TYPE_WARNING:
        {
            format(prefix, sizeof(prefix), ">>> [open.models] [Warning]:");
        }
        case LOG_TYPE_INFO:
        {
            format(prefix, sizeof(prefix), ">>> [open.models] [Info]:");
        }
        default:
        {
            format(prefix, sizeof(prefix), ">>> [open.models] [Unknown]:");
        }
    }
    printf("%s %s", prefix, text);
    if(gOpenModelsWriteLogsFile)
    {
        new File:log_file = fopen("openmodels.log", io_append);
        new string:_str_log[1024];
        new date[3];
        new hour[3];
        getdate(date[0], date[1], date[2]);
        gettime(hour[0], hour[1], hour[2]);
        format(_str_log, sizeof(_str_log), "[%02d/%02d/%02d : %02d:%02d:%02d (%d)]: %s %s\n", date[2], date[1], date[0], hour[0], hour[1], hour[2], gettime(), prefix, text);
        fwrite(log_file, _str_log);
        fclose(log_file);
    }
    return;
}

/**
 * Disable open.models loader to write .log files, making only write logs on server_log (or log.txt if using omp)
 */
stock DisableOpenModelsWriteLogs()
{
    gOpenModelsWriteLogsFile = false;
    return;
}

/**
 * Make open.models use custom mysql connection parameter, instead of using mysql.ini
 */
stock UseOpenModelsCustomConnect(const hostname[], const username[], const password[], const database[])
{
    gUsingCustomConnection = true;
    format(CustomConnectionInfo[_hostname], sizeof(CustomConnectionInfo[_hostname]), "%s", hostname);
    format(CustomConnectionInfo[_username], sizeof(CustomConnectionInfo[_username]), "%s", username);
    format(CustomConnectionInfo[_password], sizeof(CustomConnectionInfo[_password]), "%s", password);
    format(CustomConnectionInfo[_database], sizeof(CustomConnectionInfo[_database]), "%s", database);
    return;
}

static MySQL:CustomConnection()
{
    return mysql_connect(CustomConnectionInfo[_hostname], CustomConnectionInfo[_username], CustomConnectionInfo[_password], CustomConnectionInfo[_database]);
}

public OpenModels_OnDataReceive(ModelType:type)
{
    if(type == MODEL_TYPE_SKIN)
    {
        if(cache_num_rows())
        {
            new rows = cache_num_rows();
            #if defined _openmodelsDEBUG
                new string:_str_log[256];
                format(_str_log, sizeof(_str_log), "%d Skin models found on database", rows);
                OpenModels_Log(_str_log, LOG_TYPE_DEBUG);
            #endif
            new baseid, newid, string:name[64], string:dff[64], string:txd[64];
            for(new i = 0; i < rows; i++)
            {
                newid = GetNewModelId(type);
                cache_get_value_int(i, "baseid", baseid);
                cache_get_value_name(i, "name", name);
                RemoveExtension(name);
                format(dff, sizeof(dff), "%s.dff", name);
                format(txd, sizeof(txd), "%s.txd", name);
                AddCharModel(baseid, newid, dff, txd);
                CallLocalFunction("OnModelAdded", "iiisss", _:MODEL_TYPE_SKIN, newid, baseid, dff, txd, name);
                #if defined _openmodelsDEBUG
                    format(_str_log, sizeof(_str_log), "Skin %d Added. Baseid : %d, TXD : %s,\n\
                    DFF : %s, skiname (without extension) : %s", newid, baseid, txd, dff, name);
                    OpenModels_Log(_str_log, LOG_TYPE_DEBUG);
                #endif
            }
        }
        else
        {
            #if defined _openmodelsDEBUG
                OpenModels_Log("No skin models found on database", LOG_TYPE_DEBUG);
            #endif
        }
    }
    else
    {
        if(cache_num_rows())
        {
           new rows = cache_num_rows();
            #if defined _openmodelsDEBUG
                    new string:_str_log[256];
                    format(_str_log, sizeof(_str_log), "%d Objects models found on database", rows);
                    OpenModels_Log(_str_log, LOG_TYPE_DEBUG);
                #endif
            new baseid, newid, string:name[64], string:dff[64], string:txd[64];
            for(new i = 0; i < rows; i+=2) // += 2 because database inserts twice, dff and txd file names and we load both at once
            {
                newid = GetNewModelId(type);
                cache_get_value_int(i, "baseid", baseid);
                cache_get_value_name(i, "name", name);
                RemoveExtension(name);
                format(dff, sizeof(dff), "%s.dff", name);
                format(txd, sizeof(txd), "%s.txd", name);
                AddCharModel(baseid, newid, dff, txd);
                CallLocalFunction("OnModelAdded", "iiisss", _:MODEL_TYPE_OBJECT, newid, baseid, dff, txd, name);
                #if defined _openmodelsDEBUG
                    format(_str_log, sizeof(_str_log), "Object %d Added. Baseid : %d, TXD : %s,\n\
                    DFF : %s, objectname (without extension) : %s", newid, baseid, txd, dff, name);
                    OpenModels_Log(_str_log, LOG_TYPE_DEBUG);
                #endif
            }
        }
        else
        {
            #if defined _openmodelsDEBUG
                OpenModels_Log("No object models found on database", LOG_TYPE_DEBUG);
            #endif
        }
    }
    mysql_close(gOpenModelsConnection);
    OpenModels_Log("Finish loading models. MySQL connection closed. Thanks for use open.models!", LOG_TYPE_INFO);
    return;
}