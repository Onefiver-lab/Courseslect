// Module
// File: registration.cppm   Version: 0.1.0   License: AGPLv3
// Created:Wang Qingming,Huang Yulin,Li Lin       2026-01-17 17:10:39
// Description:
//
export module registration;

import std;
import student;
import courseselection;
using std::string;
using std::vector;
using std::print;

export class Registration {
public:
    std::string registration_number;
    std::chrono::system_clock::time_point registration_time;
    std::string current_status;
    std::string approval_state;
    std::shared_ptr<Student> student_info;
    std::vector<std::shared_ptr<CourseSelection>> selected_sections;

public:
    Registration(std::string reg_number, std::shared_ptr<Student> student)
        : registration_number(std::move(reg_number)), student_info(std::move(student))
        , registration_time(std::chrono::system_clock::now())
        , current_status("Pending"), approval_state("Pending") {}

    bool validate_constraints() const {
        for (size_t i = 0; i < selected_sections.size(); ++i) {
            for (size_t j = i + 1; j < selected_sections.size(); ++j) {
                if (selected_sections[i]->has_time_conflict(*selected_sections[j])) {
                    std::print("Time conflict between {} and {}\n",
                              selected_sections[i]->course->title,
                              selected_sections[j]->course->title);
                    return false;
                }
            }
        }

        int total_credits = 0;
        for (const auto& section : selected_sections) {
            total_credits += section->course->credit_value;
        }

        if (total_credits > student_info->remaining_credits()) {
            std::print("Credit limit exceeded! Student has {} credits remaining, but needs {}\n",
                      student_info->remaining_credits(), total_credits);
            return false;
        }

        return true;
    }

    bool submit_for_approval() {
        if (validate_constraints()) {
            current_status = "Submitted";
            std::print("Registration {} submitted for approval.\n", registration_number);
            return true;
        }
        return false;
    }

    bool cancel_registration() {
        current_status = "Cancelled";
        std::print("Registration {} cancelled.\n", registration_number);
        return true;
    }

    void add_course_section(std::shared_ptr<CourseSelection> section) {
        selected_sections.push_back(section);
        std::print("{} added to registration.\n", section->course->title);
    }

    bool is_pending() const {
        return current_status == "Pending";
    }

    bool is_approved() const {
        return approval_state == "Approved";
    }

    bool is_completed() const {
        return current_status == "Completed";
    }

    int total_credits() const {
        int sum = 0;
        for (const auto& section : selected_sections) {
            sum += section->course->credit_value;
        }
        return sum;
    }
};
