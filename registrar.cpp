export module registrar:registrar;
import :student;
import :course;
import :teacher;
import :secretary;
import :datamanager_interface;
// 根据编译宏选择数据层
#ifdef USE_POSTGRES
import registrar:datamanager_postgres;
#else
import registrar:datamanager_memory;
#endif
import std;
using std::string;
using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::find;

export class Registrar {
public:
    Registrar() {
        // 初始化数据层
#ifdef USE_POSTGRES
        string connStr = "user=postgres password=123456 dbname=course_selection host=localhost port=5432";
        m_dataManager = new DataManagerPostgres(connStr);
#else
        m_dataManager = new DataManagerMemory();
#endif
        initDefaultUsers(); // 测试用默认用户
    }

    ~Registrar() { delete m_dataManager; }
    int exec(); // 启动系统

private:
    DataManager* m_dataManager;
    void initDefaultUsers();
    void studentMenu();
    void teacherMenu();
    void secretaryMenu();
    void showAllCourses();
    void enrollCourse(Student* student);
    void showStudentGrades(Student* student);
    void showTeacherCourses(Teacher* teacher);
    void inputStudentGrades(Teacher* teacher);
    void createCourseBySecretary(Secretary* secretary);
    void assignTeacherBySecretary(Secretary* secretary);
    void showAllTeachers();
};

// 系统主循环
int Registrar::exec() {
    cout << "===== 选课系统 =====" << endl;
    while (true) {
        cout << "\n1.学生 2.教师 3.教学秘书 0.退出\n请选择：";
        int choice;
        cin >> choice;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1: studentMenu(); break;
            case 2: teacherMenu(); break;
            case 3: secretaryMenu(); break;
            case 0: cout << "再见！" << endl; return 0;
            default: cout << "无效选项！" << endl;
        }
    }
}

// 初始化默认测试用户
void Registrar::initDefaultUsers() {
    // 学生
    if (!m_dataManager->getStudentById("2024001")) {
        m_dataManager->addStudent(new Student("2024001", "张三"));
        m_dataManager->addStudent(new Student("2024002", "李四"));
    }
    // 教师
    if (!m_dataManager->getTeacherById("T001")) {
        m_dataManager->addTeacher(new Teacher("T001", "王老师"));
        m_dataManager->addTeacher(new Teacher("T002", "李老师"));
    }
    // 秘书
    if (!m_dataManager->getSecretaryById("S001")) {
        m_dataManager->addSecretary(new Secretary("S001", "赵秘书"));
    }
    // 课程
    if (!m_dataManager->getCourseById("C001")) {
        auto sec = m_dataManager->getSecretaryById("S001");
        auto tea = m_dataManager->getTeacherById("T001");
        auto course = sec->createCourse("C001", "C++程序设计", 3);
        sec->assignTeacherToCourse(tea, course);
        m_dataManager->addCourse(course);
    }
    m_dataManager->saveData();
}

// 学生菜单（选课、查课表、查成绩）
void Registrar::studentMenu() {
    cout << "\n===== 学生登录 =====" << endl;
    cout << "输入学生ID：";
    string id;
    getline(cin, id);
    Student* stu = m_dataManager->getStudentById(id);
    if (!stu) { cout << "ID不存在！" << endl; return; }

    while (true) {
        cout << "\n1.查课程 2.选课 3.课表 4.成绩 0.返回\n选择：";
        int choice;
        cin >> choice;
        cin.ignore();
        switch (choice) {
            case 1: showAllCourses(); break;
            case 2: enrollCourse(stu); break;
            case 3: cout << "个人课表：\n" << stu->schedule(); break;
            case 4: showStudentGrades(stu); break;
            case 0: return;
            default: cout << "无效选项！" << endl;
        }
    }
}

// 教师菜单（查课程、录成绩）
void Registrar::teacherMenu() {
    cout << "\n===== 教师登录 =====" << endl;
    cout << "输入教师ID：";
    string id;
    getline(cin, id);
    Teacher* tea = m_dataManager->getTeacherById(id);
    if (!tea) { cout << "ID不存在！" << endl; return; }

    while (true) {
        cout << "\n1.授课课程 2.录成绩 0.返回\n选择：";
        int choice;
        cin >> choice;
        cin.ignore();
        switch (choice) {
            case 1: showTeacherCourses(tea); break;
            case 2: inputStudentGrades(tea); break;
            case 0: return;
            default: cout << "无效选项！" << endl;
        }
    }
}

// 秘书菜单（创建课程、分配教师）
void Registrar::secretaryMenu() {
    cout << "\n===== 秘书登录 =====" << endl;
    cout << "输入秘书ID：";
    string id;
    getline(cin, id);
    Secretary* sec = m_dataManager->getSecretaryById(id);
    if (!sec) { cout << "ID不存在！" << endl; return; }

    while (true) {
        cout << "\n1.创建课程 2.分配教师 3.查课程 0.返回\n选择：";
        int choice;
        cin >> choice;
        cin.ignore();
        switch (choice) {
            case 1: createCourseBySecretary(sec); break;
            case 2: assignTeacherBySecretary(sec); break;
            case 3: showAllCourses(); break;
            case 0: return;
            default: cout << "无效选项！" << endl;
        }
    }
}

// 辅助函数（显示所有课程、选课、查成绩等）
void Registrar::showAllCourses() {
    cout << "\n===== 所有课程 =====" << endl;
    auto courses = m_dataManager->getAllCourses();
    if (courses.empty()) { cout << "暂无课程！" << endl; return; }
    for (auto c : courses) cout << c->info();
}

void Registrar::enrollCourse(Student* stu) {
    showAllCourses();
    cout << "输入选课ID：";
    string cid;
    getline(cin, cid);
    auto course = m_dataManager->getCourseById(cid);
    if (!course) { cout << "课程不存在！" << endl; return; }
    auto& stus = course->getStudents();
    if (find(stus.begin(), stus.end(), stu) != stus.end()) {
        cout << "已选该课程！" << endl; return;
    }
    course->addStudent(stu);
    m_dataManager->addCourse(course);
    m_dataManager->saveData();
    cout << "选课成功！" << endl;
}

void Registrar::showStudentGrades(Student* stu) {
    cout << "\n===== 个人成绩 =====" << endl;
    auto courses = m_dataManager->getAllCourses();
    bool hasGrade = false;
    for (auto c : courses) {
        auto& stus = c->getStudents();
        if (find(stus.begin(), stus.end(), stu) != stus.end()) {
            cout << format("{}: 成绩{}", c->getName(), c->getGrade(stu)) << endl;
            hasGrade = true;
        }
    }
    if (!hasGrade) cout << "暂无成绩！" << endl;
}

void Registrar::showTeacherCourses(Teacher* tea) {
    cout << "\n===== 授课课程 =====" << endl;
    auto courses = m_dataManager->getAllCourses();
    bool hasCourse = false;
    for (auto c : courses) {
        if (c->getTeacher() == tea) {
            cout << c->info();
            hasCourse = true;
        }
    }
    if (!hasCourse) cout << "暂无授课课程！" << endl;
}

void Registrar::inputStudentGrades(Teacher* tea) {
    auto courses = m_dataManager->getAllCourses();
    vector<Course*> myCourses;
    for (auto c : courses) if (c->getTeacher() == tea) myCourses.push_back(c);
    if (myCourses.empty()) { cout << "无授课课程！" << endl; return; }

    cout << "授课课程：" << endl;
    for (size_t i=0; i<myCourses.size(); i++)
        cout << format("{}: {}", i+1, myCourses[i]->getName()) << endl;

    cout << "选择课程序号：";
    int idx;
    cin >> idx;
    cin.ignore();
    if (idx <1 || idx>myCourses.size()) { cout << "无效序号！" << endl; return; }
    auto course = myCourses[idx-1];

    auto stus = course->getStudents();
    if (stus.empty()) { cout << "无学生选课！" << endl; return; }
    cout << "选课学生：" << endl;
    for (size_t i=0; i<stus.size(); i++)
        cout << format("{}: {}", i+1, stus[i]->info()) << endl;
