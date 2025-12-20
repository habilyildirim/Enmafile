#include "inc.h"
#include "macros.h"

namespace str_utils {
	std::string ltrim(const std::string& data) {
		std::string result = data;
	    result.erase(result.begin(), 
	    	std::find_if(result.begin(), result.end(),[](unsigned char ch) {
	    		return !std::isspace(ch);
	    	}));
	    	
	    return result;
	}
	
	std::string rtrim(const std::string& data) {
	    std::string result = data;
	    result.erase(
	        std::find_if(result.rbegin(), result.rend(), [](unsigned char ch) {
	            return !std::isspace(ch);
	        }).base(),
	        result.end()
	    );
	    
	    return result;
	}

	std::string trim(const std::string& data) {
		return rtrim(ltrim(data));
	}
		

	int find_key_index(const std::string& data, std::string key) {
		size_t pos = data.find(key);
		if(pos == std::string::npos) 
			return -1;

		return pos;
	}
	
	int find_index(const std::vector<std::string>& find_index_vector, const std::string& find_index_value) {
		for(int counter = 0; counter != find_index_vector.size(); counter++) {
			if(find_index_vector[counter] == find_index_value) 
				return counter;
		} 

		return -1;
	}
}

namespace enma_utils {
	std::string find_var_name(const std::string& line, char assignment_char) {
		if(line.length() < 2) 
			return ERROR_NULL_VAR_NAME;

		size_t pos = line.find(assignment_char);
	
	    std::string var_name = line.substr(0, pos);
	    size_t start = var_name.find_first_not_of(" \t");
	    size_t end = var_name.find_last_not_of(" \t");
	
		if(pos < 2) 
			return ERROR_NULL_VAR_NAME;
			
	    return var_name.substr(start, end - start + 1);
	}

	std::string find_var_value(const std::string& line, char assignment_char) {
		size_t pos = line.find(assignment_char);
	
		if(pos == std::string::npos) 
			return ERROR_NULL_VALUE;
			
		if(str_utils::ltrim(line.substr(pos + 1, line.length())).empty()) 
			return ERROR_NULL_VALUE;
			
		std::string value = line.substr(pos + 1, line.length());
		size_t start = value.find_first_not_of(" \t");
		size_t end = value.find_last_not_of(" \t");
	
		return value.substr(start, end - start + 1);	
	} 
}
