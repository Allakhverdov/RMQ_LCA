#include<iostream>
#include<vector>
#include<cmath>
#include<utility>

void preprocess(std::vector<std::pair<int, int>>&parent, std::vector<std::vector<int>>&tree, std::vector<std::vector<int>>&ancestors)
{
    for(int i = 0; i < parent.size(); ++i)
    {
        ancestors[0].push_back(parent[i].first);  // я заполнил нулевой уровень предподсчёта
    }
    for(int j = 1; j < ancestors.size(); ++j)
    {
        for(int i = 0; i < parent.size(); ++i)          //заполняю последующие уровни, пользуясь уже заполнеными предыдущими
        {
            ancestors[j].push_back(ancestors[j - 1][ancestors[j - 1][i]]); //беру i-й уровень и добавляю j - вершину
        }
    }
}

int LCA(int v, int u, std::vector<std::pair<int, int>>&parent, std::vector<std::vector<int>>&ancestors)
{
    if(parent[v].second > parent[u].second)
    {
        std::swap(u, v);                    //удостовериваюсь в том, кто находится ниже
    }
    for(int i = ancestors.size() - 1; i >= 0; --i)
    {
        if(parent[ancestors[i][u]].second >= parent[v].second)
        {
            u = ancestors[i][u];                                //выравниваю уровни
        }
    }
    if(u == v)
    {
        return v;                                               //проверка на случай если при вводе u = v
    }
    for(long long i = ancestors.size() - 1; i >= 0; --i)
    {
        if(ancestors[i][u] != ancestors[i][v])                  //поднимаюсь вверх пока не дойду до нужного уровня
        {
            u = ancestors[i][u];
            v = ancestors[i][v];
        }
    }
    return parent[v].first;
}

void calculate_depth(int v, std::vector<std::vector<int>>&tree, std::vector<std::pair<int, int>>&parent, int& depth)
{
    for(int i = 0; i < tree[v].size(); ++i)
    {
        parent[tree[v][i]].second = parent[v].second + 1;                   //предподсчитываю глубины каждого и максимальную глубину
        if(depth < parent[tree[v][i]].second)
        {
            depth = parent[tree[v][i]].second;
        }
        calculate_depth(tree[v][i], tree, parent, depth);
    }
}

int main()
{
    int N = 0;
    int M = 0;
    std::cin >> N >> M;
    std::vector<std::vector<int>>tree(N);                       //само дерево
    std::vector<std::pair<int, int>>parent(N, {0, 0});          //в i - й ячейке хранится отец i - вершины ребенка и глубина i - й вершины
    int depth = 0;
    for(int i = 1; i < N; ++i)
    {
        int v = 0;
        std::cin >> v;
        parent[i].first = v;                                    //заполняем кто чей отец
        tree[v].push_back(i);                                   //сразу строим дерево
    }
    long long a1, a2, x, y, z;
    std::cin >> a1 >> a2 >> x >> y >> z;
    calculate_depth(0, tree, parent, depth);                    //подсчитываем глубины каждой вершины и максимальную глубину
    std::vector<std::vector<int>>ancestors(log2(depth + 1) + 1);
    preprocess(parent, tree, ancestors);                        //предподсчёт
    int prev = 0;
    long long ans = 0;
    for(int i = 0; i < M; ++i)                                  //генерация и получение ответов
    {
        prev = LCA((a1 + prev) % N, a2, parent, ancestors);
        ans += prev;
        a1 = (a1 * x + a2 * y + z) % N;
        a2 = (a2 * x + a1 * y  + z) % N;
    }
    std::cout << ans;
    return 0;
}













