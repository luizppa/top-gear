#include "utils.h"

float max(float a, float b){
  if(a > b) return a;
  return b;
}

float min(float a, float b){
  if(a < b) return a;
  return b;
}

float get_delta(float W, float w, float D, float d){
  float h = (w*D)/(W+w);
  float delta = (D+h-d)/(D+h);
  return delta;
}
