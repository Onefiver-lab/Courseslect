// Module
// File: datamanager_memory.cppm   Version: 0.1.0   License: AGPLv3
// Created:Wang Qingming,Huang Yulin,Li Lin       2026-01-19 10:04:57
// Description:
//
export module registrar:datamanager_interface;
import :student;
import :course;
import :teacher;
import :secretary;
import std;

// 统一接口，确保两版本可替换
export class DataManager {
public:
    virtual ~DataManager() = default;
    // 学生操作
    virtual void addStudent(Student* student) = 0;
    virtual Student* getStudentById(string id) = 0;
    virtual vector<Student*> getAllStudents() = 0;
    // 课程操作
    virtual void addCourse(Course* course) = 0;
    virtual Course* getCourseById(string id) = 0;
    virtual vector<Course*> getAllCourses() = 0;
    // 教师操作
    virtual void addTeacher(Teacher* teacher) = 0;
    virtual Teacher* getTeacherById(string id) = 0;
    virtual vector<Teacher*> getAllTeachers() = 0;
    // 秘书操作
    virtual void addSecretary(Secretary* secretary) = 0;
    virtual Secretary* getSecretaryById(string id) = 0;
    // 数据持久化
    virtual bool saveData() = 0;
};
