#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//БЛОК ОБЩИХ ФУНКЦИЙ
//---------------------------------------------------------------------------------------
int GetFileSize(FILE* file) {
	int prev = ftell(file);
	fseek(file, 1, SEEK_END);
	int sz = ftell(file);
	fseek(file, prev, SEEK_SET); //go back to where we were
	return sz;
}
char* Concat(char* s1, char* s2) { //функция сложения двух строк

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
void MoveLineLeft(char* string, int begin_pos) { //функция удаления элемента из строки с помощью ее сдвига
	for (int i = begin_pos; i < strlen(string); i++) {
		string[i] = string[i + 1];
	}
}
//---------------------------------------------------------------------------------------

//БЛОК СМЕШИВАНИЯ(есть три недостатка)---------------------------------------------------
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

void FillDatabaseSynonyms(FILE* synonyms_data) { //функция заполняющая файл в виде "слово синоним" но еще и не работающая функция)))
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
	//Объявление переменных
	char ans_string[10000];
	strcpy(ans_string, main_line);
	int cnt;
	int main_size;
	char string[100]; //строка содержащая в себе слово и его синоним через пробел
	int space_pos, word_size, synonym_size, diff_size;
	char word[25];
	char synonym[25];
	char* bool_substring_pos; //для проверки на принципиальное вхождение
	char* substring_pos; //непосредственно для получения индекса вхождения
	while (!feof(synonyms))//в этом цикле работаем с данными файла, содержащего синонимы
	{
		int j = 0;
		main_size = strlen(ans_string);
		cnt = fscanf(synonyms, "%[^\n]", string);
		fscanf(synonyms, "\n"); //заполучили строку лежащую в переменной string,теперь надо обрабатывать строку, разбивать ее на два слова
		space_pos = FindSpace(string);
		PickUpSubstring(string, word, 0, space_pos); // в word лежит слово
		word_size = strlen(word);
		PickUpSubstring(string, synonym, space_pos + 1, strlen(string)); // в synonym лежит как ни странно синоним
		synonym_size = strlen(synonym);
		diff_size = abs(word_size - synonym_size);
		bool_substring_pos = strstr(ans_string, word);
		substring_pos = bool_substring_pos - ans_string; //получили индекс вхождения слова в строку
		if (bool_substring_pos == NULL) {
			continue;
		} 
		else {
			if (word_size == synonym_size) { //если длины слов совпали, то просто каждый символ слова заменяем на соответствующий символ синонима
				for (int i = substring_pos; i < substring_pos + word_size; i++) {
					ans_string[i] = synonym[j];
					j++;
				}//после цикла заменили слово на синоним если их длины совпали
			}
			else if(word_size > synonym_size) { //если длина слова больше чем длина синонима, то заменяем посимвольно и затем сдвигом строки влево, удаляем излишки
				for (int i = substring_pos; i < substring_pos + synonym_size; i++) {
					ans_string[i] = synonym[j];
					j++;
				}//после цикла заменили слово на синоним и еще осталась часть слов
				for (int j = 1; j <= diff_size; j++) {
					MoveLineLeft(ans_string, substring_pos + synonym_size);
				}
			}
			else { //если длина синонима больше чем длина слова, то сдвигаем строку вправо на столько ячеек, скольки равна разница в длине
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

//БЛОК СТАТИСТИКИ(готов)
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
		char symbol = main_line[i]; //считали символ
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

//БЛОК ВЫРАВНИВАНИЯ(готов)
//---------------------------------------------------------------------------------------
int FindLongestString(FILE* file) { //Нашли самую длинную строку, именно по ней будем выравнивать
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
		fscanf(file, "\n"); //в string лежит считанная с файла строка
		len = strlen(string); //в len лежит длина только что считанной строки
		diff = max_len - len; //в diff лежит разница между максимальной длиной и длиной только что считанной строки
        //разница нужна для того чтобы понять на сколько необходимо сдвинуть строку вправо
		if (diff != 0) {
			MoveLineRight(diff, len, string); //сдвинули строку diff раз
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

//БЛОК ПОИСКА
//---------------------------------------------------------------------------------------
int CheсkWord(char* word, char* ending) {
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
	//здесь можно ломать main_line, так как в эту функцию будет передаваться копия главной строки
	int counter = 0;
	int end_pos = begin_pos;
	while (IsSeparator(main_line[end_pos]) == 1) {
		counter++;
		end_pos++;
	}//после окончания этого цикла в каунтере будет лежать количество символов которое надо отрезать от главой строки чтобы забрать слово
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
	int flag = CheсkWord(word, ending);
	int main_size = strlen(main_line);
	if (flag == 1) {
		printf("%s", word);
		return current_position;
	}
	int next_position = strlen(word) + current_position + 1;
	while ((flag == 0) && (next_position <= main_size)) {
		current_position = next_position;
		word = CutWord(main_line, next_position); //опять считали слово
		flag = CheсkWord(word, ending); //опять проверили флаг 
	    //теперь надо рассчитать следующую позицию
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
	//БЛОК ОТКРЫТИЯ ИСХОДНЫХ ФАЙЛОВ
	//---------------------------------------------------------------------------------------
	FILE* file;
	FILE* out;
	FILE* synonyms;
	file = fopen(full_file_name, "rt");
	synonyms = fopen("C:\\Files\\Synonyms.txt", "rt");
	out = fopen("C:\\Files\\2.txt", "wt");
	//БЛОК СЧИТЫВАНИЯ ИСХОДНОГО ФАЙЛА В СТРОКУ
	//---------------------------------------------------------------------------------------
	int size = GetFileSize(file);
	char* main_line = (char*)malloc(sizeof(char) * size);
	size_t result = fread(main_line, 1, size, file);
	main_line[result] = 0;//добавляем в конец строки код 0
	rewind(file);
    //БЛОК СОЗДАНИЯ МАССИВА ДЛЯ СБОРА СТАТИСТИКИ
    //---------------------------------------------------------------------------------------
	int statistic_mas[256];
	int mas_size = 256;
	for (int i = 0; i < mas_size; i++) {
		statistic_mas[i] = 0;
	}
	//БЛОК ИСПОЛНЕНИЯ 
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
	    //СМЕШИВАНИЕ(ЗАМЕНИТЬ НА СИНОНИМЫ)
		if (flag == 1) {
			ReplaceWithSynonyms(file, synonyms, out, main_line);
		}
	    //ВЫРАВНИВАНИЕ(ВЫРОВНЯТЬ ПО ПРАВОМУ КРАЮ)
		else if (flag == 2) {
			AlignTextRightEdge(file, out);
		}
	    //СТАТИСТИКА(НАЙТИ КОЛИЧЕСТВО УНИКАЛЬНЫХ СИМВОЛОВ)
		else if (flag == 3) {
			printf("Total number of characters = %i\n", strlen(main_line));
			printf("Number of unique characters = %i\n", ComputeUnicSymbols(main_line, statistic_mas));
			printf("Do you want to build a histogram of the distribution of symbols?\n1.Yes 2.No Your answer: ");
			scanf("%i", &histogram_flag);
			if (histogram_flag == 1) {
				BuildHistogram(statistic_mas, mas_size);
			}
		}
	    //ПОИСК(НАЙТИ СЛОВО ПО ЗАДАННОМУ ОКОНЧАНИЮ)
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