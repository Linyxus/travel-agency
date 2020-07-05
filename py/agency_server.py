from travel_agency import *

class AgencyServer:
    def __init__(self, path):
        super().__init__()
        self.next_jid = 0
        self.city_map = CityMap(path)
        self.journey_list = []
        self.planed_journey = dict()
        self.mr_spfa_solver = MinRiskJourneySolver(self.city_map)

    def plan(self, tid, src, dest, day, dep_time, strategy):
        self.jid = self.jid + 1
        if strategy == 'min_risk':
            journey = self.mr_spfa_solver(src, dest, dep_time)
            rec = JourneyRecord(jid=f'journey-{self.next_jid}', tid=tid, day=day, journey=journey)
            self.planed_journey[rec.jid] = rec

            return rec
            
