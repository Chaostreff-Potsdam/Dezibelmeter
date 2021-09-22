#pragma once

#include <map>
#include <ostream>
#include <functional>

/**
 * Information about a configurable option
 */
struct ConfigOption {
  std::function<void(std::ostream &)> toFormFragment;
  std::function<void(std::function<String(String)>)> changeConfiguration;

  ConfigOption(const std::function<void(std::ostream &)> &toFormFragment,
               std::function<void(std::function<String(String)>)> changeConfiguration)
  : toFormFragment(toFormFragment), changeConfiguration(changeConfiguration) {}
};
