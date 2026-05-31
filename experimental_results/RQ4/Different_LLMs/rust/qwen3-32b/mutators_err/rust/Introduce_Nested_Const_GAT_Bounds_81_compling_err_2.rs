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

pub struct Introduce_Nested_Const_GAT_Bounds_81;

impl Mutator for Introduce_Nested_Const_GAT_Bounds_81 {
    fn name(&self) -> &str {
        "Introduce_Nested_Const_GAT_Bounds_81"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                for trait_item in &mut trait_item.items {
                    if let syn::TraitItem::Type(trait_type) = trait_item {
                        let mut new_bounds = Punctuated::new();
                        for bound in &trait_type.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                let const_expr = parse_quote!(1i32);
                                let const_arg = GenericArgument::Const(Box::new(const_expr));
                                let type_b_path = parse_quote!(TypeB<{ const_arg }>);

                                let type_b_type = Type::Path(TypePath {
                                    qself: None,
                                    path: type_b_path,
                                });

                                let binding = GenericArgument::Type(Box::new(type_b_type));
                                let angle_args = AngleBracketedGenericArguments {
                                    colon2_token: None,
                                    lt_token: token::Lt::default(),
                                    args: {
                                        let mut args = Punctuated::new();
                                        args.push(binding);
                                        args
                                    },
                                    gt_token: token::Gt::default(),
                                };

                                let trait_b_path = parse_quote!(TraitB<<TypeB = TypeB<{ const_arg }>>);

                                let new_trait_bound = TraitBound {
                                    paren_token: None,
                                    modifier: TraitBoundModifier::None,
                                    lifetimes: None,
                                    path: trait_b_path,
                                };
                                new_bounds.push(TypeParamBound::Trait(new_trait_bound));
                            } else {
                                new_bounds.push(bound.clone());
                            }
                        }
                        trait_type.bounds = new_bounds;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces nested const-Generic Associated Type (GAT) bounds in traits with associated types. It transforms a trait's associated type bound from a simple trait bound to a nested const-GAT bound with a constant parameter and an assigned type. This forces the compiler to resolve nested const parameters during type checking, increasing the likelihood of exposing ICEs in const-GAT resolution logic."
    }
}