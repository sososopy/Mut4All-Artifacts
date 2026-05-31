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

pub struct Replace_Static_Initializer_With_Drop_Block_128;

impl Mutator for Replace_Static_Initializer_With_Drop_Block_128 {
    fn name(&self) -> &str {
        "Replace_Static_Initializer_With_Drop_Block_128"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Static(static_item) = item {
                // Replace static initializer with a block that declares a String variable
                static_item.ty = parse_quote!(());

                let new_expr = parse_quote! {{
                    let y: String;
                    y = String::new();
                }};

                static_item.expr = Box::new(new_expr);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}