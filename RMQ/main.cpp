#include<cmath>
#include<vector>
#include<utility>
#include<iostream>
#include<algorithm>
class Sparse_Table
{
public:
    Sparse_Table(std::vector<int>& values_);
    int get_answer(int start, int end);         //функция получения ответа
private:
    std::vector<int>log;                        //предподсчёт логарифмов
    std::vector<int>power;                      //предподсчёт степеней
    std::vector<int>values;                     //вектор начальных значений
    std::vector<std::vector<std::pair<int,int>>>s_table;        //пара минимумов на соответствующих подотрезках
};

int Sparse_Table::get_answer(int start, int end)
{
    int layer = 0;
    int delt = end - start;
    if(delt == 1)                                           //если элементы границы отрезка вплотну друг к другу
    {
        return values[s_table[0][start].second];            // просто выводим саму эту пару
    }
    int log_delt = log[delt] - 1;                                               //иначе считаем на каком уровне спартстейбла искать ответ
    std::pair<int, int>pair_1 = s_table[log_delt][start];                       //находим первый интервал
    std::pair<int, int>pair_2 = s_table[log_delt][end - power[log[delt]] + 1];  //находим второй интервал
    std::vector<std::pair<int, int>>find_min
    {
        {values[pair_1.first], pair_1.first},
        {values[pair_1.second], pair_1.second},
        {values[pair_2.first], pair_2.first},
        {values[pair_2.second], pair_2.second}
    };
    std::sort(find_min.begin(), find_min.end());                //находим первый и воторой минимумы
    if(find_min[0].second != find_min[1].second)
    {
        return find_min[1].first;;                              //учитываем что один и тот же минимум может находиться в пересечении
    }
    else
    {
        return find_min[2].first;
    }
}

Sparse_Table::Sparse_Table(std::vector<int>& values_)           //строим спартстейбл
{
    values = values_;
    std::vector<std::pair<int, int>>first_layer;                //строим первый слой
    int pow = 1;
    for(int i = 0; i < std::log2(values.size()); ++i)           //предподсчёт степеней двойки
    {
        power.push_back(pow);
        pow *= 2;
    }
    for(int i = 0; i < values.size(); ++i)
    {
        log.push_back(std::log2(i));                            //предподсчёт логарифмов чисел
    }
    for(int i = 0; i < values.size() - 1; ++i)
    {
        std::pair<int, int>next_pair;                           //создаём новую пару, храним не значения а номер в начальном массиве
        if (std::min(values[i], values[i+1]) == values[i])
        {
            next_pair = {i, i+1};
        }
        else
        {
            next_pair = {i+1, i};
        }
        first_layer.push_back(next_pair);                       //заполняем слой где степень двойки 1;
    }
    s_table.push_back(first_layer);
    int i = 2;
    int power_of_two = 4;
    while (values.size() >= power_of_two)                       //создаём все остальные слои
    {
        std::vector<std::pair<int, int>>next_layer;
        for(int j = 0; j < values.size() - power_of_two + 1; ++j)
        {
            std::pair<int, int>pair_1 = s_table[i - 2][j];                      //берём соответствующие пару предыдущегоуровня
            std::pair<int, int>pair_2 = s_table[i - 2][j + power_of_two/2];
            std::vector<std::pair<int, int>>find_min
            {
                {values[pair_1.first], pair_1.first},
                {values[pair_1.second], pair_1.second},
                {values[pair_2.first], pair_2.first},
                {values[pair_2.second], pair_2.second}
            };
            std::sort(find_min.begin(), find_min.end());            //сортируем (элементов всего четыре так что это константа)
            std::pair<int, int>next_pair;
            if(find_min[0].second != find_min[1].second)            //находим минимум
            {
                next_pair = {find_min[0].second, find_min[1].second};       //учитываем что один и тот же минимум может находиться в пересечении
            }
            else
            {
                next_pair = {find_min[0].second, find_min[2].second};
            }
            next_layer.push_back(next_pair);
        }
        s_table.push_back(next_layer);  //добавляем построенный слой
        ++i;
        power_of_two *= 2;          //переходим к следующему уровню
    }
}




int main()
{
    int N = 0;
    int M = 0;
    std::cin >> N >> M;
    std::vector<int>values;
    for(int i = 0; i < N; ++i)
    {
        int val = 0;
        std::cin >> val;
        values.push_back(val);              //запоминаем данную нам последовательность
    }
    Sparse_Table st(values);                 //строим спарстейбл
    for(int i = 0; i < M; ++i)
    {
        int start = 0;
        int end = 0;
        std::cin >> start >> end;
        std::cout << st.get_answer(start - 1, end - 1) << std::endl;        //выдаём ответы
    }
}
