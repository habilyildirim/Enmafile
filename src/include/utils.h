#ifndef UTILS_H
#define UTILS_H

namespace str_utils {
	std::string ltrim(const std::string& data);
	std::string rtrim(const std::string& data);
	std::string trim(const std::string& data);
	
	int find_key_index(const std::string& data, std::string key);
	int find_index(const std::vector<std::string>& find_index_vector, const std::string& find_index_value);	
}

namespace enma_utils {
	std::string find_var_name(const std::string& line, char assignment_char);
	std::string find_var_value(const std::string& line, char assignment_char);	
}

#endif
