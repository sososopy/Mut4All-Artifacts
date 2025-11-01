use proc_macro2::{Span, Ident};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident as SynIdent,
    Item, ItemFn, ItemImpl, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
    ImplItem, TraitItem, Signature,
};

use crate::mutator::Mutator;

pub struct Mismatched_Method_Signature_127;

impl Mutator for Mismatched_Method_Signature_127 {
    fn name(&self) -> &str {
        "Mismatched_Method_Signature_127"
    }
    fn mutate(&self, file: &mut syn::File) {
        let trait_methods = collect_trait_methods(file);
        for item in &mut file.items {
            if let Item::Impl(ItemImpl { trait_, items, .. }) = item {
                if let Some((_, path, _)) = trait_ {
                    for impl_item in items {
                        if let ImplItem::Fn(method) = impl_item {
                            let method_name = &method.sig.ident;
                            if let Some(trait_item) = trait_methods.get(&(path.clone(), method_name.clone())) {
                                if has_self_reference(&trait_item.sig) {
                                    mutate_method_signature(&mut method.sig);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets function implementations within trait implementation blocks. It identifies methods in traits that include a reference to `self` and modifies the corresponding method in the implementation block by removing the `self` reference and replacing it with unrelated parameters, such as primitive types."
    }
}

fn collect_trait_methods(file: &syn::File) -> HashMap<(SynPath, Ident), syn::TraitItemFn> {
    let mut trait_methods = HashMap::new();
    for item in &file.items {
        if let Item::Trait(trait_item) = item {
            let trait_path = SynPath {
                leading_colon: None,
                segments: Punctuated::from_iter(vec![trait_item.ident.clone().into()]),
            };
            for item in &trait_item.items {
                if let TraitItem::Fn(method) = item {
                    trait_methods.insert((trait_path.clone(), method.sig.ident.clone()), method.clone());
                }
            }
        }
    }
    trait_methods
}

fn has_self_reference(sig: &Signature) -> bool {
    sig.inputs.iter().any(|arg| matches!(arg, FnArg::Receiver(_)))
}

fn mutate_method_signature(sig: &mut Signature) {
    sig.inputs.clear();
    sig.inputs.push(parse_quote!(x: u32));
    sig.inputs.push(parse_quote!(y: u32));
}