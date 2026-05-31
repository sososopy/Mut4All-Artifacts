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

pub struct Modify_Const_Trait_Impl_With_Lifetime_Annotations_307;

impl Mutator for Modify_Const_Trait_Impl_With_Lifetime_Annotations_307 {
    fn name(&self) -> &str {
        "Modify_Const_Trait_Impl_With_Lifetime_Annotations_307"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.constness.is_some() {
                    if let ReturnType::Type(_, return_type) = &func.sig.output {
                        if let Type::ImplTrait(type_impl_trait) = &**return_type {
                            if type_impl_trait.bounds.iter().any(|bound| {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    trait_bound.path.segments.iter().any(|segment| segment.ident == "Fn")
                                } else {
                                    false
                                }
                            }) {
                                let lifetime = Lifetime::new("'a", Span::call_site());
                                func.sig.generics.params.push(GenericParam::Lifetime(LifetimeParam {
                                    attrs: Vec::new(),
                                    lifetime: lifetime.clone(),
                                    colon_token: None,
                                    bounds: Punctuated::new(),
                                }));

                                let mut new_bounds = type_impl_trait.bounds.clone();
                                new_bounds.push(TypeParamBound::Lifetime(lifetime.clone()));

                                let new_return_type = Type::ImplTrait(TypeImplTrait {
                                    impl_token: type_impl_trait.impl_token,
                                    bounds: new_bounds,
                                });

                                func.sig.output = ReturnType::Type(
                                    token::RArrow {
                                        spans: [Span::call_site(), Span::call_site()],
                                    },
                                    Box::new(Type::ImplTrait(new_return_type)),
                                );

                                if let Some(block) = func.block.as_mut() {
                                    for stmt in &mut block.stmts {
                                        if let Stmt::Expr(Expr::Closure(closure), _) = stmt {
                                            if closure.constness.is_some() {
                                                if let Pat::Type(PatType { ty, .. }) = &closure.inputs[0] {
                                                    if let Type::Reference(type_ref) = &**ty {
                                                        let mut new_type = type_ref.clone();
                                                        new_type.lifetime = Some(lifetime.clone());
                                                        closure.inputs[0] = Pat::Type(PatType {
                                                            attrs: Vec::new(),
                                                            pat: Box::new(Pat::Ident(syn::PatIdent {
                                                                attrs: Vec::new(),
                                                                by_ref: None,
                                                                mutability: None,
                                                                ident: Ident::new("data", Span::call_site()),
                                                                subpat: None,
                                                            })),
                                                            colon_token: token::Colon {
                                                                spans: [Span::call_site()],
                                                            },
                                                            ty: Box::new(Type::Reference(new_type)),
                                                        });
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies const functions returning opaque types with `impl const Fn()` and modifies them to include lifetime annotations. This transformation introduces explicit lifetime parameters and updates function bodies to respect these lifetimes, particularly in closures. It aims to test the compiler's handling of lifetimes in const trait implementations, potentially revealing bugs in lifetime resolution and checking."
    }
}