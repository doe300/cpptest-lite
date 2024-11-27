#include "cpptest-main.h"

#include "cpptest.h"

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <vector>

namespace Test {
  using SuiteSupplier = std::function<Test::Suite *(void)>;

  struct SuiteEntry {
    std::string name;
    SuiteSupplier supplier;
    std::string description;
    RegistrationFlags flags;

    bool has(RegistrationFlags flag) const noexcept {
      return (static_cast<uint32_t>(flags) & static_cast<uint32_t>(flag)) == static_cast<uint32_t>(flag);
    }
  };

  // using a list here sorts entries by order of insertion
  static std::vector<SuiteEntry> availableSuites;
  static bool continueAfterFailure = true;
  // optional set of arguments to be ignored, e.g. to not report an error on it
  static std::set<std::string> ignoredArguments;

  void setContinueAfterFail(bool continueAfterFail) { Test::continueAfterFailure = continueAfterFail; }
  void ignoreArgument(const std::string &arg) { Test::ignoredArguments.emplace(arg); }

  void registerSuite(const SuiteSupplier &supplier, const std::string &parameterName, const std::string &description,
      bool runByDefault) {
    registerSuite(supplier, parameterName, description,
        runByDefault ? RegistrationFlags::NONE : RegistrationFlags::OMIT_FROM_DEFAULT);
  }

  void registerSuite(const SuiteSupplier &supplier, const std::string &parameterName, const std::string &description,
      RegistrationFlags flags) {
    if (parameterName.empty())
      throw std::invalid_argument("Test-suite parameter cannot be empty!");
    if (parameterName.find_first_of(" \"'") != std::string::npos)
      throw std::invalid_argument(std::string("Test-suite parameter is invalid: ") + parameterName);
    SuiteEntry entry;
    entry.name = std::string("--") + parameterName;
    entry.supplier = supplier;
    entry.description = description;
    entry.flags = flags;
    availableSuites.emplace_back(std::move(entry));
  }

  void printHelp(const std::string &progName) {
    static const int paramWidth = 30;
    static const int gapWidth = 4;
    std::cout << "Runs the test-suites. The list of all available test-suites can be found beneath. Passing no "
                 "argument runs all suites marked with '(default)'."
              << std::endl;
    std::cout << "Run with '" << progName << " [options]'" << std::endl;
    std::cout << std::endl;
    std::cout << std::setw(paramWidth) << "--help" << std::setw(gapWidth) << " " << "Prints this help" << std::endl;
    std::cout << std::setw(paramWidth) << "--list-tests[=<file>]" << std::setw(gapWidth) << " "
              << "Prints the list of test methods to the given output file or the standard output. Does not actually "
                 "run any tests"
              << std::endl;
    std::cout << std::setw(paramWidth) << "--test-pattern=<pattern>" << std::setw(gapWidth) << " "
              << "Runs only the test methods matching the given simplified glob pattern. Only full matches and "
                 "wildcard (via '*') matching is supported."
              << std::endl;
    std::cout << std::setw(paramWidth) << " " << std::setw(gapWidth) << " "
              << "Can be repeated to include test-methods matching any of the given patterns." << std::endl;
    std::cout << std::setw(paramWidth) << "--output=val" << std::setw(gapWidth) << " "
              << "Sets the output of the tests. Available options are: plain, colored, gcc, msvc, generic, junit. "
                 "Defaults to 'plain'"
              << std::endl;
    std::cout << std::setw(paramWidth) << "-o=val" << std::setw(gapWidth) << " "
              << "'plain' prints simple text, 'colored' uses console colors, 'gcc', 'msvc' and 'generic' use "
                 "compiler-like output syntax"
              << std::endl;
    std::cout << std::setw(paramWidth) << "--mode=val" << std::setw(gapWidth) << " "
              << "Sets the output mode to one of 'debug', 'verbose', 'terse' in order of the amount of information "
                 "printed. Defaults to 'terse'"
              << std::endl;
    std::cout << std::setw(paramWidth) << "--output-file=file" << std::setw(gapWidth) << " "
              << "Sets the optional output file to write to, defaults to 'stdout'. 'colored' output can only write to "
                 "console!"
              << std::endl;
    std::cout << std::endl;
    std::cout << "Test suites:" << std::endl;
    // sort suites by name
    std::map<std::string, SuiteEntry> sortedSuites;
    for (const auto &entry : availableSuites)
      sortedSuites.emplace(entry.name, entry);
    for (const auto &pair : sortedSuites) {
      std::cout << std::setw(paramWidth) << pair.first << std::setw(gapWidth) << " " << pair.second.description
                << (pair.second.has(RegistrationFlags::OMIT_FROM_DEFAULT) ? "" : " (default)") << std::endl;
    }
  }

  static bool matchesPattern(const std::string &name, const std::vector<std::string> &parts) {
    auto it = parts.begin();
    std::size_t pos = 0;
    for (; it != parts.end(); ++it) {
      if ((pos = name.find(*it, pos)) == std::string::npos)
        return false;
      pos += it->size();
    }
    return it == parts.end();
  }

  static std::vector<TestMethodInfo> filterTests(
      std::vector<TestMethodInfo> &&infos, const std::vector<std::vector<std::string>> &patterns) {
    infos.erase(
        std::remove_if(infos.begin(), infos.end(),
            [&patterns](const TestMethodInfo &info) {
              return std::none_of(patterns.begin(), patterns.end(),
                  [&info](const std::vector<std::string> &pattern) { return matchesPattern(info.fullName, pattern); });
            }),
        infos.end());
    return std::move(infos);
  }

  int runSuites(int argc, char **argv, const ArgumentCallback &callback) {
    std::vector<std::pair<std::unique_ptr<Test::Suite>, std::string>> selectedSuites;
    std::set<std::string> selectedSuiteNames;
    selectedSuites.reserve(argc);
    std::string outputMode = "plain";
    std::string outputFile;
    unsigned int mode = Test::TextOutput::Terse;
    std::unique_ptr<std::ostream> listOutputFile;
    std::ostream *listTestsOutput = nullptr;
    std::ostream *listSuitesOutput = nullptr;
    std::vector<std::string> testPatterns;
    for (int i = 1; i < argc; ++i) {
      std::string arg(argv[i]);
      if (arg == "--help" || arg == "-h") {
        printHelp(argv[0]);
        return EXIT_SUCCESS;
      } else if (arg.find("--list-tests") == 0) {
        if (arg.find('=') != std::string::npos) {
          listOutputFile.reset(new std::ofstream(arg.substr(arg.find('=') + 1)));
          listTestsOutput = listOutputFile.get();
        } else {
          listTestsOutput = &std::cout;
        }
      } else if (arg.find("--list-suites") == 0) {
        if (arg.find('=') != std::string::npos) {
          listOutputFile.reset(new std::ofstream(arg.substr(arg.find('=') + 1)));
          listSuitesOutput = listOutputFile.get();
        } else {
          listSuitesOutput = &std::cout;
        }
      } else if (arg.find("--test-pattern=") == 0) {
        if (arg.find('=') != std::string::npos)
          testPatterns.emplace_back(arg.substr(arg.find('=') + 1));
      } else if (arg.find("--output-file=") == 0) {
        if (arg.find('=') != std::string::npos)
          outputFile = arg.substr(arg.find('=') + 1);
      } else if (arg.find("--output=") == 0 || arg.find("-o=") == 0) {
        outputMode = arg;
      } else if (arg.find("--mode=") == 0) {
        if (arg.find("debug") != std::string::npos)
          mode = Test::TextOutput::Debug;
        else if (arg.find("verbose") != std::string::npos)
          mode = Test::TextOutput::Verbose;
        else if (arg.find("terse") != std::string::npos)
          mode = Test::TextOutput::Terse;
        else
          std::cerr << "Unrecognized output mode: " << arg << std::endl;
      } else {
        const char *name = argv[i];
        if (ignoredArguments.find(name) != ignoredArguments.end()) {
          continue;
        }
        auto it = std::find_if(availableSuites.begin(), availableSuites.end(),
            [name](const SuiteEntry &e) -> bool { return e.name == name; });
        if (it == availableSuites.end() && (!callback || !callback(arg))) {
          std::cerr << "Unknown parameter: " << argv[i] << std::endl;
          if (!callback)
            return EXIT_FAILURE;
          continue;
        }
        if (selectedSuiteNames.find(name) == selectedSuiteNames.end()) {
          selectedSuites.emplace_back(std::make_pair(it->supplier(), it->name));
          selectedSuiteNames.emplace(name);
        }
      }
    }

    std::vector<std::vector<std::string>> testSplitPatterns;
    if (!testPatterns.empty()) {
      // just as marker to select all correct suites below
      listTestsOutput = &std::cout;
      testSplitPatterns.reserve(testPatterns.size());
      for (auto &pattern : testPatterns) {
        testSplitPatterns.emplace_back();
        std::size_t pos = 0;
        while ((pos = pattern.find('*')) != std::string::npos) {
          testSplitPatterns.back().emplace_back(pattern.substr(0, pos));
          pattern.erase(0, pos + 1);
        }
        if (!pattern.empty()) {
          testSplitPatterns.back().emplace_back(pattern.substr(0, pos));
        }
      }
    }

    if (selectedSuites.empty()) {
      // only the program-name run all suites
      for (const auto &entry : availableSuites) {
        if ((!listTestsOutput && !listSuitesOutput && entry.has(RegistrationFlags::OMIT_FROM_DEFAULT)) ||
            ((listTestsOutput || listSuitesOutput) && entry.has(RegistrationFlags::OMIT_LIST_TESTS))) {
          continue;
        }
        if (selectedSuiteNames.find(entry.name) == selectedSuiteNames.end()) {
          selectedSuites.emplace_back(std::make_pair(entry.supplier(), entry.name));
          selectedSuiteNames.emplace(entry.name);
        }
      }
    }

    std::ofstream f;
    if (!outputFile.empty())
      f.open(outputFile, std::ios_base::out | std::ios_base::trunc);

    std::unique_ptr<Test::Output> realOutput;
    std::unique_ptr<Test::Output> output;

    if (outputMode.find("plain") != std::string::npos) {
      if (!outputFile.empty())
        realOutput.reset(new Test::TextOutput(mode, f));
      else
        realOutput.reset(new Test::TextOutput(mode));
    } else if (outputMode.find("colored") != std::string::npos)
      realOutput.reset(new Test::ConsoleOutput(mode));
    else if (outputMode.find("gcc") != std::string::npos) {
      if (!outputFile.empty())
        realOutput.reset(new Test::CompilerOutput(Test::CompilerOutput::FORMAT_GCC, f));
      else
        realOutput.reset(new Test::CompilerOutput(Test::CompilerOutput::FORMAT_GCC));
    } else if (outputMode.find("msvc") != std::string::npos) {
      if (!outputFile.empty())
        realOutput.reset(new Test::CompilerOutput(Test::CompilerOutput::FORMAT_MSVC, f));
      else
        realOutput.reset(new Test::CompilerOutput(Test::CompilerOutput::FORMAT_MSVC));
    } else if (outputMode.find("generic") != std::string::npos) {
      if (!outputFile.empty())
        realOutput.reset(new Test::CompilerOutput(Test::CompilerOutput::FORMAT_GENERIC, f));
      else
        realOutput.reset(new Test::CompilerOutput(Test::CompilerOutput::FORMAT_GENERIC));
    } else if (outputMode.find("junit") != std::string::npos) {
      if (!outputFile.empty())
        realOutput.reset(new Test::XMLOutput(outputFile));
      else
        realOutput.reset(new Test::XMLOutput(std::cout));
    } else {
      std::cout << "Unrecognized output: " << outputMode << std::endl;
      if (!outputFile.empty())
        realOutput.reset(new Test::TextOutput(mode, f));
      else
        realOutput.reset(new Test::TextOutput(mode));
    }

    // there could be parallel suites
    output.reset(new Test::SynchronizedOutput(*realOutput));

    if (selectedSuites.empty()) {
      std::cerr << "No Test-suites selected, exiting!" << std::endl;
      return EXIT_SUCCESS;
    }

    bool failures = false;
    for (auto &suite : selectedSuites) {
      if (!testPatterns.empty()) {
        auto matchingTests = filterTests(suite.first->listTests(), testSplitPatterns);
        if (!matchingTests.empty())
          failures = !suite.first->run(*output, matchingTests, Test::continueAfterFailure) || failures;
      } else if (listTestsOutput) {
        for (const auto &test : suite.first->listTests()) {
          *listTestsOutput << test.fullName << std::endl;
        }
      } else if (listSuitesOutput) {
        *listSuitesOutput << suite.first->getName() << ':' << suite.second << std::endl;
      } else {
        failures = !suite.first->run(*output, Test::continueAfterFailure) || failures;
      }
    }

    return failures ? EXIT_FAILURE : EXIT_SUCCESS;
  }

} // namespace Test
