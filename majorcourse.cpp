export module majorcourse;

import course;
import student;

export class MajorCourse : public Course {
public:
    bool is_elective;
    int maximum_capacity;

public:
    MajorCourse(std::string course_code, std::string course_title,
                int credits, bool elective, int capacity)
        : Course(std::move(course_code), std::move(course_title), credits)
        , is_elective(elective), maximum_capacity(capacity) {}

    bool is_student_eligible(const Student& student) const override {
        return true;
    }

    bool is_core_course() const {
        return !is_elective;
    }
};
