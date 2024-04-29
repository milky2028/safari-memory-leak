# Safari Page Refresh Memory Leak

## Setup

Install deno, then run the wasm server with deno.

```
brew install deno
deno run --allow-net --allow-read server.ts
```

## Reproducing

- Open http://localhost:8080/ in Safari.
- Refesh the page until you see `Failure. Error: Out of memory` on the page (usually takes about 7 refreshes for me)

## Details

Output is written to the page instead of the console to verify that the page breaks even with the console closed. The error still persists with Web Inspector totally disabled from the settings menu.

Comparing heap snapshots in the Timeline view shows that multiple `SharedArrayBuffer`s being retained between each refresh.

With PS web, it's more common for the page to crash with the message, "This webpage reloaded because a problem occurred" or, "This webpage reloaded because it was using significant memory." In this example, we just see that after about 7 refreshes, the page just starts throwing out of memory errors.

Compiled output from Emscripten is committed in this repo to simplify reproducing, but the outcome should be reproducible when compiling with Emscripten on your own. `compile.sh` shows the compiler flags used. Emscripten `3.1.54` was used to compile the output committed here.
