# clang-format-search

This is an implementation of the algorithm that searches for the clang-format style that minimizes the number of lines of changed code.

## Usage

Before running the search, you need to download [clang-format 6.0.0 or 6.0.1](http://releases.llvm.org/download.html).

### Quick Start

First, put the source code that you want to format by clang-format in `target_code` folder.

Next, run the search by executing the following command.

```
sh run_search.sh
```

The best style is saved to `log/<algorithm name>/best.yml` and `log/<algorithm name>/.clang-format`.
The transition of the evaluation value is saved to `log/<algorithm name>/log.csv`.

### Change Search Algorithm

The search algorithm can be selected from GA, Hill Climbing or Random Search.
The default algorithm is Hill Climbing.
If you want to change the search algorithm, change `optimizer` in `config.yml` to `ga`, `hill_climbing` or `random`.

```
---
optimizer: ga
max_evals: 500
ga_pop_size: 100
ga_tournament_size: 3
...
```

### Change Algorithm Parameter
You can also set the maximum number of evaluations by changing value of `max_evals` in `config.yml`.

When using GA as the search algorithm, you can set the population size and the tournament size by changing value of `ga_pop_size`, `ga_tournament_size`.
These three parameters are only used when using GA as the search algorithm.
