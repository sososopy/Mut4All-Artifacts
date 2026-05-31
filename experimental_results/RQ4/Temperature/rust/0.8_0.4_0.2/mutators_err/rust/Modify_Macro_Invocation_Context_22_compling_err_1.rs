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

pub struct Modify_Macro_Invocation_Context_22;

impl Mutator for Modify_Macro_Invocation_Context_22 {
    fn name(&self) -> &str {
        "Modify_Macro_Invocation_Context_22"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Macro(item_macro) = item {
                if let Some((_, ref mut tokens)) = item_macro.mac.tokens.clone().into_iter().next() {
                    if let Ok(path) = syn::parse2::<ExprPath>(tokens.clone()) {
                        let new_expr: Expr = parse_quote! {
                            {
                                #path;
                            }
                        };
                        *tokens = quote! { #new_expr };
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets macro invocations used as attribute paths, transforming them into a more standard expression context. This change tests the compiler's handling of macro expansion and scope resolution by shifting the macro's output from an attribute context to a function body context."
    }
}