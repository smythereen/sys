# sys

System layer and `common.h` for Linux x86-64 (`SYS_VARIANT=linux-x86_64`, default).

**Requires the base repository** to be installed first (project root with
`BASE`, `rules.mak`, `targets.mak`, and the `repo` unit). Clone base,
then fetch this unit with `repo add sys` or via the project starter list.

## License

This repository is dedicated to the public domain under the Unlicense
(see `LICENSE`).

Individual source files may still carry historical
`SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later` headers from
prior versions. Those headers have **not** been mass-rewritten: treat
them as pending commit-by-commit review. Do not assume every past
revision was dual-licensed; only what is in this tag is published.

## Version

Local tag `v0.1.0` (not published to GitHub in this step).

## Hardware / OS variant

`SYS_VARIANT` default `linux-x86_64`. Alternatives must be set explicitly; the
host is not sniffed. Future variants: sibling repos or `variants/<name>/`.

## Omitted (v0.1.0)

Legacy products that needed `far`/`map`/`sem` at link time are not shipped,
so sys does not `require far` (would cycle with far→sys).
