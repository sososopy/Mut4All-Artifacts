import argparse
import Mutator_Invention
import Mutator_Implementation_Synthesis
import Mutator_Refinement

def main():
    parser = argparse.ArgumentParser(description="Sequentially execute BugToMutation_Generator, MutationToCode_Generator, and Mutator_Restorer")
    parser.add_argument("--language", default="Rust", help="Language to process (Rust, C++)")
    parser.add_argument("--config", default="config.json", help="The absolute path of the configuration file")
    args = parser.parse_args()
    print("Starting BugToMutation_Generator...")
    Mutator_Invention.main(args)
    print("\nStarting MutationToCode_Generator...")
    Mutator_Implementation_Synthesis.main(args)
    print("\nStarting Mutator_Restorer...")
    Mutator_Refinement.main(args)
    print("\nOK.")

if __name__ == "__main__":
    main()