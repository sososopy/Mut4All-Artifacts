import os
import requests
import re
import json
import subprocess
import shutil
import logging
import argparse
import time

logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')
PROXY = "http://127.0.0.1:7890"

# GPT API KEY
OPENAI_API_KEY = ""
MAX_ITERATIONS = 10  
INPUT_FILE_PATH = ""
COMPILE_TEMPLATE = ""
COMPILE_FILE_PATH = ""
COMPILE_HOST = ""
COMPILE_PATH = ""
OUTPUT_JSON_PATH = ""
MUTATOR_ERR_PATH = ""
MUTATOR_OUT_PUT_PATH = ""
LANGUAGE = ""
SUFFIX = ""
COMPLIE_COMMAND = ""
ENV_VARS = ""
SEEDS_DIR = ""
SEEDS_MUTATE_DIR = ""
TOKEN_JSON = ""
MAX_RETRIES = 20

def load_config(config_path, language):
    global OPENAI_API_KEY,SEEDS_DIR,SEEDS_MUTATE_DIR,INPUT_FILE_PATH,COMPILE_TEMPLATE, COMPILE_FILE_PATH, COMPILE_PATH, OUTPUT_JSON_PATH,MUTATOR_ERR_PATH,MUTATOR_OUT_PUT_PATH,SUFFIX,COMPLIE_COMMAND,ENV_VARS,TOKEN_JSON,COMPILE_HOST
    try:
        with open(config_path, 'r', encoding='utf-8') as f:
            config = json.load(f)
        if language not in config:
            raise ValueError(f"Unsupported language: {language}.")
        
        # API_KET is initialized
        OPENAI_API_KEY = config[language]["api_key"]        
        INPUT_FILE_PATH = config[language]["code_output_file"]
        COMPILE_TEMPLATE = config[language]["restorer_test_template_file"]
        COMPILE_FILE_PATH = config[language]["restorer_test_file"]
        COMPILE_PATH = config[language]["restorer_compile_path"]
        COMPILE_HOST = config[language]["restorer_compile_host"]
        OUTPUT_JSON_PATH = config[language]["restorer_output_json"]
        MUTATOR_ERR_PATH = config[language]["restorer_output_mutator_err_path"]
        MUTATOR_OUT_PUT_PATH = config[language]["restorer_output_mutator_path"]
        SEEDS_DIR = config[language]["seeds_dir"]
        SEEDS_MUTATE_DIR = config[language]["seeds_mutate_dir"]
        TOKEN_JSON = config[language]["token_json"]
        LANGUAGE = language
        SUFFIX = config[language]["suffix"]
        COMPLIE_COMMAND = config[language]["compile_command"]
        ENV_VARS = config[language]["env_vars"]

        # Check the correctness of the assignment
        if not all([OPENAI_API_KEY,SEEDS_DIR,SEEDS_MUTATE_DIR,INPUT_FILE_PATH, COMPILE_TEMPLATE,COMPILE_FILE_PATH, COMPILE_PATH, OUTPUT_JSON_PATH,MUTATOR_ERR_PATH,MUTATOR_OUT_PUT_PATH,LANGUAGE,SUFFIX,COMPLIE_COMMAND,ENV_VARS,TOKEN_JSON]):
            raise ValueError("One or more configuration variables are empty. Please check the configuration file.")
        return config
    except FileNotFoundError:
        raise FileNotFoundError(f"The configuration file {config_path} does not exist. Please ensure that the file is located in the specified path.")
    except json.JSONDecodeError:
        raise ValueError(f"The format of the configuration file {config_path} is incorrect. Please check the JSON syntax.")
    
# read the Mutator directory without checking the compilation status
def read_mutator_files(directory):
    mutator_files = []
    for filename in os.listdir(directory):
        filepath = os.path.join(directory, filename)
        if os.path.isfile(filepath) and filename.endswith(SUFFIX):
            with open(filepath, 'r', encoding='utf-8') as f:
                content = f.read().strip()
                mutator_files.append({
                    "filename": filename,
                    "content": content
                })
    return mutator_files

# automated code mutation testing framework
class TestFramework:
    def __init__(self):
        # language specific prompt
        self.prompt_provide = ""
        self.prompt_task = ""
    # read the information of the test master file
    def read_test_file(self, path)->str:
        return ''
    # embed test code
    def embed_mutator_code(self, test_content, mutator_code,mutator_name):
        return ''
    # replace the original test code with the embedded test code
    def write_test_file(self, content):
        pass
    # compile mutator
    def run_compile(self):
        return True, ''
    # save error iteration code
    def save_err_mutator(self, mutator_name, iteration, code):
        iteration_filename = f"{mutator_name}_compling_err_{iteration}.{SUFFIX}"
        iteration_path = os.path.join(MUTATOR_ERR_PATH, iteration_filename)
        with open(iteration_path, 'w', encoding='utf-8') as f:
            f.write(code)
        return iteration_filename
    # save the correct code
    def save_compilable_code(self, file_name, code):
        pass
    # judge mutation effect
    def judge_mutation_effect(self):
        return True
    # replace specific prompt
    def get_unique_prompt(self):
        return self.prompt_provide, self.prompt_task
    
class TestFrameWorkRust(TestFramework):
    def __init__(self):
        super().__init__()
        self.prompt_provide = r""
        self.prompt_task = r"Ensure the revised code can be embedded into a main.rs file between the first and second lines, after initial imports. If panic or error occurs in the main function, it indicates that the mutator has made an error."
    def read_test_file(self, path)->str:
        with open(path, 'r', encoding='utf-8') as f:
            return f.read()
    def embed_mutator_code(self, test_content, mutator_code,mutator_name):
        embedded_content = mutator_code.rstrip() + "\n" + test_content
        embedded_content = embedded_content.replace("{MUTATOR}", mutator_name)
        return embedded_content
    def write_test_file(self, content):
        with open(COMPILE_FILE_PATH, 'w', encoding='utf-8') as f:
            f.write(content)
    def run_compile(self):
        try:
            env = os.environ.copy()
            env.update(ENV_VARS)
            
            result = subprocess.run(
                COMPLIE_COMMAND,
                cwd=COMPILE_PATH,
                capture_output=True,
                text=True,
                encoding='utf-8',
                timeout=30,
                env=env
            )
            if result.returncode == 0:
                return True, ""
            else:
                return False, result.stderr
        except subprocess.TimeoutExpired:
            return False, "compile timed out"
        except KeyError as e:
            return False, f"Missing configuration key: {e} for language {LANGUAGE}"
        except Exception as e:
            return False, f"compile failed: {str(e)}"
    def save_compilable_code(self, file_name, code):
        os.makedirs(MUTATOR_OUT_PUT_PATH, exist_ok=True)
        filename = f"{file_name}"
        filepath = os.path.join(MUTATOR_OUT_PUT_PATH, filename)
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(code)

    def judge_mutation_effect(self):
        mutation_detected = False
        for seed_file in os.listdir(SEEDS_DIR):
            if seed_file.endswith(".rs"):
                base_name = seed_file.replace(".rs", "")
                mutate_file = f"{base_name}_mutation.rs"
                if mutate_file in os.listdir(SEEDS_MUTATE_DIR):
                    seed_path = os.path.join(SEEDS_DIR, seed_file)
                    mutate_path = os.path.join(SEEDS_MUTATE_DIR, mutate_file)
                    with open(seed_path, 'r', encoding='utf-8') as f:
                        seed_content = f.read()
                    with open(mutate_path, 'r', encoding='utf-8') as f:
                        mutate_content = f.read()
                    if len(seed_content) != len(mutate_content):
                        mutation_detected = True
                        break
        return mutation_detected


class TestFrameworkCPP(TestFramework):
    def __init__(self):
        super().__init__()
        self.prompt_provide = r"Please note that this code segment is composed by combining a .h file and a .cpp file, with their respective starting points marked by the comments `//header file` and `//source file`."
        self.prompt_task = r"Is still presented in the combined form of a .h file and a .cpp file, with the starting points clearly marked by the comments `//header file` and `//source file`, respectively. Do not attempt to add or modify any statement that introduce third-party libraries (i.e., the header file inclusion statements in C++); They are confirmed to be error-free."
    def embed_mutator_code(self, test_content, mutator_code,mutator_name):
        self.mutator_no = mutator_name[mutator_name.rfind('_')+1:]
        mutator_code = mutator_code.replace("_"+self.mutator_no, "_1")
        mutator_code = mutator_code.replace(f"ASTCONSUMER({self.mutator_no})", f"ASTCONSUMER(1)")
        code_header = mutator_code[:mutator_code.find(r'//source file')]
        code_source = mutator_code[mutator_code.find(r'//source file'):]
        header_file_declaration = ""
        header_file_pattern = re.search(r'([a-zA-Z0-9_]+)_(\d+)\.h', code_source) 
        if header_file_pattern != None:
            header_file_declaration = header_file_pattern.group()
        code_source = code_source.replace(header_file_declaration, r'mutator_1.h')
        with open('mutator_1.h', 'w',encoding='utf-8') as include:
            include.write(code_header)
        with open('mutator_1.cpp', 'w',encoding='utf-8') as lib:
            lib.write(code_source)
        return mutator_code

    # compile mutator
    def run_compile(self):
        REMOTE_HEADER_FILE_DIR = 'clang-tools-extra/mutation_tool/mutator/include/'
        REMOTE_SOURCE_FILE_DIR = 'clang-tools-extra/mutation_tool/mutator/lib/'
        try:
            subprocess.run(['scp', 'mutator_1.h', f'{COMPILE_HOST}:{COMPILE_PATH + REMOTE_HEADER_FILE_DIR}'],shell=False)
            subprocess.run(['scp', 'mutator_1.cpp', f'{COMPILE_HOST}:{COMPILE_PATH + REMOTE_SOURCE_FILE_DIR}'], shell=False)
            # compile the libtooling toolchain and obtain the results(c++)
            compile_task = subprocess.Popen(['ssh', COMPILE_HOST, f'cd {COMPILE_PATH}build/ && {COMPLIE_COMMAND[0]} {COMPLIE_COMMAND[1]}'],stdout=subprocess.PIPE, stderr=subprocess.PIPE,shell=False)
            stdout, stderr= compile_task.communicate(timeout=100)
            stdout = stdout.decode('utf-8')
            if stdout.find('error') != -1:
                return False, stdout
            else:
                return True,""
        except subprocess.TimeoutExpired:
            return False, "compile timed out"
        except KeyError as e:
            return False, f"Missing configuration key: {e} for language {LANGUAGE}"
        except Exception as e:
            return False, f"compile failed: {str(e)}"
    def save_compilable_code(self, file_name, code):
        filename = f"{file_name}"
        filename = filename.strip()
        headername = filename.replace('.cpp', '.h')
        code = code.replace(r'mutator_1.h', headername)
        # code = code.replace("_1", "_"+self.mutator_no)
        # code = code.replace(f"ASTCONSUMER(1)", f"ASTCONSUMER({self.mutator_no})")
        code_header = code[:code.find(r'//source file')]
        code_source = code[code.find(r'//source file'):]
        os.makedirs(MUTATOR_OUT_PUT_PATH + '\\lib', exist_ok=True)
        os.makedirs(MUTATOR_OUT_PUT_PATH + '\\include', exist_ok=True)
        filepath_header = os.path.join(MUTATOR_OUT_PUT_PATH, 'include', headername)
        filepath_source = os.path.join(MUTATOR_OUT_PUT_PATH, 'lib', filename)
        with open(filepath_header, 'w', encoding='utf-8') as f:
            f.write(code_header)
        with open(filepath_source, 'w', encoding='utf-8') as f:
            f.write(code_source)
        
    def judge_mutation_effect(self):
        try:
            verify_task = subprocess.Popen(['ssh', COMPILE_HOST, f'cd {COMPILE_PATH} && python3 try_verify.py'], stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=False)
            verify_task.wait(timeout=7200)
            stdout, stderr = verify_task.communicate()
            stdout = stdout.decode('utf-8').strip()
            if stdout.isdigit() and int(stdout) > 0:
                return True
            else:
                return False
        except subprocess.TimeoutExpired:
            print("compile timed out")
            return False
        except KeyError as e:
            print(f"Missing configuration key: {e} for language {LANGUAGE}")
            return False
        except Exception as e:
            print(f"compile failed: {str(e)}")
            return False 

# language testing framework(complete it according to this logic)
# class TestFrameworkXXXX(TestFramework):
    # def __init__(self):
    #     super().__init__()
    #     self.prompt_provide = ""
    #     self.prompt_task = ""
    # def embed_mutator_code(self, test_content, mutator_code,mutator_name):
        # ...
    # def run_compile(self):
        # ...
    # def save_compilable_code(self, file_name, code):
        # ...
    # def judge_mutation_effect(self):
        # ...

# refinement
def fix_code(previous_code, compiler_errors, iteration,framework):
    url = "https://api.openai.com/v1/chat/completions"
    headers = {
        "Content-Type": "application/json",
        "Authorization": f"Bearer {OPENAI_API_KEY}"
    }
    prompt_provide, prompt_task = framework.get_unique_prompt()
    prompt_system = f"""
    You are a {LANGUAGE} compiler expert revising a mutator code to fix errors while preserving its mutation logic.
    """
    prompt_user = f"""
    Given the buggy mutator code and the associated compiler error messages, produce a corrected version that resolves the errors without changing the core mutation behavior. {prompt_provide}.
    1. Prioritize fixing the provided compiler errors by identifying their causes (e.g., missing imports, type mismatches) and applying targeted corrections;
    2. Ensure the revised code is syntactically valid {LANGUAGE} and compiles successfully;
    3. Ensure the revised code preserves the mutator's logic for manipulating the AST to potentially induce internal compiler errors;
    {prompt_task}.
    Output only the corrected {LANGUAGE} code in plain text, without any explanations, comments, or Markdown formatting.
    Here are the necessary inputs:
    Previous Mutator Code: {previous_code}.
    Compile Errors: {compiler_errors}.
    """

    data = {
        # The fine-tuned model
        "model": "",
        "messages": [
            {"role": "system", "content": prompt_system},
            {"role": "user", "content": prompt_user}
        ],
        "temperature": 0.2
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
                print("Max retries exceeded. Unable to complete request")
    
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

# save records
def save_record_json(json_path, record):
    records = []
    if os.path.exists(json_path) and os.path.getsize(json_path) > 0:
        try:
            with open(json_path, 'r', encoding='utf-8') as f:
                loaded_data = json.load(f)
                if isinstance(loaded_data, list):
                    records = loaded_data
                else:
                    logging.warning(f"JSON file {json_path} contains a non-list structure: {type(loaded_data)}. Initializing empty records.")
                    records = []
        except json.JSONDecodeError as e:
            logging.warning(f"Invalid JSON in {json_path}: {e}. Initializing empty records.")
            records = []
        except Exception as e:
            logging.error(f"Error reading JSON file {json_path}: {e}. Initializing empty records.")
            records = []
    else:
        logging.info(f"JSON file {json_path} does not exist or is empty. Initializing empty records.")
    
    records.append(record)
    
    os.makedirs(os.path.dirname(json_path), exist_ok=True)
    with open(json_path, 'w', encoding='utf-8') as f:
        json.dump(records, f, indent=4, ensure_ascii=False)

def clear_mutate_dir():
    try:
        if os.path.exists(SEEDS_MUTATE_DIR):
            for item in os.listdir(SEEDS_MUTATE_DIR):
                item_path = os.path.join(SEEDS_MUTATE_DIR, item)
                if os.path.isfile(item_path):
                    os.remove(item_path)
                elif os.path.isdir(item_path):
                    shutil.rmtree(item_path)
            return True
        else:
            return False
    except Exception as e:
        return False
    
# judge mutator restorer
def judge_mutator_restorer(mutator_name):
    with open(TOKEN_JSON, 'r', encoding='utf-8') as f:
        data = json.load(f)
    for record in data:
        for mutator in record.get("mutators", []):
            if mutator.get("name") == mutator_name:
                return mutator.get("mutator_restorer")
    return True

# mark mutator as fixed
def modify_mutator_restorer_in_json(mutator_name):
    with open(TOKEN_JSON, 'r', encoding='utf-8') as f:
        data = json.load(f)
    for record in data:
        for mutator in record.get("mutators", []):
            if mutator.get("name") == mutator_name:
                mutator["mutator_restorer"] = True
                break
        else:
            continue
        break
    
    with open(TOKEN_JSON, 'w', encoding='utf-8') as f:
        json.dump(data, f, indent=4, ensure_ascii=False)

# save token
def save_token_json(token,mutator_name):
    with open(TOKEN_JSON, 'r', encoding='utf-8') as file:
        data = json.load(file)
    if not isinstance(data, list):
        data = [data]
    for item in data:
        if 'mutators' in item:
            for mutator in item['mutators']:
                if mutator['name'] == mutator_name:
                    if 'restorer' not in mutator:
                        mutator['restorer'] = []
                    mutator['restorer'].extend(token)
    with open(TOKEN_JSON, 'w', encoding='utf-8') as file:
        json.dump(data, file, indent=4, ensure_ascii=False)
    
def main(args):
    load_config(args.config, args.language)
    framework = None
    # select the corresponding TestFramework based on the language
    if args.language == 'C++':
        framework = TestFrameworkCPP()
    elif args.language == 'Rust':
        framework = TestFrameWorkRust()
    # elif args.language == 'XXX':
        # framework = TestFrameWorkXXX()
    framework.write_test_file("")
    test_content = framework.read_test_file(COMPILE_TEMPLATE)
    mutator_files = read_mutator_files(INPUT_FILE_PATH)
    for mutator in mutator_files:
        mutator_name = os.path.splitext(mutator["filename"])[0]
        mutator_code = mutator["content"]
        if judge_mutator_restorer(mutator_name) == True:
            continue
        index = 0
        error_attempts = []
        token = []
        compilable = False
        final_filename = mutator["filename"]
        input_token = 0
        output_token = 0
        while index < MAX_ITERATIONS:
            index += 1
            embedded_content = framework.embed_mutator_code(test_content,mutator_code,mutator_name)
            framework.write_test_file(embedded_content)
            success, compiler_errors = framework.run_compile()
            if input_token != 0 and output_token != 0:
                token.append({
                    "input_token" : input_token,
                    "output_token" : output_token
                })
            mutator_err_filename = ""
            if not success:
                mutator_err_filename = framework.save_err_mutator(mutator_name, index, mutator_code)
                error_attempts.append({
                    "iteration": index,
                    "code": mutator_code,
                    "errors": compiler_errors,
                    "file": f"{MUTATOR_ERR_PATH}\\{mutator_err_filename}"
                })

            if success:
                compilable = True
                framework.save_compilable_code(final_filename.lower(),mutator_code)
                break
            else:
                mutator_code,token_usage = fix_code(mutator_code, compiler_errors, index,framework)
                mutator_code = re.sub(r'^```[a-zA-Z0-9+]+\n(.*)\n```$', r'\1', mutator_code, flags=re.DOTALL)
                mutator_code = mutator_code.strip()
                input_token = token_usage['input_token']
                output_token = token_usage['output_token']
        if input_token != 0 and output_token != 0:
            token.append({
                "input_token" : input_token,
                "output_token" : output_token
            })
        framework.write_test_file("")
        mutation_effect = framework.judge_mutation_effect()
        clear_mutate_dir()
        if compilable:
            record = {
                "mutator_name": mutator_name,
                "compilable": True,
                "mutate_effect" : mutation_effect,
                "error_pairs": [
                    {
                        "error_code_file": attempt["file"],
                        "errors": attempt["errors"]
                    } for attempt in error_attempts
                ],
                "label": "Positive" if mutation_effect else "Negative",
                "file": f"{MUTATOR_OUT_PUT_PATH}\\{final_filename.lower()}"
            }
            modify_mutator_restorer_in_json(mutator_name)
            save_record_json(OUTPUT_JSON_PATH, record)
        save_token_json(token,mutator_name)
        
    print(f"Processed all mutators in {MUTATOR_OUT_PUT_PATH}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate mutator code for a specific language.")
    parser.add_argument("--language", default="Rust", help="Language to process (Rust, C++).")
    parser.add_argument("--config", default="config.json", help="The absolute path of the configuration file.")
    args = parser.parse_args()
    main(args)
 