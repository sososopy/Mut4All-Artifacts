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

pub struct Replace_Array_Index_With_Trait_Constant_218;

impl Mutator for Replace_Array_Index_With_Trait_Constant_218 {
    fn name(&self) -> &str {
        "Replace_Array_Index_With_Trait_Constant_218"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let mut generic_params = Vec::new();
                for param in &func.sig.generics.params {
                    if let GenericParam::Type(type_param) = param {
                        if !type_param.bounds.is_empty() {
                            generic_params.push(type_param.ident.clone());
                        }
                    }
                }
                if !generic_params.is_empty() {
                    let generic_ident = generic_params[0].clone();
                    let mut visitor = ReplaceIndexVisitor {
                        generic_ident: generic_ident,
                    };
                    visitor.visit_block_mut(&mut func.block);
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                let mut impl_generic_params = Vec::new();
                for param in &impl_item.generics.params {
                    if let GenericParam::Type(type_param) = param {
                        if !type_param.bounds.is_empty() {
                            impl_generic_params.push(type_param.ident.clone());
                        }
                    }
                }
                if !impl_generic_params.is_empty() {
                    let impl_generic_ident = impl_generic_params[0].clone();
                    for impl_item in &mut impl_item.items {
                        if let syn::ImplItem::Fn(method) = impl_item {
                            let mut method_generic_params = Vec::new();
                            for param in &method.sig.generics.params {
                                if let GenericParam::Type(type_param) = param {
                                    if !type_param.bounds.is_empty() {
                                        method_generic_params.push(type_param.ident.clone());
                                    }
                                }
                            }
                            let method_generic_ident = method_generic_params.get(0)
                                .cloned()
                                .unwrap_or(impl_generic_ident.clone());
                            let mut visitor = ReplaceIndexVisitor {
                                generic_ident: method_generic_ident,
                            };
                            visitor.visit_block_mut(&mut method.block);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces array index expressions with a trait's associated constant in generic contexts. It targets array accesses within functions or impls that have a generic parameter with a trait bound declaring an associated constant. This transformation introduces potential out-of-bounds access during MIR optimization, testing the compiler's handling of constant evaluation and trait-based indexing in generic code."
    }
}

struct ReplaceIndexVisitor {
    generic_ident: Ident,
}

impl VisitMut for ReplaceIndexVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Index(expr_index) = expr {
            let ident = self.generic_ident.clone();
            let new_index = parse_quote!(#ident::CONST);
            *expr_index.index = new_index;
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}