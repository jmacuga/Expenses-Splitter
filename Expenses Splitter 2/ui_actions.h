#pragma once
#include "Trip.h"

void launch_app(Trip& Trip);
void add_new_trip(Trip& trip);
void interface(Trip &trip);
void show_people(Trip& const trip);
void load_history(Trip &curr_trip);
void add_participant(Trip &trip_to_init);
void add_transactions(Trip &trip_to_init);
void add_singular_transaction();
void settle(Trip& trip);
void add_collective_transaction(Trip &trip_to_init);
void add_specific_transaction(Trip &trip_to_init);
<<<<<<< HEAD
void set_atts_action(Trip &trip_to_init, Person &person_to_add);
void set_attributes(Trip &trip_to_init, Person &person_to_add);
=======
void set_atts_action(Trip& trip_to_init, int person_id);
void set_attributes(Trip& trip_to_init, int person_id);
//bool test_load_history();
template<typename T>
T numerical_input(std::string message, T min, T max);
>>>>>>> d7b17119b6e361b028f6b0b4abae470c7e0f4a22
