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

pub struct Modify_Configuration_Attributes_26;

impl Mutator for Modify_Configuration_Attributes_26 {
    fn name(&self) -> &str {
        "Modify_Configuration_Attributes_26"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                item_fn.attrs.iter_mut().for_each(|attr| {
                    if attr.path().is_ident("cfg") {
                        if let Ok(mut meta) = <Meta as TryInto<MetaList>>::try_into(attr.meta.clone()) {
                            meta.parse_nested_meta(|nested| {
                                if let Ok(Some((name, value))) = nested.value().and_then(|v| {
                                    Ok(v.to_string()
                                        .split_once('=')
                                        .map(|(a, b)| (a.trim(), b.trim())))
                                }) {
                                    if name == "target_os" {
                                        let new_value = "target(128::MAX, true)";
                                        let new_meta: MetaNameValue = parse_quote! {
                                            target = #new_value
                                        };
                                        nested = syn::meta::NestedMeta::Meta(new_meta.into());
                                    }
                                }
                                Ok(())
                            });
                            attr.meta = meta.into();
                        }
                    }
                });
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets `cfg` attributes by replacing `target_os` conditions with non-standard `target` specifications, such as `target(128::MAX, true)`. This transformation introduces unusual configuration scenarios to test the compiler's attribute parsing and evaluation robustness, potentially uncovering issues with how the compiler handles edge-case `cfg` conditions."
    }
}