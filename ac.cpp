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
} next_table[MAX];

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
int base_table[MAX];
int checker[MAX];
int fail_table[MAX];
int atState = 0;
int parent_state = 0;

int insert_table[MAX];
int atIndex = 0;
int atNodeNo = 0;
int next_node_num = 0;
int input_num;
char input[MAX][MAX];
int pos = 0;
int cur_pos;

void buildTable();
void buildFailor();
void buildOutput();
int goToFunc(int state, int c);

int main()
{
    freopen("test.txt", "r", stdin);
    memset(input, 0, sizeof(input));
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

    memset(char_appear, 0, sizeof(char_appear));

    for (int i = 0; i < input_num; i++)
    {
        char_appear[model_set[i][0].cur_char] = 1;
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
        memset(char_appear, 0, sizeof(char_appear));
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
            next_node_num = 0;
            k++;
        }
    }

    cout << "next:\t";
	for (int i = 0; i <= 12; i++)
	{
		cout << next_table[i].state << " ";
	}
	cout << endl;
	cout << "base:\t";
	for (int i = 0; i <= atState; i++)
	{
		cout << base_table[i] << " ";
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
        printf("%d ", fail_table[i]);
    }

    buildOutput();
    char tester[MAX];
    scanf("%s", tester);
    int now_state = goToFunc(0, tester[0]);
    printf("%d ", now_state);
    result[pos].state = now_state;
    result[pos++].pos = 1;
    for (cur_pos = 1; cur_pos < (int)strlen(tester); cur_pos++)
    {
        now_state = goToFunc(now_state, tester[cur_pos]);
        result[pos].state = now_state;
        result[pos++].pos = cur_pos + 1;
    }

    printf("\n\nmatch:\n");
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
        if (!next_table[j].state)
        {
            break;
        }
        j++;
    }

    base_table[parent_state] = j - (insert_table[0]);
    while (true)
    {
        int p = 0;
        for (; p < atIndex; p++)
        {
            if ((next_table[base_table[parent_state] + insert_table[p]].state) != 0)
                break;
        }
        if (p == atIndex)
        {
            break;
        }
        else
        {
            base_table[parent_state]++;
        }
    }

    for (int i = 0; i < atIndex; i++)
    {
        next_table[base_table[parent_state] + insert_table[i]].state = ++atState;
        next_table[base_table[parent_state] + insert_table[i]].character = insert_table[i];
        checker[atState] = parent_state;
    }

    memset(insert_table, 0, sizeof(0));
    atIndex = 0;
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
                if (next_table[j].state == i)
                {
                    fail_table[i] = goToFunc(fail_table[checker[i]], next_table[j].character);
                    break;
                }
            }
        }
    }
}

int goToFunc(int state, int c)
{
    int t = next_table[base_table[state] + c].state;
    if (checker[t] == state)
    {
        return t;
    }
    else if (state == 0)
    {
        return 0;
    }
    else
    {
        printf("%d ", fail_table[state]);
        result[pos].state = fail_table[state];
        result[pos++].pos = cur_pos;
        return goToFunc(fail_table[state], c);
    }
}

void buildOutput()
{
    char tt[MAX];
    for (int i = 1; i <= atState; i++)
    {
        memset(tt, 0, sizeof(tt));
        output[i].state = i;
        int temp = i;
        int num = 0;
        while (temp)
        {
            for (int j = 0; j < MAX; j++)
            {
                if (temp == next_table[j].state)
                {
                    tt[num++] = next_table[j].character;
                    break;
                }
            }
            temp = checker[temp];
        }

        int k = 0;
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