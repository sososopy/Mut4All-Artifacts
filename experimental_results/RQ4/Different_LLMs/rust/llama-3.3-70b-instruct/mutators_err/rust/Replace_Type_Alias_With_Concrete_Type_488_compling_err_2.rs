use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
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

pub struct Replace_Type_Alias_With_Concrete_Type_488;

impl Mutator for Replace_Type_Alias_With_Concrete_Type_488 {
    fn name(&self) -> &str {
        "Replace_Type_Alias_With_Concrete_Type_488"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Type(item_type) = item {
                if let Type::Path(type_path) = &*item_type.ty {
                    if let Some(segment) = type_path.path.segments.last() {
                        if let Some(alias) = get_type_alias(segment.ident.to_string(), file) {
                            let concrete_type = get_concrete_type(alias);
                            item_type.ty = parse_quote! { #concrete_type };
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces type aliases with concrete types in the seed program. It is designed to trigger bugs related to type alias impl trait. The operator applies to type aliases used in const generic parameters. To apply this mutation operator, first identify all type aliases in the seed program, then replace each type alias with a concrete type that implements the same traits as the type alias."
    }
}

fn get_type_alias(name: String, file: &syn::File) -> Option<String> {
    for item in &file.items {
        if let syn::Item::Type(item_type) = item {
            if item_type.ident.to_string() == name {
                if let Type::ImplTrait(type_impl_trait) = &*item_type.ty {
                    return Some(type_impl_trait.bounds.iter().map(|bound| bound.to_token_stream().to_string()).collect::<Vec<String>>().join(","));
                }
            }
        }
    }
    None
}

fn get_concrete_type(alias: String) -> syn::Type {
    let mut rng = thread_rng();
    let concrete_types = vec![parse_quote! { i32 }, parse_quote! { String }, parse_quote! { Vec<i32> }];
    let concrete_type = concrete_types.choose(&mut rng).unwrap();
    concrete_type.clone()
}