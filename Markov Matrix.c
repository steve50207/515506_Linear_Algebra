#include <stdio.h>
#include <string.h>
#define N 2                                            // set markov matrix size is N*N
#define MAX_YEAR_INTERVAL 100    // set maximum calculation year interval(5 years per interval)
#define MAX_DATA (int) (1e6)              // set maximum output result string size

typedef struct
{
    double m[N][N];

} matrix;

matrix Markov;                                                                // declare Markov Matirx
double coefficient_a, coefficient_b;                                // least square equation coefficient(linear)
double markov_data[MAX_YEAR_INTERVAL][N][N];   // markov matrix of the corresponding year
char area_name[N][MAX_DATA];     // area name
double people[N];                             // area initial population
double new_people[N];                    // population after once matrix multiplication
int data_start_year;                          // least square equation coefficient data start year
int data_end_year;                           // least square equation coefficient data end year
int data_year_interval;                     // least square equation coefficient data year interval(5 years/interval)
int cal_year;                                     // target year we want to calculate
int cal_year_interval;                       // year interval between data end year and target year(5 years/interval)
double result[MAX_YEAR_INTERVAL][N];    
// calculation result (year vs. area population)
char year_data[MAX_YEAR_INTERVAL][MAX_DATA]; 
// output year string (from data end year to target year)
char population_data[MAX_YEAR_INTERVAL][N][MAX_DATA];
// output population result string (year vs. area population)
void input_data()
{
    //scanf input from input_data.txt
    freopen("input_data.txt", "r", stdin);

    //input data_start_year & data_end_year & cal_year and calculate data_year_interval & cal_year_interval
    scanf("%d%d%d", &data_start_year, &data_end_year, &cal_year);
    data_year_interval = (data_end_year-data_start_year)/5;
    cal_year_interval = (cal_year-data_end_year)/5;

    for(int row = 0; row < N; row++)
    {
        for(int col = 0; col < N; col++)
        {
            //input least square equation coefficients
            scanf("%lf", &coefficient_a);
            scanf("%lf", &coefficient_b);
            for(int t=1; t<=cal_year_interval; t++)
            {
                //use linear equation coefficient to calculate the matrix
                markov_data[t][row][col] = coefficient_a*(t + data_year_interval) + coefficient_b;
            }
        }
    }
    //input area name string & area initial population
    for(int i = 0; i < N; i++)
    {
        scanf("%s", area_name[i]);
        scanf("%lf", &people[i]);
    }
}
void Calculate_People(int cal_year_interval)
{
    for(int t = 0 ; t <= cal_year_interval ; t++)
    {
        //t=0 means data end year
        if(t == 0)
        {
            for(int i = 0 ; i < N; i++)
            {
                //area initial population
                result[0][i] = people[i];
            }
        }
        else
        {
            //load markov matrix of the corresponding year from markov_data
            for(int row = 0; row < N; row++)
            {
                for(int col = 0; col < N; col++)
                {
                    Markov.m[row][col] = markov_data[t][row][col];
                }
            }

            //implement markov matrix multiplication
            for(int row = 0; row < N; row++)
            {
                double temp = 0;
                for(int col = 0; col < N; col++)
                {
                    temp += Markov.m[row][col]*people[col];
                }
                new_people[row] = temp;
            }

            //store calculation result of area population
            //and update the area population for next round calculation
            for(int i = 0 ; i < N; i++)
            {
                result[t][i] = new_people[i];
                people[i] = new_people[i];
            }
        }
    }
}
void output_data()
{
    //write the calculation results to output_data.txt
    FILE *fp = NULL;
    fp = fopen("output_data.txt", "w+");

    //output the years from data end year to cal year
    fputs("YEAR : ", fp);
    for(int t = 1; t <=  cal_year_interval; t++)
    {
        sprintf(year_data[t],"%d ", data_end_year + t*5);
        fputs(year_data[t], fp);
    }
    fputs("\n", fp);

    //output corresponding area population calculation result from data end year to cal year
    for(int i = 0; i < N; i++)
    {

        fputs(area_name[i], fp);
        fputs(" : ", fp);
        for(int t = 1; t<= cal_year_interval; t++)
        {
            sprintf(population_data[t][i],"%I64u ", (unsigned long long int)result[t][i]);
            fputs(population_data[t][i], fp);
        }
        fputs("\n", fp);
    }
    fclose(fp);
}
int main(void)
{
    //get input data from txt file
    //and use least square equation coefficient to calculate markov matrix of the corresponding year
    input_data();
    //use markov matrix and initial
    Calculate_People(cal_year_interval);
    //out the txt file including the calculation result of area population of the corresponding year
    output_data();

    return 0;
}
