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

void trim(string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {return !std::isspace(ch);}));
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {return !std::isspace(ch);}).base(), s.end());
}

struct student {
	char *name;
	int id;
	float score;

	bool checkScore() {
		if ((score < 0) || (score > 10)) 
			return false;
		return true;
	}

	student() {
		name = NULL;
		id = 0;
		score = 0.0;
	}

	~student() {
		if (name != NULL)
			delete[]name;
	}
};

struct classroom {
	student students[STUDENT_AMOUNT];

public:
	void input_students() {
		//bool function_result = false;
		int name_requirement = 1, id_requirement = 2, score_requirement = 3;
		for (int i = 0; i < STUDENT_AMOUNT; i++) {
			while (!this->input_student_info(name_requirement, i)) {
			}
			while (!this->input_student_info(id_requirement, i)) {
			}
			while (!this->input_student_info(score_requirement, i)) {
			}
			//display_student_info(i);
		}
	}

	bool checkID(int type, int index, int value) {         //for the reuse
		int max_index = 0;
		if (type == 1)    //1-when we are inputting value           
			max_index = index;
		else
			max_index = STUDENT_AMOUNT;

		for (int i = 0; i < max_index; i++)
			if (value == students[i].id)
				return false;
		return true;
	}

	bool input_student_info(int type, int index) {
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
				students[index].name = new char[MAX_BUFFER_SIZE];
				memcpy(students[index].name, input.c_str(), amount);
				students[index].name[amount] = 0;
				return true;
			}
			break;
		}
		case 2: {
			int temp = 0;
			char c = ' ';
			amount = sscanf(input.c_str(), PATTERN_ID, &temp, &c);
			if (amount == 1)                        //int valid_amount=1;
				if (checkID(1, index, temp)) {				//int operation_type=1;
					students[index].id = temp;
					return true;
				}
			break;
		}
		case 3: {
			float temp = 0;
			char c = ' ';
			amount = sscanf(input.c_str(), PATTERN_SCORE, &temp, &c);
			if (amount == 1) {
				students[index].score = temp;
				if (students[index].checkScore())
					return true;
			}
			break;
		}
		}
		
		printf(INPUT_FAIL);
		return false;
	}

	void display_student_info(int index) {		
		printf("\n");
		printf(NAME_DISPLAY, index + 1, students[index].name);
		printf(ID_DISPLAY, index + 1, students[index].id);
		printf(SCORE_DISPLAY, index + 1, students[index].score);
		printf("\n");
	}

	void sort_by_score(int type) {          //1-ascend          others-descend  
		int jmax = 0;
		for (int i = 0; i < STUDENT_AMOUNT - 1; i++) {
			jmax = STUDENT_AMOUNT - i - 1;
			for (int j = 0; j < jmax; j++)
				if (type == 1) {
					if (students[j].score > students[j + 1].score) {
						this->swap_student(j, j+1);
					}
				}
				else
					if (students[j].score < students[j + 1].score) {
						this->swap_student(j, j+1);
					}
		}
	}

	void swap_student(int x, int y) {
		//swap name
		char *temp_name = new char[MAX_BUFFER_SIZE];
		int leng1 = strlen(students[x].name);
		int leng2 = strlen(students[y].name);

		memset(temp_name, 0, MAX_BUFFER_SIZE);
		memcpy(temp_name, students[x].name, leng1);
		
		memcpy(students[x].name, students[y].name, leng2);
		students[x].name[leng2] = 0;

		memcpy(students[y].name, temp_name, leng1);
		students[y].name[leng1] = 0;

		int temp_id = students[x].id;
		students[x].id = students[y].id;
		students[y].id = temp_id;

		float temp_score = students[x].score;
		students[x].score = students[y].score;
		students[y].score = temp_score;

		delete[]temp_name;
	}

	void display_all_students() {
		for (int i = 0; i < STUDENT_AMOUNT; i++) {
			this->display_student_info(i);
		}
	}
};

int main() {
	classroom class1;
	class1.input_students();

	class1.sort_by_score(1);
	class1.display_all_students();

	class1.sort_by_score(2);
	class1.display_all_students();

	system("pause");
	return 0;
}