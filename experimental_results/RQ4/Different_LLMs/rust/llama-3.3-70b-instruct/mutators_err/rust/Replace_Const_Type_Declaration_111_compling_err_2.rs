use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
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
    ItemConst,
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

pub struct Replace_Const_Type_Declaration_111;

impl Mutator for Replace_Const_Type_Declaration_111 {
    fn name(&self) -> &str {
        "Replace_Const_Type_Declaration_111"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut primitive_types: Vec<Type> = vec![
            parse_quote!(i32),
            parse_quote!(u32),
            parse_quote!(bool),
            parse_quote!(char),
        ];

        for item in &mut file.items {
            if let Item::Const(const_item) = item {
                if let Some(const_type) = &const_item.ty {
                    let new_type = primitive_types.choose(&mut thread_rng()).unwrap().clone();
                    const_item.ty = Some(Box::new(new_type));
                } else {
                    let new_type = primitive_types.choose(&mut thread_rng()).unwrap().clone();
                    const_item.ty = Some(Box::new(new_type));
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces the type of constant declarations with a random primitive type. This transformation aims to test the compiler's handling of const generics and type inference, potentially leading to bugs related to type checking and const evaluation."
    }
}