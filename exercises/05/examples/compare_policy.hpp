#ifndef COMPARE_POLICY_HPP
#define COMPARE_POLICY_HPP

#include <cctype>
#include <string>

// Case sensitive comparator.
class CaseCompare {
public:
  bool operator()(char a, char b) const { return a == b; }
};

// Case insensitive comparator.
class NoCaseCompare {
public:
  bool operator()(char a, char b) const {
    return std::toupper(static_cast<unsigned char>(a)) ==
           std::toupper(static_cast<unsigned char>(b));
  }
};

// Equality policies, not ordering predicates for std::sort.
// Function to compare two strings according to a policy.
//
// The policy is provided as a template parameter. We assume that
// it is default-constructible and has a call operator that takes two chars
// and returns true if and only if they are considered equivalent.
// CompareString also requires the policy to be callable on a const object.
template <class Policy>
bool compare(const std::string &a, const std::string &b) {
  if (a.size() != b.size()) {
    return false;
  }

  Policy equal{};

  for (std::size_t i = 0; i < a.size(); ++i) {
    if (!equal(a[i], b[i])) {
      return false;
    }
  }

  return true;
}

// Functor to compare strings according to a policy stored as class member.
template <class Policy> class CompareString {
public:
  CompareString() : compare{} {}

  bool operator()(const std::string &a, const std::string &b) const {
    if (a.size() != b.size()) {
      return false;
    }

    for (std::size_t i = 0; i < a.size(); ++i) {
      if (!compare(a[i], b[i])) {
        return false;
      }
    }

    return true;
  }

private:
  Policy compare;
};

#endif /* COMPARE_POLICY_HPP */
