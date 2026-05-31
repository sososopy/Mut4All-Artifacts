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

pub struct Replace_Const_Generic_SizeOf_With_Dyn_Trait_398;

impl Mutator for Replace_Const_Generic_SizeOf_With_Dyn_Trait_398 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_SizeOf_With_Dyn_Trait_398"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut traits = Vec::new();
        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                traits.push(item_trait.ident.clone());
            }
        }
        if traits.is_empty() {
            return;
        }
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some(where_clause) = &mut item_impl.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let WherePredicate::Type(pred_type) = predicate {
                            if let Type::Path(type_path) = &pred_type.bounded_ty {
                                if type_path.path.segments.is_empty() {
                                    continue;
                                }
                                let mut visitor = ConstExprVisitor {
                                    traits: &traits,
                                    changed: false,
                                };
                                for bound in &mut pred_type.bounds {
                                    visitor.visit_type_param_bound_mut(bound);
                                }
                                if visitor.changed {
                                    break;
                                }
                            }
                        }
                    }
                }
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(func) = impl_item {
                        if let Some(where_clause) = &mut func.sig.generics.where_clause {
                            for predicate in &mut where_clause.predicates {
                                if let WherePredicate::Type(pred_type) = predicate {
                                    if let Type::Path(type_path) = &pred_type.bounded_ty {
                                        if type_path.path.segments.is_empty() {
                                            continue;
                                        }
                                        let mut visitor = ConstExprVisitor {
                                            traits: &traits,
                                            changed: false,
                                        };
                                        for bound in &mut pred_type.bounds {
                                            visitor.visit_type_param_bound_mut(bound);
                                        }
                                        if visitor.changed {
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if let Item::Trait(item_trait) = item {
                if let Some(where_clause) = &mut item_trait.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let WherePredicate::Type(pred_type) = predicate {
                            if let Type::Path(type_path) = &pred_type.bounded_ty {
                                if type_path.path.segments.is_empty() {
                                    continue;
                                }
                                let mut visitor = ConstExprVisitor {
                                    traits: &traits,
                                    changed: false,
                                };
                                for bound in &mut pred_type.bounds {
                                    visitor.visit_type_param_bound_mut(bound);
                                }
                                if visitor.changed {
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const generic expressions in where clauses that use intrinsics like size_of::<T>(). It identifies a trait in scope and replaces the type parameter T with a dyn Trait type, creating an unsound const expression. This transformation aims to trigger const evaluation bugs during coherence checking, as size_of on dyn traits is invalid, potentially causing ICEs or unexpected compiler behavior."
    }
}

struct ConstExprVisitor<'a> {
    traits: &'a Vec<Ident>,
    changed: bool,
}

impl<'a> VisitMut for ConstExprVisitor<'a> {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Call(ExprCall { func, args, .. }) = expr {
            if let Expr::Path(ExprPath { path, .. }) = &mut **func {
                if path.segments.last().map(|seg| seg.ident == "size_of" || seg.ident == "align_of").unwrap_or(false) {
                    if let PathArguments::AngleBracketed(args) = &mut path.segments.last_mut().unwrap().arguments {
                        if let Some(GenericArgument::Type(Type::Path(TypePath { path: type_path, .. }))) = args.args.first_mut() {
                            if type_path.segments.len() == 1 {
                                let trait_ident = self.traits.choose(&mut thread_rng()).unwrap();
                                let new_path: SynPath = parse_quote!(dyn #trait_ident);
                                *type_path = new_path;
                                self.changed = true;
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}