use syn::{parse_quote, Expr, Stmt};
use rand::thread_rng;
use rand::Rng;

struct Change_Break_Value_48;

trait Mutator {
    fn name(&self) -> &str;
    fn mutate(&self, file: &mut syn::File);
    fn chain_of_thought(&self) -> &str;
}

impl Mutator for Change_Break_Value_48 {
    fn name(&self) -> &str {
        "Change_Break_Value_48"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for statement in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = statement {
                        if let Expr::Break(break_expr) = expr {
                            let mut rng = thread_rng();
                            let new_value = match rng.gen_range(0..3) {
                                0 => {
                                    // Change to boolean value
                                    parse_quote! { true }
                                }
                                1 => {
                                    // Change to character value
                                    parse_quote! { 'a' }
                                }
                                2 => {
                                    // Change to integer value
                                    parse_quote! { 201 }
                                }
                                _ => unreachable!(),
                            };
                            *statement = parse_quote! { break #new_value; };
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets break statements in loops and changes their value to a literal of a different type. This transformation helps expose bugs in the const eval component of the Rust compiler related to type checking and constant evaluation."
    }
}