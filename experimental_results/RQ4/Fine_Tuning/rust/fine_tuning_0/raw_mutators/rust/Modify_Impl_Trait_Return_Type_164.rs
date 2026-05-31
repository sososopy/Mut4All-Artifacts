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

pub struct Modify_Impl_Trait_Return_Type_164;

impl Mutator for Modify_Impl_Trait_Return_Type_164 {
    fn name(&self) -> &str {
        "Modify_Impl_Trait_Return_Type_164"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        let mut new_bounds = Punctuated::new();
                        for bound in &type_impl_trait.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                let new_trait_bound = TraitBound {
                                    path: SynPath {
                                        leading_colon: None,
                                        segments: {
                                            let mut segments = Punctuated::new();
                                            segments.push(PathSegment {
                                                ident: Ident::new("T", Span::call_site()),
                                                arguments: PathArguments::None,
                                            });
                                            segments
                                        },
                                    },
                                    ..trait_bound.clone()
                                };
                                new_bounds.push(TypeParamBound::Trait(new_trait_bound));
                            }
                        }
                        let new_return_type = Type::ImplTrait(TypeImplTrait {
                            impl_token: type_impl_trait.impl_token,
                            bounds: new_bounds,
                        });
                        item_fn.sig.output = ReturnType::Type(Default::default(), Box::new(new_return_type));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets functions with `impl Trait` return types, specifically those with nested trait bounds. It modifies the nested traits to introduce an undeclared generic parameter `T`, creating a type inconsistency. This transformation is designed to challenge the Rust compiler's type checking and trait unification processes by introducing contradictions that can lead to internal compiler errors or unexpected behaviors."
    }
}