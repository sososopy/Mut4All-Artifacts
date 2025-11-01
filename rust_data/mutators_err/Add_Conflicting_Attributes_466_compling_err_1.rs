use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    Attribute, BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct Add_Conflicting_Attributes_466;

impl Mutator for Add_Conflicting_Attributes_466 {
    fn name(&self) -> &str {
        "Add_Conflicting_Attributes_466"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let has_attribute = func.attrs.iter().any(|attr| {
                    let path = &attr.path;
                    path.is_ident("must_use") || path.is_ident("inline") || path.is_ident("deprecated")
                });

                if has_attribute {
                    let new_attr: Attribute = parse_quote!(#[inline(always)]);
                    func.attrs.push(new_attr);
                }
            }
        }

        let mut rng = thread_rng();
        let mut function_names: Vec<Ident> = file.items.iter().filter_map(|item| {
            if let syn::Item::Fn(func) = item {
                Some(func.sig.ident.clone())
            } else {
                None
            }
        }).collect();

        if function_names.len() > 1 {
            function_names.shuffle(&mut rng);
            let caller_name = &function_names[0];
            let callee_name = &function_names[1];

            for item in &mut file.items {
                if let syn::Item::Fn(func) = item {
                    if &func.sig.ident == caller_name {
                        let call_expr: Expr = parse_quote! { #callee_name() };
                        func.block.stmts.push(Stmt::Expr(call_expr));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies functions with existing attributes like #[must_use], #[inline], or #[deprecated] and adds a potentially conflicting #[inline(always)] attribute. It ensures that the function is invoked within the same module to test the compiler's handling of attribute interactions. This transformation aims to reveal unexpected behavior or compiler crashes due to conflicting or redundant attributes."
    }
}