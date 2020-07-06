from travel_agency import *

class AgencyServer:
    def __init__(self, path):
        super().__init__()
        self.next_jid = 1
        self.city_map = CityMap(path)
        self.journey_list = []
        self.planed_journey = dict()
        mr_spfa_solver = MinRiskJourneySolver(self.city_map)
        ltmr_solver = LimitedTimeMinRiskJourneySolver(self.city_map)

        self.solvers = {
            'min_risk': mr_spfa_solver,
            'limited_time_min_risk': ltmr_solver
        }

    def plan(self, tid, src, dest, day, dep_time, time_limit, strategy):
        solver = self.solvers[strategy]
        mj = solver(src, dep_time, dest, time_limit)
        if mj._just:
            rec = JourneyRecord(jid=f'journey-{self.next_jid}', tid=tid, day=day, journey=mj._val)
            self.planed_journey[rec.jid] = rec
            self.next_jid = self.next_jid + 1

            return rec
        else:
            return None
            
