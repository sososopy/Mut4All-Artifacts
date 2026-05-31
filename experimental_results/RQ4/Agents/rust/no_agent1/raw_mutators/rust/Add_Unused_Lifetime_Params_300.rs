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

pub struct Add_Unused_Lifetime_Params_300;

impl Mutator for Add_Unused_Lifetime_Params_300 {
    fn name(&self) -> &str {
        "Add_Unused_Lifetime_Params_300"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let generics = &mut func.sig.generics;
                let lifetime_param: LifetimeParam = parse_quote!('unused);
                generics.params.push(GenericParam::Lifetime(lifetime_param.clone()));
                if let Some(where_clause) = &mut generics.where_clause {
                    where_clause.predicates.push(parse_quote!('unused: 'static));
                } else {
                    generics.where_clause = Some(parse_quote!(where 'unused: 'static));
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let generics = &mut func.sig.generics;
                        let lifetime_param: LifetimeParam = parse_quote!('unused);
                        generics.params.push(GenericParam::Lifetime(lifetime_param.clone()));
                        if let Some(where_clause) = &mut generics.where_clause {
                            where_clause.predicates.push(parse_quote!('unused: 'static));
                        } else {
                            generics.where_clause = Some(parse_quote!(where 'unused: 'static));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds an unused lifetime parameter `'unused` to function and method signatures, along with a `where 'unused: 'static` clause. This transformation introduces redundant lifetime parameters and constraints, potentially provoking issues in the compiler's lifetime resolution and checking mechanisms, especially in complex generic contexts."
    }
}