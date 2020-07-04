#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct city_map city_map_t;
typedef struct solver solver_t;
typedef struct journey journey_t;
typedef struct int_vector int_vector_t;

city_map_t *read_city_map(const char *path);

int extract_city_num(city_map_t *cm);
int extract_line_num(city_map_t *cm);

void free_city_map(city_map_t *cm);

char *get_city_name(city_map_t *cm, int cid);
int get_city_level(city_map_t *cm, int cid);
double get_city_lat(city_map_t *cm, int cid);
double get_city_lnt(city_map_t *cm, int cid);

int get_line_from(city_map_t *cm, int lid);
int get_line_to(city_map_t *cm, int lid);
int get_line_type(city_map_t *cm, int lid);
int get_line_dep_time(city_map_t *cm, int lid);
int get_line_duration(city_map_t *cm, int lid);

solver_t *init_min_risk_spfa_solver(city_map_t *cm);
journey_t *run_solver(solver_t *solver, int cid, int now, int dest);

void deinit_solver(solver_t *solver);

int get_journey_src(journey_t *journey);
int get_journey_dest(journey_t *journey);
int get_journey_length(journey_t *journey);
double get_journey_risk(journey_t *journey);
int_vector_t *get_journey_route(journey_t *journey);

void deinit_journey(journey_t *journey);

int get_int_at(int_vector_t *xs, int i);
int get_vector_length(int_vector_t *xs);
void deinit_int_vector(int_vector_t *xs);

#ifdef __cplusplus
};
#endif
