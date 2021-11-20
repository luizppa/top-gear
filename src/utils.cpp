#include "../include/utils.hpp"
#include "../include/display.hpp"

namespace top_gear {

  namespace utils {

    char* names[] = {
      "Jack",
      "Bruce",
      "Hammond",
      "Alice",
      "Ana",
      "Vincent",
      "Robert",
      "Patrick",
      "Beatrice",
      "Viktor",
      "Gregory",
      "Mordecai",
      "Marjore",
      "Otis",
      "Damian"
    };

    // Returns the biggest number in [a, b]
    float max(float a, float b){
      if(a > b) return a;
      return b;
    }

    // Returns the smallest number in [a, b]
    float min(float a, float b){
      if(a < b) return a;
      return b;
    }

    /*
      Returns the perspective variation of objects in a distance
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

    /**
     * Returns (SCREEN_WIDTH / 2) + position
    */
    float normalize(float position){
      return (SCREEN_WIDTH / 2) + position;
    }

  }

}
