// Module
// File: iregistrar.cppm   Version: 0.1.0   License: AGPLv3
// Created: Wang Qingming,Huang Yulin,Li Lin      2026-01-17 18:00:38
// Description:
//
export module iregistrar;

import std;

export class IRegistrar {
public:
    virtual ~IRegistrar() = default;

    virtual void manage_semesters() = 0;
    virtual bool create_semester(const std::string& semester_code,
                               const std::string& semester_name,
                               const std::string& start_date,
                               const std::string& end_date) = 0;
    virtual bool activate_semester(const std::string& semester_code) = 0;
    virtual bool deactivate_semester(const std::string& semester_code) = 0;

    virtual void manage_all_courses() = 0;
    virtual bool add_course(const std::string& course_code,
                          const std::string& course_name,
                          int credits,
                          const std::string& course_type) = 0;
    virtual bool remove_course(const std::string& course_code) = 0;

    virtual void manage_all_users() = 0;
    virtual bool add_user(const std::string& user_id,
                        const std::string& user_name,
                        const std::string& password,
                        const std::string& user_type) = 0;
    virtual bool remove_user(const std::string& user_id) = 0;

    virtual void process_approvals() = 0;
    virtual bool approve_registration(const std::string& reg_number) = 0;
    virtual bool reject_registration(const std::string& reg_number) = 0;

    virtual void generate_reports() = 0;
    virtual void generate_enrollment_report() = 0;
    virtual void generate_course_report() = 0;
    virtual void generate_user_report() = 0;

    virtual void backup_system_data() = 0;
    virtual void restore_system_data(const std::string& backup_file) = 0;
    virtual void show_system_status() = 0;
};
