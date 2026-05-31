use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Modify_Box_Creation_With_Allocator_2;

impl Mutator for Modify_Box_Creation_With_Allocator_2 {
    fn name(&self) -> &str {
        "Modify_Box_Creation_With_Allocator_2"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_custom_allocator = false;
        for item in &file.items {
            if let Item::Struct(item_struct) = item {
                if item_struct.ident == "CustomAllocator" {
                    has_custom_allocator = true;
                    break;
                }
            }
        }
        if !has_custom_allocator {
            let custom_allocator: ItemStruct = parse_quote! {
                struct CustomAllocator;
            };
            file.items.insert(0, Item::Struct(custom_allocator));
            let custom_allocator_impl: Item = parse_quote! {
                unsafe impl std::alloc::GlobalAlloc for CustomAllocator {
                    unsafe fn alloc(&self, layout: std::alloc::Layout) -> *mut u8 {
                        std::alloc::System.alloc(layout)
                    }
                    unsafe fn dealloc(&self, ptr: *mut u8, layout: std::alloc::Layout) {
                        std::alloc::System.dealloc(ptr, layout)
                    }
                }
            };
            file.items.insert(1, custom_allocator_impl);
        }
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut visitor = BoxInReplacer {
                    replaced: false,
                    allocator_ident: Ident::new("CustomAllocator", Span::call_site()),
                };
                visitor.visit_item_fn_mut(item_fn);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions that create a Box with an allocator reference, replacing the standard allocator with a custom allocator implementation. It first checks for the presence of a CustomAllocator struct and its GlobalAlloc implementation, adding them if absent. Then, it modifies the Box creation expression within functions to use the custom allocator. This transformation tests the compiler's handling of non-standard allocators and may expose issues in allocator trait resolution or memory management semantics."
    }
}

struct BoxInReplacer {
    replaced: bool,
    allocator_ident: Ident,
}

impl VisitMut for BoxInReplacer {
    fn visit_expr_mut(&mut self, node: &mut Expr) {
        if self.replaced {
            return;
        }
        if let Expr::Call(expr_call) = node {
            if let Expr::Path(expr_path) = &*expr_call.func {
                if expr_path.path.segments.len() == 1
                    && expr_path.path.segments[0].ident == "Box"
                {
                    if let PathArguments::AngleBracketed(angle_args) =
                        &expr_path.path.segments[0].arguments
                    {
                        if angle_args.args.len() == 1 {
                            if let GenericArgument::Type(ty) = &angle_args.args[0] {
                                if let Type::Path(type_path) = ty {
                                    if type_path.path.segments.len() == 1
                                        && type_path.path.segments[0].ident == "in"
                                    {
                                        if expr_call.args.len() == 2 {
                                            let mut new_expr: Expr = parse_quote! {
                                                Box::new_in(0, &CustomAllocator)
                                            };
                                            if let Expr::Call(new_expr_call) = &mut new_expr {
                                                new_expr_call.args[0] =
                                                    expr_call.args[0].clone();
                                            }
                                            *node = new_expr;
                                            self.replaced = true;
                                            return;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, node);
    }
}