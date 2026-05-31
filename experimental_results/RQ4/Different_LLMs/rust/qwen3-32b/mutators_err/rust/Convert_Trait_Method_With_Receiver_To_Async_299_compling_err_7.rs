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
    *,
};
use syn::{
    AttrStyle, Attribute, FnArg, Item, ItemFn, ItemStruct, ReturnType, Signature, TraitItem,
    TraitItemFn, visit_mut::VisitMut,
    parse_quote,
    token,
};

use crate::mutator::Mutator;

pub struct Convert_Trait_Method_With_Receiver_To_Async_299;

impl Mutator for Convert_Trait_Method_With_Receiver_To_Async_299 {
    fn name(&self) -> &str {
        "Convert_Trait_Method_With_Receiver_To_Async_299"
    }
    fn mutate(&self, file: &mut syn::File) {
        let has_async_feature = file
            .attrs
            .iter()
            .any(|attr| attr.path().is_ident("feature") && attr.path().segments.last().unwrap().ident == "async_fn_in_trait");

        if !has_async_feature {
            let feature_attr = parse_quote! {
                #![feature(async_fn_in_trait)]
            };
            file.attrs.insert(0, feature_attr);
        }

        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for trait_item in &mut trait_item.items {
                    if let TraitItem::Fn(method) = trait_item {
                        if let Some(first_arg) = method.sig.inputs.iter().next() {
                            if let FnArg::Receiver(_) = first_arg {
                                method.sig.inputs = method.sig.inputs.drain(1..).collect();
                                method.sig.asyncness = Some(token::Async {
                                    span: Span::call_site(),
                                });
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