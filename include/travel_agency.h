#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// 本文件为C接口文件。将C++的各种类方法与类的构造、释放包装为C API，编译导出为动态库。

typedef struct city_map city_map_t;
typedef struct solver solver_t;
typedef struct journey journey_t;
typedef struct result result_t;
typedef struct int_vector int_vector_t;
typedef struct double_vector double_vector_t;

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
solver_t *init_limited_time_min_risk_solver(city_map_t *cm);
journey_t *run_solver(solver_t *solver, int cid, int now, int dest);
result_t *maybe_run_solver(solver_t *solver, int cid, int now, int dest, int time_limit);

void deinit_solver(solver_t *solver);

int get_journey_src(journey_t *journey);
int get_journey_dest(journey_t *journey);
int get_journey_dep_time(journey_t *journey);
int get_journey_length(journey_t *journey);
double get_journey_risk(journey_t *journey);
int_vector_t *get_journey_route(journey_t *journey);
double_vector_t *get_journey_step_risk(journey_t *journey);

void deinit_journey(journey_t *journey);

bool get_result_valid(result_t *res);
journey_t *get_result_journey(result_t *res);

void deinit_result(result_t *res);

int get_int_at(int_vector_t *xs, int i);
int get_vector_length(int_vector_t *xs);
void deinit_int_vector(int_vector_t *xs);

double get_double_at(double_vector_t *xs, int i);
int get_dvector_length(double_vector_t *xs);
void deinit_double_vector(double_vector_t *xs);

#ifdef __cplusplus
};
#endif
