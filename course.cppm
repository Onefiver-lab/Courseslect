// Module
// File: course.cppm   Version: 0.1.0   License: AGPLv3
// Created:Wang Qingming,Huang Yulin,Li Lin       2026-01-17 19:09:46
// Description:
//
export module course;

import std;

using std::string;
using std::vector;
using std::shared_ptr;
using std::move;
using std::any_of;

export class Student;

export class Course {
public:
    string code;
    string title;
    int credit_value;
    vector<shared_ptr<Course>> required_prerequisites;

public:
    Course(string course_code, string course_title, int credits)
        : code(move(course_code)), title(std::move(course_title))
        , credit_value(credits) {}

    virtual ~Course() = default;

    virtual bool is_student_eligible(const Student& student) const = 0;

    void add_prerequisite(shared_ptr<Course> prerequisite_course) {
        required_prerequisites.push_back(prerequisite_course);
    }

    bool has_prerequisites() const {
        return !required_prerequisites.empty();
    }

    bool is_prerequisite_for(const Course& other) const {
        return any_of(other.required_prerequisites.begin(),
                          other.required_prerequisites.end(),
                          [this](const auto& prereq) {
                              return prereq->code == this->code;
                          });
    }
};
