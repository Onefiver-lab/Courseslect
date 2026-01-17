// Module
// File: user.cppm   Version: 0.1.0   License: AGPLv3
// Created: Wang Qingming,Huang Yulin,Li Lin      2026-01-17 19:12:31
// Description:
//
export module teacher;

import std;
import user;
import courseselection;


export class Teacher : public User {
public:
    std::string employee_number;
    std::vector<std::shared_ptr<CourseSelection>> teaching_sessions;

public:
    Teacher(std::string user_id, std::string user_name, std::string password, std::string emp_num);

    void view_teaching_courses();
    void manage_students(const std::string& course_id);
    bool input_grades(const std::string& student_id, const std::string& course_id,
                     const std::string& grade);
    bool set_capacity(const std::string& section_id, int capacity);
    void show_dashboard() override;
    void add_teaching_session(std::shared_ptr<CourseSelection> session);
};


User::User(std::string user_id, std::string user_name, std::string password)
    : id(std::move(user_id)), name(std::move(user_name))
    , password_hash(std::move(password)) {}

bool User::verify_password(const std::string& input_password) const {
    return password_hash == input_password;
}

void User::change_password(const std::string& new_password) {
    password_hash = new_password;
    std::print("{}'s password changed.\n", name);
}

bool User::login(const std::string& input_password) {
    if (verify_password(input_password)) {
        std::print("{} logged in successfully.\n", name);
        return true;
    }
    return false;
}

void User::logout() {
    std::print("{} logged out.\n", name);
}
