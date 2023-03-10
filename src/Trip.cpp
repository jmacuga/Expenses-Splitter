#include "Trip.h"
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <typeinfo>

void Trip::add_person(Person const &p) {
  for (Person trip_p : people)
    if (p == trip_p)
      return;
  people.push_back(p);
  app_seq.push_back(true);
}

// adds transaction to p_trans, changes payer's balance and other people
// balances
void Trip::add_transaction(std::shared_ptr<Transaction> p_trans) {
  if (p_trans == nullptr)
    return;
  ptransactions.push_back(p_trans);
  app_seq.push_back(false);
  float money = p_trans->get_money();
  float moneypp = 0;
  size_t incl_number = p_trans->get_included().size();
  std::vector<Person *> included;
  // if onl_numebr is 0, the transaction type is collective_transaction
  if (!incl_number) {
    for (Person &p : people)
      if (p.category_compare(p_trans->get_category())) {
        incl_number++;
        included.push_back(&p);
      }
  } else {
    for (int &idx : p_trans->get_included())
      included.push_back(&people[idx - 1]);
  }
  // in case if no one has certain attribue set as true
  if (!incl_number)
    throw WrongCategory;
  moneypp = money / (incl_number);
  for (Person *p : included)
    p->add_to_balace(-moneypp);
  people[p_trans->get_payer() - 1].add_to_balace(money);
}

void Trip::save_to_file(std::ofstream &myfile) const {
  if (!myfile.is_open())
    throw FileNotOpen;
  myfile << name << "\n\n";
  std::vector<Person>::const_iterator piterator = people.cbegin();
  std::vector<std::shared_ptr<Transaction>>::const_iterator titerator =
      ptransactions.cbegin();
  for (const bool &b : app_seq) {
    if (b) {
      myfile << (*piterator).file_input() << '\n';
      piterator++;
    } else {
      myfile << (*titerator)->file_input() << '\n';
      titerator++;
    }
  }
  myfile << "&&&";
}

void Trip::load_from_file(std::ifstream &myfile) {
  // lambda reading person info
  auto pderived = [&myfile]() {
    std::string line;
    std::string sid = "";
    std::string name = "";
    bool space_met = false;
    getline(myfile, line);
    for (const char &c : line) {
      if (c == ' ') {
        space_met = true;
        continue;
      }
      if (!space_met)
        sid += c;
      else
        name += c;
    }
    Person per(stoi(sid), name);
    getline(myfile, line);
    per.atts_setter(line);
    getline(myfile, line);
    if (line != "")
      throw WrongFileFormat;
    return per;
  };

  // lambda reading collective transaction info
  auto coltderived = [&myfile, this]() {
    std::string line;
    double balance;
    int payer;
    time_t time;
    Person::Category cat;
    getline(myfile, line);
    time = stol(line);
    getline(myfile, line);
    payer = stoi(line);
    getline(myfile, line);
    balance = stod(line);
    getline(myfile, line);
    cat = Person::Category(stoi(line));
    getline(myfile, line);
    return CollectiveTransaction(balance, payer, cat, time);
  };

  // lambda reading specific transaction info
  auto spetderived = [&myfile, this]() {
    std::string line;
    double balance;
    int payer;
    time_t time;
    Person::Category cat;
    getline(myfile, line);
    time = stol(line);
    getline(myfile, line);
    payer = stoi(line);
    getline(myfile, line);
    balance = stod(line);
    getline(myfile, line);
    cat = Person::Category(stoi(line));
    getline(myfile, line);
    std::vector<int> recievers(line.length());
    size_t idx = 0;
    for (const char &c : line) {
      recievers[idx] = (int)c - '0';
      idx++;
    }
    getline(myfile, line);
    return SpecificTransaction(balance, payer, cat, std::vector<int>(recievers),
                               time);
  };

  if (!myfile.is_open())
    throw FileNotOpen;
  // reading initial lines
  std::string line = "";
  getline(myfile, line);
  getline(myfile, line);
  if (line != "")
    throw WrongFileFormat;
  getline(myfile, line);
  // reading objects info
  while (line != "&&&") {
    if (line == "COL") {
      add_transaction(std::make_shared<CollectiveTransaction>(coltderived()));
      getline(myfile, line);
    } else if (line == "SPE") {
      add_transaction(std::make_shared<SpecificTransaction>(spetderived()));
      getline(myfile, line);
    } else if (line == "PER") {
      add_person(pderived());
      getline(myfile, line);
    } else
      throw WrongFileFormat;
  }
}

// calculate transfers between participants based on their balances
std::map<std::pair<int, int>, float> Trip::calc_transfers() {
  std::map<std::pair<int, int>, float> result;
  // lambda to compare pair by balance
  auto greater_pair = [](std::pair<int, float> const &p1,
                         std::pair<int, float> const &p2) {
    return std::abs(p2.second) > std::abs(p1.second);
  };
  // bufors of balances wich we will be zero at the end of func
  std::vector<std::pair<int, float>> pos_buffer;
  std::vector<std::pair<int, float>> neg_buffer;
  for (Person p : people) {
    if (!p.get_balance())
      continue;
    float p_balance = p.get_balance();

    auto pers_p = std::make_pair(p.get_id(), p_balance);
    if (pers_p.second > 0)
      pos_buffer.push_back(pers_p);
    else
      neg_buffer.push_back(pers_p);
  }
  std::sort(pos_buffer.begin(), pos_buffer.end(), greater_pair);
  std::sort(neg_buffer.begin(), neg_buffer.end(), greater_pair);
  // loop firstly through positive balances, than through negative
  split_money(pos_buffer, neg_buffer, result);
  split_money(neg_buffer, pos_buffer, result, true);
  return result;
}

// helper func to calc_transfers()
void Trip::split_money(std::vector<std::pair<int, float>> &first_buffer,
                       std::vector<std::pair<int, float>> &second_buffer,
                       std::map<std::pair<int, int>, float> &result,
                       bool is_first_negative) {
  for (auto fit = first_buffer.begin(); fit != first_buffer.end(); fit++) {
    if (!fit->second)
      continue;
    for (auto sit = second_buffer.begin(); sit != second_buffer.end(); sit++) {
      if (std::abs(sit->second) &&
          std::abs(sit->second) <= std::abs(fit->second)) {
        if (is_first_negative)
          result.insert({{fit->first, sit->first}, std::abs(sit->second)});
        else
          result.insert({{sit->first, fit->first}, std::abs(sit->second)});
        fit->second += sit->second;
        sit->second = 0;
      }
    }
  }
}

std::ostream &Trip::print_people(std::ostream &os) {
  // prints people with basic information
  os << "People list:\n";
  for (Person &per : people) {
    os << per.get_id() << ". " << per.get_name() << '\n';
    float rounded_money = round(per.get_balance() * 100.0f) / 100.0f;
    os << "Balance: " << rounded_money << '\n';
  }
  os << '\n';
  return os;
}

std::ostream &Trip::print_trans(std::ostream &os) {
  // prints transactions with basic information
  // using file input
  os << "Transactions history:\n\n";
  std::stringstream trstr;
  std::string line = "";
  std::string tst = std::string();
  for (std::shared_ptr<Transaction> &trans : ptransactions) {
    std::stringstream trstr;
    trstr.str(std::string());
    trstr << trans->file_input();
    getline(trstr, line);
    if (line == "COL")
      os << "Type: Collective\n";
    else if (line == "SPE")
      os << "Type: Specific\n";
    char *dt = ctime(trans->get_time());
    os << "Date: " << dt;
    getline(trstr, line);
    getline(trstr, line);
    os << "Paid: " << people.at(stoi(line) - 1).get_name() << '\n';
    getline(trstr, line);
    os << "Amount: " << line << '\n';
    getline(trstr, line);
    os << "Category: " << Person::Cat_to_str(trans->get_category()) << '\n';
    getline(trstr, line);
    if (line != "") {
      os << "For: ";
      int i = 1;
      for (char const &c : line) {
        os << people.at((int)c - '0' - 1).get_name();
        if (i < line.length())
          os << ", ";
        i++;
      }
      os << "\n\n";
    } else
      os << "\n";
  }
  return os;
}