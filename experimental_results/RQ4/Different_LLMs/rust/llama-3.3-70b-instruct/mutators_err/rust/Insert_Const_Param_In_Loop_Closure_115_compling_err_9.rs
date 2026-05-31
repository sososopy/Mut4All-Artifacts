use syn::{parse_quote, Expr, Stmt};
use crate::mutator::Mutator;

struct Insert_Const_Param_In_Loop_Closure_115;

impl Mutator for Insert_Const_Param_In_Loop_Closure_115 {
    fn name(&self) -> &str {
        "Insert_Const_Param_In_Loop_Closure_115"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::For(for_loop) = item {
                for stmt in &mut for_loop.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some(init) = &local.init {
                            if let Expr::Closure(closure) = init {
                                let const_param = parse_quote!(const M: i32);
                                closure.inputs.push(const_param);
                            }
                        }
                    }
                }
            }
            else if let syn::Item::While(while_loop) = item {
                for stmt in &mut while_loop.body.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some(init) = &local.init {
                            if let Expr::Closure(closure) = init {
                                let const_param = parse_quote!(const M: i32);
                                closure.inputs.push(const_param);
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator inserts a const parameter in the closure parameter list of loops with closures. This transformation is designed to test the compiler's ability to handle const generics and closure lifetime binders in loops, potentially triggering bugs related to these features."
    }
}