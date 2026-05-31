mod mutator;
fn main() {
    let dir: &str = "./src/seeds";
    let entries = std::fs::read_dir(dir).expect("Unable to read directory");

    // 选择算子
    for entry in entries {
        let entry = entry.expect("Failed to read entry");
        let path = entry.path();
        if path.extension().map(|e| e == "rs").unwrap_or(false) {
            let file_name: &str = path.file_name().unwrap().to_str().unwrap();

            let code = std::fs::read_to_string(&path).expect("Unable to read file");
            // Rust -> AST
            let file_ast_result = syn::parse_str(&code);
            let mut file_ast:syn::File = match file_ast_result {
                Ok(ast) => ast,
                Err(e) => {
                    println!("Failed to parse file '{}': {}", file_name, e);
                    continue;
                }
            };
            {MUTATOR}.mutate(&mut file_ast);
            // AST -> Rust
            let output_code = quote::quote! { #file_ast };
            let mutated_file_name = format!("{}_mutation.rs", file_name.trim_end_matches(".rs"));
            let mutated_code = output_code.to_string();
            let target_dir = "./src/seeds_mutate";
            std::fs::create_dir_all(target_dir).expect("Unable to create target directory");
            let path_mutate: String = mutated_file_name.clone();
            
            // 将变异后的内容写入新文件
            let mutated_file_path = std::path::Path::new(target_dir).join(mutated_file_name);
            std::fs::write(mutated_file_path, mutated_code).expect("Unable to write mutated file");
        }
    }
}

