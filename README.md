# Mut4All-Artifacts

This is the project repository of our research paper: Mut4All: Fuzzing Compilers via LLM-Synthesized Mutators Learned from Bug Reports. 

This project documents our Mut4All tool and some key documents. The detailed project layout is shown below.

## Project Layout Overview

Here are the main useful files and directories:

```
|-- Mut4All-Artifacts/
	|-- Mut4All ## An automatic mutator generator
		|-- bug-labels				## Bugs classified by tags
		|-- mutator_examples		## Human handwritten mutator
		|-- mutator_template  		## Mutator template
		|-- Bug-labels.txt			## Bug tags
		|-- config.json             ## Configuration information		
		|-- main.py					## Main
		|-- Mutator_Invention.py    ## Generate mutator description
		|-- Mutator_Implementation_Synthesis.py ## Generate raw mutator
		|-- Mutator_Refinement.py   ## Verify and fix
		|-- token.json				## The record of tokens
		|-- records.json			## The record of valid mutators
	|-- rust_data ## The generated Rust mutator file
		|-- mutator_description 	## Rust mutator description
		|-- raw_mutators 			## Unverified and unfixed Rust mutators
		|-- mutators 				## Rust mutators
		|-- mutators_err			## Incorrect Rust mutators
		|-- records.json			## The record of rust valid mutators
	|-- c++_data ## The generated C++ mutator file
		|-- mutator_description 	## C++ mutator description
		|-- raw_mutators 			## Unverified and unfixed C++ mutators
		|-- mutators 				## C++ mutators
		|-- mutators_err			## Incorrect C++ mutators
		|-- records.json			## The record of c++ valid mutators
	|-- fine_tune
		|-- rust
			|-- rust_fine_tune.jsonl ## Fine-tune the dataset
			|-- rust_fine_tune_*.txt
			|-- rust_fine_tune_*.rs
		|-- c++
			|-- c++_fine_tune.jsonl ## Fine-tune the dataset
			|-- c++_fine_tune_*.txt
			|-- c++_fine_tune_*.cpp
	|-- experimental_results ## Experimental Data Results
		|-- RQ2
			|-- c++_500_700
			|-- rust_500_700
		|-- RQ3
			|-- rust_uncovered_bugs.xlsx
			|-- c++_uncovered_bugs.xlsx
		|-- RQ4
			|-- Agents
				|-- c++
					|-- no_agent1
					|-- no_agent3
				|-- rust
					|-- no_agent1
					|-- no_agent3
			|-- Different_LLMs
				|-- c++
					|-- deepseek-v3.2
					|-- llama-3.3-70b-instruct
					|-- qwen3-32b
				|-- rust
					|-- deepseek-v3.2
					|-- llama-3.3-70b-instruct
					|-- qwen3-32b
			|-- Fine_Tuning
				|-- c++
					|-- fine_tuning_0
					|-- fine_tuning_10+10
					|-- fine_tuning_20+20
				|-- rust
					|-- fine_tuning_0
					|-- fine_tuning_10+10
					|-- fine_tuning_20+20
			|-- Refinement_Rounds
				|-- c++_valid_mutator_by_round.csv
				|-- rust_valid_mutator_by_round.csv
			|-- Seed_Enhancement
				|-- c++.xlsx
				|-- rust.xlsx
			|-- Temperature
				|-- c++
					|-- ...
				|-- rust
					|-- ...
		|-- RQ5
			|-- token.xlsx
		|-- RQ6
			|-- c++_valid_mutators.xlsx
			|-- c++_invalid_mutators.xlsx
			|-- rust _valid_mutators.xlsx
			|-- rust _invalid_mutators.xlsx
```



## Run the Mut4All

- Before using Mut4All, please fill in the `api_key`, `url`, and `model` fields in `config.json`, as shown below:

```json
{
    "api_key": "",
    "url": "",
    "model": ""
}
```

Please make sure that `Bug-labels.txt` contains the tag names of the bug reports you want to use. These tag names should correspond to the folders under `bug-labels/rust/` or `bug-labels/c++/`. Mut4All will load bug reports according to the selected language and the labels listed in this file.


```
python main.py --language <language-name> --config ./config.json
```

> We only offer Rust and C++ in the tool.

## Uncovered Bugs

Table `/experimental_results/RQ3/*_uncovered_bugs.xlsx` records all the bugs reported with the corresponding ISSUE IDs and status. 
