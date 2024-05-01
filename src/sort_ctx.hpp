#pragma once

#include <coroutine>
#include <memory>
#include <span>
#include <vector>

namespace sv {
enum class SortBehavior {
  None,
  Compare,
  Swap,
  Set,
};

struct SortEvent {
  SortBehavior behavior;
  int arg0, arg1;
};

class Progress {
public:
  struct promise_type {
    SortEvent event{SortBehavior::None, 0, 0};

    Progress get_return_object() {
      return Progress{std::coroutine_handle<promise_type>::from_promise(*this)};
    }

    std::suspend_always initial_suspend() { return {}; }
    std::suspend_always yield_value(SortEvent &&event) {
      this->event = std::forward<SortEvent>(event);
      return {};
    }
    void return_void() {}
    std::suspend_always final_suspend() noexcept { return {}; }
    void unhandled_exception() {}
  };

  std::coroutine_handle<promise_type> handler;

  Progress(std::coroutine_handle<promise_type> handler) : handler{handler} {}

  ~Progress() {
    if (handler) {
      handler.destroy();
    }
  }

  explicit operator bool() {
    fill();
    return !handler.done();
  }

  SortEvent operator()() {
    fill();
    full = false;
    return std::move(handler.promise().event);
  }

private:
  bool full = false;

  void fill() {
    if (!full) {
      handler();
      full = true;
    }
  }
};

class SortContext {
public:
  SortContext(size_t size = 256);
  ~SortContext();

  void SetSize(size_t size);

  SortEvent Next();

  std::span<const int> GetArray() const;

  void Shuffle();
  void BubbleSort();
  void SelectionSort();
  void InsertionSort();
  void MergeSort();
  void QuickSort();

private:
  std::vector<int> m_Array;
  Progress *m_Progress;
};
} // namespace sv
