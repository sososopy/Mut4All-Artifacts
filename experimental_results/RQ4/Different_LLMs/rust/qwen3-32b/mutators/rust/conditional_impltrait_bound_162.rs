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

pub struct Conditional_ImplTrait_Bound_162;

impl Mutator for Conditional_ImplTrait_Bound_162 {
    fn name(&self) -> &str {
        "Conditional_ImplTrait_Bound_162"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut additional_items = Vec::new();
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        if type_impl_trait.bounds.len() == 1 {
                            let original_trait = type_impl_trait.bounds[0].clone();
                            let new_trait_name = if let TypeParamBound::Trait(trait_bound) = &original_trait {
                                if trait_bound.path.is_ident("Debug") {
                                    "Clone"
                                } else if trait_bound.path.is_ident("Clone") {
                                    "Debug"
                                } else {
                                    "Clone"
                                }
                            } else {
                                "Clone"
                            };
                            let new_trait = TraitBound {
                                paren_token: None,
                                modifier: TraitBoundModifier::None,
                                lifetimes: None,
                                path: SynPath {
                                    leading_colon: None,
                                    segments: {
                                        let mut segs = Punctuated::new();
                                        segs.push(PathSegment {
                                            ident: Ident::new(new_trait_name, Span::call_site()),
                                            arguments: PathArguments::None,
                                        });
                                        segs
                                    },
                                },
                            };
                            let mut new_fn = func.clone();
                            new_fn.attrs.clear();
                            new_fn.attrs.push(parse_quote!(#[cfg(mutated)]));
                            if let ReturnType::Type(_, ref mut ty) = new_fn.sig.output {
                                *ty = Box::new(Type::ImplTrait(TypeImplTrait {
                                    impl_token: token::Impl::default(),
                                    bounds: {
                                        let mut bounds = Punctuated::new();
                                        bounds.push(TypeParamBound::Trait(new_trait.clone()));
                                        bounds
                                    },
                                }));
                            }
                            additional_items.push(Item::Fn(new_fn));
                            
                            func.attrs.push(parse_quote!(#[cfg(not(mutated))]));
                            
                            let mut foo_fn = func.clone();
                            foo_fn.attrs.clear();
                            foo_fn.sig.ident = Ident::new("foo", Span::call_site());
                            if let ReturnType::Type(_, ref mut ty) = foo_fn.sig.output {
                                *ty = Box::new(Type::ImplTrait(TypeImplTrait {
                                    impl_token: token::Impl::default(),
                                    bounds: {
                                        let mut bounds = Punctuated::new();
                                        bounds.push(TypeParamBound::Trait(new_trait.clone()));
                                        bounds
                                    },
                                }));
                            }
                            additional_items.push(Item::Fn(foo_fn));
                            
                            let test_fn_name = format!("test_{}", func.sig.ident);
                            let test_fn_ident = Ident::new(&test_fn_name, Span::call_site());
                            let original_fn_ident = func.sig.ident.clone();
                            let test_fn = parse_quote! {
                                fn #test_fn_ident() {
                                    same_output(#original_fn_ident, foo);
                                    same_output(foo, #original_fn_ident);
                                }
                            };
                            additional_items.push(Item::Fn(test_fn));
                        }
                    }
                }
            }
        }
        file.items.extend(additional_items);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces a function returning `impl Trait` with two conditionally compiled versions using `#[cfg]` attributes. It introduces a new function `foo` with a different opaque trait bound and adds a test function that compares the outputs of the original and `foo` using a higher-order function like `same_output`. This forces the compiler to compare opaque types under different trait bounds, increasing the chance of exposing bugs in trait unification and opaque type resolution."
    }
}