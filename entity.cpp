// entity.ixx（统一存放所有实体类的完整定义，彻底解决不完整类型和循环依赖）
export module registrar:entity;
import std;


using std::string;
using std::vector;
using std::map;
using std::find;
using std::format;
using std::shared_ptr;
using std::enable_shared_from_this;
using std::invalid_argument;
using std::stoi;

// 基础实体基类（所有业务实体的公共父类）
export class BaseEntity {
public:
    BaseEntity(string id, string name) : m_id(std::move(id)), m_name(std::move(name)) {}
    virtual ~BaseEntity() = default;

    string getId() const { return m_id; }
    string getName() const { return m_name; }
    bool hasId(const string& id) const { return m_id == id; }
    virtual string info() const = 0; // 纯虚函数，强制子类实现

private:
    string m_id;
    string m_name;
};

// 提前声明类（解决类内部互相引用的问题）
export class Course;
export class Student;
export class Teacher;
export class Secretary;

// 共享智能指针类型别名
export using SharedStudent = shared_ptr<Student>;
export using SharedTeacher = shared_ptr<Teacher>;
export using SharedCourse = shared_ptr<Course>;
export using SharedSecretary = shared_ptr<Secretary>;
export using StudentList = vector<SharedStudent>;
export using CourseList = vector<SharedCourse>;


export class Student : public BaseEntity, public enable_shared_from_this<Student> {
public:
    Student(string id, string name) : BaseEntity(std::move(id), std::move(name)) {}

    string info() const override {
        return format("学生ID: {}, 姓名: {}\n", getId(), getName());
    }

    void enrollIn(SharedCourse course) {
        auto it = find(_courses.begin(), _courses.end(), course);
        if (it != _courses.end()) return;
        _courses.push_back(course);
    }

    string schedule(); // 仅声明，延后实现


    const CourseList& getCourses() const { return _courses; }

private:
    CourseList _courses; // 成员变量是课程列表，对应getCourses()
};




// 2. 教师类
export class Teacher : public BaseEntity, public enable_shared_from_this<Teacher> {
public:
    Teacher(string id, string name) : BaseEntity(std::move(id), std::move(name)) {}

    string info() const override {
        return format("教师ID: {}, 姓名: {}", getId(), getName());
    }

    void addCourse(SharedCourse course) {
        auto it = find(m_courses.begin(), m_courses.end(), course);
        if (it != m_courses.end()) return;
        m_courses.push_back(course);
    }

    bool gradeStudent(SharedCourse course, SharedStudent student, string grade); // 延后实现

    CourseList getCourses() const { return m_courses; }

private:
    CourseList m_courses;
};

// 3. 课程类
export class Course : public BaseEntity, public enable_shared_from_this<Course> {
public:
    Course(string id, string name, int credits)
        : BaseEntity(std::move(id), std::move(name)), m_credits(credits) {
        if (credits < 1 || credits > 10) {
            throw invalid_argument("学分必须在1-10之间");
        }
    }

    string info() const override {
        string teacherInfo = m_teacher ? m_teacher->info() : "无授课教师";
        return format("课程ID: {}, 名称: {}, 学分: {}, 授课教师: {}\n",
                      getId(), getName(), m_credits, teacherInfo);
    }

    void addStudent(SharedStudent student) {
        auto it = find(m_students.begin(), m_students.end(), student);
        if (it != m_students.end()) return;
        m_students.push_back(student);
        student->enrollIn(shared_from_this());
    }

    void setTeacher(SharedTeacher teacher) {
        if (!teacher) return;
        m_teacher = teacher;
        teacher->addCourse(shared_from_this());
    }

    bool assignGrade(SharedStudent student, string grade) {
        if (!isValidGrade(grade)) return false;
        auto it = find(m_students.begin(), m_students.end(), student);
        if (it == m_students.end()) return false;
        m_grades[student] = std::move(grade);
        return true;
    }

    string getGrade(SharedStudent student) const {
        auto it = m_grades.find(student);
        return it != m_grades.end() ? it->second : "未评定";
    }

    SharedTeacher getTeacher() const { return m_teacher; }
    const StudentList& getStudents() const { return m_students; }

    int getCredits() const { return m_credits; }

private:
    int m_credits;
    SharedTeacher m_teacher{nullptr};
    StudentList m_students;
    map<SharedStudent, string> m_grades;

    // 内部调用成绩校验函数
    bool isValidGrade(const string& grade) {
        if (grade == "A" || grade == "B" || grade == "C" || grade == "D" || grade == "F") {
            return true;
        }
        try {
            int score = stoi(grade);
            return score >= 0 && score <= 100;
        } catch (...) {
            return false;
        }
    }
};

// 4. 秘书类
export class Secretary : public BaseEntity, public enable_shared_from_this<Secretary> {
public:
    Secretary(string id, string name) : BaseEntity(std::move(id), std::move(name)) {}

    string info() const override {
        return format("秘书ID: {}, 姓名: {}", getId(), getName());
    }

    SharedCourse createCourse(string courseId, string courseName, int credits) {
        return shared_ptr<Course>(new Course(std::move(courseId), std::move(courseName), credits));
    }

    bool assignTeacherToCourse(SharedTeacher teacher, SharedCourse course) {
        if (!teacher || !course) return false;
        course->setTeacher(teacher);
        return true;
    }
};


inline string Student::schedule() {
    string scheduleStr;
    for (const auto& course : _courses) {
        scheduleStr += course->info(); // Course已完整定义，无报错
    }
    return scheduleStr.empty() ? "暂无选课\n" : scheduleStr;
}


inline bool Teacher::gradeStudent(SharedCourse course, SharedStudent student, string grade) {
    if (course->getTeacher() != shared_from_this()) return false;
    auto& students = course->getStudents();
    if (find(students.begin(), students.end(), student) == students.end()) return false;
    return course->assignGrade(student, std::move(grade));
}

// 全局共享成绩校验工具函数
export bool isValidGrade(const string& grade) {
    if (grade == "A" || grade == "B" || grade == "C" || grade == "D" || grade == "F") {
        return true;
    }
    try {
        int score = stoi(grade);
        return score >= 0 && score <= 100;
    } catch (...) {
        return false;
    }
}
