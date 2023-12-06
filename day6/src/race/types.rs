#[derive(Debug)]
pub struct Race {
    pub time: u64,
    pub distance: u64,
}

impl Race {
    pub fn is_wining_charge_time(&self, charge_time: u64) -> bool {
        // we increase the speed by 1mm per 1ms of charging, linearly
        let speed = charge_time;
        let remaining_time = self.time - charge_time;
        let distance_travelled = speed * remaining_time;
        return distance_travelled > self.distance;
    }

    pub fn is_lower_bound(&self, point: u64) -> bool {
        if point == 0 && self.is_wining_charge_time(point) {
            return true;
        }
        return self.is_wining_charge_time(point) && !self.is_wining_charge_time(point - 1);
    }

    pub fn is_upper_bound(&self, point: u64) -> bool {
        // if point >= self.time && self.is_wining_charge_time(self.time) {
        if point == self.time && self.is_wining_charge_time(point) {
            return true;
        }
        return self.is_wining_charge_time(point) && !self.is_wining_charge_time(point + 1);
    }
}
