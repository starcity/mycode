 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include <sys/types.h>
#include <sys/stat.h>
 
#include <fcntl.h>
 
#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/util.h>
#include <event2/keyvalq_struct.h>
 
 
/* Callback used for the /dump URI, and for every non-GET request:
 * dumps all information to stdout and gives back a trivial 200 ok */
static void
dump_request_cb(struct evhttp_request *req, void *arg)
{

}
 
/* This callback gets invoked when we get any http request that doesn't match
 * any other callback.  Like any evhttp server callback, it has a simple job:
 * it must eventually call evhttp_send_error() or evhttp_send_reply().
 */
static void
send_document_cb(struct evhttp_request *req, void *arg)
{
 

}
 
static void
syntax(void)
{
	fprintf(stdout, "Syntax: http-server <docroot>\n");
}
 
int
main(int argc, char **argv)
{
	struct event_base *base;
	struct evhttp *http;
	struct evhttp_bound_socket *handle;
 
	base = event_base_new();
	if (!base) {
		fprintf(stderr, "Couldn't create an event_base: exiting\n");
		return 1;
	}
 
	/* Create a new evhttp object to handle requests. */
	http = evhttp_new(base);
	if (!http) {
		fprintf(stderr, "couldn't create evhttp. Exiting.\n");
		return 1;
	}
 
	/* The /dump URI will dump all requests to stdout and say 200 ok. */
//	evhttp_set_cb(http, "/dump", dump_request_cb, NULL);
 
	/* We want to accept arbitrary requests, so we need to set a "generic"
	 * cb.  We can also add callbacks for specific paths. */
	evhttp_set_gencb(http, send_document_cb, argv[1]);
 
	/* Now we tell the evhttp what port to listen on */
	handle = evhttp_bind_socket_with_handle(http, "0.0.0.0", 8080);
	if (!handle) {
		fprintf(stderr, "couldn't bind to port %d. Exiting.\n",
		    8080);
		return 1;
	}
 
	event_base_dispatch(base);
 
	return 0;
}
