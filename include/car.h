#ifndef CAR_H
#define CAR_H

#define CAR_SPEED 1;

typedef struct {
  int side; // which side of the road
  int progress; // how far has car moved in relation to the screen
  int alive; // is car active
} Car;

extern void initialize_car(Car* car, int side);
// v represents "distance" travelled by the player's car
extern void update_car(Car* car, int v);
extern void draw_car(Car* car);

#endif
