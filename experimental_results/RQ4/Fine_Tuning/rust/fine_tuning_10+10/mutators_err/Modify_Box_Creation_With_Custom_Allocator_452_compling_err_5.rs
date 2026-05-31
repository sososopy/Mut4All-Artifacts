use crate::mutator::Mutator;
use syn::{parse_quote, Expr, Item, Stmt};

pub struct Modify_Box_Creation_With_Custom_Allocator_452;

impl Mutator for Modify_Box_Creation_With_Custom_Allocator_452 {
    fn name(&self) -> &str {
        "Modify_Box_Creation_With_Custom_Allocator_452"
    }
    fn mutate(&self, file: &mut syn::File) {
        let custom_allocator_decl: Item = parse_quote! {
            struct CustomAllocator;

            unsafe impl std::alloc::GlobalAlloc for CustomAllocator {
                unsafe fn alloc(&self, layout: std::alloc::Layout) -> *mut u8 {
                    std::alloc::System.alloc(layout)
                }
                unsafe fn dealloc(&self, ptr: *mut u8, layout: std::alloc::Layout) {
                    std::alloc::System.dealloc(ptr, layout)
                }
            }
        };

        file.items.insert(0, custom_allocator_decl);

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if func.sig.ident != "main" {
                    for stmt in &mut func.block.stmts {
                        if let Stmt::Expr(Expr::Call(expr_call)) = stmt {
                            if let Expr::Path(expr_path) = *expr_call.func {
                                if expr_path.path.segments.iter().any(|seg| seg.ident == "Box") {
                                    let args = &mut expr_call.args;
                                    if args.len() == 2 {
                                        if let Some(Expr::Reference(expr_ref)) = args.last() {
                                            if let Expr::Path(path) = *expr_ref.expr {
                                                if path.path.is_ident("std::alloc::Global") {
                                                    let custom_allocator_expr: Expr = parse_quote! { &CustomAllocator };
                                                    args.pop();
                                                    args.push(custom_allocator_expr);
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
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies Box creation expressions that use the standard allocator by replacing them with a custom allocator. It first declares a `CustomAllocator` struct implementing `GlobalAlloc`, then scans for Box allocations using the standard allocator and replaces them with the custom implementation. This tests the compiler's allocator handling and interaction with non-standard allocators, potentially revealing issues in memory management or allocation logic."
    }
}