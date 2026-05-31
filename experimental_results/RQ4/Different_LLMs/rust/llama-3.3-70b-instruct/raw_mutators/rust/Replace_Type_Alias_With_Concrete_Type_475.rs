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

pub struct Replace_Type_Alias_With_Concrete_Type_475;

impl Mutator for Replace_Type_Alias_With_Concrete_Type_475 {
    fn name(&self) -> &str {
        "Replace_Type_Alias_With_Concrete_Type_475"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Type(item_type) = item {
                if let Type::ImplTrait(type_impl_trait) = &*item_type.ty {
                    let concrete_type = get_concrete_type(type_impl_trait);
                    item_type.ty = Box::new(concrete_type);
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces type aliases with their concrete types. It aims to test the compiler's handling of type aliases and their interactions with other language features."
    }
}

fn get_concrete_type(type_impl_trait: &TypeImplTrait) -> Type {
    let mut rng = thread_rng();
    let concrete_types = vec![
        Type::Path(TypePath {
            qself: None,
            path: SynPath {
                leading_colon: None,
                segments: Punctuated::from_iter(vec![PathSegment {
                    ident: Ident::new("i32", Span::call_site()),
                    arguments: PathArguments::None,
                }]),
            },
        }),
        Type::Path(TypePath {
            qself: None,
            path: SynPath {
                leading_colon: None,
                segments: Punctuated::from_iter(vec![PathSegment {
                    ident: Ident::new("String", Span::call_site()),
                    arguments: PathArguments::None,
                }]),
            },
        }),
    ];

    let concrete_type = concrete_types.choose(&mut rng).unwrap().clone();
    concrete_type
}