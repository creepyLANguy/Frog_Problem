#include <random>
#include "windows.h"
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

//int runs = 100000000;
int runs = 1000000;

const int lillypads_lower = 2;
const int lillypads_upper = 35;

const bool logEachSimSet = false;

////////////////////////////////////////////////////////////////////////////////

string GetFormattedRuns()
{
  string input = to_string(runs);
  string buff = "";
  int counter = 0;
  for (int i = input.length() - 1; i >= 0; --i)
  {
    buff = input[i] + buff;
    ++counter;
    if (counter == 3)
    {
      buff = "," + buff;
      counter = 0;
    }
  }
  return buff;
}

////////////////////////////////////////////////////////////////////////////////

string formattedRuns = GetFormattedRuns();
double* averages = new double[(lillypads_upper - lillypads_lower) + 1]{ 0 };

typedef mt19937 rng_type;
const uniform_real_distribution<> udist(0, 1);

////////////////////////////////////////////////////////////////////////////////

double CalcAverage(const int lillypads, int* bucketsArray)
{
  double sum = 0;
  for (int i = 0; i < lillypads; ++i)
  {
    sum += (i + 1) * bucketsArray[i];
  }

  return sum / runs;
}

////////////////////////////////////////////////////////////////////////////////

void SaveLog(const string filename, const int lillypads, int* bucketsArray, const double avg)
{
  ofstream txt(filename);

  txt << "mean" << " : " << avg << "\n";
  txt << "pads" << " : " << lillypads << "\n";
  txt << "runs" << " : " << runs << " (" << formattedRuns << ")" << "\n";
  txt << "\n";

  for (int i = 0; i < lillypads; ++i)
  {
    txt << i + 1 << "\t:\t" << bucketsArray[i] << "\n";
  }
}

////////////////////////////////////////////////////////////////////////////////

double RunSim(rng_type& rng, const int lillypads)
{
  int* bucketsArray = new int[lillypads] {0};

  for (int i = 0; i < runs; ++i)
  {
    int leaps = 0;
    int position = 0;

    while (position != lillypads)
    {
      position += static_cast<int>(udist(rng) * (lillypads - position)) + 1;
      ++leaps;
    }

    ++bucketsArray[leaps - 1];
  }

  const string filename = to_string(lillypads) + "_" + to_string(abs(static_cast<int>(GetTickCount()))) + ".txt";
  const double avg = CalcAverage(lillypads, bucketsArray);

  if (logEachSimSet)
  {
    SaveLog(filename, lillypads, bucketsArray, avg);
  }
  
  return avg;
}

////////////////////////////////////////////////////////////////////////////////

void main()
{
  rng_type rng;
  rng.seed(abs(static_cast<int>(GetTickCount())));

  GetFormattedRuns();

  cout << "[" << lillypads_lower << "," << lillypads_upper << "]" << "=>" << formattedRuns << endl;

  for (int i = lillypads_lower; i <= lillypads_upper; ++i)
  {
    cout << "Runnning lillypad count of : " << i << endl;
    averages[i - lillypads_lower] = RunSim(rng, i);
  }

  const string filename = "AllAverages_" + to_string(abs(static_cast<int>(GetTickCount()))) + ".csv";
  ofstream txt(filename);
  for (int i = 0; i <= lillypads_upper - lillypads_lower; ++i)
  {
    txt << to_string(i + lillypads_lower) + "," + to_string(averages[i]) + "\n";
  }
}

////////////////////////////////////////////////////////////////////////////////