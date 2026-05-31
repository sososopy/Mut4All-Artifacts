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

pub struct Mutate_Static_Type_Annotation_65;

impl Mutator for Mutate_Static_Type_Annotation_65 {
    fn name(&self) -> &str {
        "Mutate_Static_Type_Annotation_65"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Static(static_item) = item {
                if static_item.ty.is_some() {
                    static_item.ty = None;
                    static_item.colon_token = None;
                } else {
                    static_item.eq_token = token::Eq { spans: Span::call_site() };
                    static_item.eq_token = token::Eq { spans: Span::call_site() };
                }
                break;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets static item declarations. If the static includes a colon and type annotation, it removes the colon and type, leaving only the identifier and initializer. If the static lacks a type annotation, it duplicates the equals sign, creating a duplicated `=` token. This transformation produces ill-typed static definitions that may trigger validation errors during const-eval, testing the compiler's handling of static item parsing and type checking."
    }
}