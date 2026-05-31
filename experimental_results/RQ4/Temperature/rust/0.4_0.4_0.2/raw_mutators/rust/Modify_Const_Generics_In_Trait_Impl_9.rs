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

pub struct Modify_Const_Generics_In_Trait_Impl_9;

impl Mutator for Modify_Const_Generics_In_Trait_Impl_9 {
    fn name(&self) -> &str {
        "Modify_Const_Generics_In_Trait_Impl_9"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if path.segments.iter().any(|seg| seg.ident == "ExampleTrait") {
                        for impl_item in &mut item_impl.items {
                            if let syn::ImplItem::Fn(func) = impl_item {
                                if let Some(first_param) = func.sig.generics.params.first_mut() {
                                    match first_param {
                                        GenericParam::Const(const_param) => {
                                            // Mutation 1: Change the const generic value
                                            const_param.ident = Ident::new("M", Span::call_site());

                                            // Mutation 2: Remove the const generic parameter
                                            func.sig.generics.params.clear();

                                            // Mutation 3: Add an additional unused const generic parameter
                                            func.sig.generics.params.push(parse_quote!(const UNUSED: usize));
                                        }
                                        _ => {}
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