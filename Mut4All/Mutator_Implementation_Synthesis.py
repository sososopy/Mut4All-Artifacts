import os
import requests
import re
import json
import argparse
import time

PROXY = "http://127.0.0.1:7890"

# GPT API KEY
OPENAI_API_KEY = ""
INPUT_FILE = ""
TEMPLATE_PATH = ""
LANGUAGE = ""
OUTPUT_FILE = ""
SUFFIX = ""
TOKEN_JSON = ""
EXAMPLE_DIR_PATH = ""
MAX_RETRIES = 20

# load
def load_config(config_path, language):
    global INPUT_FILE, TEMPLATE_PATH, LANGUAGE, OUTPUT_FILE,SUFFIX,TOKEN_JSON,EXAMPLE_DIR_PATH,OPENAI_API_KEY
    try:
        with open(config_path, 'r', encoding='utf-8') as f:
            config = json.load(f)
        if language not in config:
            raise ValueError(f"Unsupported language: {language}.")
        
        # API_KET is initialized
        OPENAI_API_KEY = config[language]["api_key"]
        INPUT_FILE = config[language]["mutation_output_file"]
        TEMPLATE_PATH = config[language]["mutator_template"]
        EXAMPLE_DIR_PATH = config[language]["mutator_examples"]
        LANGUAGE = language
        OUTPUT_FILE = config[language]["code_output_file"]
        SUFFIX = config[language]["suffix"]
        TOKEN_JSON = config[language]["token_json"]

        # Check the correctness of the assignment
        if not all([INPUT_FILE, TEMPLATE_PATH, LANGUAGE, OUTPUT_FILE,SUFFIX,TOKEN_JSON,EXAMPLE_DIR_PATH,OPENAI_API_KEY]):
            raise ValueError("One or more configuration variables are empty. Please check the configuration file.")
        return config
    except FileNotFoundError:
        raise FileNotFoundError(f"The configuration file {config_path} does not exist. Please ensure that the file is located in the specified path.")
    except json.JSONDecodeError:
        raise ValueError(f"The format of the configuration file {config_path} is incorrect. Please check the JSON syntax.")
    
# read mutation
def read_mutation_suggestions(directory):
    mutation_data = []
    for filename in os.listdir(directory):
        filepath = os.path.join(directory, filename)
        if os.path.isfile(filepath) and filename.endswith(".txt"):
            with open(filepath, 'r', encoding='utf-8') as f:
                content = f.read().strip()
                filename_without_ext = os.path.splitext(filename)[0]
                mutation_data.append({
                    "filename": filename_without_ext,
                    "content": content
                })
    return mutation_data

def split_mutation_operators(content):
    pattern1 = r'Mutation Operator \d+:.*\n'
    pattern3 = r'Mutation Operator'
    pattern2 = r'#### Mutation Operator \d+:.*\n'
    pattern4 = r'Mutator \d+:.*\n'
    pattern5 = r'Mutator'
    pattern6 = r'####Mutator \d+:.*\n'
    matches = list(re.finditer(pattern1, content))
    
    if not matches:
        matches = list(re.finditer(pattern2, content))
    if not matches:
        matches = list(re.finditer(pattern3, content))
    if not matches:
        matches = list(re.finditer(pattern4, content))
    if not matches:
        matches = list(re.finditer(pattern5, content))
    if not matches:
        matches = list(re.finditer(pattern6, content))
    if not matches:
        return []
    operators = []
    for i in range(len(matches)):
        start = matches[i].start()
        end = matches[i + 1].start() if i + 1 < len(matches) else len(content)
        operator_content = content[start:end].strip()
        operators.append(operator_content)
    return operators

# read mutator template
def read_mutator_template(template_path):
    with open(template_path, 'r', encoding='utf-8') as f:
        return f.read()

# generate mutator
def generate_mutator(mutation_content,mutator_index):
    url = "https://api.openai.com/v1/chat/completions"
    headers = {
        "Content-Type": "application/json",
        "Authorization": f"Bearer {OPENAI_API_KEY}"
    }

    template = read_mutator_template(TEMPLATE_PATH)

    # Human-written mutators
    example_files = [file for file in os.listdir(EXAMPLE_DIR_PATH) if file.find('example') != -1]
    examples = [f'Example {no + 1}:\n\n' + read_mutator_template(os.path.join(EXAMPLE_DIR_PATH, file)) for no, file in enumerate(example_files) ]
    example = '\n\n'.join(examples)

    prompt_system =f"""
    You are an expert in {LANGUAGE} compiler development and AST manipulation.
    """
    prompt_user = f"""
    Based on the provided mutator specification and example mutators for the target language, complete the {LANGUAGE} mutator template to implement the intended transformation.
    1. Ensure that the generated code is syntactically valid {LANGUAGE};
    2. Correctly implement the mutation logic as described in the specification;
    3. Follow the structure of the provided template;
    4. Use appropriate {LANGUAGE} APIs for AST manipulation;
    5. Includes all necessary imports and dependencies;
    6. Analyze the provided mutator suggestion and implement only this specific mutator by completing the provided {LANGUAGE} mutator template;
    7. Preserves the template's existing comments and structure;
    8. If the suggestion requires new types, variables, or functions, declare them appropriately;
    9. Provide only the complete {LANGUAGE} code for the mutator, matching the template's style;
    10. Do not wrap the output in Markdown code blocks (e.g., ```{LANGUAGE} or ```). Output only the raw {LANGUAGE} code;
    11. Do not implement the mutator based on the examples provided in mutation_content. Instead, focus on designing and implementing complex mutation effects, ideally creating a mutator that maximizes coverage of the seed program's code;
    12. Design a unique name that can clearly demonstrate the function of the mutator, replace "Mutator_" in the code template. Name words are separated by '_', and the first letter of each word is capitalized (for example : Add_Const_Generics), and attach "_{mutator_index}". Be sure to specify this name separately on the last line at the end of the output text;
    Output only the filled-in {LANGUAGE} code, without additional explanations, comments beyond those in the template, or Markdown code blocks.
    Output only the completed {LANGUAGE} code, without any additional explanations or formatting, ensuring that no comments beyond those in the template or Markdown code blocks are included.
    Here are the necessary inputs:
    Mutator Specification: {mutation_content}.
    Mutator Template for {LANGUAGE} : {template}.
    Human-written {LANGUAGE} mutators: {example}.
    """
    data = {
        # The fine-tuned model
        "model": "",
        "messages": [
            {"role": "system", "content": prompt_system},
            {"role": "user", "content": prompt_user}
        ],
        "temperature": 0.4
    }

    proxies = {
        "http": PROXY,
        "https": PROXY
    }

    for attempt in range(MAX_RETRIES):
        try:
            response = requests.post(url, headers=headers, json=data, proxies=proxies)
            response.raise_for_status()
            break
        except requests.exceptions.RequestException as e:
            print(f"Attempt {attempt + 1} failed: {e}")
            if attempt < MAX_RETRIES - 1:
                time.sleep(7)
            else:
                print("Max retries exceeded. Unable to complete request.")
    response_data = response.json()
    if "choices" in response_data:
        code = response_data['choices'][0]['message']['content']
        code = re.sub(r'^```[a-zA-Z0-9+]+\n(.*)\n```$', r'\1', code, flags=re.DOTALL)
        code = code.strip()
        token_usage = {
            "input_token": response_data.get("usage", {}).get("prompt_tokens", 0),
            "output_token": response_data.get("usage", {}).get("completion_tokens", 0),
        }
        return code,token_usage
    else:
        return f"Error: {response_data}"

def extract_and_remove_suffix(code):
    pattern = r'(.*\n)?([^\n]*)$'
    match = re.search(pattern, code, re.DOTALL)
    if match:
        last_line = match.group(2).strip()
        cleaned_code = code[:match.start(2)].rstrip('\n')
        if not last_line:
            raise ValueError("Last line is empty.")
        return last_line, cleaned_code
    else:
        raise ValueError("No last line found in the code.")

# save raw mutator
def write_mutator_file(output_dir, mutator_code, mutator_name):
    output_path = os.path.join(output_dir, f"{mutator_name}.{SUFFIX}")
    with open(output_path, 'w', encoding='utf-8') as f:
        f.write(mutator_code)

# save token
def save_token(input_token,output_token,mutation_name,mutator_name):
    mutation_filename = f"{mutation_name}.txt"
    new_mutator = {
        "name": mutator_name,
        "input_token": input_token,
        "output_token": output_token,
        "mutator_restorer" : False,
    }
    with open(TOKEN_JSON, 'r', encoding='utf-8') as f:
        existing_data = json.load(f)
    for record in existing_data:
        if record.get("mutation") == mutation_filename:
            if "mutators" not in record:
                record["mutators"] = []
            record["mutators"].append(new_mutator)
            break
    
    with open(TOKEN_JSON, 'w', encoding='utf-8') as f:
        json.dump(existing_data, f, indent=4, ensure_ascii=False)

def modify_mutation_to_code_in_json(mutation_name):
    mutation_filename = f"{mutation_name}.txt"
    with open(TOKEN_JSON, 'r', encoding='utf-8') as f:
        existing_data = json.load(f)
    for record in existing_data:
        if record.get("mutation") == mutation_filename:
            record["mutation_to_code"] = True
            break
    with open(TOKEN_JSON, 'w', encoding='utf-8') as f:
        json.dump(existing_data, f, indent=4, ensure_ascii=False)

def judge_mutation_to_code(mutation_name):
    mutation_filename = f"{mutation_name}.txt"
    with open(TOKEN_JSON, 'r', encoding='utf-8') as f:
        existing_data = json.load(f)
    for record in existing_data:
        if record.get("mutation") == mutation_filename:
            return record.get("mutation_to_code", False)
    return False

def main(args):
    load_config(args.config, args.language)
    mutator_index = 1
    mutation_data_list = read_mutation_suggestions(INPUT_FILE)
    for mutation_data in mutation_data_list:
        mutation_name = next(iter({mutation_data["filename"]}))
        if judge_mutation_to_code(mutation_name):
            print(f"Skip {mutation_name}, because this mutation has already generated the mutator.")
            continue
        mutation_operators = split_mutation_operators(mutation_data["content"])
        for operator_content in mutation_operators:
            code,token_usage = generate_mutator(operator_content, mutator_index)
            # [mutator_name,mutator_code]
            mutator_code = extract_and_remove_suffix(code)
            write_mutator_file(OUTPUT_FILE, mutator_code[1],mutator_code[0])
            mutator_index += 1
            input_token = token_usage['input_token']
            output_token = token_usage['output_token']
            save_token(input_token,output_token,mutation_name,mutator_code[0])
        modify_mutation_to_code_in_json(mutation_name)

    print(f"Generated all mutators in {OUTPUT_FILE}.")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate mutator code for a specific language.")
    parser.add_argument("--language", default="Rust", help="LANGUAGE to process (Rust, C++).")
    parser.add_argument("--config", default="config.json", help="The absolute path of the configuration file.")
    args = parser.parse_args()
    main(args)
