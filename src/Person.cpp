#include "Transactions.h"
#include <cmath>
#include <sstream>

Person::Person(unsigned int pid, std::string nm) : id(pid), name(nm) {
  balance = 0.0;
  atts[Category::food] = true;
  atts[Category::alcohol] = true;
  atts[Category::nicotine] = false;
  atts[Category::meat] = true;
  atts[Category::gluten] = true;
  atts[Category::dairy] = true;
  atts[Category::other] = true;
}

// set attributes based on given attribute code
void Person::atts_setter(std::string att_code) {
  std::string::const_iterator code_iter = att_code.cbegin();
  std::map<Category, bool>::iterator at_iter = atts.begin();
  for (code_iter, at_iter; at_iter != atts.end(); ++code_iter, ++at_iter)
    if (*(code_iter) == '1')
      (*at_iter).second = true;
    else if (*(code_iter) == '0')
      (*at_iter).second = false;
    else
      throw WrongFileFormat;
  if (at_iter != atts.end() && code_iter != att_code.cend())
    throw WrongFileFormat;
}

bool Person::operator==(const Person &other) const {
  return (atts == other.get_atts() &&
          Transaction::fl_cmp(balance, other.get_balance()) &&
          name == other.get_name() && id == other.get_id());
}

bool Person::operator!=(const Person &other) const { return !(*this == other); }

void Person::add_to_balace(float amount) { balance += amount; }

std::string Person::file_input() const {
  std::stringstream instr;
  instr << "PER\n";
  instr << id << " " << name << '\n';
  for (std::pair<const Category, bool> pa : atts)
    instr << int(std::get<1>(pa));
  instr << '\n';
  return instr.str();
}

std::string Person::Cat_to_str(Category c) {
  switch (c) {
  case Category::food:
    return "food";
    break;
  case Category::nicotine:
    return "nicotine";
    break;
  case Category::alcohol:
    return "alcohol";
    break;
  case Category::meat:
    return "meat";
    break;
  case Category::gluten:
    return "gluten";
    break;
  case Category::dairy:
    return "dairy";
    break;
  case Category::other:
    return "other";
    break;
  default:
    return "NOT_A_CATEGORY";
  }
}

bool Person::category_compare(Category trans_c) const {
  std::map<Category, bool>::const_iterator elem = atts.find(trans_c);
  if (elem == atts.end())
    return false;
  return elem->second;
};

std::string Person::print_atts() {
  std::string out;
  out = "Current attributes:\n";
  int i = 1;
  for (auto c : atts) {
    out += std::to_string(i) + ". " + Cat_to_str(c.first) + " [" +
           (c.second ? "YES" : "NO") + "]\n";
    i++;
  };
  return out;
}