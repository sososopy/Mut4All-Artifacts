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

pub struct Modify_Box_Allocator_Type_4;

impl Mutator for Modify_Box_Allocator_Type_4 {
    fn name(&self) -> &str {
        "Modify_Box_Allocator_Type_4"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut visitor = BoxAllocatorVisitor::default();
                visitor.visit_block_mut(&mut item_fn.block);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets Box instantiations with a custom allocator and modifies them to use the global allocator. It changes the type parameter to std::alloc::Global and adjusts the new_in method call to new, ensuring compatibility with the default allocation behavior. This transformation explores different allocator paths in the compiler, potentially revealing bugs related to allocator handling and debuginfo generation."
    }
}

#[derive(Default)]
struct BoxAllocatorVisitor {
    allocator_idents: Vec<Ident>,
}

impl VisitMut for BoxAllocatorVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Call(expr_call) = expr {
            if let Expr::Path(expr_path) = &*expr_call.func {
                if expr_path.path.is_ident("Box") {
                    let mut new_args = expr_call.args.clone();
                    new_args.clear();
                    new_args.push(parse_quote!(std::alloc::Global));
                    expr_call.args = new_args;
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }

    fn visit_type_mut(&mut self, ty: &mut Type) {
        if let Type::Path(type_path) = ty {
            if type_path.path.is_ident("Box") {
                if let PathArguments::AngleBracketed(angle_bracketed) =
                    &mut type_path.path.segments.last_mut().unwrap().arguments
                {
                    if angle_bracketed.args.len() == 2 {
                        angle_bracketed.args.pop();
                        angle_bracketed
                            .args
                            .push(parse_quote!(std::alloc::Global));
                    }
                }
            }
        }
        syn::visit_mut::visit_type_mut(self, ty);
    }
}