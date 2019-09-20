#ifndef __HTTP__H__
#define __HTTP__H__


class Https {
  public:
    Https();
    void addValue(int val);
    void flushData();

  private:
    int* values;
    int bufferSize;
};

#endif
