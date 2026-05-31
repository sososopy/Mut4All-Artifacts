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

pub struct Modify_Const_Trait_Implementation_With_Lifetime_Annotations_307;

impl Mutator for Modify_Const_Trait_Implementation_With_Lifetime_Annotations_307 {
    fn name(&self) -> &str {
        "Modify_Const_Trait_Implementation_With_Lifetime_Annotations_307"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.constness.is_none() {
                    continue;
                }
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        let mut has_lifetime = false;
                        for bound in &type_impl_trait.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                if let Some(lifetimes) = &trait_bound.lifetimes {
                                    has_lifetime = true;
                                    break;
                                }
                            }
                        }
                        let lifetime_name = if has_lifetime {
                            Lifetime::new("'_", Span::call_site())
                        } else {
                            Lifetime::new("'a", Span::call_site())
                        };
                        let lifetime_param = LifetimeParam {
                            attrs: Vec::new(),
                            lifetime: lifetime_name.clone(),
                            colon_token: None,
                            bounds: Punctuated::new(),
                        };
                        if !has_lifetime {
                            item_fn.sig.generics.params.push(GenericParam::Lifetime(
                                lifetime_param.clone(),
                            ));
                        }
                        let mut new_bounds = Punctuated::new();
                        for bound in &type_impl_trait.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                let new_bound = TypeParamBound::Trait(TraitBound {
                                    lifetimes: trait_bound.lifetimes.clone().or_else(|| {
                                        Some(BoundLifetimes {
                                            for_token: Default::default(),
                                            lt_token: token::Lt::default(),
                                            gt_token: token::Gt::default(),
                                            lifetimes: {
                                                let mut lifetimes = Punctuated::new();
                                                lifetimes.push(syn::GenericParam::Lifetime(
                                                    lifetime_param.clone(),
                                                ));
                                                lifetimes
                                            },
                                        })
                                    }),
                                    ..trait_bound.clone()
                                });
                                new_bounds.push(new_bound);
                            } else {
                                new_bounds.push(bound.clone());
                            }
                        }
                        let new_return_type = Type::ImplTrait(TypeImplTrait {
                            impl_token: type_impl_trait.impl_token,
                            bounds: new_bounds,
                        });
                        item_fn.sig.output = ReturnType::Type(
                            Default::default(),
                            Box::new(new_return_type),
                        );
                        let mut visitor = ClosureVisitor {
                            lifetime: lifetime_name,
                        };
                        visitor.visit_block_mut(&mut item_fn.block);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutator targets const functions returning opaque types with const trait bounds. It introduces or modifies lifetime annotations in the return type to ensure explicit lifetime constraints. This transformation forces the compiler to enforce stricter lifetime checks, particularly in const contexts, potentially exposing bugs related to lifetime inference, const trait implementation, and borrow checking."
    }
}

struct ClosureVisitor {
    lifetime: Lifetime,
}

impl VisitMut for ClosureVisitor {
    fn visit_expr_closure_mut(&mut self, node: &mut ExprClosure) {
        if node.constness.is_none() {
            return;
        }
        if node.lifetimes.is_none() {
            node.lifetimes = Some(BoundLifetimes {
                for_token: Default::default(),
                lt_token: token::Lt::default(),
                gt_token: token::Gt::default(),
                lifetimes: {
                    let mut lifetimes = Punctuated::new();
                    lifetimes.push(syn::GenericParam::Lifetime(LifetimeParam {
                        attrs: Vec::new(),
                        lifetime: self.lifetime.clone(),
                        colon_token: None,
                        bounds: Punctuated::new(),
                    }));
                    lifetimes
                },
            });
        }
        if let Some(inputs) = node.inputs.as_deref_mut() {
            for input in inputs {
                if let Pat::Type(pat_type) = input {
                    if let Type::Reference(type_reference) = &mut *pat_type.ty {
                        if type_reference.lifetime.is_none() {
                            type_reference.lifetime = Some(self.lifetime.clone());
                        }
                    }
                }
            }
        }
    }
}