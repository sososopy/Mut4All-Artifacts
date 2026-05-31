use syn::{File, Item, Stmt, Expr, PathArguments, PathSegment};
use crate::mutator::Mutator;

struct Replace_Const_Function_Call_With_Non_Const_Call_310;

impl Mutator for Replace_Const_Function_Call_With_Non_Const_Call_310 {
    fn name(&self) -> &str {
        "Replace_Const_Function_Call_With_Non_Const_Call_310"
    }

    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Call(call) = expr {
                            if let Expr::Path(path) = &*call.func {
                                if let Some(const_token) = path.path.segments.first() {
                                    if let PathArguments::AngleBracketed(angle_bracketed) = &const_token.arguments {
                                        if angle_bracketed.args.is_empty() {
                                            // Remove const keyword
                                            let mut new_path = path.path.clone();
                                            if let Some(first_segment) = new_path.segments.first_mut() {
                                                first_segment.arguments = PathArguments::None;
                                            }
                                            call.func = Box::new(Expr::Path(syn::ExprPath {
                                                attrs: vec![],
                                                path: new_path,
                                                qself: None,
                                            }));
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces const function calls with non-const function calls. It targets function calls within the code and removes the const keyword if present, changing the function call from const to non-const. This transformation aims to test the compiler's handling of const functions and closures, potentially triggering bugs in the const evaluation system."
    }
}