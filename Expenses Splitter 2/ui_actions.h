#pragma once
#include "Trip.h"

void launch_app(Trip& Trip);
void add_new_trip(Trip& trip);
void interface(Trip &trip);
void load_history(Trip &curr_trip);
// void add_participants(Trip &trip_to_init);
void add_participant(Trip &trip_to_init);
void add_transactions(Trip &trip_to_init);
void add_singular_transaction();
void settle(Trip& trip);
void add_collective_transaction(Trip &trip_to_init);
void add_specific_transaction(Trip &trip_to_init);
void set_atts_action(Trip &trip_to_init, Person &person_to_add);
void set_attributes(Trip &trip_to_init, Person &person_to_add);