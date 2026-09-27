# Contributing

How to work on this repository as a team member.

[日本語](CONTRIBUTING.ja.md)

## Teams and folders

| Folder | Who works there | Contents |
| --- | --- | --- |
| [`hardware/`](hardware) | Hardware team | CAD, laser-cut files, parts list |
| [`software/`](software) | Software team | ESP32 firmware, later ROS 2 and tools |
| [`drafts/`](drafts) | Everyone | Sketches and early ideas |
| [`docs/`](docs) | Everyone; the maintainer decides | Settled design and open questions |

Every folder has a guide, `README.md` in English and `README.ja.md` in
Japanese, that explains what goes there and how. Read it before you start.
Folder owners are listed in [`.github/CODEOWNERS`](.github/CODEOWNERS);
GitHub asks them to review changes to their folders.

## Getting started

1. Accept the invitation to the repository.
2. Clone it:
   ```sh
   git clone https://github.com/kosuke-satake/esp32-ros2-rover.git
   ```
3. Read [README.md](README.md), [docs/architecture.md](docs/architecture.md)
   and the guide of the folder you will work in.

## Making a change

This is the usual workflow. Hardware members who are new to Git use a
personal branch instead; see the next section.

1. Start from the latest `main`:
   ```sh
   git switch main
   git pull
   ```
2. Create a branch named `<type>/<short-description>`:
   ```sh
   git switch -c feat/chassis-base
   ```
   Types: `feat/` (something new), `fix/` (a correction), `docs/`
   (documents only), `refactor/` (restructuring without changing behaviour),
   `chore/` (settings and maintenance).
3. Commit in small, meaningful steps. Write the subject as a short imperative
   sentence in English, for example "Add chassis base plate". When the reason
   is not obvious, explain it in the body.
4. Push the branch and open a pull request to `main`. Say what changed and
   why. For hardware, add a screenshot or photo.
5. CI must pass, and the folder owner reviews. The maintainer merges.

`main` is protected: never push to it directly.

## Personal branches for hardware members

Hardware members who are new to Git each have one branch that they keep
using, instead of creating a branch for every change:

- `hardware/member-a`
- `hardware/member-b`

The maintainer tells each member which branch is theirs. Only commit to
your own branch.

### Adding files in the browser

No Git installation is needed.

1. Open the repository on GitHub and choose your branch in the branch menu
   at the top left (it shows `main` at first).
2. Open the folder you want to add to, for example `drafts/`,
   `hardware/cad/<part>/` or `hardware/laser-cut/`. Follow the guide
   (`README.ja.md` or `README.md`) of that folder.
3. Choose **Add file** → **Upload files** and drop in your files. To change
   an existing file, upload a file with the same name; it replaces the old
   one.
4. Under **Commit changes**, write a short English message saying what you
   did, for example "Add chassis base sketch".
5. Make sure **Commit directly to the `hardware/member-a` branch** (your
   branch) is selected, then press **Commit changes**.

The browser accepts files up to 25 MB. For larger files, or many files at
once, use [GitHub Desktop](https://desktop.github.com/): clone the
repository, pick your branch under **Current Branch**, commit, then press
**Push origin**.

### Getting your work into `main`

- When a piece of work is ready, open a pull request from your branch to
  `main`: on your branch, choose **Contribute** → **Open pull request**. The
  maintainer can also open it for you.
- After it is merged, keep committing to the same branch. Do not delete it.
- If the pull request says the branch is out of date, press
  **Update branch**.

## Do not commit

- Passwords, API keys or tokens. Wi-Fi credentials go in `secrets.h`, which
  Git ignores.
- Build output, caches and OS files such as `.DS_Store`. `.gitignore`
  already covers the usual ones.
- Files over 50 MB without asking first. GitHub rejects files over 100 MB.

## Language

Code, comments, commit messages, pull requests and documents are in English.
Folder guides and this file also have a Japanese version (`*.ja.md`). When
you change one, update the other in the same pull request.

## Design decisions

[docs/architecture.md](docs/architecture.md) holds the decisions that are
settled. To change one, open an issue or a pull request that explains why;
the maintainer decides. Undecided ideas go to [`drafts/`](drafts) or
[docs/open-questions.md](docs/open-questions.md).

## AI assistants

AI coding assistants are welcome. Their instructions are in
[AGENTS.md](AGENTS.md), and their changes go through the same pull requests
and reviews.
