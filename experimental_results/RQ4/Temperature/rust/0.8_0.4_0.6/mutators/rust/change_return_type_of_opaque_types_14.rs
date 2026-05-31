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

pub struct Change_Return_Type_Of_Opaque_Types_14;

impl Mutator for Change_Return_Type_Of_Opaque_Types_14 {
    fn name(&self) -> &str {
        "Change_Return_Type_Of_Opaque_Types_14"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &mut item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &mut **return_type {
                        let mut rng = thread_rng();
                        let traits = vec!["Sized", "Clone"];
                        let new_trait = traits.choose(&mut rng).unwrap();
                        let new_trait_bound = syn::TypeParamBound::Trait(syn::TraitBound {
                            paren_token: None,
                            modifier: syn::TraitBoundModifier::None,
                            lifetimes: None,
                            path: syn::Path {
                                leading_colon: None,
                                segments: {
                                    let mut segs = Punctuated::new();
                                    segs.push(syn::PathSegment {
                                        ident: Ident::new(new_trait, Span::call_site()),
                                        arguments: syn::PathArguments::None,
                                    });
                                    segs
                                },
                            },
                        });
                        type_impl_trait.bounds.clear();
                        type_impl_trait.bounds.push(new_trait_bound);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with `impl Trait` return types, swapping the trait with another compatible one, such as `Sized` or `Clone`. This tests the compiler's handling of opaque types and trait substitution across different configurations, potentially exposing type system bugs."
    }
}