#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <mysql/jdbc.h>
using namespace sql;

class string;
#include <vector>
#include <boost/optional.hpp>
#include <boost/any.hpp>
#include "Tables.h"
#include "DatabaseItem.h"
#include <map>

#define DB "student_sys"
#define DB_HOST "localhost"
#define DB_USER "eddy"
#define DB_PASS ""

class Warning;
class Student;
class Lecturer;
class Course;
class Module;
class Exam;
class ExamQuestion;
class ExamAnswer;
class ExamGrade;
class ModuleGrade;
class LecturerAccount;
class StudentAccount;

/**
 * The purpose of this class is to abstract out most of the database components and the database used from the rest of the system
 * i.e. abstracting out the tasks for querying or inserting data into the database
 *  Note, however methods for executing queries are still provided for any of the limited use cases
 */
class DatabaseManager
{
private:
    Driver *driver;
    Connection *connection;
    Statement *stmt;               //the statement to be used to execute queries
    std::vector<Warning> warnings; //keeps record of any warnings/exceptions that occurred
    static const std::map<Tables, std::string> tableNames;
    void clearTable(Tables table);
    std::vector<ExamQuestion> getAllExamQuestions(int examID);
    void setLastExamID();
    //following private methods throw their exceptions to their public callers
    //private add methods for answers and questions
    bool add(const ExamAnswer &answer);
    bool add(const ExamQuestion &question);
    //private update methods to update questions and answers (these do not throw keymismatch since keys are what will be updated)
    bool update(const ExamAnswer &oldAnswer, const ExamAnswer &newAnswer);
    bool update(const ExamQuestion &oldQuestion, const ExamQuestion &newQuestion);

public:
    //with remove methods figure out what to do if a foreign key constraint prevents it
    //add methods to perform updates
    DatabaseManager(std::string database = DB, std::string url = DB_HOST, std::string user = DB_USER, std::string pass = DB_PASS);
    ~DatabaseManager();
    
    //Overloaded add methods to add different entities to the database
    bool add(const Lecturer &lecturer);
    bool add(const Course &course);
    bool add(const Student &student);
    bool add(const Module &module);
    bool add(const Exam &exam);
    bool add(const ExamGrade &examGrade);
    bool add(const LecturerAccount &lecturerAccount); //const and reference here doesnt seem to work with inheritance because it caused bad_alloc as debigger said memory not accessible for e.g department
    bool add(const StudentAccount &studentAccount);

    //Overloaded remove methods to remove different entities from the database
    bool remove(const Lecturer &lecturer);
    bool remove(const Course &course);
    bool remove(const Student &student);
    bool remove(const Module &module);
    bool remove(const Exam &exam); //can't have exam as const has in the removal process, in order to find the exam, the id for that exam needs to be retrieved from the database
    bool remove(const ExamGrade &examGrade);
    bool remove(const LecturerAccount &lecturerAccount);
    bool remove(const StudentAccount &studentAccount);

    //Overloaded update methods to change different entities in the database, the entity must have same ID, or primary key but a different attribute, if the id (first 1 or 2 params does not match, e.g. id != updatedLecturer.getID(), KeyMismatch exception is thrown)
    bool update(int id, const Lecturer &updatedLecturer);
    bool update(int id, const Student &updatedStudent);
    bool update(std::string id, const Course &updatedCourse);
    bool update(std::string code, const Module &updatedModule);
    //must ensure that each question of the exam, it's answer count is >= what it was before and same with question counts
    bool update(const Exam &oldExam, const Exam &updatedExam);
    bool update(const Student &student, const Exam &exam, const ExamGrade &updatedExamGrade);
    bool update(const Lecturer &lecturer, const LecturerAccount &updatedLecturerAccount);
    bool update(const Student &student, const StudentAccount &updatedStudentAccount);

    //Various get methods to retrieve INDIVIDUAL entities from the database
    boost::optional<Lecturer> getLecturer(int id);
    boost::optional<Course> getCourse(std::string id);
    boost::optional<Student> getStudent(int id);
    boost::optional<Module> getModule(std::string code);
    boost::optional<Exam> getExam(int id);
    boost::optional<ExamGrade> getExamGrade(const Student &student, const Exam &exam);
    boost::optional<ModuleGrade> getModuleGrade(const Module &module, const Student &student);
    boost::optional<LecturerAccount> getLecturerAccount(int id);
    boost::optional<StudentAccount> getStudentAccount(int id);

    //Various get methods to retrieve ALL entities of a type from the database
    std::vector<Lecturer> getAllLecturers();
    std::vector<Course> getAllCourses();
    std::vector<Student> getAllStudents();
    std::vector<Module> getAllModules();
    std::vector<Exam> getAllExams();
    std::vector<ExamGrade> getAllExamGrades();
    std::vector<ModuleGrade> getAllModuleGrades();
    std::vector<LecturerAccount> getAllLecturerAccounts();
    std::vector<StudentAccount> getAllStudentAccounts();


    int getLecturerID(Lecturer &lecturer); 
    int getStudentID(Student &student);
    int getExamID(Exam &exam);

    //Procedure to calculate module grades for the specified module and student
    void calculateModuleGrades(std::string module, const Student &student);

    //Overloaded contains methods to check if the database contains the entity
    /*bool contains(const Lecturer &lecturer);
    bool contains(const Course &course);
    bool contains(const Student &student);
    bool contains(const Module &module);
    bool contains(const Exam &exam);
    bool contains(const ExamGrade &examGrade);
    bool contains(const LecturerAccount &lecturerAccount);
    bool contains(const StudentAccount &studentAccount);*/
    bool contains(const DatabaseItem &item);

    //Various execute methods
    ResultSet *executeQuery(std::string query);
    void execute(std::string query);
    int executeUpdate(std::string query);

    //Clears the database
    void clearDatabase();

    //Warnings generated by the database
    std::vector<Warning> getWarnings() const;
    void clearWarnings();
    void writeWarningsToLog();
};

#endif // DATABASE_MANAGER_H