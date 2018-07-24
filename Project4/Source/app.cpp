#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<iostream>
#include<string>
#include<algorithm>
#include<locale>
#include<cctype>

using namespace std;

#define STUDENT_AMOUNT 10

#define INPUT_FAIL "Input fail. Input again.\n"
#define REQUIRE_NAME "Input student %d name:"
#define	REQUIRE_ID "Input student %d id:"
#define REQUIRE_SCORE "Input student %d score:"
#define PATTERN_ID "%d%c"
#define PATTERN_SCORE "%f%c"
#define NAME_DISPLAY "Student %d name:%s\n"
#define ID_DISPLAY "Student %d id:%d\n"
#define SCORE_DISPLAY "Student %d score:%0.2f\n"

#define MAX_BUFFER_SIZE 20
#define ASCEND_ORDER 1
#define DESCEND_ORDER 2
#define DISCARD_VALUE 5

void trim(string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {return !std::isspace(ch);}));
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {return !std::isspace(ch);}).base(), s.end());
}

typedef struct _Student {
	char *name;
	int id;
	float score;
	struct _Student *next;

	bool checkScore() {
		if ((score < 0) || (score > 10)) 
			return false;
		return true;
	}

	_Student() {
		name = NULL;
		id = 0;
		score = 0.0;
		next = NULL;
	}

	~_Student() {
		if (name != NULL)
			delete[]name;
	}
} Student;

struct Classroom {
	Student *students;
	int student_amount;

	Classroom() {
		students = new Student;
		student_amount = 0;
	}

public:
	void input_students() {
		//bool function_result = false;
		int name_requirement = 1, id_requirement = 2, score_requirement = 3;
		Student *iter = students, *pre = NULL;
		int i = 0;

		while(true) {						
			while (!this->input_student_info(iter, name_requirement, i)) {
			}
			while (!this->input_student_info(iter, id_requirement, i)) {
			}
			if (iter->id == 0) {
				pre->next = NULL;
				delete iter;
				break;
			}
			while (!this->input_student_info(iter, score_requirement, i)) {
			}
			pre = iter;
			iter = new Student;
			pre->next = iter;
			i++;
			//display_student_info(i);
		}

		student_amount = i;
	}

	bool checkID(int value) {         //for the reuse
		Student *iter = students;
		while (iter->next != NULL) {
			if (iter->id == value)
				return false;
			iter = iter->next;
		}
		return true;
	}

	bool input_student_info(Student *students,int type, int index) {
		string input = "";
		int amount = 0;

		switch (type) {
		case 1:
			printf(REQUIRE_NAME, index + 1);
			break;
		case 2:
			printf(REQUIRE_ID, index + 1);
			break;
		case 3:
			printf(REQUIRE_SCORE, index + 1);
			break;
		}
		getline(cin, input);
		trim(input);

		switch (type) {
		case 1: {
			amount = input.length();
			if (amount > 0) {
				students->name = new char[MAX_BUFFER_SIZE];
				memcpy(students->name, input.c_str(), amount);
				students->name[amount] = 0;
				return true;
			}
			break;
		}
		case 2: {
			int temp = 0;
			char c = ' ';
			amount = sscanf(input.c_str(), PATTERN_ID, &temp, &c);
			if (amount == 1)                        //int valid_amount=1;
				if (checkID(temp)) {				
					students->id = temp;
					return true;
				}
			break;
		}
		case 3: {
			float temp = 0;
			char c = ' ';
			amount = sscanf(input.c_str(), PATTERN_SCORE, &temp, &c);
			if (amount == 1) {
				students->score = temp;
				if (students->checkScore())
					return true;
			}
			break;
		}
		}
		
		printf(INPUT_FAIL);
		return false;
	}

	void display_student_info(int index, Student *iter) {		
		printf("\n");
		printf(NAME_DISPLAY, index + 1, iter->name);
		printf(ID_DISPLAY, index + 1, iter->id);
		printf(SCORE_DISPLAY, index + 1, iter->score);
		printf("\n");
	}

	void sort_by_score(int type) {          //1-ascend          others-descend  	
		Student *pre = NULL, *first = NULL, *second = NULL;
		for (int i = 0; i < this->student_amount - 1; i++) {
			first = students;
			pre = NULL;
			while (first->next != NULL) {         //for (int i=0; i<this->student_amount - 2 ;i++)
				second = first->next;
				if (type == ASCEND_ORDER) {
					if (first->score > second->score)
						swap_student(pre, first, second);
				}
				else
					if (first->score < second->score)
						swap_student(pre, first, second);
				if (pre == NULL)
					students = first;
				pre = first;
				first = first->next;
			}
		}
	}

	void swap_student(Student *&pre, Student *&x, Student *&y) {
		if (pre!=NULL)
			pre->next = y;
		x->next = y->next;
		y->next = x;

		Student *temp = x;                //second -> first, first->second
		x = y;
		y = temp;
	}

	void display_all_students() {
		Student *iter = students;
		int i = 0;
		while (iter != NULL) {
			display_student_info(i, iter);
			iter = iter->next;
			i++;
		}			
	}

	void remove_student(int min_score) {
		Student *iter = students, *pre = NULL;
		while (iter != NULL) {
			if (iter->score < min_score) {
				if (iter != students) {
					pre->next = iter->next;
					delete iter;
					iter = pre->next;
				}
				else {
					students = iter->next;
					delete iter;
					iter = students;
				}
				this->student_amount--;
				continue;
			} 
			pre = iter;
			iter = iter->next;
		}
	}
};

int main() {
	Classroom class1;
	class1.input_students();

	class1.sort_by_score(ASCEND_ORDER);
	class1.display_all_students();

	class1.sort_by_score(DESCEND_ORDER);
	class1.display_all_students();

	system("pause");
	return 0;
}