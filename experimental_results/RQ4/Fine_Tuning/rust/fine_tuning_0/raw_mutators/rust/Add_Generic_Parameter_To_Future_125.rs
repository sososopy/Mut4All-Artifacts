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

pub struct Add_Generic_Parameter_To_Future_125;

impl Mutator for Add_Generic_Parameter_To_Future_125 {
    fn name(&self) -> &str {
        "Add_Generic_Parameter_To_Future_125"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                let mut found_future = false;
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        if let syn::ReturnType::Type(_, ref return_type) = method.sig.output {
                            if let syn::Type::Path(TypePath { path, .. }) = &**return_type {
                                if path.segments.last().unwrap().ident == "Future" {
                                    found_future = true;
                                    let generics = &mut method.sig.generics;
                                    generics.params.push(parse_quote!(T: Default));
                                    if let Some(where_clause) = &mut generics.where_clause {
                                        where_clause.predicates.push(parse_quote!(T: Default));
                                    } else {
                                        generics.where_clause = Some(parse_quote!(where T: Default));
                                    }
                                }
                            }
                        }
                    }
                }
                if found_future {
                    let generics = &mut item_impl.generics;
                    generics.params.push(parse_quote!(T));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait implementations that involve functions returning a `Future` type. It introduces a new generic parameter `T` with a `Default` bound, which is incorporated into the function's return type and the trait implementation's associated types. This mutation aims to test the compiler's handling of complex generic parameter interactions and type inference within asynchronous contexts."
    }
}