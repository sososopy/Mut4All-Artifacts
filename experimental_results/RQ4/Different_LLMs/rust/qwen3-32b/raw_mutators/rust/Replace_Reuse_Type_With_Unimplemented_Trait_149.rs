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

pub struct Replace_Reuse_Type_With_Unimplemented_Trait_149;

impl Mutator for Replace_Reuse_Type_With_Unimplemented_Trait_149 {
    fn name(&self) -> &str {
        "Replace_Reuse_Type_With_Unimplemented_Trait_149"
    }
    fn mutate(&self, file: &mut syn::File) {
        // Collect all user-defined type identifiers (structs, enums, unions)
        let mut existing_types = Vec::new();
        for item in &mut file.items {
            if let syn::Item::Struct(s) = item {
                existing_types.push(s.ident.clone());
            }
            if let syn::Item::Enum(e) = item {
                existing_types.push(e.ident.clone());
            }
            if let syn::Item::Union(u) = item {
                existing_types.push(u.ident.clone());
            }
        }

        // Early return if no types to replace with
        if existing_types.is_empty() {
            return;
        }

        // Traverse all expressions in the file
        struct ExprCollector {
            exprs: Vec<&'static mut Expr>,
        }

        impl VisitMut for ExprCollector {
            fn visit_expr_mut(&mut self, node: &mut Expr) {
                if let Expr::Path(expr_path) = node {
                    // Check if path starts with "reuse" and has a QSelf
                    if let Some(first_segment) = expr_path.path.segments.first() {
                        if first_segment.ident == "reuse" && expr_path.qself.is_some() {
                            self.exprs.push(node);
                        }
                    }
                }
                visit_mut::visit_expr_mut(self, node);
            }
        }

        let mut collector = ExprCollector { exprs: Vec::new() };
        collector.visit_file_mut(file);

        // Apply mutation to collected expressions
        for expr in collector.exprs {
            if let Expr::Path(expr_path) = expr {
                if let Some(qself) = &mut expr_path.qself {
                    let mut rng = thread_rng();
                    if let Some(new_type) = existing_types.choose(&mut rng) {
                        // Replace the type in the QSelf with a random existing type
                        qself.ty = Box::new(parse_quote! { #new_type });
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies 'reuse' delegations and replaces the source type with an existing type that does not implement the required trait. This introduces invalid trait method calls, potentially exposing bugs in the compiler's handling of trait-bound method resolution and delegation logic."
    }
}