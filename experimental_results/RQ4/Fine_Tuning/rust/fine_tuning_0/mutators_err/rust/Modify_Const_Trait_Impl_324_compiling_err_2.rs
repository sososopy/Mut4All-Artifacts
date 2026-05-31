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

pub struct Modify_Const_Trait_Impl_324;

impl Mutator for Modify_Const_Trait_Impl_324 {
    fn name(&self) -> &str {
        "Modify_Const_Trait_Impl_324"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                if !item_trait.attrs.iter().any(|attr| attr.path().is_ident("const_trait")) {
                    item_trait.attrs.push(parse_quote!(#[const_trait]));
                }
            }

            if let Item::Impl(item_impl) = item {
                if item_impl.generics.constness.is_some() {
                    let mut has_non_const_method = false;

                    for impl_item in &mut item_impl.items {
                        if let ImplItem::Fn(method) = impl_item {
                            if !method.sig.inputs.iter().any(|arg| matches!(arg, FnArg::Receiver(_))) {
                                has_non_const_method = true;
                                break;
                            }
                        }
                    }

                    if has_non_const_method {
                        let new_method: ImplItemFn = parse_quote! {
                            fn new_method<A: ~const MyTrait>(&self) -> i32 {
                                42
                            }
                        };
                        item_impl.items.push(ImplItem::Fn(new_method));
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        ""
    }
}