#pragma once

/*
 * @brief An interface that represent the inner workings of a system
 */
class System {
  /*
   * @brief Runs the system for a tick
   */
  protected:
    virtual void run_system();
    virtual System& operator=(const System&) = delete;
    virtual System& operator=(System&&) = delete;
    virtual ~System() {};
};

