use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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
            if let syn::Item::Fn(func) = item {
                let mut visitor = BoxAllocatorVisitor;
                visitor.visit_item_fn_mut(func);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies Box instantiations with custom allocators and modifies them to use the global allocator. This transformation tests the compiler's handling of allocator APIs and interactions with different allocation strategies."
    }
}

struct BoxAllocatorVisitor;

impl VisitMut for BoxAllocatorVisitor {
    fn visit_expr_call_mut(&mut self, node: &mut ExprCall) {
        if let Expr::Path(ExprPath { ref mut path, .. }) = *node.func {
            if let Some(segment) = path.segments.last_mut() {
                if segment.ident == "new_in" {
                    if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                        if let Some(GenericArgument::Type(ty)) = args.args.last_mut() {
                            *ty = parse_quote!(std::alloc::Global);
                        }
                    }
                    segment.ident = Ident::new("new", segment.ident.span());
                    node.args.pop();
                }
            }
        }
        visit_mut::visit_expr_call_mut(self, node);
    }
}