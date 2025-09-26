#include"symbol_filter.h"

/* function to create new string with filter */
std::string replace_foutrh_symbol_(const std::string& string_, size_t step_, char filter_symbol_) {

	/** create temporary string */
	std::string temp_string_ = string_;	

	/**
	* cycle, that check every 4th symbol (index[3] - first, and += 4) (^^)
	*/
	for (size_t it_str_ = step_ - 1; it_str_ < temp_string_.size(); it_str_ += step_) {
		if (temp_string_[it_str_] == ' ') {
			it_str_++;
		}
		/** change current symbol in temporary string to choisen symbol */
		temp_string_[it_str_] = filter_symbol_;
	}

	/** return copy of new string */
	return temp_string_;
}