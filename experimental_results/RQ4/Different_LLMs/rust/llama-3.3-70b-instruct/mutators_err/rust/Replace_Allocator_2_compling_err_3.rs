use syn::{parse_quote, Expr, ExprCall, ExprPath, Stmt};
use rand::thread_rng;
use crate::mutator::Mutator;

struct Replace_Allocator_2;

impl Mutator for Replace_Allocator_2 {
    fn name(&self) -> &str {
        "Replace_Allocator_2"
    }

    fn mutate(&self, file: &mut syn::File) {
        let allocators = vec![
            "std::alloc::Global",
            "std::alloc::System",
            "std::alloc::Allocator",
        ];

        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Call(ExprCall { func, args, .. }) = expr {
                            if let Expr::Path(ExprPath { path, .. }) = *func {
                                if path.is_ident("Box") && path.segments.last().unwrap().ident == "new_in" {
                                    let allocator_index = thread_rng().gen_range(0..allocators.len());
                                    let new_allocator = allocators[allocator_index];
                                    let new_expr = parse_quote! {
                                        Box::new_in(#args[0], #args[1], &#new_allocator)
                                    };
                                    *expr = new_expr;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces the existing allocator with a different one in the code. This transformation aims to test the compiler's handling of different allocators and their effects on memory layout and allocation."
    }
}