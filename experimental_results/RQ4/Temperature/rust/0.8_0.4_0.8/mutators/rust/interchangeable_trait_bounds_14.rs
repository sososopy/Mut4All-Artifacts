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

pub struct Interchangeable_Trait_Bounds_14;

impl Mutator for Interchangeable_Trait_Bounds_14 {
    fn name(&self) -> &str {
        "Interchangeable_Trait_Bounds_14"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let syn::ReturnType::Type(_, ref mut ty) = func.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**ty {
                        let mut rng = thread_rng();
                        let mut bounds = type_impl_trait.bounds.iter().cloned().collect::<Vec<_>>();
                        bounds.shuffle(&mut rng);

                        if bounds.len() > 1 {
                            let new_bound = bounds[1].clone();
                            *ty = Box::new(Type::ImplTrait(TypeImplTrait {
                                impl_token: token::Impl {
                                    span: Span::call_site(),
                                },
                                bounds: {
                                    let mut new_bounds = Punctuated::new();
                                    new_bounds.push(new_bound);
                                    new_bounds
                                },
                            }));
                        }
                    }
                }

                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::ImplTrait(type_impl_trait) = &*pat_type.ty {
                            let mut rng = thread_rng();
                            let mut bounds = type_impl_trait.bounds.iter().cloned().collect::<Vec<_>>();
                            bounds.shuffle(&mut rng);

                            if bounds.len() > 1 {
                                let new_bound = bounds[1].clone();
                                pat_type.ty = Box::new(Type::ImplTrait(TypeImplTrait {
                                    impl_token: token::Impl {
                                        span: Span::call_site(),
                                    },
                                    bounds: {
                                        let mut new_bounds = Punctuated::new();
                                        new_bounds.push(new_bound);
                                        new_bounds
                                    },
                                }));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions with `impl Trait` in their return types or parameters. It randomly swaps the trait bounds with another compatible trait bound, assuming multiple bounds are present. This transformation tests the compiler's handling of interchangeable trait bounds in function signatures, potentially exposing issues in type inference and trait resolution."
    }
}