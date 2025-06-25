std_msgs/Header header

# Current gear enumeration
Gear state

# Gear reject flag
bool reject

# Status
bool driver_activity
bool enabled
uint8 trans_curr_gear
bool ignore_driver

uint8 trans_hi_lo
uint16 clutch_position
bool clutch_sw
uint8 clutch_state
bool clutch_driver_activity

# Faults
bool gear_select_system_fault
bool gear_mismatch_flash

uint8 rolling_counter
