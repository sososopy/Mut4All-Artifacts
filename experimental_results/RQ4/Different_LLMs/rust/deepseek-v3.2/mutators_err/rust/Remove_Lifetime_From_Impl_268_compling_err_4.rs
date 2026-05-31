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

pub struct Remove_Lifetime_From_Impl_268;

impl Mutator for Remove_Lifetime_From_Impl_268 {
    fn name(&self) -> &str {
        "Remove_Lifetime_From_Impl_268"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                let mut lifetime_params: Vec<LifetimeParam> = Vec::new();
                for param in &item_impl.generics.params {
                    if let GenericParam::Lifetime(lifetime_param) = param {
                        lifetime_params.push(lifetime_param.clone());
                    }
                }
                if !lifetime_params.is_empty() {
                    let mut rng = thread_rng();
                    let index = rng.gen_range(0..lifetime_params.len());
                    let removed_lifetime = lifetime_params[index].lifetime.ident.to_string();
                    let mut new_params = Punctuated::new();
                    for param in &item_impl.generics.params {
                        if let GenericParam::Lifetime(lifetime_param) = param {
                            if lifetime_param.lifetime.ident.to_string() != removed_lifetime {
                                new_params.push(param.clone());
                            }
                        } else {
                            new_params.push(param.clone());
                        }
                    }
                    item_impl.generics.params = new_params;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait implementations where a generic lifetime parameter is used in the impl header but is not declared in the impl's generic parameter list. Specifically, it applies to an impl block that references a lifetime parameter (like 'a) within the type it is implementing for, but that lifetime parameter is absent from the angle brackets after the impl keyword. The operator modifies the seed by removing the declaration of a lifetime parameter from the impl's generic parameter list while keeping its usage in the trait implementation type. If the impl already has at least one generic lifetime parameter declared, select one such lifetime parameter and delete its declaration. Ensure the lifetime name remains used within the type after the for keyword. If no lifetime parameters are present in the impl's generic parameter list, this operator does not apply. The change is made only to the impl block; surrounding items are unchanged. For example, if the seed contains 'impl<'a, T> Trait for Type<'a, T>', mutate to 'impl<T> Trait for Type<'a, T>'. If the seed contains 'impl<'a, 'b> Trait for Struct<'a, 'b>', mutate to 'impl<'b> Trait for Struct<'a, 'b>' (removing 'a) or 'impl<'a> Trait for Struct<'a, 'b>' (removing 'b), chosen arbitrarily. Do not introduce new names; use only those already present."
    }
}