#pragma once

#include "Output.h"
#include "comparisons.h"
#include "formatting.h"

#include <chrono>
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <vector>
#ifdef __has_include
#if __has_include(<source_location>) && __has_include(<string_view>)
#include <source_location>
#include <string_view>
#endif
#endif

namespace Test {
  struct TestMethodInfo {
    std::uintptr_t reference;
    std::string fullName;
  };

  struct AssertionFailedException : public std::runtime_error {
    AssertionFailedException() : std::runtime_error{"Test assertion failed"} {}
  };

  /*!
   * Any test-class must extend this class
   */
  class Suite {
  public:
    Suite();
    explicit Suite(const std::string &name);
    Suite(const Suite &) = delete;
    Suite(Suite &&) noexcept = default;
    virtual ~Suite() noexcept = default;

    Suite &operator=(const Suite &) = delete;
    Suite &operator=(Suite &&) = default;

    /*!
     * Adds another Test::Suite to this suite.
     * All associated suites will be run after this suite has finished
     */
    void add(const std::shared_ptr<Test::Suite> &suite);

    /*!
     * Runs all the registered test-methods in this suite
     *
     * \param out The output to print the results to
     * \param continueOnError whether to continue running after a test failed
     */
    virtual bool run(Output &out, bool continueOnError = true);
    virtual bool run(Output &out, const std::vector<TestMethodInfo> &selectedMethods, bool continueOnError = true);

    /*!
     * Lists all test-methods to be run in this suite
     */
    virtual std::vector<TestMethodInfo> listTests() const;

    std::string getName() const { return suiteName; }

  protected:
    //! Test-method without any parameter
    using SimpleTestMethod = void (Suite::*)();
    //! Test-method with an arbitrary number of arguments of arbitrary types
    template <typename... T>
    using ParameterizedTestMethod = void (Suite::*)(T... args);

    void setSuiteName(const std::string &filePath);
    void addTest(SimpleTestMethod method, const std::string &funcName);

#if defined(__clang__) || (defined(__GNUC__) && __GNUC__ < 5)
    // Need extra handling for clang++, see https://llvm.org/bugs/show_bug.cgi?id=25695 and
    // https://llvm.org/bugs/show_bug.cgi?id=25250 Same for GCC <= 4.8, see
    // https://gcc.gnu.org/bugzilla/show_bug.cgi?id=41933
    template <typename T>
    inline void addTest(ParameterizedTestMethod<T> method, const std::string &funcName, const T arg0) {
      testMethods.emplace_back(TestMethod(funcName, method, arg0));
    }

    template <typename T, typename U>
    inline void addTest(ParameterizedTestMethod<T, U> method, const std::string &funcName, const T arg0, const U arg1) {
      testMethods.emplace_back(TestMethod(funcName, method, arg0, arg1));
    }

    template <typename T, typename U, typename V>
    inline void addTest(ParameterizedTestMethod<T, U, V> method, const std::string &funcName, const T arg0,
        const U arg1, const V arg2) {
      testMethods.emplace_back(TestMethod(funcName, method, arg0, arg1, arg2));
    }
#else
    template <typename... T>
    inline void addTest(ParameterizedTestMethod<T...> method, const std::string &funcName, const T... args) {
      testMethods.emplace_back(TestMethod(funcName, method, args...));
    }
#endif

    void testSucceeded(Assertion &&assertion);
    void testFailed(Assertion &&assertion);
    void testFailed(
        const char *fileName, uint32_t lineNumber, std::string &&errorMessage, const std::string &userMessage = "");

    void testRun(bool success, const char *fileName, uint32_t lineNumber, std::string &&errorMessage,
        const std::string &userMessage = "");

    inline void testRun(bool success, const char *fileName, uint32_t lineNumber, const char *errorMessage,
        const std::string &userMessage = "") {
      testRun(success, fileName, lineNumber, std::string{errorMessage ? errorMessage : ""}, userMessage);
    }

    template <typename Func>
    void testRun(
        bool success, const char *fileName, uint32_t lineNumber, Func &&generateError, const std::string &userMessage) {
      testRun(success, fileName, lineNumber, !success ? generateError() : std::string{}, userMessage);
    }

    const std::string &toMessage(const std::string &msg) const { return msg; }
    std::string toMessage(const char *msg) const { return std::string{msg ? msg : ""}; }

    inline bool continueAfterFailure() { return continueAfterFail; }

    /*!
     * This method can be overridden to execute code before the test-methods in this class are run
     *
     * \return A boolean value, if false, the whole suite will not be executed (inclusive \ref tearDown)
     */
    virtual bool setup() {
      // does nothing by default
      return true;
    }

    /*!
     * This method can be overridden to run code after all the test-methods have finished
     */
    virtual void tear_down() {
      // does nothing by default
    }

    /*!
     * Override this method to run code before every test-method
     *
     * \return A boolean value. If false, the test-method will not be executed (inclusive \ref after)
     */
    virtual bool before(const std::string &methodName) {
      // does nothing by default
      (void)methodName;
      return true;
    }

    /*!
     * Override this method to run code after every test-method
     */
    virtual void after(const std::string &methodName, const bool success) {
      // does nothing by default
      (void)methodName;
      (void)success;
    }

    /*!
     * Returns whether the current test method has already failed (but continued to execute)
     */
    inline bool hasFailed() { return !currentTestSucceeded; }

#if defined(__cpp_lib_source_location) && defined(__cpp_lib_string_view)
    inline void testFail(std::string_view msg, std::source_location loc = std::source_location::current()) {
      testFailed(Test::Assertion(loc.file_name(), loc.line(), "", std::string{msg}));
      if (!continueAfterFailure())
        throw AssertionFailedException{};
    }

    inline void testAssert(
        bool condition, std::string_view msg = "", std::source_location loc = std::source_location::current()) {
      testRun(condition, loc.file_name(), loc.line(), "Assertion failed", std::string{msg});
    }

    template <typename Func>
    inline void testAssert(
        Func &&condition, std::string_view msg = "", std::source_location loc = std::source_location::current()) {
      testRun(
          condition(), loc.file_name(), loc.line(),
          [] { return "Assertion failed for condition: " + toPrettyTypeName(typeid(Func)); }, std::string{msg});
    }

    template <typename T, typename U>
    inline void testAssertEquals(T &&expected, U &&value, std::string_view msg = "",
        std::source_location loc = std::source_location::current()) {
      testRun(
          Test::Comparisons::isSame(expected, value), loc.file_name(), loc.line(),
          [&] { return "Got " + Test::Formats::to_string(value) + ", expected " + Test::Formats::to_string(expected); },
          std::string{msg});
    }

    template <typename T>
    inline void testAssertDelta(T &&expected, T &&value, T &&delta, std::string_view msg = "",
        std::source_location loc = std::source_location::current()) {
      testRun(
          Test::Comparisons::inMaxDistance(expected, value, delta), loc.file_name(), loc.line(),
          [&] {
            return "Got " + Test::Formats::to_string(value) + ", expected " + Test::Formats::to_string(expected) +
                   " +/- " + Test::Formats::to_string(delta);
          },
          std::string{msg});
    }

    template <typename T>
    inline void testAssertUlp(T &&expected, T &&value, T &&numULP, std::string_view msg = "",
        std::source_location loc = std::source_location::current()) {
      using Type = std::remove_reference_t<decltype(expected)>;
      auto delta = (expected) * static_cast<Type>(numULP) * std::numeric_limits<Type>::epsilon();
      testRun(
          Test::Comparisons::inMaxDistance(expected, value, delta), loc.file_name(), loc.line(),
          [&] {
            return "Got " + Test::Formats::to_string(value) + ", expected " + Test::Formats::to_string(expected) +
                   " +/- " + (Test::Formats::to_string(delta) + " (") + (Test::Formats::to_string(numULP) + " ULP)");
          },
          std::string{msg});
    }

    template <typename Exception, typename Func>
    inline void testThrows(
        Func &&expression, std::string_view msg = "", std::source_location loc = std::source_location::current()) {
      const std::string EXCEPTION_NAME = toPrettyTypeName(typeid(Exception));
      try {
        expression();
        /*If we get here, no exception was thrown*/
        testFailed(loc.file_name(), loc.line(), "Expected exception of type " + EXCEPTION_NAME + " was not thrown",
            std::string{msg});
      } catch (const Exception &) {
        testSucceeded(Test::Assertion(loc.file_name(), loc.line()));
      } catch (std::exception &ex) {
        /*If we get here, wrong exception was thrown*/
        testFailed(loc.file_name(), loc.line(),
            "Wrong Exception of type " + toPrettyTypeName(typeid(ex)) + " was thrown: " + ex.what(), std::string{msg});
      } catch (...) {
        /* Any other type than an exception was thrown*/
        testFailed(loc.file_name(), loc.line(),
            "A non-exception-type was thrown, expected exception of type: " + EXCEPTION_NAME, std::string{msg});
      }
    }

    template <typename Func>
    inline void testThrowsAnything(
        Func &&expression, std::string_view msg = "", std::source_location loc = std::source_location::current()) {
      try {
        expression();
        /*If we get here, no exception was thrown*/
        testFailed(loc.file_name(), loc.line(), "Expected exception was not thrown", std::string{msg});
      } catch (std::exception &ex) {
        testSucceeded(Test::Assertion(loc.file_name(), loc.line()));
      } catch (...) {
        /* Any other type than an exception was thrown*/
        testFailed(loc.file_name(), loc.line(), "A non-exception-type was thrown");
      }
    }

    template <typename Func>
    inline void testThrowsNothing(
        Func &&expression, std::string_view msg = "", std::source_location loc = std::source_location::current()) {
      try {
        expression();
      } catch (std::exception &ex) {
        /*If we get here, an exception was thrown*/
        testFailed(loc.file_name(), loc.line(),
            "Exception of type " + toPrettyTypeName(typeid(ex)) + " was thrown: " + ex.what(), std::string{msg});
      } catch (...) {
        /* Any other type than an exception was thrown*/
        testFailed(loc.file_name(), loc.line(), "A non-exception-type was thrown");
      }
    }

    template <typename Predicate, typename T>
    inline void testPredicate(Predicate &&predicate, T &&value, std::string_view msg = "",
        std::source_location loc = std::source_location::current()) {
      testRun(
          predicate(value), loc.file_name(), loc.line(),
          [&] {
            return "Value " + Test::Formats::to_string(value) +
                   " did not match the predicate: " + toPrettyTypeName(typeid(Predicate));
          },
          std::string{msg});
    }

    template <typename Predicate, typename T, typename U>
    inline void testBiPredicate(Predicate &&predicate, T &&value0, U &&value1, std::string_view msg = "",
        std::source_location loc = std::source_location::current()) {
      testRun(
          predicate(value0, value1), loc.file_name(), loc.line(),
          [&] {
            return "Values " + Test::Formats::to_string(value0) + " and " + Test::Formats::to_string(value1) +
                   " did not match the bi-predicate: " + toPrettyTypeName(typeid(Predicate));
          },
          std::string{msg});
    }

    inline void testAbort(std::string_view msg, std::source_location loc = std::source_location::current()) {
      testFailed(Test::Assertion(loc.file_name(), loc.line(), "Test-method aborted", std::string{msg}));
      throw AssertionFailedException{};
    }
#endif

  private:
    struct TestMethod {
      const std::string name;
      const std::function<void(Suite *)> functor;
      const std::string argString;

      TestMethod() : name("") {}

      TestMethod(const std::string &methodName, SimpleTestMethod method) : name(methodName), functor(method), argString({}) {}

#if defined(__clang__) || (defined(__GNUC__) && __GNUC__ < 5)
      template <typename T>
      TestMethod(const std::string &methodName, ParameterizedTestMethod<T> method, const T arg0)
          : name(methodName), functor([arg0, method](Suite *suite) { (suite->*method)(arg0); }),
            argString(joinStrings(arg0)) {}

      template <typename T, typename U>
      TestMethod(const std::string &methodName, ParameterizedTestMethod<T, U> method, const T arg0, const U arg1)
          : name(methodName), functor([arg0, arg1, method](Suite *suite) { (suite->*method)(arg0, arg1); }),
            argString(joinStrings(arg0, arg1)) {}

      template <typename T, typename U, typename V>
      TestMethod(
          const std::string &methodName, ParameterizedTestMethod<T, U, V> method, const T arg0, const U arg1, const V arg2)
          : name(methodName), functor([arg0, arg1, arg2, method](Suite *suite) { (suite->*method)(arg0, arg1, arg2); }),
            argString(joinStrings(arg0, arg1, arg2)) {}
#else
      template <typename... T>
      TestMethod(const std::string &methodName, ParameterizedTestMethod<T...> method, const T... args)
          : name(methodName), functor([args..., method](Suite *suite) { (suite->*method)(args...); }),
            argString(joinStrings(args...)) {}
#endif

      inline void operator()(Suite *suite) const { functor(suite); }

      template <typename T, typename... R>
      static inline std::string joinStrings(const T &t, const R &...remainder) {
        if (sizeof...(R) == 0)
          return Test::Formats::to_string(t);
        return (Test::Formats::to_string(t) + ", ") + joinStrings(remainder...);
      }

      template <typename... R>
      static inline std::string joinStrings(const std::string &t, const R &...remainder) {
        // special case to enquote std::string
        if (sizeof...(R) == 0)
          return std::string("\"") + t + "\"";
        return (std::string("\"") + t + "\", ") + joinStrings(remainder...);
      }

      template <typename... R>
      static inline std::string joinStrings(const R &.../* remainder */) {
        // is never called, but must exist to not throw compilation errors
        return "";
      }
    };

    std::string suiteName;
    std::vector<TestMethod> testMethods;
    std::vector<std::shared_ptr<Test::Suite>> subSuites;
    std::string currentTestMethodName;
    std::string currentTestMethodArgs;
    std::chrono::microseconds totalDuration;
    Output *output;
    uint32_t positiveTestMethods;
    bool continueAfterFail;
    bool currentTestSucceeded;

    std::pair<bool, std::chrono::microseconds> runTestMethod(const TestMethod &method);

    std::vector<std::reference_wrapper<const TestMethod>> filterTests(
        const std::vector<TestMethodInfo> &selectedMethods);

    static std::string toPrettyTypeName(const std::type_info &type);

    // ParallelSuite needs access to subSuites
    friend class ParallelSuite;
  };

  /*!
   * Registers a simple test-method
   */
#define TEST_ADD(func)                                                                                                 \
  this->setSuiteName(__FILE__);                                                                                        \
  this->addTest(static_cast<Test::Suite::SimpleTestMethod>((&func)), #func)
  /*!
   * Registers a test-method taking a single argument of type std::string or a c-style string-literal
   */
#define TEST_ADD_WITH_STRING(func, string)                                                                             \
  this->setSuiteName(__FILE__);                                                                                        \
  this->addTest<std::basic_string<char>>(                                                                              \
      static_cast<Test::Suite::ParameterizedTestMethod<std::basic_string<char>>>((&func)), #func, string)
  /*!
   * Registers a test-method taking a single argument of type c-string
   */
#define TEST_ADD_WITH_STRING_LITERAL(func, stringLiteral)                                                              \
  this->setSuiteName(__FILE__);                                                                                        \
  this->addTest<char *>(static_cast<Test::Suite::ParameterizedTestMethod<char *>>((&func)), #func, stringLiteral)
  /*!
   * Registers a test-method accepting a single argument of type int (or any type which can be coerced from int)
   */
#define TEST_ADD_WITH_INTEGER(func, number)                                                                            \
  this->setSuiteName(__FILE__);                                                                                        \
  this->addTest<int>(static_cast<Test::Suite::ParameterizedTestMethod<int>>((&func)), #func, number)
  /*!
   * Registers a test-method which takes an argument to a pointer of arbitrary data
   */
#define TEST_ADD_WITH_POINTER(func, pointer)                                                                           \
  this->setSuiteName(__FILE__);                                                                                        \
  this->addTest<void *>(static_cast<Test::Suite::ParameterizedTestMethod<void *>>((&func)), #func, pointer)
  /*!
   * Registers a test-method with a single argument of arbitrary type
   */
#define TEST_ADD_SINGLE_ARGUMENT(func, arg)                                                                            \
  this->setSuiteName(__FILE__);                                                                                        \
  this->addTest<decltype(arg)>(static_cast<Test::Suite::ParameterizedTestMethod<decltype(arg)>>((&func)), #func, arg)
  /*!
   * Registers a test-method with two arguments of arbitrary types
   */
#define TEST_ADD_TWO_ARGUMENTS(func, arg0, arg1)                                                                       \
  this->setSuiteName(__FILE__);                                                                                        \
  this->addTest<decltype(arg0), decltype(arg1)>(                                                                       \
      static_cast<Test::Suite::ParameterizedTestMethod<decltype(arg0), decltype(arg1)>>((&func)), #func, arg0, arg1)
  /*!
   * Registers a test-method with three arguments of arbitrary types
   */
#define TEST_ADD_THREE_ARGUMENTS(func, arg0, arg1, arg2)                                                               \
  this->setSuiteName(__FILE__);                                                                                        \
  this->addTest<decltype(arg0), decltype(arg1), decltype(arg2)>(                                                       \
      static_cast<Test::Suite::ParameterizedTestMethod<decltype(arg0), decltype(arg1), decltype(arg2)>>((&func)),      \
      #func, arg0, arg1, arg2)
} // namespace Test
