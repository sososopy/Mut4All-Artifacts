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
			|-- rust_uncovered_bugs.xlsx
			|-- c++_uncovered_bugs.xlsx
		|-- RQ3
			|-- token.xlsx
		|-- RQ4
			|-- c++_valid_mutators.xlsx
			|-- c++_invalid_mutators.xlsx
			|-- c++_token.json		##c++ tokens
			|-- rust _valid_mutators.xlsx
			|-- rust _invalid_mutators.xlsx
			|-- rust_token.json		##rust tokens
```



## Run the Mut4All

- Before using the Mut4All tool, please ensure that you have entered the api_key in config.json and selected the model you wish to use in Mutator_Invention.py, Mutator_Implementation_Synthesis.py, and Mutator_Refinement.py.

```
python main.py --language <language-name> --config ./config.json
```

> We only offer Rust and C++ in the tool.

## Uncovered Bugs

Table `/experimental_results/RQ2/*_uncovered_bugs.xlsx` records all the bugs reported with the corresponding ISSUE IDs and status. 
