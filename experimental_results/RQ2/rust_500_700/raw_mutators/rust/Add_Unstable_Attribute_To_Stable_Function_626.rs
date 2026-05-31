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

pub struct Add_Unstable_Attribute_To_Stable_Function_626;

impl Mutator for Add_Unstable_Attribute_To_Stable_Function_626 {
    fn name(&self) -> &str {
        "Add_Unstable_Attribute_To_Stable_Function_626"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if func.attrs.iter().any(|attr| attr.path.is_ident("stable")) {
                            let unstable_attr: syn::Attribute = parse_quote! {
                                #[unstable(feature = "conflicting_feature", issue = "none")]
                            };
                            func.attrs.insert(0, unstable_attr);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions within structs or traits that are currently marked as stable. It adds an `#[unstable]` attribute with a non-existent feature name to create a conflict in stability attributes, testing the compiler's handling of stability and feature gating mechanisms."
    }
}