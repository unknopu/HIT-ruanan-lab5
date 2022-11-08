#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <queue>
#include <stack>

using namespace std;
struct node
{
    int cur_char;
    int parent_char;
} model_set[256][256];

struct next_node
{
    int state;
    int character;
} next_table[256];

struct output_node
{
    int state;
    char str[256];
    bool available;
} output[256];

struct result_node
{
    int pos;
    int state;
} result[256];
queue<int> q;

int char_appear[256];
int base_table[256];
int checker[256];
int fail_table[256];
int atState = 0;
int parent_state = 0;

int insert_table[256];
int cur_index = 0;
int cur_node_num = 0;
int next_node_num = 0;
int input_num;
char input[256][256];
int pos = 0;
int cur_pos;

void BuildTable();
void BuildFailTable();
void BuildOutputTable();
int goToFunc(int state, int c);

int main()
{
    freopen("input.txt", "r", stdin);
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

    for (int j = 0; j < 256; j++)
    {
        if (char_appear[j])
        {
            q.push(j);
            insert_table[cur_index++] = j;
        }
    }

    cur_node_num = cur_index;
    BuildTable();

    int k = 1;
    while (!q.empty())
    {
        memset(char_appear, 0, sizeof(char_appear));
        int qhead = q.front();
        q.pop();
        cur_node_num--;

        for (int i = 0; i < input_num; i++)
        {
            if (model_set[i][k].cur_char == 0)
            {
                continue;
            }

            if (qhead == model_set[i][k].parent_char)
            {
                char_appear[model_set[i][k].cur_char] = 1;
            }
        }

        for (int j = 0; j < 256; j++)
        {
            if (char_appear[j])
            {
                q.push(j);
                insert_table[cur_index++] = j;
                next_node_num++;
            }
        }

        BuildTable();
        if (!cur_node_num)
        {
            cur_node_num = next_node_num;
            k++;
            next_node_num = 0;
        }
    }

    printf("\n\nnext:\n");
    for (int i = 0; i <= 12; i++)
    {
        printf("%d ", next_table[i].state);
    }

    printf("\n\nbase:\n");
    for (int i = 0; i <= atState; i++)
    {
        printf("%d ", base_table[i]);
    }

    printf("check:\n");
    for (int i = 0; i <= 12; i++)
    {
        printf("%d ", checker[i]);
    }

    BuildFailTable();

    printf("\n\nfail:\n");
    for (int i = 0; i <= atState; i++)
    {
        printf("%d ", fail_table[i]);
    }

    BuildOutputTable();

    char test_char[256];
    scanf("%s", test_char);
    int now_state = goToFunc(0, test_char[0]);
    printf("%d ", now_state);

    result[pos].state = now_state;
    result[pos++].pos = 1;
    for (cur_pos = 1; cur_pos < (int)strlen(test_char); cur_pos++)
    {
        now_state = goToFunc(now_state, test_char[cur_pos]);
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

void BuildTable()
{
    if (!cur_index)
    {
        return;
    }

    int j = 1;
    for (; j < 256; j++)
    {
        if (!next_table[j].state)
        {
            break;
        }
    }

    base_table[parent_state] = j - (insert_table[0]);
    bool flag = 1;
    while (flag)
    {
        int p = 0;
        for (; p < cur_index; p++)
        {
            if ((next_table[base_table[parent_state] + insert_table[p]].state) != 0)
                break;
        }
        if (p == cur_index)
        {
            flag = 0;
        }
        else
        {
            base_table[parent_state]++;
        }
    }

    for (int i = 0; i < cur_index; i++)
    {
        next_table[base_table[parent_state] + insert_table[i]].state = ++atState;
        next_table[base_table[parent_state] + insert_table[i]].character = insert_table[i];
        checker[atState] = parent_state;
    }

    memset(insert_table, 0, sizeof(0));
    cur_index = 0;
    parent_state++;
}

void BuildFailTable()
{
    for (int i = 0; i <= atState; i++)
    {
        if (!checker[i])
        {
            continue;
        }
        else
        {
            for (int j = 0; j < 256; j++)
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

void BuildOutputTable()
{
    char tt[256];
    for (int i = 1; i <= atState; i++)
    {
        memset(tt, 0, sizeof(tt));
        output[i].state = i;
        int temp = i;
        int num = 0;
        while (temp)
        {
            for (int j = 0; j < 256; j++)
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