# GLEE Sensor Library

This is a library houses standard functionality for sensors used by GLEE. Common types for sensor configuration and data enables efficent control when working with different kinds of sensors. 

# Structure # 
## Data Types ##
* sensor_info_t
* sensor_float_vec_t
* sensor_unit16_vec_t

### Sensor Info: ###
* Id: Identification number
* Name: String representation of name
* Debug: Debugging flag

### Sensor Vectors: ###
The following sensor vector datatypes are used to store standard x,y,z values (could switch to arrays for performance testing)
* sensor_float_vec_t -> <x,y,z>
* sensor_unit16_vec_t -> <x,y,z>

## Child (GLEE_Sensor Dependent) Libraries: ##
* GLEE_TMP117 
* GLEE_AK09940
* GLEE_ICM20602

## Physics Constants ##
TODO: Identified constants used accross sensors and document them here
