use proc_macro2::{Span, *};
use quote::*;
use rand::{seq::SliceRandom, thread_rng, Rng};
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

pub struct Mismatched_Method_Signature_In_Trait_And_Impl_317;

impl Mutator for Mismatched_Method_Signature_In_Trait_And_Impl_317 {
    fn name(&self) -> &str {
        "Mismatched_Method_Signature_In_Trait_And_Impl_317"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut target_trait = None;
        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                for item in &item_trait.items {
                    if let TraitItem::Fn(method) = item {
                        if method.sig.receiver().is_some() {
                            target_trait = Some(item_trait.ident.clone());
                            break;
                        }
                    }
                }
            }
        }
        if let Some(target_trait) = target_trait {
            for item in &mut file.items {
                if let Item::Impl(item_impl) = item {
                    if item_impl
                        .trait_
                        .as_ref()
                        .map(|(_, path, _)| path.is_ident(&target_trait))
                        .unwrap_or(false)
                    {
                        let mut target_method = None;
                        for (i, item) in item_impl.items.iter().enumerate() {
                            if let ImplItem::Fn(method) = item {
                                if method.sig.receiver().is_some() {
                                    target_method = Some(i);
                                    break;
                                }
                            }
                        }
                        if let Some(target_method) = target_method {
                            if let ImplItem::Fn(method) = &mut item_impl.items[target_method] {
                                let mut new_inputs = Punctuated::new();
                                let mut rng = thread_rng();
                                let n = rng.gen_range(1..=3);
                                for i in 0..n {
                                    let ty: Type = if i % 2 == 0 {
                                        parse_quote!(u32)
                                    } else {
                                        parse_quote!(i32)
                                    };
                                    new_inputs.push(parse_quote!(x: #ty));
                                }
                                method.sig.inputs = new_inputs;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "Identify a trait with a method that has a `self` receiver, then find an impl block for that trait and replace the corresponding method's signature to remove `self` and add unrelated parameters."
    }
}