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

pub struct Replace_Box_Allocator_With_Global_3;

impl Mutator for Replace_Box_Allocator_With_Global_3 {
    fn name(&self) -> &str {
        "Replace_Box_Allocator_With_Global_3"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_global = false;
        for item in &file.items {
            if let Item::Use(use_tree) = item {
                if let syn::UseTree::Path(p) = &use_tree.tree {
                    let mut segments = Vec::new();
                    let mut current = p;
                    loop {
                        segments.push(current.ident.to_string());
                        match &current.tree {
                            syn::UseTree::Path(next_p) => {
                                current = next_p;
                            }
                            syn::UseTree::Name(n) => {
                                segments.push(n.ident.to_string());
                                break;
                            }
                            _ => break,
                        }
                    }
                    if segments.len() == 3 &&
                       segments[0] == "std" &&
                       segments[1] == "alloc" &&
                       segments[2] == "Global" {
                        has_global = true;
                        break;
                    }
                }
            }
        }
        if !has_global {
            let use_stmt = parse_quote! { use std::alloc::Global; };
            file.items.insert(0, Item::Use(use_stmt));
        }

        struct BoxAllocatorVisitor;
        impl VisitMut for BoxAllocatorVisitor {
            fn visit_expr_call_mut(&mut self, call: &mut ExprCall) {
                if let Expr::Path(ExprPath { path, .. }) = &*call.func {
                    if path.segments.len() >= 2 {
                        let first = &path.segments[0];
                        let last = path.segments.last().unwrap();
                        if first.ident == "Box" && last.ident == "new" {
                            let new_path = parse_quote! { Box::new_in };
                            call.func = Box::new(new_path);
                            let allocator = parse_quote! { Global };
                            call.args.push(allocator);
                        } else if first.ident == "Box" && last.ident == "from" {
                            let new_path = parse_quote! { Box::from_box_in };
                            call.func = Box::new(new_path);
                            let allocator = parse_quote! { Global };
                            call.args.push(allocator);
                        }
                    }
                }
                syn::visit_mut::visit_expr_call_mut(self, call);
            }
        }

        let mut visitor = BoxAllocatorVisitor;
        for item in &mut file.items {
            visitor.visit_item_mut(item);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces all Box allocations (Box::new and Box::from) with Box allocations that use a custom allocator via Box::new_in and Box::from_box_in, respectively. It introduces use std::alloc::Global; if not present and appends Global as the allocator. This stresses the compiler's handling of allocator API invariants, type inference, and memory management semantics."
    }
}