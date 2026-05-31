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

pub struct Alter_Trait_Bound_In_Const_Impl_32;

impl Mutator for Alter_Trait_Bound_In_Const_Impl_32 {
    fn name(&self) -> &str {
        "Alter_Trait_Bound_In_Const_Impl_32"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                if item_impl.constness.is_some() {
                    if let Some((_, path, _)) = &item_impl.trait_ {
                        let mut rng = thread_rng();
                        if rng.gen_bool(0.5) {
                            // Remove const from trait bound
                            item_impl.constness = None;
                        } else {
                            // Introduce an incompatible trait bound
                            let new_trait_bound: TraitBound = parse_quote!(SomeOtherTrait);
                            if let Some(generics) = &mut item_impl.generics.where_clause {
                                generics.predicates.push(parse_quote!(T: #new_trait_bound));
                            } else {
                                item_impl.generics.where_clause = Some(parse_quote!(where T: #new_trait_bound));
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