#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <time.h>
#include <ctime>
#include <Windows.h>
#define DIVIDER 100
int tabs = 0;
int flag = 0;

typedef struct tree
{
	int leaf;
	tree* right_branch;
	tree* left_branch;
} Branches;
typedef struct element 
{
	int value;
	element* step;
} List;
void add_branch(Branches* branch, int element, int flag)
{
	if (flag == 0)
	{
		branch->leaf = element;
		branch->left_branch = 0;
		branch->right_branch = 0;
	}
	else
	{
		if (branch->leaf < element)
		{
			if (branch->right_branch == NULL)
			{
				flag = 0;
				branch->right_branch = (Branches*)malloc(sizeof(Branches));
			}
			add_branch(branch->right_branch, element, flag);
		}
		if (branch->leaf > element)
		{
			if (branch->left_branch == NULL)
			{
				flag = 0;
				branch->left_branch = (Branches*)malloc(sizeof(Branches));
			}
			add_branch(branch->left_branch, element, flag);
		}
	}
}
Branches* search_element(Branches* branch, int element)
{
	Branches* cold = (Branches*)malloc(sizeof(Branches));
	if (branch->leaf == element)
		cold = branch;
	while (branch->leaf != element)
	{
		if (branch->leaf > element)
		{
			cold = branch;
			branch = branch->left_branch;
		}
		if (branch->leaf < element)
		{
			cold = branch;
			branch = branch->right_branch;
		}
	}
	return cold;
}
Branches* delete_branch(Branches* branch, int element)
{
	
	Branches* perfect, * cont, * branch_min, * prom, *begin;
	begin = branch;
	branch = search_element(branch, element);
	if (element > branch->leaf)
		perfect = branch->right_branch;
	else if (element < branch->leaf)
		perfect = branch->left_branch;
	else perfect = branch;
	
	if (!perfect->right_branch)
		branch_min = perfect->left_branch;
	else 
	{
		prom = perfect->right_branch;
		if (prom->left_branch== NULL) {

			prom->left_branch = perfect->left_branch;
			branch_min = prom;
		}
		else 
		{
			branch_min = prom->left_branch;
			while (branch_min->left_branch != NULL) {
				prom = branch_min;
				branch_min = prom->left_branch;
			}
			prom->left_branch = branch_min->right_branch;
			branch_min->left_branch = perfect->left_branch;
			branch_min->right_branch = perfect->right_branch;
		}
	}
	if (element > branch->leaf)
		branch->right_branch = branch_min;
	else if (element < branch->leaf)
		branch->left_branch = branch_min;
	else begin = branch_min;
	delete perfect;
	return begin;
}
void print_branch(Branches* branch)
{
	if (!branch&&tabs!=0) return; 
	tabs += 5; 
	print_branch(branch->left_branch); 
	for (int i = 0; i < tabs; i++) printf(" "); 
	printf("%d\n", branch->leaf);
	print_branch(branch->right_branch);
	tabs -= 5; 
	
}
List* delete_list(List *head, int num_del)
{	
	List* last = head, *begin=head;
	List* perfect_future;
	int flag = 0;
	if (last->value == num_del && flag == 0)
	{
		flag++;
		begin = last->step;
		free(last);
		return begin;
	}
	while (last != NULL&& last->value!=num_del)
	{
		perfect_future = last;
		last = last->step;
	}
	if (last != NULL)
	{
		perfect_future->step = last->step;
		free(last);
	}
	else printf(" Элемент не найден");
	return begin;
}
List *push(List* head, int number)
{
	List* num = (List*)malloc(sizeof(List));
	num->value = number;
	num->step = head;//обратная запись 
	head = num;
	return head;
}
void print(List* head)
{
	int numbik = 0;
	printf("\n");
	while (head != NULL)
	{
		numbik = head->value;
		head = head->step;
		printf("%d\n", numbik);
	}
}
void connect_with_user(List* head, Branches* branch)
{
	LARGE_INTEGER frequency;
	LARGE_INTEGER t1, t2;
	double elapsedTime;
	char move, filter; 
	int number = 0, num_del = 0, flag=0, begin=0;
	QueryPerformanceFrequency(&frequency);
	begin = head->value;

	list:
	printf("\nДерево - 1\nСписок - 2\nВыход - 4\nВвод: ");
	filter = getchar();
	getchar();

continue_1:
	if (filter == '4') return;
	printf("\nУдалить - 1\nДобавить - 2\nВыход - 3\nВвод: ");
	move = getchar();
	getchar();
	switch (move)
	{
	case '1': 
		if (filter == '2') 
		{
			printf(" Какую цифру из списка вы желаете удалить: ");
			scanf("%d", &num_del);
			getchar();
			QueryPerformanceCounter(&t1);
			head = delete_list(head, num_del);
			QueryPerformanceCounter(&t2);
			elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;
			printf("\nУдаление список: %f\n", elapsedTime);
			print(head);
		}
		if (filter == '1') {
			printf(" Какую узел из дерева вы желаете удалить: ");
			scanf("%d", &num_del);
			getchar();
			QueryPerformanceCounter(&t1);
			branch = delete_branch(branch, num_del);
			QueryPerformanceCounter(&t2);
			elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;
			printf("\nУдаление дерево: %f\n", elapsedTime);
			
			print_branch(branch);
		}
		
		goto continue_1;
	case '2':
		printf(" Какое значение хотите добавить: ");
		scanf("%d", &number);
		getchar();
		if (filter == '2') 
		{
			QueryPerformanceCounter(&t1);
			head = push(head, number);
			QueryPerformanceCounter(&t2);
			elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;
			printf("\nВставка список: %f\n", elapsedTime);
			print(head);
		}
		if (filter == '1') 
		{
			QueryPerformanceCounter(&t1);
			add_branch(branch, number, 1);
			QueryPerformanceCounter(&t2);
			elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;
			printf("\nВставка дерево: %f\n", elapsedTime);
			print_branch(branch);
		}
		goto continue_1;
	case '3': 
		goto list;
	default: printf(" Такая команда отсутсвтует, попытайтесь снова\n"); 
		goto list;
	}
}
int main()
{
	srand(113567);
	int number, i=0, count=0, flag=0;
	char move;
	char* locale = setlocale(LC_ALL, "Russian");
	List* head = NULL; 
	Branches* branch = (Branches*)malloc(sizeof(Branches));
	printf("Сколько чисел сгенерировать: ");
	scanf("%d", &count);
	getchar();
	while (i < count)
	{
		number = rand()%1000000;
		i++;
		printf("%d ", number);
		head = push(head, number);
		add_branch(branch, number, flag);
		flag++;
	}
	print_branch(branch);
	connect_with_user(head, branch);
}



