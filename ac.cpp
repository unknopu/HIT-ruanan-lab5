#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <queue>
#include <stack>

#define MAX 256
#define DEFAULT 0

using namespace std;
struct node
{
    int cur_char;
    int parent_char;
} model_set[MAX][MAX];

struct next_node
{
    int state;
    int character;
} nextor[MAX];

struct output_node
{
    int state;
    char str[MAX];
    bool available;
} output[MAX];

struct result_node
{
    int pos;
    int state;
} result[MAX];

queue<int> q;
int char_appear[MAX];
int basor[MAX];
int checker[MAX];
int failor[MAX];
int atState = DEFAULT;
int parent_state = DEFAULT;
int insert_table[MAX];
int atIndex = DEFAULT;
int atNodeNo = DEFAULT;
int next_node_num = DEFAULT;
int input_num;
char input[MAX][MAX];
int pos = DEFAULT;
int cur_pos;

void buildTable();
void buildFailor();
void buildOutput();
int goToFunc(int state, int c);

int main()
{
    // freopen("test.txt", "r", stdin);
	FILE* file = freopen("test.txt", "r", stdin);

    memset(input, DEFAULT, sizeof(input));
    scanf("%d", &input_num);

    int row_len;
    int max_row_len = -1;
    
    for (int i = 0; i < input_num; i++)
    {
        scanf(" %s", input[i]);
        row_len = strlen(input[i]);
        if (row_len > max_row_len) 
        {
            max_row_len = row_len;
        }

        for (int j = 0; j < row_len; j++)
        {
            model_set[i][j].cur_char = input[i][j];

            if (!j) 
            { 
                model_set[i][j].parent_char = 0;
            }
            else
            {
                model_set[i][j].parent_char = model_set[i][j - 1].cur_char;
            }   
        }
    }

    memset(char_appear, DEFAULT, sizeof(char_appear));
    for (int i = 0; i < input_num; i++)
    {
        char_appear[model_set[i][DEFAULT].cur_char] = 1;
    }

    for (int j = 0; j < MAX; j++)
    {
        if (char_appear[j])
        {
            q.push(j);
            insert_table[atIndex++] = j;
        }
    }

    atNodeNo = atIndex;
    buildTable();

    int k = 1;
    while (!q.empty())
    {
        memset(char_appear, DEFAULT, sizeof(char_appear));
        int qhead = q.front();
        q.pop();
        atNodeNo--;
        for (int i = 0; i < input_num; i++)
        {
            if (!model_set[i][k].cur_char)
            {
                continue;
            }

            if (qhead == model_set[i][k].parent_char)
            {
                char_appear[model_set[i][k].cur_char] = 1;
            }
        }

        for (int j = 0; j < MAX; j++)
        {
            if (char_appear[j])
            {
                q.push(j);
                insert_table[atIndex++] = j;
                next_node_num++;
            }
        }

        buildTable();
        if (!atNodeNo)
        {
            atNodeNo = next_node_num;
            next_node_num = DEFAULT;
            k++;
        }
    }

    cout << "next:\t";
	for (int i = 0; i <= 12; i++)
	{
		cout << nextor[i].state << " ";
	}
	cout << endl;
	cout << "base:\t";
	for (int i = 0; i <= atState; i++)
	{
		cout << basor[i] << " ";
	}
	cout << endl;
	cout << "check:\t";
	for (int i = 0; i <= 12; i++)
	{
		cout << checker[i] << " ";
	}

    buildFailor();
    printf("\nfail:\t");
    for (int i = 0; i <= atState; i++)
    {
		cout << failor[i] << " ";
    }

    buildOutput();
    char tester[MAX];
    scanf("%s", tester);
    int now_state = goToFunc(DEFAULT, tester[DEFAULT]);
    result[pos].state = now_state;
    result[pos++].pos = 1;
    for (cur_pos = 1; cur_pos < (int)strlen(tester); cur_pos++)
    {
        now_state = goToFunc(now_state, tester[cur_pos]);
        result[pos].state = now_state;
        result[pos++].pos = cur_pos + 1;
    }

    cout << "\n\nmatched patterns are:" << endl;
    for (int j = 0; j < pos; j++)
    {
        for (int i = 0; i <= atState; i++)
        {
            if (output[i].available == 1 && output[i].state == result[j].state)
            {
                printf("%ld   %s\n", result[j].pos - strlen(output[i].str), output[i].str);
            }
        }
    }
	getchar();
    fclose(stdin);
    return 0;
}

void buildTable()
{
    if (!atIndex)
    {
        return;
    }

    int j = 1;
    while (true)
    {
        if (!nextor[j].state)
        {
            break;
        }
        j++;
    }

    basor[parent_state] = j - (insert_table[DEFAULT]);
    while (true)
    {
        int p = 0;
        for (; p < atIndex; p++)
        {
            if ((nextor[basor[parent_state] + insert_table[p]].state) != 0)
                break;
        }
        if (p == atIndex)
        {
            break;
        }
        else
        {
            basor[parent_state]++;
        }
    }

    for (int i = 0; i < atIndex; i++)
    {
        nextor[basor[parent_state] + insert_table[i]].state = ++atState;
        nextor[basor[parent_state] + insert_table[i]].character = insert_table[i];
        checker[atState] = parent_state;
    }

    memset(insert_table, DEFAULT, sizeof(DEFAULT));
    atIndex = DEFAULT;
    parent_state++;
}

void buildFailor()
{
    for (int i = 0; i <= atState; i++)
    {
        if (checker[i])
        {
            for (int j = 0; j < MAX; j++)
            {
                if (nextor[j].state == i)
                {
                    failor[i] = goToFunc(failor[checker[i]], nextor[j].character);
                    break;
                }
            }
        }
    }
}

int goToFunc(int state, int c)
{
    int t = nextor[basor[state] + c].state;
    if (checker[t] == state)
    {
        return t;
    }
    else if (state == DEFAULT)
    {
        return 0;
    }
    else
    {
        printf("%d ", failor[state]);
        result[pos].state = failor[state];
        result[pos++].pos = cur_pos;
        return goToFunc(failor[state], c);
    }
}

void buildOutput()
{
    char tt[MAX];
    for (int i = 1; i <= atState; i++)
    {
        memset(tt, DEFAULT, sizeof(tt));
        output[i].state = i;
        int temp = i;
        int num = DEFAULT;
        while (temp)
        {
            for (int j = 0; j < MAX; j++)
            {
                if (temp == nextor[j].state)
                {
                    tt[num++] = nextor[j].character;
                    break;
                }
            }
            temp = checker[temp];
        }

        int k = DEFAULT;
        for (int p = num - 1; p >= 0; p--)
        {
            output[i].str[k++] = tt[p];
        }

        for (int q = 0; q < input_num; q++)
        {
            if (!strcmp(input[q], output[i].str))
            {
                output[i].available = 1;
                break;
            }
        }
    }
}