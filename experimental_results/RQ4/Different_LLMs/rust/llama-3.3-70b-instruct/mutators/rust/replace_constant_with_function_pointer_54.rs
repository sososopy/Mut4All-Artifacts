use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use rand::prelude::IteratorRandom;
use regex::Regex;
use std::{
    collections::HashSet,
    default,
    fs,
    ops::Range,
    panic,
    path::Path,
    process::Command,
};
use syn::{
    BoundLifetimes,
    Expr,
    ExprCall,
    ExprPath,
    File,
    FnArg,
    GenericArgument,
    GenericParam,
    Ident,
    Item,
    ItemFn,
    ItemStruct,
    Lifetime,
    LifetimeParam,
    Local,
    Pat,
    PatType,
    Path as SynPath,
    PathArguments,
    ReturnType,
    Stmt,
    TraitBound,
    TraitBoundModifier,
    Type,
    TypeImplTrait,
    TypeParamBound,
    TypePath,
    parse_quote,
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

pub struct Replace_Constant_With_Function_Pointer_54;

impl Mutator for Replace_Constant_With_Function_Pointer_54 {
    fn name(&self) -> &str {
        "Replace_Constant_With_Function_Pointer_54"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut functions: Vec<syn::ItemFn> = Vec::new();

        // Collect all functions in the file
        for item in &file.items {
            if let Item::Fn(func) = item {
                functions.push(func.clone());
            }
        }

        // Iterate over all items in the file
        for item in &mut file.items {
            if let Item::Const(const_item) = item {
                // Check if the constant is of type Fn
                if let Type::Path(TypePath {
                    qself: None,
                    path: SynPath {
                        segments,
                        ..
                    },
                }) = &*const_item.ty
                {
                    if let Some(segment) = segments.first() {
                        if segment.ident == "Fn" {
                            // Get a random function with the same signature
                            let random_function = functions.choose(&mut thread_rng()).unwrap();

                            // Replace the constant with the random function
                            *const_item.expr = parse_quote! {
                                #random_function
                            };
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces constants of type Fn with a random function from the same scope. This transformation aims to test the compiler's handling of function pointers and constant evaluation."
    }
}