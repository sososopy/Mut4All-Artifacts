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

pub struct Introduce_Negative_Impl_For_Pointer_684;

impl Mutator for Introduce_Negative_Impl_For_Pointer_684 {
    fn name(&self) -> &str {
        "Introduce_Negative_Impl_For_Pointer_684"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut existing_structs = HashSet::new();
        for item in &file.items {
            if let Item::Struct(ItemStruct { ident, .. }) = item {
                existing_structs.insert(ident.clone());
            }
        }

        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if path.segments.last().unwrap().ident == "AutoTrait" {
                        if let Type::Path(TypePath { path: impl_path, .. }) = &*item_impl.self_ty {
                            if let Some(last_segment) = impl_path.segments.last() {
                                if existing_structs.contains(&last_segment.ident) {
                                    let neg_impl: Item = parse_quote! {
                                        impl !AutoTrait for *const #impl_path {}
                                    };
                                    file.items.push(neg_impl);
                                    break;
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