# sys

System support library and shared `common.h` for the unit build system on
Linux x86-64. Every other public unit either includes `common.h` or links
`libsys.a` for process helpers, tracing, paths, and string utilities.

**Requires the [base](https://github.com/smythereen/base) repository** to be
installed first (project root with `BASE`, `rules.mak`, `targets.mak`, and the
`repo` unit). Clone base, then fetch this unit with `repo add sys` or via the
project starter list.

## What it provides

Exported headers land in `~/cmn/`. The static library is `~/lib/libsys.a`.

### Shared types and macros (`common.h`)

- Fixed-width aliases (`u8`/`s8` … `u64`/`s64`, `u_t`/`s_t`, float aliases,
  optional `f80`/`f128`/`f_t` where the host has them).
- Trace helpers (`Trace`, `If_Trace_Level`, `SYS_TRC_LVL`) wired to
  `TraceLevel` / `TracePointer`.
- Event severity enum (`EVENT_CRITICAL` … `EVENT_CLEARED`) for logging.
- Prototypes for the library entry points below.

### Process and execution

- `Fork`, `ForkFunction`, `Fork_AP_Func`, `Spawn`, `CheckChildren`,
  `Check_Func_Children`, `FlushChildren`, `ChildExit` — fork wrappers with
  optional child-death callbacks.
- `Exec` / `Exec_AP` — `execve` helpers that take printf-style or `va_list`
  argument vectors.
- `System` / `SystemAP` — printf-style wrappers around `system(3)`.
- `get_stat`, `lineage` — read `/proc/<pid>/stat` and walk the parent chain.
- `AddQuitRoutine`, `CallItQuits` — register and run shutdown hooks.

### Logging and diagnostics

- `SysPrt` / `SysPrtAP`, `Initialize_Print`, `Reset_Print`, `New_Err_File` —
  timestamped trace printing (optionally to a rotating log).
- `EventLog`, `EventLevelLog`, `ErrorLog` — event-file logging.
- `FatalError`, `SystemError`, `SystemWarn` — log then exit (or warn).

### Paths, files, and strings

- `Name_Translate` — resolve an env-var name to a path (with cycle guard).
- `Get_Path`, `File_Path` — build directory / file paths from env + pieces.
- `MkDir` — recursive `mkdir`.
- `Add_Line`, `Write_Text` — append printf-formatted lines to a file or fd.
- `Trim`, `strupr`, `strlwr`, `fixed2long`.

### Headers only (no matching `.c` in this public tree)

- `gf.h` / `gf8.h` — inline Galois-field GF(2⁸) helpers.
- `port.h` — RS-232 port API declarations (implementation not shipped here).
- `bitops.h` — atomic bit-op declarations (see Defects).

`sysprt.c` is the only library object that pulls in atomic headers
(`#include <atomic_32.h>`).

## Hardware / OS variant

`SYS_VARIANT` defaults to `linux-x86_64` in base `rules.mak`. This repository
ships that variant only (`VARIANT` file and makefile guard). The host is
**not** sniffed; set `SYS_VARIANT` explicitly for any other target (not
supported by this tree yet).

## Makefile / locations

```
sys/makefile          # include ../rules.mak + ../targets.mak
sys/VARIANT           # linux-x86_64
sys/UNIT.deps         # base 1; require atomic
```

Unit makefile sets `LIBRARIES =` (empty): `libsys.a` has **no** `-l` edges to
other unit libraries. Products that used to link `-lmap -lsem -lfar` are not
part of this public tree.

Build from a project rooted at `$HOME` (or `HOME=/path/to/project`):

```sh
cd ~/sys && make header && make library
# installs ~/cmn/common.h (and other headers) and ~/lib/libsys.a
```

## System prerequisites

- GNU make, gcc (C), `ar`, Linux with `/proc` (for `get_stat` / `lineage`).
- glibc headers (`unistd`, `sys/*`, etc.).
- Public unit **atomic** headers installed to `~/cmn` before compiling
  `sysprt.c`.

## Direct public-unit dependencies

| Unit | Kind | Evidence |
|------|------|----------|
| [atomic](https://github.com/smythereen/atomic) | headers at compile time | `sysprt.c` includes `<atomic_32.h>`; `UNIT.deps` has `require atomic`; library builds only after atomic headers are in `~/cmn` |

**Not a dependency:** [mio](https://github.com/smythereen/mio). `nm` on
`libsys.a` shows no mio / `Mem_` / `stamp_l` symbols. The library links with
only libc undefineds. A consumer links with `-lsys` alone.

## Indirect dependencies

None beyond what **atomic** needs at compile time (`common.h` types — the
usual sys↔atomic header cycle; fetch order installs both before compile).

```
sys ──(headers)──► atomic
 ▲                    │
 └──── common.h ◄─────┘   (atomic headers include <common.h>)
```

## Usage examples (adapted from unit tests / verified)

Mutable buffer required for `Trim` (it writes in place):

```c
#include <common.h>
#include <stdio.h>

int main(void) {
	char s[] = "  hi  ";
	char buf[64];
	printf("trim='%s' path='%s'\n", Trim(s), Get_Path(buf, sizeof buf, "/tmp"));
	return 0;
}
```

```sh
gcc -o use_sys use_sys.c -I$HOME/cmn -L$HOME/lib -lsys
./use_sys
# trim='hi' path='/tmp/'
```

Link without `-lsys` fails with undefined `Trim` / `Get_Path`.

## Version

Release tag: `v0.2.0` (documentation update on the same tag line; version
number unchanged).

## License

The original files are offered under `Apache-2.0 OR GPL-2.0-or-later`;
you choose either license. See `LICENSE` and `LICENSES/`.

Copyright notices are retained. Third-party-derived files keep their own
licenses; `atomic_32.h` (from the atomic unit) is `GPL-2.0-only`.

## Defects (reported, not changed)

- `bitops.h` includes `<linux/config.h>` (absent on modern distros) and has
  no implementing `.c` in this public tree.
- `port.h` declares RS-232 helpers with no `port.c` implementation here.
