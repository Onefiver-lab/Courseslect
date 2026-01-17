export module publiccourse;
import course;
import major;
import student;
import <vector>;
import <memory>;
import <algorithm>;

using std::string;
using std::vector;
using std::shared_ptr;
using std::move;
using std::any_of;

using std::vector;

export class PublicCourse : public Course {
public:
    int maximum_capacity;
    std::vector<shared_ptr<Major>> allowed_majors;

public:
    PublicCourse(string course_code, string course_title,
                int credits, int capacity)
        : Course(move(course_code), move(course_title), credits)
        , maximum_capacity(capacity) {}

    bool is_student_eligible(const Student& student) const override {
        if (allowed_majors.empty()) {
            return true;
        }

        return std::any_of(allowed_majors.begin(), allowed_majors.end(),
            [&student](const auto& major) {
                return major->code == student.major_info->code;
            });
    }

    void allow_major(shared_ptr<Major> major) {
        allowed_majors.push_back(major);
    }

    bool is_major_allowed(const std::string& major_code) const {
        return any_of(allowed_majors.begin(), allowed_majors.end(),
            [&major_code](const auto& major) {
                return major->code == major_code;
            });
    }

    int available_seats() const {
        return maximum_capacity;
    }
};
