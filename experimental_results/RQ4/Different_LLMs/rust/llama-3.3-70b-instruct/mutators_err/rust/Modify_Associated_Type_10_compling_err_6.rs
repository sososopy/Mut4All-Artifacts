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

pub struct Modify_Associated_Type_10;

impl Mutator for Modify_Associated_Type_10 {
    fn name(&self) -> &str {
        "Modify_Associated_Type_10"
    }

    fn mutate(&self, file: &mut syn::File) {
        let replacement_type = get_replacement_type(file);
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Type(associated_type) = impl_item {
                        associated_type.ty = replacement_type.clone();
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets associated types in trait implementations and replaces them with a compatible type from the program's scope. This transformation aims to test the compiler's handling of associated type resolution and may uncover bugs related to type normalization and inference."
    }
}

fn get_replacement_type(file: &syn::File) -> syn::Type {
    let mut types: Vec<syn::Type> = Vec::new();
    for item in &file.items {
        if let syn::Item::Struct(item_struct) = item {
            types.push(syn::Type::Path(TypePath {
                qself: None,
                path: item_struct.ident.clone().into(),
            }));
        } else if let syn::Item::Enum(item_enum) = item {
            types.push(syn::Type::Path(TypePath {
                qself: None,
                path: item_enum.ident.clone().into(),
            }));
        }
    }
    if types.is_empty() {
        return syn::Type::Path(TypePath {
            qself: None,
            path: parse_quote!(::i32).into(),
        });
    }
    let mut rng = thread_rng();
    types.choose(&mut rng).unwrap().clone()
}