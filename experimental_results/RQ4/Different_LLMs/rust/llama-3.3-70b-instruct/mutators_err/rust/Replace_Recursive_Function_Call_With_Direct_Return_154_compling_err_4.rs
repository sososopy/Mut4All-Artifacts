use syn::{parse_quote, Expr, FnArg, ReturnType, Stmt, Type};
use crate::mutator::Mutator;

struct Replace_Recursive_Function_Call_With_Direct_Return_154;

impl Mutator for Replace_Recursive_Function_Call_With_Direct_Return_154 {
    fn name(&self) -> &str {
        "Replace_Recursive_Function_Call_With_Direct_Return_154"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let ReturnType::Type(_, return_type) = &func.sig.output {
                    if let Type::ImplTrait(_) = &**return_type {
                        let mut new_block = func.block.clone();
                        new_block.stmts.clear();
                        for arg in &func.sig.inputs {
                            if let FnArg::Typed(pat_type) = arg {
                                let placeholder_expr: Expr = parse_quote! { #pat_type };
                                new_block.stmts.push(Stmt::Expr(placeholder_expr, None));
                            }
                        }
                        func.block = new_block;
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets recursive functions that return impl Trait, replacing the recursive function call with a direct return of the input value. This mutation is designed to expose bugs related to recursive functions and trait implementations."
    }
}