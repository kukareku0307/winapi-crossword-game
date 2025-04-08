


/* Курсовая: Кроссворд */
/* Яшин Максим */

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <conio.h>
#include <stdlib.h>		
#include <time.h>	// для инициализации рандомайзера


#include <Windows.h>
#define _WIN32_WINNT_ 0x0A00



#define SIZE 20
#define LENGTH 16
#define DOWN -1
#define ACROSS 1

 



//--//
#define bt_start_id 500
#define edit_id 501
#define bt_enter_id 502
#define bt_check_id 503
#define bt_info_id 504
#define bt_ent_id 505
//--//





//#################################################### Глобальные переменные ####################################################

char words[SIZE][LENGTH];//WORDS   Это три независимые переменные, которыми мы можем легко манипулировать в функциях.
char words_win[SIZE][LENGTH];//ввод в графике
char board[15][15];
char boardDuplicate[15][15];
char boardDuplicate2[15][15];

//char checkletter[15][15];
int wordNum;
int numb = 0;
//int nice=0;


int onBoard[20][4]; 
/*Это массив, который будет отслеживать, какие слова находятся на доске, в каком направлении и в каком месте. [.][0]: на доске - 0 = нет, 1 = есть.
[.][1] указывает направление, в котором они находятся — ВНИЗ, ПОПЕРЕЧНО или 0, если нет на доске. [.][2] = строка, [.][3]= столбец*/

char clueStr[SIZE][LENGTH];
char clueStr_win[SIZE][LENGTH+40];
int winClue;

void sortWords(int);
void capitalize(void);
void printWords(void);
void makeBoard(void);
void printBoard(void);
void putOnBoard(int,int,int,int);
void lookForMatch(int,int,int,int);
int checkBoard(int);
void copyBoard1(void);
void copyBoard2(void);
void restoreBoard(void);
void checkBounds(int, int, int, int, int, int, int);
void printEmptyBoard(void);
void scramble_str(void);
void printClues(void);


//####################################################################################################################################### ГРАФИКА


HWND wn_check;
LRESULT WndProc_check_but(HWND wn_check, UINT message, WPARAM wparam, LPARAM lparam) {

	if (message == WM_DESTROY)//приложение закрылось
		PostQuitMessage(0);//создаёт сообщение о закрытии окна приложения

	else return DefWindowProcA(wn_check, message, wparam, lparam);//полностью завершает программу
}

LRESULT check_but(HWND wn_start) {

	WNDCLASSA wcl_3;//контекст устройства
	memset(&wcl_3, 0, sizeof(WNDCLASSA));
	wcl_3.lpszClassName = "ChildClass";
	wcl_3.lpfnWndProc = (WNDPROC)(WndProc_check_but);//фунция обработки сообщений
	RegisterClass(&wcl_3);


	wn_check = CreateWindow("ChildClass", "Ответы", WS_OVERLAPPEDWINDOW, 10, 10, 640, 640, wn_start, NULL, NULL, NULL);
	ShowWindow(wn_check, SW_NORMAL);

	char windboard[10];
	int i, j;
	for (i = 0; i < 15; i++) {
		for (j = 0; j < 15; j++) {
			HWND bt_field;
			if (board[i][j] == '-') {
				bt_field = CreateWindow("edit", "-", WS_VISIBLE | WS_CHILD, 10 + j * 40, i * 40, 40, 40, wn_check, NULL, NULL, NULL);
			}
			else {
				snprintf(windboard, 10, "%c", board[i][j]);
				bt_field = CreateWindow("edit", windboard, WS_VISIBLE | WS_CHILD, 10 + j * 40, i * 40, 40, 40, wn_check, NULL, NULL, NULL);
			}
		}
	}

	MSG msg_st;

	while (GetMessage(&msg_st, NULL, 0, 0))//очередь сообщений
	{
		DispatchMessage(&msg_st);//Отправляет сообщение в процедуру окна
	}

	return 0;
}



HWND wn_start;
LRESULT WndProc_start(HWND wn_start, UINT message, WPARAM wparam, LPARAM lparam){
	if (message == WM_DESTROY) {
		PostQuitMessage(0);
	}
	/*else if (message == bt_check_id) {
		int check_nice = 2;
		for (int i = 0; i < 15; i++) {
			if (check_nice == 1)
				break;
			for (int j = 0; j < 15; i++) {
				if (checkletter[i][j] == board[i][j])
					check_nice = 0;
				else {
					check_nice = 1;
					break;
				}


			}
		}
		if (check_nice == 0) {
			check_but(wn_start);
			printf("rek");
		}
		else {
			check_but(wn_start);
			printf("rek");
		}
	}

	/*else if (message == bt_ent_id) {
		GetWindowText(lparam, checkletter[LOWORD(wparam) / 15], 15);
	}/
	else if (message == WM_KEYDOWN)
	{
		for (int i = 0; i < 225; i+15) {
			if (LOWORD(wparam) == i + 1000) {
				GetWindowText(lparam, checkletter[i/15], 15);
			}
		}
	}*/

	else if (LOWORD(wparam) == bt_check_id)
		check_but(wn_start);

	else return DefWindowProcA(wn_start, message, wparam, lparam);
	
	
}

LRESULT start() {

	WNDCLASSA wcl_1;//контекст устройства
	memset(&wcl_1, 0, sizeof(WNDCLASSA));
	wcl_1.lpszClassName = "my window 2";
	wcl_1.lpfnWndProc = (WNDPROC)(WndProc_start);//фунция обработки сообщений
	RegisterClass(&wcl_1);


	wn_start = CreateWindow("my window 2", "Crossword", WS_OVERLAPPEDWINDOW , 10, 10, 1200, 640, NULL, NULL, NULL, NULL);
	ShowWindow(wn_start, SW_NORMAL);



	HWND exit = CreateWindow("button", "Ответы", WS_VISIBLE | WS_CHILD, 1070, 20, 80, 40, wn_start, bt_check_id, NULL, NULL);


	//HWND not = CreateWindow("static", "Ответы", WS_VISIBLE | WS_CHILD, 1070, 20, 80, 40, wn_start, bt_check_id, NULL, NULL);


	//HWND nike = CreateWindow("button", "Ответы", WS_VISIBLE | WS_CHILD, 1130, 300, 50, 20, wn_start, bt_ent_id, NULL, NULL);

	/*for (int i = 0; i < 16; i++) {
		char buf_fiel[10];
		sprintf(buf_fiel, "%d", i);
		HWND edit_field= CreateWindow("edit", buf_fiel, WS_VISIBLE | WS_CHILD, 20 + i * 40, 0, 40, 40, wn_start, NULL, NULL, NULL);
	}*/
	char windboard[10];
	int i, j;
	int nice =1000;
	for (i = 0; i < 15; i++) {
		for (j = 0; j < 15; j++) {
			HWND bt_field;
			if (board[i][j] != '-') {
				bt_field = CreateWindow("edit", "#", WS_VISIBLE | WS_CHILD, 10 + j * 40, i * 40, 40, 40, wn_start, nice, NULL, NULL);
				//checkletter[i][j] = "#";
			}
			else {
				snprintf(windboard, 10, "%c", board[i][j]);
				bt_field = CreateWindow("edit", windboard, WS_VISIBLE | WS_CHILD, 10 + j * 40,i * 40, 40, 40, wn_start, nice, NULL, NULL);
				//checkletter[i][j] = board[i][j];
			}
			nice++;
		}
	}

	/*int c = 0;
	while (c < winClue) {
		HWND st_clue = CreateWindow("static", clueStr_win[c], WS_VISIBLE | WS_CHILD, 650, 20 + c * 30, 400, 30, wn_start, NULL, NULL, NULL);
		c++;
	}*/

	HWND st_clue_begin = CreateWindow("static", "Начало по X и Y |  Направление   |  Анаграмма", WS_VISIBLE | WS_CHILD, 650, 20, 350, 30, wn_start, NULL, NULL, NULL);
	//HWND st_clue_stick = CreateWindow("static", "______________|_______________|_____________", WS_VISIBLE | WS_CHILD, 650, 30, 350, 30, wn_start, NULL, NULL, NULL);
	for (int k=0,i = 0; i < SIZE; i++) {
		if (onBoard[i][0] == 1) {
			int direc = onBoard[i][1];
			int row = onBoard[i][2];
			int col = onBoard[i][3];
			int len = strlen(words[i]);
			if (direc == 1)
				sprintf(clueStr_win[i], " X:%-2d" " Y:%-5d" "%-18s" " %-20s", col + 1, row + 1, "| По горизонтали | ", clueStr[i]);

			else 
				sprintf(clueStr_win[i], " X:%-2d" " Y:%-5d" "%-18s" " %-20s", col + 1, row + 1, "| По вертикали    |", clueStr[i]);
			HWND st_clue = CreateWindow("static", clueStr_win[i], WS_VISIBLE | WS_CHILD, 650, 70 + k * 30, 350, 30, wn_start, NULL, NULL, NULL);
			k++;
		}
	}


	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

}



HWND wn_info;
LRESULT WndProc_info(HWND wn_info, UINT message, WPARAM wparam, LPARAM lparam) {

	if (message == WM_DESTROY) {//приложение закрылось
		PostQuitMessage(0);//создаёт сообщение о закрытии окна приложения
		return 0;
	}

	else return DefWindowProcA(wn_info, message, wparam, lparam);//полностью завершает программу
}

LRESULT info(HWND hwnd) {

	WNDCLASSA wcl_2;//контекст устройства
	memset(&wcl_2, 0, sizeof(WNDCLASSA));
	wcl_2.lpszClassName = "ChildClass";
	wcl_2.lpfnWndProc = (WNDPROC)(WndProc_info);//фунция обработки сообщений
	RegisterClass(&wcl_2);


	wn_info = CreateWindow("ChildClass", "Information", WS_OVERLAPPEDWINDOW, 10, 10, 600, 400, hwnd, NULL, NULL, NULL);
	ShowWindow(wn_info, SW_NORMAL);

	HWND text_info = CreateWindow("static", "Курсовую работу выполнил студент Политеха группы 4851001\\10002 (ИКиЗИ) Яшин Максим 2022г.", WS_VISIBLE | WS_CHILD, 50, 200, 500, 50, wn_info, NULL, NULL, NULL);
	//  HWND text_info_2 = CreateWindow("static", "", WS_VISIBLE | WS_CHILD, 180, 500, 80, 20, wn_start, (HMENU)text_1_id, NULL, NULL);


	MSG msg_st;

	while (GetMessage(&msg_st, NULL, 0, 0))//очередь сообщений
	{
		DispatchMessage(&msg_st);//Отправляет сообщение в процедуру окна
	}

	return 0;
}



HWND enter_words;
HWND count;
LRESULT WINAPI WndProc_host_window(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) 
{
	int i = 0;

	char count_num[50];

	if (message == WM_DESTROY) {
		wordNum = numb;
		PostQuitMessage(0);
	}

	else if (message == WM_KEYDOWN)
		printf("code = %d\n", wparam);
		

	else if (LOWORD(wparam) == edit_id) {
		GetWindowText(lparam, words_win[numb], LENGTH);
	}

	else if (LOWORD(wparam) == bt_enter_id) {
		if (numb < SIZE) {
			strcpy(words[numb], words_win[numb]);
			wordNum = numb;
			sprintf(count_num, "%s" "%d", "Количество введенных слов: ", numb + 1);
			SetWindowText(count, count_num);
			SetWindowText(enter_words, "");
			numb++;
			//printf("%s\n", words[numb - 1]);
		}
	}

	else if (LOWORD(wparam) == bt_info_id) {
		info(hwnd);
	}

	else if (LOWORD(wparam) == bt_start_id) {

		wordNum = numb;
		PostQuitMessage(0);
		return 0;
		/*
		srand(time(NULL));

		int max = 0;
		int k = 0;
		int i, j;

		sortWords(SIZE);
		capitalize();

		makeBoard();

		for (i = 0; i < wordNum; i++) {
			for (j = 0; j < 2; j++) {
				onBoard[i][j] = 0;
			}
		}

		int firstWordLength = strlen(words[0]);
		int firstWordStartCol = ((15 - firstWordLength) / 2);
		putOnBoard(0, ACROSS, firstWordStartCol, 7);

		lookForMatch(0, wordNum, 7, firstWordStartCol);

		*/

		//start();
	}


	else return DefWindowProcA(hwnd, message, wparam, lparam);

};

LRESULT host_window() {

	WNDCLASSA wcl; {
		memset(&wcl, 0, sizeof(WNDCLASSA));
		wcl.lpszClassName = "my window";
		wcl.lpfnWndProc = WndProc_host_window;
	}
	RegisterClassA(&wcl);

	HWND hwnd = CreateWindow("my window", "Crossword by max", WS_OVERLAPPEDWINDOW | WS_EX_NOINHERITLAYOUT, 10, 10, 1200, 640, NULL, NULL, NULL, NULL);

	enter_words = CreateWindow("edit", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_RIGHT, 340, 320, 150, 50, hwnd, (HMENU)edit_id, NULL, NULL);

	HWND bt_enter = CreateWindow("button", "enter", WS_VISIBLE | WS_CHILD, 540, 320, 100, 50, hwnd, (HMENU)bt_enter_id, NULL, NULL);

	count = CreateWindow("static", "Количество введенных слов: 0", WS_VISIBLE | WS_CHILD, 340,280, 250, 20, hwnd, NULL, NULL, NULL);

	HWND bt_start = CreateWindow("button", "start", WS_VISIBLE | WS_CHILD, 740, 320, 100, 50, hwnd, (HMENU)bt_start_id, NULL, NULL);

	HWND bt_info;
	bt_info = CreateWindow("button", "Developer information", WS_VISIBLE | WS_CHILD, 490, 430, 200, 50, hwnd, (HMENU)bt_info_id, NULL, NULL);

	ShowWindow(hwnd, SW_SHOWNORMAL);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

}




//###############################




int main(void){
	setlocale(LC_ALL, "Rus");

	host_window();

	//-----------------------------------------------------------------------------------------------------------
	srand( time(NULL) );	//инициализация генератора рандома	
	
int max = 0;
	int k = 0;
	int i,j;
	/*
	printf("Enter a list of words:\n");

	while (k < SIZE) {							//WHILE добавляет слова
		scanf("%s", words[k]);	
		if(strlen(words[k]) > 15){
			printf("\nСлово огромное, введи другое.\n");
			k--;
		}				

		if (strcmp(words[k], ".") == 0) {
			wordNum = k;
			break;
		}
		wordNum = k;
		k++;
	}
	//	*/

	printf("\n");
	sortWords(SIZE);
	capitalize();
	printWords();

	makeBoard();

	for( i = 0 ; i < wordNum ; i++){
		for(j = 0 ; j < 2 ; j++){
			onBoard[i][j] = 0;
		}
	}	

	/*ЧАСТЬ 1: 
	Прога принимает слова, сортирует их, делает все буквы большими, заполняет доску минусами, 
	также обновляем массив "onBoard" который имеет такое же количество елементов сколько слов*/

	int firstWordLength = strlen(words[0]);
	int firstWordStartCol = ( (15-firstWordLength) / 2);
	putOnBoard(0, ACROSS , firstWordStartCol , 7);

	/* ЧАСТЬ 2: Тут мы ставим первое слово по центру горизонтально. также заполняем onBoard[0][0] единицей, так мы понимаем что слово на доске,
	и onBoard[0][1] ставим ACROSS, что указывает направление слово*/

	/* Переменные для следующей части 
	prevWord - последнее слово которое ставили на доску, слово для которого ищем совпадение.
	wordCount - порядкоый номер слова которое сравнивают. ( wordCount = 2 ) = "Третье слово чекается"
	wordNum - кол во слов всего
	*/

	lookForMatch(0,wordNum, 7 , firstWordStartCol);

	/*ЧАСТЬ 3:  Стартуем со второго слова (первое слово уже на доске так что его игнорим), и проверяем каждую букву
	второго слова с первой буквой первого слова, и дальше вторую букву первого слова, и так далее.
	Когда нашлось совпадение, ставим на доску. В процессе, "onBoard" значение найденных слов обновляются так что функция будет пропускать их. */
	
	printf("\nTHE FINAL BOARD:\n");
	printBoard();
	printEmptyBoard();
	scramble_str();
	//printClues();
	
	

	start();
}



//############   ФУНКЦИИ   ################//
void sortWords( int wordNum){

	int ix;
	char maxword[LENGTH];

	for (ix = 0; ix < SIZE; ix++) {		                 
		if ( strlen(words[ix+1]) > strlen(words[ix]) ) {			//Идет по списку и делает пузырьковую сортировку от большего к меньшему слову
			strcpy(maxword,words[ix+1]);
			strcpy(words[ix+1],words[ix]);
			strcpy(words[ix],maxword);
			ix = -1;												//Ставим -1 и начинаем снова с нуля после инкрементирования
		}
	}
}

void capitalize(void){

	int ix;
	int ic;

	for(ix = 0; ix <= wordNum; ix++){
		for(ic = 0; ic < LENGTH; ic++){
			if ((words[ix][ic]) != '-') {
				words[ix][ic] = toupper(words[ix][ic]);
			}
			if (words[ix][ic] == 'я') {
				words[ix][ic] = 'Я';
			}
		}
	}
}

void printWords(void){
	int i;
	for(i = 0 ; i <= wordNum ; i++){
		if (strcmp(words[i], ".") == 0){
			break;
		}
		else
		printf("%s\n",words[i]);
	}
}

void makeBoard(void){
	int i,j;
	for(i = 0 ; i < 15 ; i++){
		for(j = 0 ; j < 15 ; j++){
			board[i][j] = '-';
		}
	}
}

void printBoard(void){
	int i,j;
	printf("\n");
	for( i = 0 ; i < 15 ; i++){
		for( j = 0 ; j < 15 ; j++){
			printf("%c ", board[i][j]);
		}
		printf("\n");
	}
}

void lookForMatch(int prevWordNum, int totalWords, int prevRow , int prevCol){



	int i = 0, j = 0;
	int wordCount;
	int boardCheck;
	for(wordCount = 0 ; wordCount < totalWords ; wordCount++){      		

		if(onBoard[wordCount][0] == 0){											//Если слово не на доске, проверяем подойдет ли оно для последнего добавленного слова на доске
				
			for( j = 0 ; j < strlen(words[wordCount]) && !onBoard[wordCount][0] ; j++){					//j = буква в потенциально подходящем слове
				for(i = 0 ; i < strlen(words[prevWordNum]) ; i++){				//i = буква слова на котором мы ищем совпадение		
						
						if(words[prevWordNum][i] == words[wordCount][j]){	
							int newDirection = onBoard[prevWordNum][1] * -1; //умножаем на минус один чтобы дать направление противоположное слову на котором ищем совпадение					
							checkBounds(totalWords, wordCount, newDirection, prevCol, prevRow, i, j);
						}
						if(onBoard[wordCount][0] == 1) break;
				}
				/*if (onBoard[wordCount][0] == 1) {
					wordCount = 0;
					i = 0;
					j = 0;
					break;
				}*/
			}

		}
	}
}

void putOnBoard(int wordNumber, int direction, int col, int row){

	copyBoard1();
	
	int i;
	int wordLength;
	wordLength = strlen(words[wordNumber]);
	
	switch (direction){

		case DOWN:
			for( i = 0 ; i < wordLength ; i++){
				board[row+i][col] = words[wordNumber][i];
			}
			break;
		case ACROSS:
			for( i = 0 ; i < wordLength ; i++){
				board[row][col+i] = words[wordNumber][i];
			}
		break;
	}


	onBoard[wordNumber][0] = 1;
	onBoard[wordNumber][1] = direction; // Направление
	onBoard[wordNumber][2] = row;
	onBoard[wordNumber][3]= col;
}

int checkBoard(int wordMatch) {
	int i, j;
	int check3 = 0;

	////// PART 1 ////////////////////////// Смотрим переградило ли новое слово другие слова 


	for (i = 0; i < 15; i++) {
		for (j = 0; j < 15; j++) {
			if ((boardDuplicate[i][j] != '-') && (board[i][j] != boardDuplicate[i][j])) { //Это означает что новое слово переградило какое то другое
				restoreBoard();
				check3 = 1;
			}
		}
	}

	///////// PART 2 ////////////////////////// Смотрим есть ли слова которые стоят рядом, хотя по идее не должны стоять. 

		//Сначала удалим все вертикальные слова с доски. Дальше, если два подряд слова стоят вертикально, то удаляем слово.
	copyBoard2();
	for (i = 0; i < 20; i++) {
		if ((onBoard[i][0] == 1) && (onBoard[i][1] == DOWN)) {
			int wordLength = strlen(words[i]);
			int row = onBoard[i][2];
			int col = onBoard[i][3];
			for (j = /*здесь нолик на единичку менял->*/ 1; j < wordLength-1; j++) {
				boardDuplicate2[row + j][col] = '-';
			}

		}
	}

	for (i = 0; i < 15; i++) {
		for (j = 0; j < 15; j++) {
			if ((boardDuplicate2[i][j] != '-') && (boardDuplicate2[i + 1][j] != '-')) {
				restoreBoard();
				check3 = 1;
			}
		}
	}

	//Дальше делаем тоже самое с горизонтальными словами

	copyBoard2();
	for (i = 0; i < 20; i++) {
		if ((onBoard[i][0] == 1) && (onBoard[i][1] == ACROSS)) {
			int wordLength = strlen(words[i]);
			int row = onBoard[i][2];
			int col = onBoard[i][3];
			for (j = /*здесь нолик на единичку менял->*/ 1; j < wordLength-1; j++) {
				boardDuplicate2[row][col + j] = '-';
			}

		}
	}
	for (i = 0; i < 15; i++) {
		for (j = 0; j < 15; j++) {
			if ((boardDuplicate2[i][j] != '-') && (boardDuplicate2[i][j + 1] != '-')) {
				restoreBoard();
				check3 = 1;
			}
		}
	}


	////////Смотри переграждает ли начало нашего слова другие слова //////

	int rowStart = onBoard[wordMatch][2];
	int colStart = onBoard[wordMatch][3];
	int letterNum = strlen(words[wordMatch]);
	int rowFinish = rowStart + letterNum;
	int colFinish = colStart + letterNum;

	if (onBoard[wordMatch][1] == DOWN) {											//Если вертикальное
		if ((rowStart != 0) && (board[rowStart - 1][colStart] != '-')) {			//Если первая буквы не в первом ряду и слот выше занят другой буквой
			restoreBoard();
			check3 = 1;
		}
		else if ((rowFinish + 1 != 15) && (board[rowFinish][colStart] != '-')) {
			restoreBoard();
			check3 = 1;
		}
	}
	else if (onBoard[wordMatch][1] == ACROSS) {											//Если горизонтальное
		if ((colStart != 0) && (board[rowStart][colStart - 1] != '-')) {			//Если первая буквы не в первом ряду и слот выше занят другой буквой
			restoreBoard();
			check3 = 1;
		}
		else if ((colFinish + 1 != 15) && (board[rowStart][colFinish] != '-')) {
			restoreBoard();
			check3 = 1;
		}
	}
	//////МАКСИМ
	/*for (i = 0; i < letterNum; i++) {
		if (onBoard[wordMatch][1] == DOWN) {
			if (((board[rowStart + i][colStart + 1] == '-') && (board[rowStart + i][colStart - 1] != '-')) || ((board[rowStart + i][colStart + 1] != '-') && (board[rowStart + i][colStart - 1] == '-'))) {
				restoreBoard();
				check3 = 1;
			}
		}
	}*/




	return(check3);
}

void copyBoard1(void){	
	int i, j;
	for( i = 0 ; i < 15 ; i++){
		for( j = 0 ; j < 15 ; j++){
			boardDuplicate[i][j] = board[i][j];
		}
	}
}

void copyBoard2(void){	
	int i, j;
	for( i = 0 ; i < 15 ; i++){
		for( j = 0 ; j < 15 ; j++){
			boardDuplicate2[i][j] = board[i][j];
		}
	}
}

void restoreBoard(void){
	int k, l;
	for(k = 0 ; k < 15 ; k++){
		for(l = 0 ; l < 15 ; l++){
				board[k][l] = boardDuplicate[k][l];
		}
	}
}

void checkBounds(int totalWords, int matchNum, int dir, int prevCol, int prevRow, int xi, int xj){


	int check = 0;
	int check2 = 1;
	int wordLength = strlen(words[matchNum]);

	//Проверяем выходит ли слова за границы, если да то не ставим его
	if(dir == ACROSS){
		if((prevCol - xj) < 0 ){
			check = 1;
		}
		else if(  ((prevCol - xj) + wordLength) > 15  ){
			check = 1;
		}
	}
	else if(dir == DOWN){
		if((prevRow - xj) < 0 ){
			check = 1;
		}
		else if(  ((prevRow - xj) + wordLength) > 15  ){
			check = 1;
		}
	}

	//если слово не выходит за границы, оставляем его и вызываем следующие проверки слова 
	if(check == 0){
		
		check2 = 0; 
		if(dir == ACROSS){
			putOnBoard(matchNum, dir, prevCol - xj , prevRow + xi);
		}
		else if(dir == DOWN){
			putOnBoard(matchNum, dir, prevCol + xi , prevRow - xj);
		}

		//printBoard();

		check2 = checkBoard(matchNum); // если ошибка, check2 == 1, и слово будет удалено с доски and onBoard обновится.
	

		if (check2 == 0) { 
			lookForMatch( matchNum, totalWords, onBoard[matchNum][2], onBoard[matchNum][3] ); //Функция вызывет сама себя рекурсивно	
		}
		else if (check2 == 1){
			onBoard[matchNum][0] = 0;				// Делаем нули потому что убрали слово с доски
			onBoard[matchNum][1] = 0;
			onBoard[matchNum][2] = 0;
			onBoard[matchNum][3] = 0;
		}
	}
}

/*scramble_str() --> перемешать буквы в словах слова (делаем анаграммы)*/

void scramble_str(void){

	int iw = 0;
	int ic = 0;
	int strLen;

	int in;

	int det[LENGTH] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	int *ptr[LENGTH];	//рандомим 15 указателей
	char *ptrchar;
	int test;

	while(iw < wordNum){
			strLen = strlen(words[iw]);
			ptrchar = &words[iw][0];
			ic = 0;
		while(*ptrchar != '\0') {
			test = rand() % strLen;
			det[ic] = test;
			ptr[ic] = &det[ic];	//назначить адрес целого числа (используем *, чтобы указать на значение)

			if (ic > 0) {
				for (in = ic - 1; in >= 0; in--){
					if (*ptr[ic] == *ptr[in]) {
						test = rand() % strLen;
						det[ic] = test;
						ptr[ic] = &det[ic];
						in = ic;
					}
				}
			}

			clueStr[iw][ic] = words[iw][*ptr[ic]];

			ptrchar++;
			ic++;
		}

		*ptrchar = '\0';
		iw++;
	}
}

void printEmptyBoard(void){
	int i,j;
	printf("\n");
	for( i = 0 ; i < 15 ; i++){
		for( j = 0 ; j < 15 ; j++){
			if ((board[i][j]) != '-') {
				printf("  ");	
			}
			else {
				printf("# ");	
			}
			
		}
		printf("\n");
	}
}



/*void printClues(void) {
	int i, j, k = 0, n = 0, w, down = 0;
	int strLen;

	char cmpStr[LENGTH];
	char *ptrcmp; //
	char clueWords[SIZE][LENGTH];

	printf("\n");
	printf("Clues:\n");
	printf("\n");

	for( i = 0 ; i < 15 ; i++){
		for( j = 0 ; j < 15 ; j++){	
																				
			if ( (((board[i][j-1]) == '-') || (i == 0)) && ((board[i][j]) != '-') && ((board[i][j + 1]) != '-')) {

				ptrcmp = &board[i][j];								// 
				k = 0;												// 		
				while(*ptrcmp != '-') {								//				
					cmpStr[k] = *ptrcmp;							//					
					k++;
					ptrcmp++;
				}
				cmpStr[k] = '\0';
				
				for(w = 0 ; w < wordNum ; w++) {
					if (strcmp(cmpStr, words[w]) == 0) {
						strcpy(clueWords[n], clueStr[w]);
						n++;
						break;
					}
				}
				printf("%-2d, %-3d %-4s %-5s\n", i+1, j+1, "Across", clueWords[n-1]);
				//sprintf(clueStr_win[winClue], "%-2d, %-3d %-4s %-5s\n", i + 1, j + 1, "Across", clueWords[n - 1]);
				//winClue++;
			}

			if ( (((board[i-1][j]) == '-') || (j == 0)) && ((board[i][j]) != '-') && ((board[i + 1][j]) != '-')) {

				ptrcmp = &board[i][j];
				k = 0, down = 0;
				while(*ptrcmp != '-') {
					cmpStr[k] = *ptrcmp;
					down++;
					ptrcmp = &board[i+down][j];
					k++;
				}
				cmpStr[k] = '\0';
				
				for(w = 0 ; w < wordNum ; w++) {
					if (strcmp(cmpStr, words[w]) == 0) {
						strcpy(clueWords[n], clueStr[w]);
						n++;
						break;
					}
				}
				printf("%-2d, %-3d %-6s %-5s\n", i+1, j+1, "Down", clueWords[n-1]);
				//sprintf(clueStr_win[winClue],"%-2d, %-3d %-6s %-5s\n", i + 1, j + 1, "Down", clueWords[n - 1]);
				//winClue++;
			}
		}
	}
}*/