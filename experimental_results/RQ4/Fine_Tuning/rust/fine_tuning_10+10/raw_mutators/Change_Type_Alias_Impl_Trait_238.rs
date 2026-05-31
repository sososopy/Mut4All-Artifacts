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

pub struct Change_Type_Alias_Impl_Trait_238;

impl Mutator for Change_Type_Alias_Impl_Trait_238 {
    fn name(&self) -> &str {
        "Change_Type_Alias_Impl_Trait_238"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut new_items = Vec::new();
                for stmt in &item_fn.block.stmts {
                    if let Stmt::Item(Item::Type(type_alias)) = stmt {
                        if let Type::ImplTrait(type_impl_trait) = &*type_alias.ty {
                            let concrete_type = match type_impl_trait.bounds.iter().next() {
                                Some(TypeParamBound::Trait(trait_bound)) => {
                                    if trait_bound.path.is_ident("Sized") {
                                        Some(parse_quote!((T,)))
                                    } else {
                                        None
                                    }
                                }
                                _ => None,
                            };
                            if let Some(concrete_type) = concrete_type {
                                let new_type_alias = Item::Type(syn::ItemType {
                                    ty: Box::new(concrete_type),
                                    ..type_alias.clone()
                                });
                                new_items.push(Stmt::Item(Item::Type(new_type_alias)));
                                continue;
                            }
                        }
                    }
                    new_items.push(stmt.clone());
                }
                item_fn.block.stmts = new_items;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets type aliases using `impl Trait` syntax within functions or modules. It replaces `impl Trait` with a concrete type that satisfies the trait, such as replacing `impl Sized` with `(T,)`. This encourages explicit typing and helps uncover bugs related to type inference."
    }
}