#ifndef CAR_H
#define CAR_H

#define CAR_SPEED 100

typedef struct {
  int side; // which side of the road
  float progress; // how far has car moved in relation to the screen
  int alive; // is car active
} Car;

extern void initialize_car(Car* car);
extern void start_car(Car* car, int side);
// v represents "distance" travelled by the player's car
extern void update_car(Car* car, float v);
extern void draw_car(Car* car);

#endif
