#pragma once
#include <vector>
#include <memory>
#include "Transactions.h"

class Trip {
;	std::vector<Person> people;
	std::vector<std::shared_ptr<Transaction>> ptransactions;
	std::string name;
public:
// two Trip constructors, one with file handle to file from which should it be recreated
	Trip(std::string nm): name(nm) {};
	Trip(std::string nm, std::ifstream &myfile): name(nm) { load_from_file(myfile); };
	void add_person(Person const&); //adds  person
	void add_transaction(std::shared_ptr<Transaction> p_trans); //adds pointer to transaction, settles peoples balances
	std::vector<Person> *get_people() {  return &people;  };
	std::vector<std::shared_ptr<Transaction>> get_ptransactions() {  return ptransactions;  };
	void save_to_file(std::ofstream &myfile) const;
	void load_from_file(std::ifstream &myfile);
};