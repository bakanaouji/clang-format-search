# clang-format-search

Searching styles that minimize the amount of code change by clang-format.

## Usage

### Quick Start
First, put the source code to format by clang-format in `target_code` folder.

Next, run the search by executing the following command.

```
sh run.sh
```

The best styles are saved to `log/<algorithm name>/best.json`.
The transition of the evaluation value is saved in `log/<algorithm name>/log.csv`.

### Change Search Algorithm

The search algorithm can be selected from GA, Hill Climbing, Random Search.
The default algorithm is Hill Climbing.
If you want to change the search algorithm, change `optimizer` in `config.json` to `ga` or `random`.

```
{
  "optimizer": "ga",
  "max_evals": 500,
  "ga_pop_size": 100,
  "ga_tournament_size": 3
}
```

### Change Algorithm Parameter
You can also set the maximum number of evaluations by changing value of `max_evals` in `config.json`.

When using GA as the search algorithm, you can set the population size and the tournament size by changing value of `ga_pop_size` and `ga_tournament_size`.
These two parameters are only used when using GA as the search algorithm.
