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

pub struct Replace_Lifetime_Parameter_130;

impl Mutator for Replace_Lifetime_Parameter_130 {
    fn name(&self) -> &str {
        "Replace_Lifetime_Parameter_130"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                if let syn::Generics::None = trait_item.generics {
                    trait_item.generics = syn::Generics::default();
                }
                if let syn::Generics::WhereClause(_) = trait_item.generics {
                    continue;
                }
                let generics = &mut trait_item.generics;
                if let syn::Generics {
                    params: ref mut params,
                    ..
                } = generics {
                    for param in params {
                        if let GenericParam::Lifetime(lifetime_param) = param {
                            let new_lifetime = Lifetime::new(&format!("'{}", thread_rng().r#gen::<char>()), Span::call_site());
                            lifetime_param.lifetime = new_lifetime;
                        }
                    }
                }
            }

            if let syn::Item::Impl(impl_item) = item {
                if let syn::Generics::None = impl_item.generics {
                    impl_item.generics = syn::Generics::default();
                }
                if let syn::Generics::WhereClause(_) = impl_item.generics {
                    continue;
                }
                let generics = &mut impl_item.generics;
                if let syn::Generics {
                    params: ref mut params,
                    ..
                } = generics {
                    for param in params {
                        if let GenericParam::Lifetime(lifetime_param) = param {
                            let new_lifetime = Lifetime::new(&format!("'{}", thread_rng().r#gen::<char>()), Span::call_site());
                            lifetime_param.lifetime = new_lifetime;
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces existing lifetime parameters in trait definitions and implementations with new ones. This transformation tests the compiler's ability to handle changes in lifetime relationships and their implications on associated types and trait implementations."
    }
}