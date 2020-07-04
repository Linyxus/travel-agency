#include "travel_agency.h"
#include "CityMap.hh"
#include "Solver.h"
#include "Journey.hh"
#include <cstring>

city_map_t *read_city_map(const char *path) {
    auto pcm = new tagc::CityMap(tagc::io::load_city_map(path));
    return reinterpret_cast<city_map_t *>(pcm);
}

int extract_city_num(city_map_t *pcm) {
    auto cm = reinterpret_cast<tagc::CityMap *>(pcm);

    return cm->num_cities();
}

int extract_line_num(city_map_t *pcm) {
    auto cm = reinterpret_cast<tagc::CityMap *>(pcm);

    return cm->lines().size();
}

void free_city_map(city_map_t *cm) {
    auto pcm = reinterpret_cast<tagc::CityMap *>(cm);
    delete pcm;
}

char *get_city_name(city_map_t *cm, int cid) {
    auto pcm = reinterpret_cast<tagc::CityMap *>(cm);

    return strdup(pcm->cities()[cid].name.c_str());
}

int get_city_level(city_map_t *cm, int cid) {
    auto pcm = reinterpret_cast<tagc::CityMap *>(cm);

    return pcm->cities()[cid].level;
}

double get_city_lat(city_map_t *cm, int cid) {
    auto pcm = reinterpret_cast<tagc::CityMap *>(cm);

    return pcm->cities()[cid].lat;
}

double get_city_lnt(city_map_t *cm, int cid) {
    auto pcm = reinterpret_cast<tagc::CityMap *>(cm);

    return pcm->cities()[cid].lnt;
}

int get_line_from(city_map_t *cm, int lid) {
    auto pcm = reinterpret_cast<tagc::CityMap *>(cm);

    return pcm->lines()[lid].from;
}

int get_line_to(city_map_t *cm, int lid) {
    auto pcm = reinterpret_cast<tagc::CityMap *>(cm);

    return pcm->lines()[lid].to;
}

int get_line_type(city_map_t *cm, int lid) {
    auto pcm = reinterpret_cast<tagc::CityMap *>(cm);

    return tagc::line_type_to_int(pcm->lines()[lid].type);
}

int get_line_dep_time(city_map_t *cm, int lid) {
    auto pcm = reinterpret_cast<tagc::CityMap *>(cm);

    return pcm->lines()[lid].dep_time;
}

int get_line_duration(city_map_t *cm, int lid) {
    auto pcm = reinterpret_cast<tagc::CityMap *>(cm);

    return pcm->lines()[lid].duration;
}

solver_t *init_min_risk_spfa_solver(city_map_t *cm) {
    auto pcm = reinterpret_cast<tagc::CityMap *>(cm);
    tagc::Solver *solver = new tagc::MinRiskSPFASolver(*pcm);

    return reinterpret_cast<solver_t *>(solver);
}

journey_t *run_solver(solver_t *solver, int cid, int now, int dest) {
    auto psolver = reinterpret_cast<tagc::Solver *>(solver);
    auto ret = new tagc::Journey(psolver->solve(cid, now, dest));

    return reinterpret_cast<journey_t *>(ret);
}

void deinit_solver(solver_t *solver) {
    auto psolver = reinterpret_cast<tagc::Solver *>(solver);

    delete psolver;
}

int get_journey_src(journey_t *journey) {
    auto pj = reinterpret_cast<tagc::Journey *>(journey);

    return pj->src();
}

int get_journey_dest(journey_t *journey) {
    auto pj = reinterpret_cast<tagc::Journey *>(journey);

    return pj->dest();
}

int get_journey_length(journey_t *journey) {
    auto pj = reinterpret_cast<tagc::Journey *>(journey);

    return pj->length();
}

double get_journey_risk(journey_t *journey) {
    auto pj = reinterpret_cast<tagc::Journey *>(journey);

    return pj->risk();
}

int_vector_t *get_journey_route(journey_t *journey) {
    auto pj = reinterpret_cast<tagc::Journey *>(journey);

    auto ret = new std::vector<int>{ pj->to_line_id_list() };

    return reinterpret_cast<int_vector_t *>(ret);
}

void deinit_journey(journey_t *journey) {
    auto pj = reinterpret_cast<tagc::Journey *>(journey);
    delete pj;
}

int get_int_at(int_vector_t *xs, int i) {
    auto pxs = reinterpret_cast<std::vector<int> *>(xs);
    return pxs->at(i);
}

int get_vector_length(int_vector_t *xs) {
    auto pxs = reinterpret_cast<std::vector<int> *>(xs);
    return pxs->size();
}

void deinit_int_vector(int_vector_t *xs) {
    auto pxs = reinterpret_cast<std::vector<int> *>(xs);
    delete pxs;
}
