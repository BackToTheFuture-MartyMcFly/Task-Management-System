#pragma once

#include <string>

class User 
{
private:
	int id;
	std::string name;
	std::string email;
	std::string password_hash;

public:
	User() = default;
	User(int, const std::string&, const std::string&);

	void setId(int);
	void setName(const std::string&);
	void setEmail(const std::string&);
	void setPasswordHash(const std::string&);

	int getId() const;
	std::string getName() const;
	std::string getEmail() const;
	std::string getPasswordHash() const;

	bool operator== (const User&) const;

};