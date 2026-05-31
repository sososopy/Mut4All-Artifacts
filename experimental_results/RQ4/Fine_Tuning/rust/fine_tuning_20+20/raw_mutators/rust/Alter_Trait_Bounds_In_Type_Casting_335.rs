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

pub struct Alter_Trait_Bounds_In_Type_Casting_335;

impl Mutator for Alter_Trait_Bounds_In_Type_Casting_335 {
    fn name(&self) -> &str {
        "Alter_Trait_Bounds_In_Type_Casting_335"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                struct DynStarAsVisitor {
                    targets: Vec<usize>,
                }
                impl<'ast> Visit<'ast> for DynStarAsVisitor {
                    fn visit_expr_as(&mut self, node: &'ast ExprAs) {
                        if let Type::TraitObject(type_trait_object) = &*node.ty {
                            if type_trait_object.dyn_token.is_some() {
                                self.targets.push(node.as_token.span.start().line);
                            }
                        }
                        syn::visit::visit_expr_as(self, node);
                    }
                }
                let mut visitor = DynStarAsVisitor { targets: vec![] };
                visitor.visit_item_fn(item_fn);
                let mut rng = thread_rng();
                let target_line = visitor.targets.choose(&mut rng);
                if let Some(target_line) = target_line {
                    let mut selected_type: Option<Type> = None;
                    for stmt in &item_fn.block.stmts {
                        if stmt.span().start().line == *target_line {
                            if let Stmt::Local(local) = stmt {
                                if let Some((_, expr)) = &local.init {
                                    if let Expr::Type(expr_type) = expr.as_ref() {
                                        selected_type = Some((*expr_type.ty).clone());
                                    }
                                }
                            }
                        }
                    }
                    if let Some(selected_type) = selected_type {
                        let mut mutated_type = selected_type.clone();
                        struct TypeMutator {
                            target_line: usize,
                            mutated_type: Type,
                        }
                        impl VisitMut for TypeMutator {
                            fn visit_expr_as_mut(&mut self, node: &mut ExprAs) {
                                if node.as_token.span.start().line == self.target_line {
                                    node.ty = Box::new(self.mutated_type.clone());
                                }
                                syn::visit_mut::visit_expr_as_mut(self, node);
                            }
                        }
                        if let Type::TraitObject(type_trait_object) = &mut mutated_type {
                            let mut bounds: Vec<_> = type_trait_object.bounds.iter().collect();
                            let mut rng = thread_rng();
                            let choice = rng.gen_range(0..3);
                            match choice {
                                0 => {
                                    let additional_traits = vec![
                                        "Clone", "Debug", "Default", "PartialEq", "PartialOrd",
                                    ];
                                    let mut available_traits: Vec<&str> = additional_traits
                                        .iter()
                                        .filter(|&&t| {
                                            !bounds.iter().any(|b| {
                                                if let TypeParamBound::Trait(trait_bound) = b {
                                                    trait_bound.path.is_ident(t)
                                                } else {
                                                    false
                                                }
                                            })
                                        })
                                        .cloned()
                                        .collect();
                                    available_traits.shuffle(&mut rng);
                                    if let Some(trait_name) = available_traits.first() {
                                        let new_bound: TypeParamBound = parse_quote!(#trait_name);
                                        bounds.push(new_bound);
                                    }
                                }
                                1 => {
                                    if bounds.len() > 1 {
                                        bounds.remove(rng.gen_range(0..bounds.len()));
                                    }
                                }
                                2 => {
                                    let additional_traits = vec![
                                        "Clone", "Debug", "Default", "PartialEq", "PartialOrd",
                                    ];
                                    let mut available_traits: Vec<&str> = additional_traits
                                        .iter()
                                        .filter(|&&t| {
                                            !bounds.iter().any(|b| {
                                                if let TypeParamBound::Trait(trait_bound) = b {
                                                    trait_bound.path.is_ident(t)
                                                } else {
                                                    false
                                                }
                                            })
                                        })
                                        .cloned()
                                        .collect();
                                    available_traits.shuffle(&mut rng);
                                    if let Some(trait_name) = available_traits.first() {
                                        let new_bound: TypeParamBound = parse_quote!(#trait_name);
                                        let replace_index = rng.gen_range(0..bounds.len());
                                        bounds[replace_index] = new_bound;
                                    }
                                }
                                _ => {}
                            }
                            type_trait_object.bounds = Punctuated::from_iter(bounds);
                        }
                        let mut mutator = TypeMutator {
                            target_line: *target_line,
                            mutated_type,
                        };
                        mutator.visit_item_fn_mut(item_fn);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutator targets type casting expressions involving `dyn* Trait` with additional trait bounds. It randomly selects a `dyn*` type cast and mutates the trait bounds by either adding a new unrelated bound, removing an existing bound, or replacing a bound with another. This transformation leverages the complexity of trait bounds in type casting to stress-test the compiler's trait resolution and type checking mechanisms, especially in the context of the newer `dyn*` feature."
    }
}