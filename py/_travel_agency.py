import ctypes

lib = ctypes.cdll.LoadLibrary("../build/default/libtravel_agency.dylib")

# read_city_map
lib.read_city_map.argtypes = [ctypes.c_char_p]
lib.read_city_map.restype = ctypes.c_void_p

# extract_city_num
lib.extract_city_num.argtypes = [ctypes.c_void_p]
lib.extract_city_num.restype = ctypes.c_int

# extract_line_num
lib.extract_line_num.argtypes = [ctypes.c_void_p]
lib.extract_line_num.restype = ctypes.c_int

# free_city_map
lib.free_city_map.argtypes = [ctypes.c_void_p]
lib.free_city_map.restype = ctypes.c_void_p

# get_city_name
lib.get_city_name.argtypes = [ctypes.c_void_p, ctypes.c_int]
lib.get_city_name.restype = ctypes.c_char_p

# get_city_level
lib.get_city_level.argtypes = [ctypes.c_void_p, ctypes.c_int]
lib.get_city_level.restype = ctypes.c_int

# get_city_lat
lib.get_city_lat.argtypes = [ctypes.c_void_p, ctypes.c_int]
lib.get_city_lat.restype = ctypes.c_double

# get_city_lnt
lib.get_city_lnt.argtypes = [ctypes.c_void_p, ctypes.c_int]
lib.get_city_lnt.restype = ctypes.c_double

# get_line_from
# int get_line_from(city_map_t *cm, int lid);
lib.get_line_from.argtypes = [ctypes.c_void_p, ctypes.c_int]
lib.get_line_from.restype = ctypes.c_int

# get_line_to
# int get_line_to(city_map_t *cm, int lid);
lib.get_line_to.argtypes = [ctypes.c_void_p, ctypes.c_int]
lib.get_line_to.restype = ctypes.c_int

# get_line_type
# int get_line_type(city_map_t *cm, int lid);
lib.get_line_type.argtypes = [ctypes.c_void_p, ctypes.c_int]
lib.get_line_type.restype = ctypes.c_int

# get_line_dep_time
# int get_line_dep_time(city_map_t *cm, int lid);
lib.get_line_dep_time.argtypes = [ctypes.c_void_p, ctypes.c_int]
lib.get_line_dep_time.restype = ctypes.c_int

# get_line_duration
# int get_line_duration(city_map_t *cm, int lid);
lib.get_line_duration.argtypes = [ctypes.c_void_p, ctypes.c_int]
lib.get_line_duration.restype = ctypes.c_int

# init_min_risk_spfa_solver
lib.init_min_risk_spfa_solver.argtypes = [ctypes.c_void_p]
lib.init_min_risk_spfa_solver.restype = ctypes.c_void_p

# run_solver
lib.run_solver.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_int, ctypes.c_int]
lib.run_solver.restype = ctypes.c_void_p

# deinit_solver
# void deinit_solver(solver_t *solver);
lib.deinit_solver.argtypes = [ctypes.c_void_p]
lib.deinit_solver.restype = ctypes.c_void_p

# get_journey_src
# int get_journey_src(journey_t *journey);
lib.get_journey_src.argtypes = [ctypes.c_void_p]
lib.get_journey_src.restype = ctypes.c_int

# get_journey_dest
# int get_journey_dest(journey_t *journey);
lib.get_journey_dest.argtypes = [ctypes.c_void_p]
lib.get_journey_dest.restype = ctypes.c_int

# get_journey_length
# int get_journey_length(journey_t *journey);
lib.get_journey_length.argtypes = [ctypes.c_void_p]
lib.get_journey_length.restype = ctypes.c_int

# get_journey_risk
# double get_journey_risk(journey_t *journey);
lib.get_journey_risk.argtypes = [ctypes.c_void_p]
lib.get_journey_risk.restype = ctypes.c_double

# get_journey_route
# int_vector_t *get_journey_route(journey_t *journey);
lib.get_journey_route.argtypes = [ctypes.c_void_p]
lib.get_journey_route.restype = ctypes.c_void_p

# deinit_journey
lib.deinit_journey.argtypes = [ctypes.c_void_p]
lib.deinit_journey.restype = ctypes.c_void_p

# get_int_at
# int get_int_at(int_vector_t *xs, int i);
lib.get_int_at.argtypes = [ctypes.c_void_p, ctypes.c_int]
lib.get_int_at.restype = ctypes.c_int

# get_vector_length
# int get_vector_length(int_vector_t *xs);
lib.get_vector_length.argtypes = [ctypes.c_void_p]
lib.get_vector_length.restype = ctypes.c_int

# deinit_int_vector
lib.deinit_int_vector.argtypes = [ctypes.c_void_p]
lib.deinit_int_vector.restype = ctypes.c_void_p
