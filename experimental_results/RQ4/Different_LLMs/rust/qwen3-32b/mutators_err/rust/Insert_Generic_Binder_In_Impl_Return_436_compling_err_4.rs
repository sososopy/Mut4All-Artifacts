use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, SynPath,
    PathArguments, ReturnType, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, parse_quote,
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

pub struct Insert_Generic_Binder_In_Impl_Return_436;

impl Mutator for Insert_Generic_Binder_In_Impl_Return_436 {
    fn name(&self) -> &str {
        "Insert_Generic_Binder_In_Impl_Return_436"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let generics = &func.sig.generics;
                if generics.params.is_empty() {
                    continue;
                }

                if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        if let Some(first_param) = generics.params.first() {
                            if let GenericParam::Type(type_param) = first_param {
                                let cloned_param = type_param.clone();
                                let mut new_bounds = type_impl_trait.bounds.clone();
                                let lifetime_bound = TypeParamBound::Lifetime(syn::Lifetime::new("'_", Span::call_site()));
                                new_bounds.push(lifetime_bound);

                                let colon_token = <Token![:]>::default();
                                let new_for_impl = TypeForImpl {
                                    for_token: Some(<Token![for]>::default()),
                                    generic_params: Punctuated::from_iter(vec![GenericParam::Type(cloned_param)]),
                                    impl_token: type_impl_trait.impl_token.clone(),
                                    colon_token: colon_token,
                                    bounds: new_bounds,
                                };

                                *return_type = Box::new(Type::ForImpl(new_for_impl));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator inserts a generic binder using an existing type parameter in an impl Trait return type, forming a nested binder scope. This transformation tests the compiler's ability to resolve binder hierarchies and manage bound variable scopes in nested generic contexts, potentially exposing mismanagement in type inference or binder resolution logic."
    }
}