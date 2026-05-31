impl Mutator for Inject_Typo_In_Function_Call_232 {
    fn name(&self) -> &str {
        "Inject_Typo_In_Function_Call_232"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr) = stmt {
                        if let Expr::Call(ExprCall { func, .. }) = expr {
                            if let Expr::Path(ExprPath { path, .. }) = **func {
                                let segments = &path.segments;
                                if let Some(segment) = segments.iter().last() {
                                    let ident = &segment.ident;
                                    let new_ident = self.generate_typo(ident);
                                    let new_path = Path {
                                        leading_colon: path.leading_colon,
                                        segments: Punctuated::from_iter(
                                            segments.iter().take(segments.len() - 1).map(|s| s.clone()),
                                        ),
                                    };
                                    let new_segment = PathSegment {
                                        ident: new_ident,
                                        arguments: segment.arguments.clone(),
                                    };
                                    new_path.segments.push(new_segment);
                                    *func = Box::new(Expr::Path(ExprPath {
                                        attrs: vec![],
                                        qself: None,
                                        path: new_path,
                                    }));
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator injects typos into function calls by replacing the function name with a similar but incorrect name. This transformation aims to trigger parser-related bugs and test the compiler's ability to handle unexpected tokens in the code."
    }
}

impl Inject_Typo_In_Function_Call_232 {
    fn generate_typo(&self, ident: &ProcMacroIdent) -> ProcMacroIdent {
        let mut rng = rand::thread_rng();
        let name = ident.to_string();
        let chars: Vec<char> = name.chars().collect();
        let index = rng.gen_range(0..chars.len());
        let new_char = self.get_random_char();
        let mut new_name = String::new();
        for (i, c) in chars.iter().enumerate() {
            if i == index {
                new_name.push(new_char);
            } else {
                new_name.push(*c);
            }
        }
        ProcMacroIdent::new(&new_name, Span::call_site())
    }

    fn get_random_char(&self) -> char {
        let mut rng = rand::thread_rng();
        let chars = "abcdefghijklmnopqrstuvwxyz";
        let index = rng.gen_range(0..chars.len());
        chars.chars().nth(index).unwrap()
    }
}