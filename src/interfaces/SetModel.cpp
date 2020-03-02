#ifndef SETMODEL_CPP
#define SETMODEL_CPP

class SetModel{

public:
  virtual ~SetModel() {}

  virtual void init() = 0;
  virtual void parse_config() = 0;
};


#endif //SETMODEL_CPP
