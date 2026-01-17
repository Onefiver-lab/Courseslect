// Module
// File: major.cppm   Version: 0.1.0   License: AGPLv3
// Created:Wang Qingming,Huang Yulin,Li Lin       2026-01-17 19:07:55
// Description:
//
export module major;

import std;
import majorcourse;
import student;

export class Major {
public:
    std::string code;
    std::string title;
    std::vector<std::shared_ptr<MajorCourse>> core_courses;
    std::vector<std::shared_ptr<MajorCourse>> elective_courses;

public:
    Major(std::string major_code, std::string major_title)
        : code(std::move(major_code)), title(std::move(major_title)) {}

    bool meets_requirements(const Student& student) const {
        return true;
    }

    void add_core_course(std::shared_ptr<MajorCourse> course) {
        core_courses.push_back(course);
    }

    void add_elective_course(std::shared_ptr<MajorCourse> course) {
        elective_courses.push_back(course);
    }

    int total_required_credits() const {
        int total = 0;
        for (const auto& course : core_courses) {
            total += course->credit_value;
        }
        return total;
    }

    bool has_course(const std::string& course_code) const {
        auto in_core = std::any_of(core_courses.begin(), core_courses.end(),
            [&course_code](const auto& course) {
                return course->code == course_code;
            });

        if (in_core) return true;

        return std::any_of(elective_courses.begin(), elective_courses.end(),
            [&course_code](const auto& course) {
                return course->code == course_code;
            });
    }
};
