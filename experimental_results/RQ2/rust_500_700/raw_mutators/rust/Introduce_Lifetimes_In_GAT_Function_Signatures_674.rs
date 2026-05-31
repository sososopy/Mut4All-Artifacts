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

pub struct Introduce_Lifetimes_In_GAT_Function_Signatures_674;

impl Mutator for Introduce_Lifetimes_In_GAT_Function_Signatures_674 {
    fn name(&self) -> &str {
        "Introduce_Lifetimes_In_GAT_Function_Signatures_674"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if let ReturnType::Type(_, return_type) = &func.sig.output {
                            if let Type::Path(type_path) = &**return_type {
                                if type_path.path.segments.iter().any(|seg| seg.ident == "AssocType") {
                                    let new_lifetime: Lifetime = parse_quote!('b);
                                    if !func.sig.generics.params.iter().any(|param| matches!(param, GenericParam::Lifetime(_))) {
                                        func.sig.generics.params.push(GenericParam::Lifetime(LifetimeParam::new(new_lifetime.clone())));
                                    }
                                    for input in &mut func.sig.inputs {
                                        if let FnArg::Receiver(receiver) = input {
                                            receiver.reference = Some((token::And::default(), Some(new_lifetime.clone())));
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