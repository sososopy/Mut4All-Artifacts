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

pub struct Add_Clone_Bound_To_Traits_610;

impl Mutator for Add_Clone_Bound_To_Traits_610 {
    fn name(&self) -> &str {
        "Add_Clone_Bound_To_Traits_610"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            match item {
                Item::Fn(item_fn) => {
                    if let Some(where_clause) = &mut item_fn.sig.generics.where_clause {
                        where_clause.predicates.push(parse_quote!(Self: Clone));
                        item_fn.block.stmts.push(parse_quote! {
                            let _y = self.clone();
                        });
                    }
                }
                Item::Impl(item_impl) => {
                    if let Some(where_clause) = &mut item_impl.generics.where_clause {
                        where_clause.predicates.push(parse_quote!(Self: Clone));
                    }
                    for impl_item in &mut item_impl.items {
                        if let ImplItem::Fn(method) = impl_item {
                            method.block.stmts.push(parse_quote! {
                                let _y = self.clone();
                            });
                        }
                    }
                }
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}