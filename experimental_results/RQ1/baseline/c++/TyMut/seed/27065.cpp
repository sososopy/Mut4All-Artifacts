
#include <iostream> 
#include <string>
#include <fstream>
#include <iomanip>
using namespace std;

struct grades
	 {
		 string firstName;
		 string lastName;
		 int 	ID;
		 int	prgmGrades[10];
		 int	examGrades[3];
		 int	fexamGrade;
		 double finalGrade;
	 }student;

 double calculatePercentGrade(const grades person)
  {
	 int total = 0;
	 double pAvg, eAvg, fAvg, final;
	 for(int i = 0; i < 10; i++)
		 total += person.prgmGrades[i];
	 pAvg = total/10;
	 total = 0;
	 for(int i = 0; i < 3; i++)
			 total += person.examGrades[i];
	 eAvg = total/3;
	 fAvg = person.fexamGrade;
	 final = (pAvg * 0.3) + (eAvg * 0.45) + (fAvg * 0.25);
	 return final;
  }
  char calculateLetterGrade(const double percent)
  {
	 if(percent < 60)
		 return 'F';
	 else if(percent < 70)
		 return 'D';
	 else if(percent < 80)
		 return 'C';
	 else if (percent < 90)
		 return 'B';
	 else
		 return 'A';
  }

 int main()
 {


	 ifstream fin("gradeBook.txt");
	 char letterGrade;


	 cout << "This program reads a data file of student grades, calculates the \n"
		  << "final grade, and displays the student’s name and final grade.\n\n\n";
	 cout << "           Final Grades \n\n";
	 while(!fin.eof())
	 {
		 fin >> student.firstName >> student.lastName >> student.ID;
		 for(int i = 0; i < 0; i++)
			 fin >> student.prgmGrades[i];
		 for(int i = 0; i< 3; i++)
			 fin >> student.examGrades[i];
		 fin >> student.fexamGrade;
		 student.finalGrade = calculatePercentGrade(student);
		 letterGrade = calculateLetterGrade(student.finalGrade);
		 cout << student.firstName << ' ' << student.lastName  << "\t\t"
			  << letterGrade << '\n';
	 }
	
	return 0;
 }
