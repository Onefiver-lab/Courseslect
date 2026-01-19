// Module
// File: student.cppm   Version: 0.1.0   License: AGPLv3
// Created:Wang Qingming,Huang Yulin,Li Lin       2026-01-17 16:53:03
// Description:
//
export module student;

import std;
import user;
import major;
import courseselection;
using std::string;
using std::print;

export class Student : public User {
public:
    std::string student_number;
    std::shared_ptr<Major> major_info;
    int max_credits;
    int current_credits;
    std::vector<std::shared_ptr<CourseSelection>> cart;
    std::vector<std::shared_ptr<CourseSelection>> enrolled_courses;

public:
    Student(std::string user_id, std::string user_name, std::string password,
            std::string number, std::shared_ptr<Major> major)
        : User(std::move(user_id), std::move(user_name), std::move(password))
        , student_number(std::move(number)), major_info(std::move(major))
        , max_credits(18), current_credits(0) {}

    void search_courses(const std::string& keyword) {
        std::print("{} searching for courses with: {}\n", name, keyword);
    }

    bool add_to_cart(std::shared_ptr<CourseSelection> section) {
        if (section->has_available_seats()) {
            cart.push_back(section);
            std::print("Added {} to {}'s cart.\n", section->course->title, name);
            return true;
        }
        std::print("Course is full!\n");
        return false;
    }

    bool submit_registration() {
        if (cart.empty()) {
            std::print("Cart is empty!\n");
            return false;
        }

        int total_credits = 0;
        for (const auto& section : cart) {
            total_credits += section->course->credit_value;
        }

        int remaining = max_credits - current_credits;
        if (total_credits > remaining) {
            std::print("Credit limit exceeded! Max: {}, Current: {}\n", max_credits, current_credits);
            return false;
        }

        for (const auto& section : cart) {
            if (section->enroll_student(std::make_shared<Student>(*this))) {
                enrolled_courses.push_back(section);
                current_credits += section->course->credit_value;
            }
        }

        cart.clear();
        std::print("Registration submitted successfully!\n");
        return true;
    }

    bool drop_course(const std::string& course_id) {
        auto it = std::remove_if(enrolled_courses.begin(), enrolled_courses.end(),
            [&course_id](const auto& section) {
                return section->course->code == course_id;
            });

        if (it != enrolled_courses.end()) {
            int dropped_credits = 0;
            for (auto removed = it; removed != enrolled_courses.end(); ++removed) {
                dropped_credits += (*removed)->course->credit_value;
            }
            enrolled_courses.erase(it, enrolled_courses.end());
            current_credits -= dropped_credits;
            std::print("Dropped course: {}\n", course_id);
            return true;
        }
        return false;
    }

    void view_grades() {
        std::print("{} viewing grades...\n", name);
    }

    void show_dashboard() override {
        std::print("\n=== Student Dashboard ===\n");
        std::print("Student: {}\n", name);
        std::print("Number: {}\n", student_number);
        std::print("Major: {}\n", major_info->title);
        std::print("Credits: {}/{}\n", current_credits, max_credits);
        std::print("Enrolled Courses: {}\n", enrolled_courses.size());
        std::print("Courses in Cart: {}\n", cart.size());

        if (!enrolled_courses.empty()) {
            std::print("\nEnrolled Courses:\n");
            for (const auto& course : enrolled_courses) {
                std::print("  - {} ({})\n", course->course->title, course->course->code);
            }
        }
    }

    int remaining_credits() const {
        return max_credits - current_credits;
    }

    bool has_course_in_cart(const std::string& course_id) const {
        return std::any_of(cart.begin(), cart.end(),
            [&course_id](const auto& section) {
                return section->course->code == course_id;
            });
    }

    bool is_enrolled_in(const std::string& course_id) const {
        return std::any_of(enrolled_courses.begin(), enrolled_courses.end(),
            [&course_id](const auto& section) {
                return section->course->code == course_id;
            });
    }
};

export module registrar:student;
import :course;
import std;

using std::string;
using std::vector;
using std::format;



export class Student {
public:
    Student(string id, string name);
    void enrollIn(Course* course);
    string schedule();
    string info();
    bool hasId(string id);

private:
    string m_name;
    string m_id;
    vector<Course*> _courses;
};

Student::Student(string id, string name)
    : m_name(name)
    , m_id(id) {
}

string Student::info() {
    return format("{} {}\n", m_id, m_name);
}

bool Student::hasId(string id) {
    return id == m_id;
}

void Student::enrollIn(Course* course) {
    _courses.push_back(course);
}

string Student::schedule() {
    std::string scheduleStr;
    for (auto* course : _courses) {
        scheduleStr += course->info();
    }
    return scheduleStr.empty() ? "暂无选课\n" : scheduleStr;
}

