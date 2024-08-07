#include <emscripten.h>
#include <emscripten/bind.h>
#include <stdio.h>
#include <functional>
#include <thread>

EMSCRIPTEN_DECLARE_VAL_TYPE(EmscriptenPromiseWrapper);

auto EMSCRIPTEN_PROMISE_QUEUE_KEY = "__EMSCRIPTEN_PROMISE_QUEUE__";
int32_t id = 0;

auto promisify(std::function<void()> fn) {
  std::function<EmscriptenPromiseWrapper()> f = [fn] {
    int32_t promise_id = ++id;

    // clang-format off
    MAIN_THREAD_ASYNC_EM_ASM({
      const promiseQueueKey = UTF8ToString($0);
      if (!globalThis[promiseQueueKey]) {
        globalThis[promiseQueueKey] = new Map();
      }

      const promiseData = { id: $1 };
      promiseData.promise = new Promise((resolve, reject) => {
        promiseData.resolve = resolve;
        promiseData.reject = reject;
      });

      globalThis[promiseQueueKey].set($1, promiseData);
    }, EMSCRIPTEN_PROMISE_QUEUE_KEY, promise_id);
    // clang-format on

    std::thread([fn, promise_id] {
      fn();

      // clang-format off
      MAIN_THREAD_ASYNC_EM_ASM({
        const promiseQueueKey = UTF8ToString($0);
        const map = globalThis[promiseQueueKey];
        const promise = map.get($1);

        promise?.resolve();
      }, EMSCRIPTEN_PROMISE_QUEUE_KEY, promise_id);
      // clang-format on
    }).detach();

    auto promise_map = emscripten::val::global(EMSCRIPTEN_PROMISE_QUEUE_KEY);
    return promise_map.call<EmscriptenPromiseWrapper>("get", promise_id);
  };

  return f;
}

auto print_from_thread() {
  return promisify([] {
    printf("Awaitably printed from thread.\n");
    return emscripten::val::undefined();
  })();
}

EMSCRIPTEN_BINDINGS(module) {
  emscripten::function("print_from_thread", &print_from_thread);
  emscripten::register_type<EmscriptenPromiseWrapper>(
      "{ id: number; promise: Promise<void>; resolve: VoidFunction; reject: VoidFunction; } | undefined");
}