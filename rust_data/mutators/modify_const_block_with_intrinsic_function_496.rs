use crate::mutator::Mutator;
use syn::{parse_quote, Expr, Local, LocalInit, Stmt};

pub struct Modify_Const_Block_With_Intrinsic_Function_496;

impl Mutator for Modify_Const_Block_With_Intrinsic_Function_496 {
    fn name(&self) -> &str {
        "Modify_Const_Block_With_Intrinsic_Function_496"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Local(Local {
                        init: Some(LocalInit { expr, .. }),
                        ..
                    }) = stmt
                    {
                        if let Expr::Block(ref mut expr_block) = **expr {
                            for stmt in &mut expr_block.block.stmts {
                                if let Stmt::Expr(Expr::Path(expr_path), _) = stmt {
                                    if let Some(last_segment) = expr_path.path.segments.last() {
                                        if last_segment.ident == "transmute" {
                                            let new_expr: Expr = parse_quote! {
                                                core::intrinsics::volatile_load::<u8>
                                            };
                                            *stmt = Stmt::Expr(new_expr, None);
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
        "The mutation operator targets constant blocks within functions that utilize intrinsic functions. It replaces the intrinsic function call, such as `core::mem::transmute`, with another intrinsic function like `core::intrinsics::volatile_load`. The replacement ensures the new intrinsic function has the same input and output types, maintaining type correctness. This transformation tests the compiler's handling of intrinsic function replacements and its ability to manage type consistency within constant blocks."
    }
}