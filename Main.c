#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//���� ����� �������
//---------------------------------------------------------------------------------------
int GetFileSize(FILE* file) {
	int prev = ftell(file);
	fseek(file, 1, SEEK_END);
	int sz = ftell(file);
	fseek(file, prev, SEEK_SET); //go back to where we were
	return sz;
}
char* Concat(char* s1, char* s2) { //������� �������� ���� �����

	size_t len1 = strlen(s1);
	size_t len2 = strlen(s2);

	char* result = malloc(len1 + len2 + 1);

	if (!result) {
		return NULL;
	}

	memcpy(result, s1, len1);
	memcpy(result + len1, s2, len2 + 1);

	return result;
}
void ShowMenu() {
	printf("Select an action on the file:\n");
	printf("1.Replace words with synonyms ");
	printf("2.Align to the right edge ");
	printf("3.Calculate the total number and number of different characters in the file ");
	printf("4.Find a word by a given ending ");
	printf("0.Finish editing\n");
}
char* MakeFullFilePath() {
	char file_name[100];
	char file_path[100];
	int file_location_flag = 0;
	char standart_file_location[100] = "C:\\Files\\";
	char* full_file_name;
	printf("Please enter the name of the source file taking into account the format: ");
	scanf("%s", file_name);
	printf("Do you want to use a standard file location?\n1.Yes 2.No Your answer: ");
	scanf("%i", &file_location_flag);
	if (file_location_flag == 1) {
		full_file_name = Concat(standart_file_location, file_name);
	}
	else {
		printf("Please write the path to your file: ");
		scanf("%s", file_path);
		full_file_name = Concat(file_path, file_name);
	}
	if (!full_file_name) {
		return NULL;
	}
	return full_file_name;

}
void MoveLineLeft(char* string, int begin_pos) { //������� �������� �������� �� ������ � ������� �� ������
	for (int i = begin_pos; i < strlen(string); i++) {
		string[i] = string[i + 1];
	}
}
//---------------------------------------------------------------------------------------

//���� ����������(���� ��� ����������)---------------------------------------------------
int FindSpace(char text[]) {
	for (int i = 0; i < strlen(text); i++) {
		if (text[i] == ' ')
			return i;
	}
	return -1;
}

void PickUpSubstring(char text[], char substring[], int begin, int end) {
	int pos = 0;
	for (int i = begin; i < end; i++) {
		substring[pos] = text[i];
		pos++;
	}
	substring[pos] = 0;
}

void FillDatabaseSynonyms(FILE* synonyms_data) { //������� ����������� ���� � ���� "����� �������" �� ��� � �� ���������� �������)))
	printf("Please enter the word and its synonym in the format: word synonym. If you want to finish filling the database, enter 0\n");
	int flag = 1;
	char string[100];
	while (flag == 1) {
		fgets(string, 100, stdin);
		if (strlen(string) == 2) {
			flag = 0;
		}
		else
			fprintf(synonyms_data, "%s", string);
	}
}

void ReplaceWord(char* string, char* word, char* pos, int size) {
	int j = 0;
	for (int i = 0; i < pos + size - 1; i++) {
		string[i] = word[j];
		j++;
	}
}

void ReplaceWithSynonyms(FILE* file, FILE* synonyms, FILE* out, char* main_line) {
	//���������� ����������
	char ans_string[10000];
	strcpy(ans_string, main_line);
	int cnt;
	int main_size;
	char string[100]; //������ ���������� � ���� ����� � ��� ������� ����� ������
	int space_pos, word_size, synonym_size, diff_size;
	char word[25];
	char synonym[25];
	char* bool_substring_pos; //��� �������� �� �������������� ���������
	char* substring_pos; //��������������� ��� ��������� ������� ���������
	while (!feof(synonyms))//� ���� ����� �������� � ������� �����, ����������� ��������
	{
		int j = 0;
		main_size = strlen(ans_string);
		cnt = fscanf(synonyms, "%[^\n]", string);
		fscanf(synonyms, "\n"); //���������� ������ ������� � ���������� string,������ ���� ������������ ������, ��������� �� �� ��� �����
		space_pos = FindSpace(string);
		PickUpSubstring(string, word, 0, space_pos); // � word ����� �����
		word_size = strlen(word);
		PickUpSubstring(string, synonym, space_pos + 1, strlen(string)); // � synonym ����� ��� �� ������� �������
		synonym_size = strlen(synonym);
		diff_size = abs(word_size - synonym_size);
		bool_substring_pos = strstr(ans_string, word);
		substring_pos = bool_substring_pos - ans_string; //�������� ������ ��������� ����� � ������
		if (bool_substring_pos == NULL) {
			continue;
		} 
		else {
			if (word_size == synonym_size) { //���� ����� ���� �������, �� ������ ������ ������ ����� �������� �� ��������������� ������ ��������
				for (int i = substring_pos; i < substring_pos + word_size; i++) {
					ans_string[i] = synonym[j];
					j++;
				}//����� ����� �������� ����� �� ������� ���� �� ����� �������
			}
			else if(word_size > synonym_size) { //���� ����� ����� ������ ��� ����� ��������, �� �������� ����������� � ����� ������� ������ �����, ������� �������
				for (int i = substring_pos; i < substring_pos + synonym_size; i++) {
					ans_string[i] = synonym[j];
					j++;
				}//����� ����� �������� ����� �� ������� � ��� �������� ����� ����
				for (int j = 1; j <= diff_size; j++) {
					MoveLineLeft(ans_string, substring_pos + synonym_size);
				}
			}
			else { //���� ����� �������� ������ ��� ����� �����, �� �������� ������ ������ �� ������� �����, ������� ����� ������� � �����
				int z = main_size;
				for (int i = 1; i <= diff_size; i++) {
					while (z > substring_pos+word_size-1) {
						ans_string[z + 1] = ans_string[z];
						z--;
					}
					z = strlen(ans_string);
				}
				for (int i = substring_pos; i < substring_pos + synonym_size; i++) {
					ans_string[i] = synonym[j];
					j++;
				}
			}
		}
	}
	printf("%s", ans_string);
	fprintf(out, "%s", ans_string);
	fclose(out);
}
//---------------------------------------------------------------------------------------

//���� ����������(�����)
//---------------------------------------------------------------------------------------
int CheckEntry(char* string, char symbol) {
	for (int i = 0; i < strlen(string); i++) {
		if (string[i] == symbol) {
			return 1;
		}
	}
	return 0;
}

int ComputeUnicSymbols(char* main_line, int* statistic_mas) {
	int size = strlen(main_line);
	char mas[100]="";
	int j = 0;
	mas[j] = main_line[0];
	for (int i = 1; i < size; i++) {
		char symbol = main_line[i]; //������� ������
		statistic_mas[(int)symbol] += 1;
		if (CheckEntry(mas, symbol) == 0) {
			j++;
			mas[j] = main_line[i];
		}
	}
	return strlen(mas);
}

void Print(int num) {
	for (int i=0; i<=num; i++)
		printf("%c", (char)254);
}

void BuildHistogram(int* mas, int size) {
	for (int i = 0; i < size; i++) {
		if (mas[i] != 0 && i != '\n' && i != ' ') {
			printf("%c : ", (char)i);
			Print(mas[i]);
			printf("\n");
		}
	}  
}
//---------------------------------------------------------------------------------------

//���� ������������(�����)
//---------------------------------------------------------------------------------------
int FindLongestString(FILE* file) { //����� ����� ������� ������, ������ �� ��� ����� �����������
	int cnt;
	int max_len = -1;
	char string[1000];
	while (!feof(file)) {
		cnt = fscanf(file, "%[^\n]", string);
		fscanf(file, "\n");
		int len = strlen(string);
		if (len > max_len) {
			max_len = len;
		}
	}
	rewind(file);
	return max_len;
}
void MoveLineRight(int diff, int len, char* string) {
	int z = len;
	for (int i = 1; i <= diff; i++) {
		while (z >= 0) {
			string[z + 1] = string[z];
			z--;
		}
		z = strlen(string);
	}
}
void AlignTextRightEdge(FILE* file, FILE* out) {
	char string[10000];
	int cnt, diff, len;
	int max_len = FindLongestString(file);
	while (!feof(file)) {
		cnt = fscanf(file, "%[^\n]", string);
		fscanf(file, "\n"); //� string ����� ��������� � ����� ������
		len = strlen(string); //� len ����� ����� ������ ��� ��������� ������
		diff = max_len - len; //� diff ����� ������� ����� ������������ ������ � ������ ������ ��� ��������� ������
        //������� ����� ��� ���� ����� ������ �� ������� ���������� �������� ������ ������
		if (diff != 0) {
			MoveLineRight(diff, len, string); //�������� ������ diff ���
			for (int i = 0; i < diff; i++) {
				string[i] = ' ';
			}
		}
		printf("%s\n", string);
		fprintf(out, "%s\n", string);
	}
	fclose(out);
}
//---------------------------------------------------------------------------------------

//���� ������
//---------------------------------------------------------------------------------------
int Che�kWord(char* word, char* ending) {
	int flag = 1;
	int word_size = strlen(word);
	int counter = strlen(ending);
	int word_index = word_size-1;
	int ending_index = counter - 1;
	while ((counter > 0) && (flag ==1)) {
		if (word[word_index] == ending[ending_index]) {
			counter--;
			word_index--;
			ending_index--;
		}
		else {
			return 0;
		}
	}
	return 1;
}

int IsSeparator(char symbol) {
	char separators[] = { ' ', ',', '.', '!', '?', ';', ':', '\n' };
	for (int i = 0; i < 8; i++) {
		if (symbol == separators[i])
			return 0;
	}
	return 1;
}

char* CutWord(char* main_line, int begin_pos) {
	//����� ����� ������ main_line, ��� ��� � ��� ������� ����� ������������ ����� ������� ������
	int counter = 0;
	int end_pos = begin_pos;
	while (IsSeparator(main_line[end_pos]) == 1) {
		counter++;
		end_pos++;
	}//����� ��������� ����� ����� � �������� ����� ������ ���������� �������� ������� ���� �������� �� ������ ������ ����� ������� �����
	char* word = malloc((counter) * sizeof(char));
	int j = 0;
	for (int i = begin_pos; i <end_pos; i++) {
		word[j] = main_line[i];
		j++;
	}
	word[counter] = 0;
	return word;
}

int FindWordGivenEnding(char* main_line) {
	char ending[10];
	int current_position = 0;
	printf("Please, enter the ending: ");
	scanf("%s", ending);
	char* word = CutWord(main_line, current_position);
	int flag = Che�kWord(word, ending);
	int main_size = strlen(main_line);
	if (flag == 1) {
		printf("%s", word);
		return current_position;
	}
	int next_position = strlen(word) + current_position + 1;
	while ((flag == 0) && (next_position <= main_size)) {
		current_position = next_position;
		word = CutWord(main_line, next_position); //����� ������� �����
		flag = Che�kWord(word, ending); //����� ��������� ���� 
	    //������ ���� ���������� ��������� �������
		next_position = strlen(word) + current_position + 1;
	}
	if (next_position <= main_size) {
		printf("%s ", word);
		return(current_position);
	}
	else {
		printf("There is no word with this ending\n");
		return 0;
	}
}
//---------------------------------------------------------------------------------------

int main()
{
	int cnt;
	char string[10000];
	char* full_file_name = MakeFullFilePath();
	int flag = 10;
	int histogram_flag = 0;
	int file_location_flag = 0;
	int change_file_flag = 0;
	printf("Your file: %s\n", full_file_name);
	//���� �������� �������� ������
	//---------------------------------------------------------------------------------------
	FILE* file;
	FILE* out;
	FILE* synonyms;
	file = fopen(full_file_name, "rt");
	synonyms = fopen("C:\\Files\\Synonyms.txt", "rt");
	out = fopen("C:\\Files\\2.txt", "wt");
	//���� ���������� ��������� ����� � ������
	//---------------------------------------------------------------------------------------
	int size = GetFileSize(file);
	char* main_line = (char*)malloc(sizeof(char) * size);
	size_t result = fread(main_line, 1, size, file);
	main_line[result] = 0;//��������� � ����� ������ ��� 0
	rewind(file);
    //���� �������� ������� ��� ����� ����������
    //---------------------------------------------------------------------------------------
	int statistic_mas[256];
	int mas_size = 256;
	for (int i = 0; i < mas_size; i++) {
		statistic_mas[i] = 0;
	}
	//���� ���������� 
	//---------------------------------------------------------------------------------------
	while (flag != 0) {
		file = fopen(full_file_name, "rt");
		synonyms = fopen("C:\\Files\\Synonyms.txt", "rt");
		out = fopen("C:\\Files\\2.txt", "wt");
		size = GetFileSize(file);
		main_line = (char*)malloc(sizeof(char) * size);
		result = fread(main_line, 1, size, file);
		main_line[result] = 0;
		rewind(file);
		ShowMenu();
		scanf("%i", &flag);
	    //����������(�������� �� ��������)
		if (flag == 1) {
			ReplaceWithSynonyms(file, synonyms, out, main_line);
		}
	    //������������(��������� �� ������� ����)
		else if (flag == 2) {
			AlignTextRightEdge(file, out);
		}
	    //����������(����� ���������� ���������� ��������)
		else if (flag == 3) {
			printf("Total number of characters = %i\n", strlen(main_line));
			printf("Number of unique characters = %i\n", ComputeUnicSymbols(main_line, statistic_mas));
			printf("Do you want to build a histogram of the distribution of symbols?\n1.Yes 2.No Your answer: ");
			scanf("%i", &histogram_flag);
			if (histogram_flag == 1) {
				BuildHistogram(statistic_mas, mas_size);
			}
		}
	    //�����(����� ����� �� ��������� ���������)
		else if (flag == 4) {
			printf("Word position = %i", FindWordGivenEnding(main_line));
		}
		else if (flag == 0) {
			return 0;
		}
		printf("\nDo you want to choose a different source file?\n1.Yes 2.No Your answer:  ");
		scanf("%i", &change_file_flag);
		if (change_file_flag == 1) {
			full_file_name = MakeFullFilePath();
			printf("Your file: %s\n", full_file_name);
		}
	}
}