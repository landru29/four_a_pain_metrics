#ifndef __HTTP__H__
#define __HTTP__H__

typedef struct Sensor Sensor;
struct Sensor
{
    int oven;
    int ambient;
};

class Https {
  public:
    Https();
    void addValue(int oven, int ambient);
    void flushData();
    void addValueAndFlush(int val, int ambient);

  private:
    Sensor* sensor;
    int bufferSize;
};

#endif
