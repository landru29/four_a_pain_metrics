#ifndef __WARP_TEN__H__
#define __WARP_TEN__H__


class WarpTen {
  public:
    WarpTen();

    unsigned long getTimestamp();

  private:
    static const char* url;
};

#endif