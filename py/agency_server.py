from travel_agency import *

class AgencyServer:
    def __init__(self, path):
        super().__init__()
        self.city_map = CityMap(path)
        self.journey_list = []
        self.mr_spfa_solver = MinRiskJourneySolver(self.city_map)
