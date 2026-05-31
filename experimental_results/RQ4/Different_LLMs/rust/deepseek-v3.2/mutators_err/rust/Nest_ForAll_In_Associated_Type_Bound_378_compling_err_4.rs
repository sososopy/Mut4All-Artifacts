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

pub struct Nest_ForAll_In_Associated_Type_Bound_378;

impl Mutator for Nest_ForAll_In_Associated_Type_Bound_378 {
    fn name(&self) -> &str {
        "Nest_ForAll_In_Associated_Type_Bound_378"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = NestForAllVisitor {
            lifetime_counter: 0,
            const_counter: -0,
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets where clauses with forall binders and associated type bounds, nesting additional forall binders inside associated type bounds. It transforms patterns like `for<'a> T: TraitA<'a, AsA: TraitB<'a>>` into `for<'a> T: TraitA<'a, AsA: for<'z1> TraitB<'a, 'z1>>`. It introduces fresh lifetime parameters (e.g., 'z1, 'z2) or const parameters (e.g., N1) to create deeper nesting, reusing existing types and traits. This transformation stresses the compiler's handling of complex nested higher-ranked trait bounds and associated type constraints, potentially exposing bugs in trait resolution, lifetime inference, or const generic evaluation."
    }
}

struct NestForAllVisitor {
    lifetime_counter: usize,
    const_counter: usize,
}

impl VisitMut for NestForAllVisitor {
    fn visit_where_predicate_mut(&mut self, node: &mut WherePredicate) {
        if let WherePredicate::Type(pred_type) = node {
            self.mutate_predicate_type(pred_type);
        }
        syn::visit_mut::visit_where_predicate_mut(self, node);
    }
}

impl NestForAllVisitor {
    fn mutate_predicate_type(&mut self, pred_type: &mut PredicateType) {
        if let Type::TraitObject(trait_obj) = &mut pred_type.bounded_ty {
            for bound in &mut trait_obj.bounds {
                if let TypeParamBound::Trait(trait_bound) = bound {
                    self.mutate_trait_bound(trait_bound);
                }
            }
        }
    }

    fn mutate_trait_bound(&mut self, trait_bound: &mut TraitBound) {
        if let Some(bound_lifetimes) = &mut trait_bound.lifetimes {
            self.mutate_bound_lifetimes(bound_lifetimes);
        }
        for segment in &mut trait_bound.path.segments {
            if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                for arg in &mut args.args {
                    if let GenericArgument::Type(ty) = arg {
                        self.mutate_type_in_assoc_bound(ty);
                    }
                }
            }
        }
    }

    fn mutate_bound_lifetimes(&mut self, bound_lifetimes: &mut BoundLifetimes) {
        for lifetime_param in &mut bound_lifetimes.lifetimes {
            self.mutate_lifetime_param(lifetime_param);
        }
    }

    fn mutate_lifetime_param(&mut self, _lifetime_param: &mut GenericParam) {
    }

    fn mutate_type_in_assoc_bound(&mut self, ty: &mut Type) {
        if let Type::Path(type_path) = ty {
            for segment in &mut type_path.path.segments {
                if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                    for arg in &mut args.args {
                        if let GenericArgument::Type(inner_ty) = arg {
                            if let Type::TraitObject(trait_obj) = inner_ty {
                                for bound in &mut trait_obj.bounds {
                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                        self.nest_forall_in_trait_bound(trait_bound);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn nest_forall_in_trait_bound(&mut self, trait_bound: &mut TraitBound) {
        let mut new_lifetimes = Punctuated::new();
        self.lifetime_counter += 1;
        let fresh_lifetime = LifetimeParam {
            attrs: Vec::new(),
            lifetime: Lifetime::new(&format!("'z{}", self.lifetime_counter), Span::call_site()),
            colon_token: None,
            bounds: Punctuated::new(),
        };
        new_lifetimes.push(GenericParam::Lifetime(fresh_lifetime));

        let mut new_args = Punctuated::new();
        if let PathArguments::AngleBracketed(args) = &trait_bound.path.segments.last().unwrap().arguments {
            for arg in &args.args {
                match arg {
                    GenericArgument::Lifetime(lt) => new_args.push(GenericArgument::Lifetime(lt.clone())),
                    GenericArgument::Type(ty) => new_args.push(GenericArgument::Type(ty.clone())),
                    GenericArgument::Const(expr) => new_args.push(GenericArgument::Const(expr.clone())),
                    _ => new_args.push(arg.clone()),
                }
            }
        }
        self.lifetime_counter += 1;
        new_args.push(GenericArgument::Lifetime(Lifetime::new(&format!("'z{}", self.lifetime_counter), Span::call_site())));

        let mut new_segments = trait_bound.path.segments.clone();
        if let Some(last_segment) = new_segments.last_mut() {
            last_segment.arguments = PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                colon2_token: None,
                lt_token: token::Lt::default(),
                args: new_args,
                gt_token: token::Gt::default(),
            });
        }

        trait_bound.lifetimes = Some(BoundLifetimes {
            for_token: token::For::default(),
            lt_token: token::Lt::default(),
            lifetimes: new_lifetimes,
            gt_token: token::Gt::default(),
        });
        trait_bound.path.segments = new_segments;
    }
}