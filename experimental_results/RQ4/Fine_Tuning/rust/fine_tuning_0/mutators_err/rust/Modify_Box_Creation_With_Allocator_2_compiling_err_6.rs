use proc_macro2::{Span, TokenStream};
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
use syn::{
    parse_quote, spanned::Spanned, visit_mut::VisitMut, Expr, ExprCall, ExprPath, File, Item, ItemFn, Stmt,
};

use crate::mutator::Mutator;

pub struct Modify_Box_Creation_With_Allocator_2;

impl Mutator for Modify_Box_Creation_With_Allocator_2 {
    fn name(&self) -> &str {
        "Modify_Box_Creation_With_Allocator_2"
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
            if let Item::Fn(item_fn) = item {
                for stmt in &mut item_fn.block.stmts {
                    if let Stmt::Expr(Expr::Call(expr_call)) = stmt {
                        if let Expr::Path(expr_path) = &*expr_call.func {
                            if expr_path.path.segments.last().unwrap().ident == "new_in" {
                                if let Some(Expr::Reference(ref_expr)) = expr_call.args.last() {
                                    if let Expr::Path(ref_expr_path) = &*ref_expr.expr {
                                        if ref_expr_path.path.segments.last().unwrap().ident == "Global" {
                                            *expr_call.args.last_mut().unwrap() = parse_quote!(&CustomAllocator);
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
        "The mutation operator targets functions that create a Box with a specific allocator. It inserts a custom allocator and modifies the Box creation to use this allocator instead of the standard Global allocator. This transformation tests the compiler's handling of custom allocators and their interaction with Box allocations."
    }
}