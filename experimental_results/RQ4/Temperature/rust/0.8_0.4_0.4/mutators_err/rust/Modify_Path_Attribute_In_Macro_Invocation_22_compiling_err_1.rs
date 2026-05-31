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

pub struct Modify_Path_Attribute_In_Macro_Invocation_22;

impl Mutator for Modify_Path_Attribute_In_Macro_Invocation_22 {
    fn name(&self) -> &str {
        "Modify_Path_Attribute_In_Macro_Invocation_22"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(item_macro) = item {
                if let Some((_, ref mut tokens)) = item_macro.mac.tokens.into_iter().next() {
                    if let Ok(mut expr_path) = syn::parse2::<ExprPath>(tokens.clone()) {
                        if let Some(last_segment) = expr_path.path.segments.last_mut() {
                            last_segment.ident = Ident::new("non_existent_path", Span::call_site());
                            *tokens = quote! { #expr_path };
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}