use syn::{File, Item, ItemFn, Block, Stmt, Expr, ExprClosure};
use proc_macro2::Span;
use crate::mutator::Mutator;

struct Replace_Closure_Capture_By_Reference_With_By_Value_43;

impl Mutator for Replace_Closure_Capture_By_Reference_With_By_Value_43 {
    fn name(&self) -> &str {
        "Replace_Closure_Capture_By_Reference_With_By_Value_43"
    }

    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Closure(closure) = expr {
                            if let Some(capture) = &mut closure.capture {
                                if capture.is_move() {
                                    *capture = syn::Capture::ByValue;
                                }
                                for cap in &mut closure.capture {
                                    if let syn::Capture::ByRef(ref_mut) = cap {
                                        *cap = syn::Capture::ByValue;
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
        "The mutation operator replaces closure captures by reference with captures by value. It targets closures that capture variables from their environment and changes the capture mode from reference to value. This transformation aims to test the compiler's handling of closure captures and variable lifetimes, potentially exposing bugs related to capture modes and ownership."
    }
}