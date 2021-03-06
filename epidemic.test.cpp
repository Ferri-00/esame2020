#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "epidemic.hpp"

TEST_CASE("testing contact(), get() and set()") {
  // creating a board with infected people in the 4 corners
  int a = 5;
  int b = 1;
  int c = 3;
  int d = 4;
  Board population(5, 1);
  population.set(a, a, I);
  population.set(b, b, I);
  population.set(b, a, I);
  population.set(a, b, I);
  population.set(c, d, I);
  population.set(d, c, I);

  CHECK(population.get(a, a) == I);
  CHECK(population.get(c, c) == S);

  CHECK(population.contact(3, 3, 1) == 2);
  CHECK(population.contact(3, 3, 2) == 6);
}

// every check is done on the previous situation
  TEST_CASE("testing epidemic, infection(), avg_time()") {
  int dim = 120;

  double density;
  double ratInf;
  double pInf;
  int avgTime;
  int range;

  std::random_device rd;
  std::mt19937 gen(rd());

  std::uniform_real_distribution<> p(0., 1.);
  std::uniform_int_distribution<> t(1, 40);
  std::uniform_int_distribution<> r(1, 5);

  density = p(gen);
  ratInf = p(gen);
  pInf = p(gen);
  avgTime = t(gen);
  range = r(gen);

  int S = static_cast<int>(dim * dim * density);
  int I = static_cast<int>(dim * dim * density * ratInf);
  Situation So = {0, S, 0, 0};

  Board population(dim, density);
  //Board::push_back(So);
  //CHECK(population.situation() == So);

  population.infection(ratInf);
  auto [pop1,sit1] =  population.epidemic(pInf, avgTime, range, false);
  population = pop1;
  Board::push_back(sit1);
    
  Situation Suno = {1, S - I, I, 0};
  CHECK(sit1 == Suno);


  while (population.situation().i != 0) {
    auto [pop,sit] =  population.epidemic(pInf, avgTime, range, false);
    population = pop;
    Board::push_back(sit);
      
    CHECK((population.situation().i + population.situation().s +
               population.situation().r ==
           S));
  }

  CHECK(population.avg_time() == doctest::Approx(avgTime).epsilon(0.5));
}

