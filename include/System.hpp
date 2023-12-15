
class System {
public:
  System(const System &) = default;
  System(System &&) = delete;
  virtual System &operator=(const System &) = delete;
  virtual System &operator=(System &&) = delete;
  virtual void run() = 0;
  virtual ~System();
};
