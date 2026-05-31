use proc_macro2::Span;
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

pub struct Replace_Impl_Method_Param_Type_With_Different_Type_313;

impl Mutator for Replace_Impl_Method_Param_Type_With_Different_Type_313 {
    fn name(&self) -> &str {
        "Replace_Impl_Method_Param_Type_With_Different_Type_313"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut const_traits = Vec::new();
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                if has_const_trait_attr(trait_item) {
                    let trait_methods = collect_trait_methods(trait_item);
                    const_traits.push(trait_methods);
                }
            }
        }

        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                if let Some((trait_path, ..)) = &item_impl.trait_ {
                    let trait_name = trait_path.segments.last().unwrap().ident.to_string();
                    for const_trait_methods in &const_traits {
                        for impl_item in &mut item_impl.items {
                            if let syn::ImplItem::Fn(method) = impl_item {
                                if const_trait_methods.contains(&method.sig.ident) {
                                    for arg in &mut method.sig.inputs {
                                        if let FnArg::Typed(pat_type) = arg {
                                            let new_type = get_replacement_type();
                                            *pat_type.ty = new_type;
                                        }
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
        "The mutation operator targets const trait implementations and modifies method parameter types to create mismatches with the trait's declaration. This forces the compiler to handle invalid const trait method signatures, potentially exposing bugs in const evaluation and trait resolution, especially in complex generic or const expression contexts."
    }
}

fn has_const_trait_attr(trait_item: &syn::ItemTrait) -> bool {
    trait_item.attrs.iter().any(|attr| {
        if let syn::Meta::Path(path) = attr.meta.clone() {
            path.segments.iter().any(|seg| seg.ident == "const_trait")
        } else {
            false
        }
    })
}

fn collect_trait_methods(trait_item: &syn::ItemTrait) -> Vec<Ident> {
    let mut methods = Vec::new();
    for item in &trait_item.items {
        if let syn::TraitItem::Fn(method) = item {
            methods.push(method.sig.ident.clone());
        }
    }
    methods
}

fn get_replacement_type() -> Type {
    let mut rng = thread_rng();
    let idx = rng.gen_range(0..6);
    match idx {
        0 => parse_quote!(usize),
        1 => parse_quote!(i32),
        2 => parse_quote!(String),
        3 => parse_quote!(i64),
        4 => parse_quote!(u32),
        5 => parse_quote!(f32),
        _ => parse_quote!(usize),
    }
}