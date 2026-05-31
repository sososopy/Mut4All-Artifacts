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

pub struct Replace_Unsized_Iterator_Item_189;

impl Mutator for Replace_Unsized_Iterator_Item_189 {
    fn name(&self) -> &str {
        "Replace_Unsized_Iterator_Item_189"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                for arg in &mut item_fn.sig.inputs {
                    if let FnArg::Typed(PatType { ty, .. }) = arg {
                        if let Type::ImplTrait(TypeImplTrait { bounds, .. }) = &**ty {
                            for bound in bounds.iter() {
                                if let TypeParamBound::Trait(TraitBound { path, .. }) = bound {
                                    if let Some(segment) = path.segments.last() {
                                        if segment.ident == "Iterator" {
                                            if let PathArguments::AngleBracketed(args) = &segment.arguments {
                                                for arg in args.args.iter() {
                                                    if let GenericArgument::AssocType(assoc) = arg {
                                                        if assoc.ident == "Item" {
                                                            if let Some(ty) = &assoc.ty {
                                                                if is_unsized_type(ty) {
                                    let new_type = pick_alternative_unsized_type(ty);
                                    assoc.ty = Some(Box::new(new_type));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets function signatures with generic parameters bound by Iterator<Item = U> where U is an unsized type. It replaces U with another unsized type V (e.g., str -> [u8], [u8] -> dyn core::fmt::Debug) to provoke layout computation errors. This transformation stresses the compiler's handling of unsized types in generic contexts, potentially triggering ICEs or miscompilations related to trait object or slice layout."
    }
}

fn is_unsized_type(ty: &Type) -> bool {
    match ty {
        Type::Slice(_) => true,
        Type::TraitObject(_) => true,
        Type::Path(TypePath { path, .. }) => {
            let last_segment = path.segments.last();
            if let Some(segment) = last_segment {
                segment.ident == "str"
            } else {
                false
            }
        }
        _ => false,
    }
}

fn pick_alternative_unsized_type(original: &Type) -> Type {
    let mut rng = thread_rng();
    let candidates: Vec<Type> = vec![
        parse_quote!([u8]),
        parse_quote!(dyn core::fmt::Debug),
        parse_quote!(str),
    ];
    candidates.choose(&mut rng).unwrap().clone()
}