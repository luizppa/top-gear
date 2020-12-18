#include "../include/utils.h"

char* names[50] = {"Jack", "Bruce", "Hammond", "Alice", "Ana", "Vincent", "Robert", "Patrick", "Beatrice", "Viktor", "Gregory", "Mordecai", "Marjore", "Otis", "Damian"};

// Returns the biggest number in [a, b]
float max(float a, float b){
  if(a > b) return a;
  return b;
}

// Returns the smaller number in [a, b]
float min(float a, float b){
  if(a < b) return a;
  return b;
}

// Returns the perspective variation of objects in a distance
/*
  W = Track width at origin
  w = Track aparent width at the end of the visible spam
  D = Max visible distance
  d = Object distance
*/
float get_delta(float W, float w, float D, float d){
  float h = (w*D)/(W+w);
  float delta = (D+h-d)/(D+h);
  return delta;
  // return 1;
}
