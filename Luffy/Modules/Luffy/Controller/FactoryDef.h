#ifndef _FACTORY_DEF_H
#define _FACTORY_DEF_H


#define KEYWORD_STATIONNAME	"STATION"
#define KEYWORD_MODELNAME	"MODEL_NAME"
#define KEYWORD_WORKINGDIR	"WORKING_DIR"
#define KEYWORD_CONFIGDIR	"CONFIG_DIR"
#define KEYWORD_FORCEPASS	"FORCEPASS"
#define KEYWORD_LOGDIR		"DLL_LOG_PATH"
#define KEYWORD_TOOLVERSION	"TOOL_VERSION"
#define KEYWORD_COMPORTLIST	"COM"
#define KEYWORD_CURSLOT		"CURRENT_SLOT"
#define KEYWORD_CURCOMPORT	"CURRENT_COMPORT"


typedef void (*p_fn_event_callback)(const char* szEvent, long nParam);
typedef void (*p_fn_log_callback) (const char* szLogSink , const char* szLog, int iSlot) ;
typedef void (*p_fn_processor_callback)(const char* szEvent, long nParam, int iSlot);


#endif // End of #ifndef _FACTORY_DEF_H
