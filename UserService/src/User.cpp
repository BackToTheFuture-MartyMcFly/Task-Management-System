#include "User.h"


User::User(int id, const std::string& name, const std::string& email)
	: id(id), name(name), email(email) {}


void User::setId(int id_) {
	id = id_;
}

void User::setName(const std::string& name_) {
	name = name_;
}

void User::setEmail(const std::string& email_) {
	email = email_;
}

void User::setPasswordHash(const std::string& password_) {
	
}

int User::getId() const{
	return id;
}

std::string User::getName() const{
	return  name;
}

std::string User::getEmail() const{
	return email;
}

std::string User::getPasswordHash() const {
	return password_hash;
}

bool User::operator== (const User& user) const {
	return (id == user.getId()) && (name == user.getName()) && (email == user.getEmail());
}