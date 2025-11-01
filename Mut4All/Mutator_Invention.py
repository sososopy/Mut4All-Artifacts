import os
import requests
import argparse
import json
import time

PROXY = "http://127.0.0.1:7890"

# GPT API KEY
OPENAI_API_KEY = ""
INPUT_FILE = ""
BUG_LABELS_PATH = ""
LANGUAGE = ""
OUTPUT_FILE = ""
TOKEN_JSON = ""
MAX_RETRIES = 20

# load
def load_config(config_path, language):
    global INPUT_FILE, BUG_LABELS_PATH, LANGUAGE, OUTPUT_FILE,TOKEN_JSON,OPENAI_API_KEY
    try:
        with open(config_path, 'r', encoding='utf-8') as f:
            config = json.load(f)
        if language not in config:
            raise ValueError(f"Unsupported language: {language}.")
        
        # API_KET is initialized
        OPENAI_API_KEY = config[language]["api_key"]
        INPUT_FILE = config[language]["bug-labels_name"]
        BUG_LABELS_PATH = config[language]["bug_labels_file"]
        LANGUAGE = language
        OUTPUT_FILE = config[language]["mutation_output_file"]
        TOKEN_JSON = config[language]["token_json"]
        # check the correctness of the assignment
        if not all([INPUT_FILE, BUG_LABELS_PATH, LANGUAGE, OUTPUT_FILE,TOKEN_JSON,OPENAI_API_KEY]):
            raise ValueError("One or more configuration variables are empty. Please check the configuration file.")
        return config
    except FileNotFoundError:
        raise FileNotFoundError(f"The configuration file {config_path} does not exist. Please ensure that the file is located in the specified path.")
    except json.JSONDecodeError:
        raise ValueError(f"The format of the configuration file {config_path} is incorrect. Please check the JSON syntax.")


def extract_Tag(line):
    return line.strip() or None

def send_to_gpt(Tag, file_content):
    url = "https://api.openai.com/v1/chat/completions"

    headers = {
        "Content-Type": "application/json",
        "Authorization": f"Bearer {OPENAI_API_KEY}"
    }

    prompt_system = f"""
    You are an expert in {LANGUAGE} compiler testing.
    """

    prompt_user = f"""
    Here is a bug report for the {LANGUAGE} compiler labeled by {Tag}. Summarize the structures that {LANGUAGE} compiler bugs are more likely to trigger, and design mutation operators that are likely to expose such bugs.
    1. Target a specific code construct and clearly state where it applies;
    2. Customized data names, types, and variable names in bug messages are not allowed in the description and examples of the mutation operator, which should be widely adapted to seed programs;
    3. Achieve high coverage across different program structures and features;
    4. Use examples of pseudo-code form or natural language descriptions to illustrate mutated content to avoid undeclared variables or types;
    5. Note that preference is given to using variables, types, or functions that already exist in the seed program. If new elements are introduced, specify how they should be declared, avoiding ambiguous terms such as 'complex' or 'invalid'; Clearly outline what changes need to be made;
    The output must be plain text. Do not use the Markdown format. Design exactly one mutation operator per request. The beginning of each mutation starts with a mutation operator.
    Here are the contents of the bug report: {file_content}.
    """
    data = {
        # select model
        "model": "",
        "messages": [
            {"role": "system", "content": prompt_system},
            {"role": "user", "content": prompt_user}
        ],
        "temperature": 0.8
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

    # response = requests.post(url, headers=headers, json=data, proxies=proxies)
    response_data = response.json()

    if "choices" in response_data:
        mutation_description = response_data['choices'][0]['message']['content']
        token_usage = {
            "prompt_tokens": response_data.get("usage", {}).get("prompt_tokens", 0),
            "completion_tokens": response_data.get("usage", {}).get("completion_tokens", 0),
        }
        # print({token_usage['prompt_tokens']})
        # print({token_usage['completion_tokens']})
        return mutation_description,token_usage
    else:
        error_msg = f"Error: {response_data}."
        return error_msg, {"error": "No choices in response."}

# save mutation description
def save_mutation_description(Tag, original_filename, mutation_description):
    output_directory = OUTPUT_FILE
    
    if not os.path.exists(output_directory):
        os.makedirs(output_directory)

    output_filename = f"{Tag}_{original_filename}"
    file_path = os.path.join(output_directory, output_filename)
    
    with open(file_path, 'w', encoding='utf-8') as file:
        file.write(mutation_description)
    
    print(f"Mutation description saved to {file_path}.")

# save token
def save_token(input_token,output_token,output_filename):
    token_data = {
        "mutation": output_filename,
        "input": input_token,
        "output_token": output_token,
        "mutation_to_code": False
    }
    if os.path.exists(TOKEN_JSON):
        try:
            with open(TOKEN_JSON, 'r', encoding='utf-8') as f:
                existing_data = json.load(f)
        except json.JSONDecodeError:
            existing_data = []
    else:
        existing_data = []
    
    if not isinstance(existing_data, list):
        existing_data = [existing_data]
    existing_data.append(token_data)
    with open(TOKEN_JSON, 'w', encoding='utf-8') as f:
        json.dump(existing_data, f, indent=4, ensure_ascii=False)

def process_txt_files(Tag,Bug_labels_file_path):
    if os.path.exists(Bug_labels_file_path):
        for filename in os.listdir(Bug_labels_file_path):
            if filename.endswith('.txt'):
                file_path = os.path.join(Bug_labels_file_path, filename)
                output_filename = f"{Tag}_{filename}"
                output_file_path = os.path.join(OUTPUT_FILE, output_filename)
                if os.path.exists(output_file_path):
                    print(f"Skip {output_filename}, which already exists in the OUTPUT_FILE folder.")
                    continue
                with open(file_path, 'r', encoding='utf-8') as file:
                    content = file.read().strip()
                    print(f"Sending file {filename} content to GPT-API...")
                    # generate mutation
                    mutation_description,token_usage= send_to_gpt(Tag, content)
                    input_token = token_usage['prompt_tokens']
                    output_token = token_usage['completion_tokens']
                    # save mutation
                    save_mutation_description(Tag, filename, mutation_description)
                    # save token
                    save_token(input_token,output_token,output_filename)
    else:
        print(f"{Bug_labels_file_path} does not exist.")

def main(args):
    load_config(args.config, args.language)
    with open(INPUT_FILE, 'r', encoding='utf-8') as file:
        for line in file:
            Tag = extract_Tag(line)
            if Tag:
                print(f"Processing bug labels: {Tag}.")
                Bug_labels_file_path = f"{BUG_LABELS_PATH}\\{Tag}"
                process_txt_files(Tag, Bug_labels_file_path)
            else:
                print(f"Skipping invalid line: {line.strip()}.")
    print(f"Generated all mutations in {OUTPUT_FILE}.")

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="Process bug reports for a specific language.")
    parser.add_argument("--language", default="Rust", help="LANGUAGE to process (Rust, C++).")
    parser.add_argument("--config", default="config.json", help="The absolute path of the configuration file.")
    args = parser.parse_args()
    main(args)