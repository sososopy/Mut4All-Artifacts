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

pub struct Replace_Concrete_Ref_With_Impl_Trait_409;

impl Mutator for Replace_Concrete_Ref_With_Impl_Trait_409 {
    fn name(&self) -> &str {
        "Replace_Concrete_Ref_With_Impl_Trait_409"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_usage = HashSet::new();
        let mut visitor = TraitCollector {
            traits: &mut trait_usage,
        };
        visitor.visit_file(&file);
        let common_traits = vec!["Debug", "Clone", "PartialEq", "PartialOrd", "Eq", "Ord", "Hash", "Default", "Display"];
        let mut rng = thread_rng();
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                for stmt in &mut item_fn.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Pat::Type(PatType { ty, .. }) = &mut local.pat {
                            if let Some(ty) = ty {
                                if let Type::Reference(type_ref) = &**ty {
                                    if let Type::Path(type_path) = &*type_ref.elem {
                                        let concrete_type_name = type_path.path.segments.last().map(|seg| seg.ident.to_string()).unwrap_or_default();
                                        let candidate_traits: Vec<&str> = common_traits.iter().filter(|&&trait_name| trait_usage.contains(trait_name)).copied().collect();
                                        let selected_trait = if !candidate_traits.is_empty() {
                                            candidate_traits.choose(&mut rng).copied()
                                        } else {
                                            common_traits.choose(&mut rng).copied()
                                        };
                                        if let Some(trait_name) = selected_trait {
                                            let new_type = if type_ref.mutability.is_some() {
                                                parse_quote!(&mut impl #trait_name)
                                            } else {
                                                parse_quote!(&impl #trait_name)
                                            };
                                            if let Pat::Type(PatType { ty, .. }) = &mut local.pat {
                                                *ty = Some(Box::new(new_type));
                                            }
                                            if let Some(init) = &mut local.init {
                                                if let Expr::Reference(_) = &*init.expr {
                                                } else {
                                                    let expr = init.expr.clone();
                                                    let new_expr = if type_ref.mutability.is_some() {
                                                        parse_quote!(&mut #expr)
                                                    } else {
                                                        parse_quote!(& #expr)
                                                    };
                                                    init.expr = Box::new(new_expr);
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
        "The mutation operator targets local variable declarations with reference type annotations (e.g., `&ConcreteType`). It replaces the concrete type with an `impl Trait` bound, where the trait is selected from commonly used traits (Debug, Clone, etc.) or introduced via use statements if needed. The initialization expression is adjusted to be a reference if it isn't already. This transformation tests the compiler's ability to handle opaque reference types, trait inference, and reference coercion in local contexts, potentially uncovering bugs in type resolution and trait-based polymorphism."
    }
}

struct TraitCollector<'a> {
    traits: &'a mut HashSet<String>,
}

impl<'a> Visit<'a> for TraitCollector<'a> {
    fn visit_path(&mut self, path: &'a syn::Path) {
        if let Some(segment) = path.segments.last() {
            let ident = segment.ident.to_string();
            if ident.chars().next().map(|c| c.is_ascii_uppercase()).unwrap_or(false) {
                self.traits.insert(ident);
            }
        }
        syn::visit::visit_path(self, path);
    }
}