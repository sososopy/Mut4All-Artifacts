use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemTrait, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
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

pub struct Replace_Trait_Method_Self_With_Mismatched_Arguments_317;

impl Mutator for Replace_Trait_Method_Self_With_Mismatched_Arguments_317 {
    fn name(&self) -> &str {
        "Replace_Trait_Method_Self_With_Mismatched_Arguments_317"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut const_traits = Vec::new();
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                let has_attr = trait_item.attrs.iter().any(|attr| {
                    if let Some(ident) = attr.path().get_ident() {
                        ident == "const_trait"
                    } else {
                        false
                    }
                });
                if has_attr {
                    let mut methods_with_self = Vec::new();
                    for trait_item in &trait_item.items {
                        if let syn::TraitItem::Fn(method) = trait_item {
                            if let Some(first_arg) = method.sig.inputs.first() {
                                if let FnArg::Receiver(receiver) = first_arg {
                                    if receiver.reference.is_some() && receiver.mutability.is_none() {
                                        methods_with_self.push(method.sig.ident.clone());
                                    }
                                }
                            }
                        }
                    }
                    const_traits.push((trait_item.ident.clone(), methods_with_self));
                }
            }
        }

        for item in &mut file.items {
            if let syn::Item::Impl(ref mut item_impl) = item {
                if let Some((_, trait_path, _)) = &item_impl.trait_ {
                    if let Some(syn::Defaultness::Const(_)) = item_impl.defaultness {
                        let trait_name = trait_path.segments.last().unwrap().ident.to_string();
                        for (trait_ident, methods) in &const_traits {
                            if trait_ident == trait_name {
                                for impl_item in &mut item_impl.items {
                                    if let syn::ImplItem::Fn(func) = impl_item {
                                        if methods.contains(&func.sig.ident) {
                                            func.sig.inputs = parse_quote! { (a: u8, b: bool) };
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
        ""
    }
}