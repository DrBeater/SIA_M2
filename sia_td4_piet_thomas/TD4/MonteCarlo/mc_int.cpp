#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <random>

#include "gnuplot_i.hpp"

using namespace std;

double int_mc(double(*)(double), double&, double, double, int);
double f(double);
void wait_for_key ();

int main()
{
  srand(time(NULL));

  Gnuplot g("lines");

  double a = 0.0;              // left endpoint
  double b = 1.0;               // right endpoint
  int N = 500;

  double varest, mc;
  std::vector<double> x, err, var_e, var_t;

  for (int n=5; n <= N; n++)
  {
    mc = int_mc(f, varest, a, b, n);

    /* variance analytique */
    double var_a = (16./9.) / n;

    x.push_back(n);
    err.push_back(fabs(mc-1.0));
    var_e.push_back(var_a);           // variance analyptique
    var_t.push_back(varest);                // variance numérique
  }

  g.reset_plot();
  g.set_grid();
  g.set_style("lines").plot_xy(x,err,"erreur d'estimation");
  g.set_style("lines").plot_xy(x,var_e,"variance analyptique");
  g.set_style("lines").plot_xy(x,var_t,"variance numerique");

  wait_for_key();

  return 0;
}

/*
   Fonction à intégrer
*/
double f(double x) 
{
  double y = 5 * x * x * x * x;
  return y;
}

/* Integration de Monte-Carlo */
double int_mc(double(*f)(double), double& varest, double a, double b, int n)
{
  uniform_real_distribution<float> dist(a,b);
  random_device rd;
  mt19937 mt(rd());

  double res = 0;
  double res2 = 0;

  for (int i = 0; i < n; i++) {

    double rand = dist(mt);
    res += f(rand);
    res2 += pow(f(rand),2);

  }
  
  res /= n;
  res2 /= n;

  varest = (res2 - pow(res,2)) / (n-1);

  return res;
}


void wait_for_key ()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)  // every keypress registered, also arrow keys
  cout << endl << "Press any key to continue..." << endl;

  FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
  _getch();
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
  cout << endl << "Press ENTER to continue..." << endl;

  std::cin.clear();
  std::cin.ignore(std::cin.rdbuf()->in_avail());
  std::cin.get();
#endif
  return;
}