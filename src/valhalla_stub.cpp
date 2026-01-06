// Stub implementations for valhalla_wrapper functions
// Used when Valhalla is not available at compile time
// All functions return errors indicating Valhalla is not available

#include "valhalla_wrapper.h"
#include <cstring>

static const char *g_stub_error = "Valhalla not available: extension built without Valhalla support";

extern "C" {

ValhallaRouter *valhalla_init(const char *config_path) {
	(void)config_path;
	return nullptr;
}

ValhallaRouter *valhalla_init_from_json(const char *config_json) {
	(void)config_json;
	return nullptr;
}

int valhalla_is_ready(ValhallaRouter *router) {
	(void)router;
	return 0;
}

int valhalla_route(ValhallaRouter *router, double lat1, double lon1, double lat2, double lon2, const char *costing,
                   ValhallaRouteResult *out_result, ValhallaPoint *out_points, int max_points) {
	(void)router;
	(void)lat1;
	(void)lon1;
	(void)lat2;
	(void)lon2;
	(void)costing;
	(void)out_result;
	(void)out_points;
	(void)max_points;
	return -1;
}

int valhalla_route_wkt(ValhallaRouter *router, const char *from_wkt, const char *to_wkt, const char *costing,
                       ValhallaRouteResult *out_result, ValhallaPoint *out_points, int max_points) {
	(void)router;
	(void)from_wkt;
	(void)to_wkt;
	(void)costing;
	(void)out_result;
	(void)out_points;
	(void)max_points;
	return -1;
}

int valhalla_route_wkb(ValhallaRouter *router, const unsigned char *from_wkb, int from_wkb_len,
                       const unsigned char *to_wkb, int to_wkb_len, const char *costing,
                       ValhallaRouteResult *out_result, ValhallaPoint *out_points, int max_points) {
	(void)router;
	(void)from_wkb;
	(void)from_wkb_len;
	(void)to_wkb;
	(void)to_wkb_len;
	(void)costing;
	(void)out_result;
	(void)out_points;
	(void)max_points;
	return -1;
}

int valhalla_matrix(ValhallaRouter *router, const double *src_lats, const double *src_lons, int src_count,
                    const double *dst_lats, const double *dst_lons, int dst_count, const char *costing,
                    ValhallaMatrixEntry *out_entries) {
	(void)router;
	(void)src_lats;
	(void)src_lons;
	(void)src_count;
	(void)dst_lats;
	(void)dst_lons;
	(void)dst_count;
	(void)costing;
	(void)out_entries;
	return -1;
}

int valhalla_isochrone(ValhallaRouter *router, double lat, double lon, const double *contour_minutes, int contour_count,
                       const char *costing, ValhallaIsochroneContour *out_contours) {
	(void)router;
	(void)lat;
	(void)lon;
	(void)contour_minutes;
	(void)contour_count;
	(void)costing;
	(void)out_contours;
	return -1;
}

int valhalla_locate(ValhallaRouter *router, double lat, double lon, const char *costing, double *out_lat,
                    double *out_lon) {
	(void)router;
	(void)lat;
	(void)lon;
	(void)costing;
	(void)out_lat;
	(void)out_lon;
	return -1;
}

char *valhalla_request(ValhallaRouter *router, const char *action, const char *request_json) {
	(void)router;
	(void)action;
	(void)request_json;
	return nullptr;
}

void valhalla_free_string(char *str) {
	(void)str;
}

const char *valhalla_last_error(void) {
	return g_stub_error;
}

void valhalla_free(ValhallaRouter *router) {
	(void)router;
}

const char *valhalla_version(void) {
	return "stub-no-valhalla";
}

} // extern "C"
