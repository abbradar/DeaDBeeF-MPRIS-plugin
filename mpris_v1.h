#ifndef __MPRIS_SERVER_H_
#define __MPRIS_SERVER_H_
#include <glib-2.0/glib.h>
#include "../../deadbeef.h"

#define MPRIS_INTERFACE         "org.freedesktop.MediaPlayer"
#define MPRIS_SERVICE 			"org.mpris.deadbeef"
#define MPRIS_ROOT_PATH 		"/"
#define MPRIS_PLAYER_PATH 		"/Player"
#define MPRIS_TRACKLIST_PATH 	"/TrackList"

//Method names
#define MPRIS_METHOD_IDENTITY 			"Identity"
#define MPRIS_METHOD_QUIT 				"Quit"
#define MPRIS_METHOD_VERSION			"MprisVersion"
#define MPRIS_METHOD_GETMETA			"GetMetadata"
#define MPRIS_METHOD_GETCURRENTTRACK	"GetCurrentTrack"
#define MPRIS_METHOD_GETLENGTH			"GetLength"
#define MPRIS_METHOD_ADDTRACK			"AddTrack"
#define MPRIS_METHOD_DELTRACK			"DelTrack"
#define MPRIS_METHOD_SETLOOP			"SetLoop"
#define MPRIS_METHOD_SETRANDOM			"SetRandom"
#define MPRIS_METHOD_NEXT				"Next"
#define MPRIS_METHOD_PREV				"Prev"
#define MPRIS_METHOD_PAUSE				"Pause"
#define MPRIS_METHOD_STOP				"Stop"
#define MPRIS_METHOD_PLAY				"Play"
#define MPRIS_METHOD_REPEAT				"Repeat"
#define MPRIS_METHOD_GETSTATUS			"GetStatus"
#define MPRIS_METHOD_GETCAPS			"GetCaps"
#define MPRIS_METHOD_VOLUMESET			"VolumeSet"
#define MPRIS_METHOD_VOLUMEGET			"VolumeGet"
#define MPRIS_METHOD_POSITIONSET		"PositionSet"
#define MPRIS_METHOD_POSITIONGET		"PositionGet"

//Signal names
#define MPRIS_SIGNAL_TRACKCHANGE		"TrackChange"
#define MPRIS_SIGNAL_STATUSCHANGE		"StatusChange"
#define MPRIS_SIGNAL_CAPSCHANGE			"CapsChange"
#define MPRIS_SIGNAL_TRACKLISTCHAGE		"TrackListChange"

#define MPRIS__DEBUG 1

#ifndef MPRIS__DEBUG
	#define debug(...)
#else
	#define debug(...)  do_debug(__VA_ARGS__)
#endif

void do_debug(const char *fmt, ...);

/*
 * The deadbeef apis
 */
extern DB_functions_t *deadbeef;

/*
 * The server struct.
 */
typedef struct _DB_mpris_server DB_mpris_server;

/*
 * return status.
 */
enum{
	DB_MPRIS_OK,
	DB_MPRIS_ERROR
};

/*
 * Start and stop the MPRIS server.
 * Usage:
 * 		db_mpris_server *srv;
 * 		db_mpris_server_start(&srv);
 * 		...
 * 		db_mpris_server_stop(srv);
 * 		srv = NULL;
 */
gint DB_mpris_server_start(DB_mpris_server **srv);
gint DB_mpris_server_stop(DB_mpris_server *srv);

/*
 * emit the signal
 */
void DB_mpris_emit_trackchange();
void DB_mpris_emit_stauschange();
void DB_mpris_emit_capschange();

void DB_mpris_emit_tracklistchange();
#endif
