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

pub struct Modify_Associated_Type_Bound_Lifetime_492;

impl Mutator for Modify_Associated_Type_Bound_Lifetime_492 {
    fn name(&self) -> &str {
        "Modify_Associated_Type_Bound_Lifetime_492"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = AssociatedTypeBoundVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets associated type bounds within higher-ranked trait bounds (HRTBs) in trait definitions and impl blocks. It modifies lifetime constraints on associated types that involve Future traits, changing explicit lifetime bounds (e.g., +'a) to +'static or removing them entirely. This creates mismatched lifetime requirements between the future output and the HRTB, potentially triggering ICEs in the compiler's trait resolution for async functions and unboxed closures."
    }
}

struct AssociatedTypeBoundVisitor;

impl VisitMut for AssociatedTypeBoundVisitor {
    fn visit_item_mut(&mut self, item: &mut Item) {
        match item {
            Item::Trait(item_trait) => {
                self.visit_trait_item_mut(item_trait);
            }
            Item::Impl(item_impl) => {
                self.visit_impl_item_mut(item_impl);
            }
            _ => {}
        }
        syn::visit_mut::visit_item_mut(self, item);
    }
}

impl AssociatedTypeBoundVisitor {
    fn visit_trait_item_mut(&mut self, item_trait: &mut ItemTrait) {
        for item in &mut item_trait.items {
            if let TraitItem::Type(assoc_type) = item {
                self.process_associated_type_bounds(&mut assoc_type.bounds);
            }
        }
        
        for param in &mut item_trait.generics.params {
            if let GenericParam::Type(type_param) = param {
                for bound in &mut type_param.bounds {
                    self.process_type_param_bound(bound);
                }
            }
        }
        
        if let Some(where_clause) = &mut item_trait.generics.where_clause {
            for predicate in &mut where_clause.predicates {
                self.process_where_predicate(predicate);
            }
        }
    }
    
    fn visit_impl_item_mut(&mut self, item_impl: &mut ItemImpl) {
        for param in &mut item_impl.generics.params {
            if let GenericParam::Type(type_param) = param {
                for bound in &mut type_param.bounds {
                    self.process_type_param_bound(bound);
                }
            }
        }
        
        if let Some(where_clause) = &mut item_impl.generics.where_clause {
            for predicate in &mut where_clause.predicates {
                self.process_where_predicate(predicate);
            }
        }
        
        for impl_item in &mut item_impl.items {
            if let ImplItem::Type(assoc_type) = impl_item {
                self.process_associated_type_bounds(&mut assoc_type.ty.bounds);
            }
        }
    }
    
    fn process_associated_type_bounds(&mut self, bounds: &mut Punctuated<TypeParamBound, Plus>) {
        let mut new_bounds = Punctuated::new();
        let mut rng = thread_rng();
        
        for bound in bounds.iter() {
            match bound {
                TypeParamBound::Trait(trait_bound) => {
                    if self.should_modify_trait_bound(trait_bound) {
                        let mut modified_bound = trait_bound.clone();
                        self.modify_trait_bound_lifetime(&mut modified_bound, &mut rng);
                        new_bounds.push(TypeParamBound::Trait(modified_bound));
                    } else {
                        new_bounds.push(bound.clone());
                    }
                }
                _ => {
                    new_bounds.push(bound.clone());
                }
            }
        }
        
        *bounds = new_bounds;
    }
    
    fn process_type_param_bound(&mut self, bound: &mut TypeParamBound) {
        if let TypeParamBound::Trait(trait_bound) = bound {
            if self.should_modify_trait_bound(trait_bound) {
                let mut rng = thread_rng();
                self.modify_trait_bound_lifetime(trait_bound, &mut rng);
            }
        }
    }
    
    fn process_where_predicate(&mut self, predicate: &mut WherePredicate) {
        match predicate {
            WherePredicate::Type(pred_type) => {
                for bound in &mut pred_type.bounds {
                    self.process_type_param_bound(bound);
                }
            }
            WherePredicate::Lifetime(_) => {}
            WherePredicate::Eq(pred_eq) => {
                for bound in &mut pred_eq.bounds {
                    self.process_type_param_bound(bound);
                }
            }
        }
    }
    
    fn should_modify_trait_bound(&self, trait_bound: &TraitBound) -> bool {
        let path_string = trait_bound.path.segments.iter()
            .map(|seg| seg.ident.to_string())
            .collect::<Vec<_>>()
            .join("::");
        
        path_string.contains("Future") && self.has_higher_ranked_lifetime(trait_bound)
    }
    
    fn has_higher_ranked_lifetime(&self, trait_bound: &TraitBound) -> bool {
        trait_bound.lifetimes.as_ref().map_or(false, |lifetimes| {
            lifetimes.lifetimes.iter().any(|lt| {
                if let GenericParam::Lifetime(lt_param) = lt {
                    lt_param.lifetime.ident.to_string().starts_with('\'')
                } else {
                    false
                }
            })
        })
    }
    
    fn modify_trait_bound_lifetime(&mut self, trait_bound: &mut TraitBound, rng: &mut impl Rng) {
        let modification_type = rng.gen_range(0..3);
        
        match modification_type {
            0 => {
                trait_bound.lifetimes = Some(BoundLifetimes {
                    for_token: token::For::default(),
                    lt_token: token::Lt::default(),
                    lifetimes: {
                        let mut lifetimes = Punctuated::new();
                        lifetimes.push(GenericParam::Lifetime(LifetimeParam {
                            attrs: Vec::new(),
                            lifetime: Lifetime::new("'static", Span::call_site()),
                            colon_token: None,
                            bounds: Punctuated::new(),
                        }));
                        lifetimes
                    },
                    gt_token: token::Gt::default(),
                });
            }
            1 => {
                trait_bound.lifetimes = None;
            }
            _ => {
                if let Some(lifetimes) = &mut trait_bound.lifetimes {
                    for lt_param in &mut lifetimes.lifetimes {
                        if let GenericParam::Lifetime(lt_param) = lt_param {
                            lt_param.lifetime = Lifetime::new("'static", Span::call_site());
                        }
                    }
                }
            }
        }
    }
}