#ifndef WA_REQUEST_HPP
#define WA_REQUEST_HPP

#include <memory>
#include <string>
#include <sstream>
#include <map>
#include <future>

#if defined(__GNUC__) && __cplusplus >= 201103L
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

#include <urdl/istream.hpp>

#if defined(__GNUC__) && __cplusplus >= 201103L
# pragma GCC diagnostic pop
#endif

namespace wa
{

namespace detail
{

struct sender
{
  // request parameter would be copied anyway - use move semantic for it if possible
  template <class Handler>
  auto operator()(std::string request, Handler handler) const -> typename std::result_of<Handler(std::string)>::type
  {
    using namespace std::string_literals;

    static const std::string address_prefix = "http://www.wolframalpha.com/input/?i="s;

    // replace request symbols for symbol_map
    for (const auto& k: symbol_map)
    {
      const auto place = request.find(k.first);
      if (place != std::string::npos)
        request.replace(place, 1u, k.second);
    }

    const std::string address = address_prefix + request;
    urdl::istream is(address);
    if (!is)
    {
      throw std::logic_error("Can't open url "s + address);
    }

    std::string content;
    std::getline(is, content, static_cast<char>(std::char_traits<char>::eof()));

    return handler(content);
  }

  static const std::map<char, std::string> symbol_map;
};

const std::map<char, std::string> sender::symbol_map =
  {
    {',', "%2C"},
    {'+', "%2B"},
    {'/', "%2F"},
    {' ', "+"}
  };

// type erased async_sender
struct async_sender: sender
{
private:
  using sender::operator(); // hide it

  struct base_holder
  {
    virtual ~base_holder()
    {
    }
  };

  template <class T>
  struct holder: base_holder
  {
    using result_type = T;

    template <class Handler>
    holder(const std::string& request, Handler handler, const std::launch policy):
      future{std::async(policy, detail::sender{}, request, handler)}
    {
    }

    std::future<T> future;
  };

public:
  template <class Handler>
  async_sender(const std::string& request, Handler handler, const std::launch policy = std::launch::async):
    held{std::make_unique<holder<typename std::result_of<Handler (std::string)>::type > >(request, handler, policy)}
  {
  }

  template<class T>
  T get() // non const
  {
    return dynamic_cast<holder<T>&>(*held).future.get();
  }

private:
  std::unique_ptr<base_holder> held;
};

struct get_result_handler
{
  std::string operator()(const std::string& str) const
  {
    using namespace std::string_literals;

    static const std::string prefix1 = "result"s;
    static const std::string prefix2 = "/input/?i="s;
    static const std::string prefix3 = "&lk"s;

    std::istringstream ss(str);
    std::string line;
    while (std::getline(ss, line))
    {
      const size_t first_found_pos = line.find(prefix1);
      if (first_found_pos != std::string::npos)
      {
        const size_t second_found_pos = line.find(prefix2, first_found_pos);
        if (second_found_pos != std::string::npos)
        {
          const size_t third_found_pos = line.find(prefix3, second_found_pos);

          const size_t result_pos = second_found_pos + prefix2.size();
          return line.substr(result_pos, third_found_pos - result_pos);
        }
      }
    }

    return std::string{};
  }
};

} // namespace detail

class request
{
public:
  explicit request(const std::string& str)
  {
  }

private:
};

} // namespace wa

#endif // WA_REQUEST_HPP
