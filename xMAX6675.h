#ifndef XMAX6675_H
#define XMAX6675_H

class xMAX6675
{
 public:
  void attach(int MOSI, int MISO, int SCK, int SSn);
  double readC(void);
 private:
  int pSSn;
};
#endif
