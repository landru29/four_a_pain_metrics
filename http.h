#ifndef __HTTP__H__
#define __HTTP__H__

typedef struct Sensor Sensor;
struct Sensor
{
    int oven;
    int ambient;
    long timestamp;
};

class Https {
  public:
    Https();
    void addValue(int oven, int ambient, long timestamp);
    void flushData();
    void addValueAndFlush(int val, int ambient, long timestamp);
    int getBufferSize();

  private:
    Sensor* sensor;
    int bufferSize;

    static const char* url;
};

#endif
