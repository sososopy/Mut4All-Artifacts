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

pub struct Replace_Yield_With_Block_And_Match_118;

impl Mutator for Replace_Yield_With_Block_And_Match_118 {
    fn name(&self) -> &str {
        "Replace_Yield_With_Block_And_Match_118"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = YieldVisitor {
            mutator: self,
            mutable_bindings: HashSet::new(),
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets yield expressions within generators (coroutines). It wraps each yield expression in a block that introduces a mutable reference variable initialized to 0, yields, then uses a match expression on the dereferenced variable to modify it. This transformation aims to stress the compiler's layout calculation for generator fields, especially when references to sized types may become unsized due to layout computation, potentially triggering assertion failures."
    }
}

struct YieldVisitor<'a> {
    mutator: &'a Replace_Yield_With_Block_And_Match_118,
    mutable_bindings: HashSet<String>,
}

impl<'a> VisitMut for YieldVisitor<'a> {
    fn visit_expr_mut(&mut self, expr: &mut syn::Expr) {
        if let syn::Expr::Yield(yield_expr) = expr {
            let mut existing_mutable_binding_name = None;
            for binding in &self.mutable_bindings {
                if binding.starts_with("x") || binding.starts_with("y") || binding.starts_with("z") {
                    existing_mutable_binding_name = Some(binding.clone());
                    break;
                }
            }
            let var_name = existing_mutable_binding_name.unwrap_or_else(|| {
                let mut rng = thread_rng();
                let names = ["x", "y", "z", "a", "b", "c"];
                let chosen = names.choose(&mut rng).unwrap();
                format!("{}", chosen)
            });
            let var_ident = Ident::new(&var_name, Span::call_site());
            let new_block: syn::ExprBlock = parse_quote! {
                {
                    let mut #var_ident = 0;
                    let #var_ident = &mut #var_ident;
                    yield;
                    *#var_ident += match { *"" }.len() { _ => 0, };
                }
            };
            *expr = syn::Expr::Block(new_block);
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
    fn visit_local_mut(&mut self, local: &mut syn::Local) {
        if let syn::Pat::Ident(pat_ident) = &local.pat {
            if pat_ident.mutability.is_some() {
                self.mutable_bindings.insert(pat_ident.ident.to_string());
            }
        }
        syn::visit_mut::visit_local_mut(self, local);
    }
}