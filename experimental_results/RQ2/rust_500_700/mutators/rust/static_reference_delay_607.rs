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

pub struct Static_Reference_Delay_607;

impl Mutator for Static_Reference_Delay_607 {
    fn name(&self) -> &str {
        "Static_Reference_Delay_607"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut new_statics = Vec::new();

        for item in &mut file.items {
            if let syn::Item::Static(static_item) = item {
                if let syn::Expr::Call(ref call_expr) = *static_item.expr {
                    if let syn::Expr::Path(ref path_expr) = *call_expr.func {
                        if path_expr.path.segments.last().unwrap().ident == "const_fn" {
                            let intermediate_ident = Ident::new(
                                &format!("intermediate_{}", static_item.ident),
                                Span::call_site(),
                            );

                            let intermediate_static: syn::Item = parse_quote! {
                                static #intermediate_ident: #static_item.ty = #static_item.expr;
                            };

                            new_statics.push(intermediate_static);

                            static_item.expr = Box::new(parse_quote! { #intermediate_ident });
                        }
                    }
                }
            }
        }

        file.items.extend(new_statics);
    }

    fn chain_of_thought(&self) -> &str {
        "This mutator introduces an intermediate static variable for static initializations that use a `const fn`. By doing so, it delays the reference to the original static, potentially exposing issues with initialization order and static references in the Rust compiler."
    }
}