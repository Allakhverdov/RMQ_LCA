// Дан массив из целых чисел a1, a2, ..., an (индексация с 1!). Для каждого запроса [left, right] найдите такой подотрезок al, al+1, ..., ar этого массива (1 <= left <= l <= r <= right <= n), что сумма чисел al + al+1 + ... + ar является максимально возможной. Требуемое время ответа на запрос - O(log n).
// Формат ввода

// Входные данные содержат один или несколько тестовых примеров. Описание каждого из них начинается с двух чисел n и m - длины массива и числа интересующих подотрезков. В следующей строке содержится n чисел — элементы массива. Каждое из этих чисел по абсолютной величине не превосходит 10000. Далее следуют описания подотрезков, каждое описание состоит из двух чисел left и right , обозначающих левый и правый конец подотрезка (1 <= left <= right <= n). Суммарная длина всех массивов, а также суммарное число подотрезков не превосходит 100000.
// Формат вывода

// Для каждого из тестовых примеров выведите m чисел: искомую максимальную сумму для каждого из подотрезков.

#include<iostream>
#include<vector>
#include<cmath>
class data                      //тут храним единицу данные
{
public:
    int sum = 0;                //тут будем хранить сумму на подотрезке
    int pref = 0;               //тут максимальная сумма на префиксе
    int suff = 0;               //максимальная мумма на суффиксе
    int ans = 0;                //максимальная сумма на подотрезке
    void create_data(int sum, int pref, int suff, int ans);         //заполнение соответствующего отрезка данными
    data make_parent(data left, data right);                        //создание родителя по двум детям
    void create_one_value(int value);                               //создание в вырожденном случае когда отрезок из одной ячейки
};

void data::create_data(int sum, int pref, int suff, int ans)        //заполнение соответствующего отрезка данными
{
    this->sum = sum;
    this->pref = pref;
    this->suff = suff;
    this->ans = ans;
}

void data::create_one_value(int value)                              //создание в вырожденном случае когда отрезок из одной ячейки
{
    this->sum = value;
    this->pref = this->suff = this->ans = value;//std::max(0, value);
}

data make_parent(data left, data right)                             //создание родителя по двум детям
{
    data parent;
    parent.sum = left.sum + right.sum;
    parent.pref = std::max(left.pref, left.sum + right.pref);
    parent.suff = std::max(right.suff, right.sum + left.suff);
    parent.ans = std::max(std::max(left.ans, right.ans), left.suff + right.pref);
    return parent;
}

void build_tree(std::vector<data>&tree, std::vector<int>&data_entered, int v,               //строим дерево отрезков
                int left_border, int right_border)
{
    if(left_border == right_border)                                     //если дошли до единичных отрезков то заполняем их
    {
        tree[v].create_one_value(data_entered[left_border]);
    }
    else
    {
        int mid = (left_border + right_border)/2;                       //разбиваем отрезок пополам и работаем с каждой частью
        build_tree(tree, data_entered,(v+1)*2-1 , left_border, mid);
        build_tree(tree, data_entered,(v+1)*2, mid+1, right_border);
        tree[v] = make_parent(tree[(v+1)*2-1], tree[(v+1)*2]);          //соединяем их
    }
    int a = 0;
}

data response_to_a_request(int v, int left, int right,
                           int segment_left, int segment_right, std::vector<data>&tree)
{
    if( left == segment_left && right == segment_right)
    {
        return tree[v];
    }
    int mid = (left + right)/2;
    if(segment_right <= mid)                        //бъём массив пополам если наш подотрезок левее то отсекаем правую часть
    {
        return response_to_a_request((v+1)*2-1, left, mid, segment_left, segment_right, tree);
    }
    if(segment_left > mid)                          //бъём массив пополам если наш подотрезок правее то отсекаем левую часть
    {
        return response_to_a_request((v+1)*2, mid + 1, right, segment_left, segment_right, tree);
    }
    else            //если подотрезок разбился пополам то обработаем их поотднльности и склеим
    {
        return make_parent(response_to_a_request((v+1)*2-1, left, mid, segment_left, mid, tree),
                           response_to_a_request((v+1)*2, mid + 1, right, mid + 1, segment_right, tree));
    }
}

int main()
{
    while (true)
    {
        if(std::cin.fail()){break;}     //пока возможен ввод, читаем данные
        int N = 0;
        int M = 0;
        std::cin >> N >> M;
        int new_N = N;
        int power = 1;
        while (true)                    //считаем первую степень двойки большую N
        {
            if(power >= N)
            {
                new_N = power;
                break;
            }
            power *= 2;
        }
        std::vector<int>data_entered(new_N);        //входные данные
        std::vector<data>tree(2 * new_N - 1);       //тут будет храниться дерево
        for(int i = 0; i < N; ++i)
        {
            std::cin >> data_entered[i];            //заполняем данные
        }
        for(int i = N + 1; i < new_N; ++i)
        {
            data_entered[i] = 0;                    //остаток добиваем нулями
        }
        build_tree(tree, data_entered, 0, 0, new_N - 1);     //строим дерево отрезков
        for(int i = 0; i < M; ++i)
        {
            int left = 0;
            int right = 0;
            std::cin >> left >> right;
            if(left == right)
            {
                std::cout << std::endl << tree[new_N + left - 2].ans ;
                continue;
            }
            std::cout << std::endl << response_to_a_request(0, 0, new_N - 1,                    //выводим ответ
                                                            left - 1, right - 1, tree).ans;
            int a  = 0;
        }
    }
    return 0;
}
