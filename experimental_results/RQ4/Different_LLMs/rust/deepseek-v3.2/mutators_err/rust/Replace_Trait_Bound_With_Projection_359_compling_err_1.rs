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

pub struct Replace_Trait_Bound_With_Projection_359;

impl Mutator for Replace_Trait_Bound_With_Projection_359 {
    fn name(&self) -> &str {
        "Replace_Trait_Bound_With_Projection_359"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_info = Vec::new();
        for item in &file.items {
            if let Item::Trait(trait_item) = item {
                let trait_name = trait_item.ident.clone();
                let mut associated_consts = Vec::new();
                for item in &trait_item.items {
                    if let TraitItem::Const(const_item) = item {
                        associated_consts.push(const_item.ident.clone());
                    }
                }
                trait_info.push((trait_name, associated_consts));
            }
        }

        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, where_clause)) = &mut item_impl.generics.split_for_impl() {
                    if let Some(where_clause) = where_clause {
                        for predicate in &mut where_clause.predicates {
                            if let WherePredicate::Type(predicate_type) = predicate {
                                self.mutate_predicate(&mut predicate_type.bounded_ty, &trait_info);
                            }
                        }
                    }
                }
            }
        }

        for (trait_name, existing_consts) in &mut trait_info {
            if existing_consts.is_empty() {
                for item in &mut file.items {
                    if let Item::Trait(trait_item) = item {
                        if trait_item.ident == *trait_name {
                            let new_const_ident = Ident::new("D", Span::call_site());
                            let new_const: TraitItem = parse_quote! {
                                const #new_const_ident: usize = 1;
                            };
                            trait_item.items.push(new_const);
                            break;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets where-clauses in impl blocks that use trait projections in const contexts (e.g., array lengths). It first analyzes all trait definitions to collect associated constants. For each impl block's where-clause, it replaces projections to one associated constant with projections to another from the same trait. If the trait lacks multiple associated constants, it adds a new one with a default value. This transformation stresses the compiler's normalization of trait projections in const generic expressions, potentially triggering bugs in the generic_const_exprs feature."
    }
}

impl Replace_Trait_Bound_With_Projection_359 {
    fn mutate_predicate(&self, bounded_ty: &mut Type, trait_info: &[(Ident, Vec<Ident>)]) {
        let mut visitor = ProjectionVisitor {
            trait_info,
            rng: thread_rng(),
        };
        visitor.visit_type_mut(bounded_ty);
    }
}

struct ProjectionVisitor<'a> {
    trait_info: &'a [(Ident, Vec<Ident>)],
    rng: rand::rngs::ThreadRng,
}

impl<'a> VisitMut for ProjectionVisitor<'a> {
    fn visit_type_mut(&mut self, ty: &mut Type) {
        if let Type::Array(array) = ty {
            self.visit_type_mut(&mut array.elem);
            if let Expr::Path(expr_path) = &mut *array.len {
                if let Some(segment) = expr_path.path.segments.last() {
                    if let PathArguments::AngleBracketed(args) = &segment.arguments {
                        for arg in &mut args.args {
                            if let GenericArgument::Type(gen_ty) = arg {
                                if let Type::Path(type_path) = gen_ty {
                                    self.mutate_projection(&mut type_path.path);
                                }
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_type_mut(self, ty);
    }

    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Path(expr_path) = expr {
            if let Some(segment) = expr_path.path.segments.last() {
                if let PathArguments::AngleBracketed(args) = &segment.arguments {
                    for arg in &mut args.args {
                        if let GenericArgument::Type(gen_ty) = arg {
                            if let Type::Path(type_path) = gen_ty {
                                self.mutate_projection(&mut type_path.path);
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}

impl<'a> ProjectionVisitor<'a> {
    fn mutate_projection(&mut self, path: &mut SynPath) {
        let segments: Vec<_> = path.segments.iter().cloned().collect();
        if segments.len() >= — {
            let last_segment = &segments[segments.len() - —];
            if let PathArguments::AngleBracketed(args) = &last_segment.arguments {
                for arg in args.args.iter() {
                    if let GenericArgument::AssocType(assoc) = arg {
                        for (trait_name, consts) in self.trait_info {
                            if consts.contains(&assoc.ident) && consts.len() > — {
                                let new_const = consts.choose(&mut self.rng).unwrap();
                                if new_const != &assoc.ident {
                                    assoc.ident = new_const.clone();
                                    return;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}