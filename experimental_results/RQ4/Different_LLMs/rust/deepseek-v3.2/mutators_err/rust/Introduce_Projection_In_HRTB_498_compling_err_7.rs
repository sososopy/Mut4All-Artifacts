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

pub struct Introduce_Projection_In_HRTB_498;

impl Mutator for Introduce_Projection_In_HRTB_498 {
    fn name(&self) -> &str {
        "Introduce_Projection_In_HRTB_498"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ProjectionHRTBMutatorVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator locates higher-ranked trait bounds (for<'a>) that contain associated type projections where the Self in the projection is a generic parameter from the outer scope. It replaces the Self in the projection with a different existing generic parameter, or introduces a new generic parameter if none exists. This transformation creates potentially invalid trait bounds where the Self type may not implement the required trait, testing the compiler's handling of projection type mismatches in HRTB contexts and its ability to detect or resolve such inconsistencies."
    }
}

struct ProjectionHRTBMutatorVisitor;

impl VisitMut for ProjectionHRTBMutatorVisitor {
    fn visit_item_fn_mut(&mut self, i: &mut ItemFn) {
        self.visit_generics_mut(&mut i.sig.generics);
        syn::visit_mut::visit_item_fn_mut(self, i);
    }
    
    fn visit_impl_item_fn_mut(&mut self, i: &mut syn::ImplItemFn) {
        self.visit_generics_mut(&mut i.sig.generics);
        syn::visit_mut::visit_impl_item_fn_mut(self, i);
    }
    
    fn visit_generics_mut(&mut self, generics: &mut syn::Generics) {
        let params: Vec<_> = generics.params.iter().filter_map(|p| match p {
            GenericParam::Type(tp) => Some(tp.ident.clone()),
            _ => None,
        }).collect();
        
        let mut new_param_added = false;
        let mut new_param_name = Ident::new("O", Span::call_site());
        
        if params.is_empty() {
            generics.params.push(parse_quote!(O));
            new_param_added = true;
        }
        
        let mut predicates_to_modify: Vec<syn::WherePredicate> = Vec::new();
        
        if let Some(where_clause) = &mut generics.where_clause {
            for predicate in &mut where_clause.predicates {
                if let syn::WherePredicate::Type(pred_type) = predicate {
                    self.visit_type_mut(&mut pred_type.bounded_ty);
                }
            }
        }
        
        for bound in generics.params.iter_mut() {
            if let GenericParam::Type(tp) = bound {
                for bound in &mut tp.bounds {
                    if let TypeParamBound::Trait(trait_bound) = bound {
                        self.visit_trait_bound_mut_with_params(trait_bound, &params, &mut new_param_added, &mut new_param_name, generics);
                    }
                }
            }
        }
        
        syn::visit_mut::visit_generics_mut(self, generics);
    }
    
    fn visit_trait_bound_mut(&mut self, i: &mut syn::TraitBound) {
        self.visit_trait_bound_mut_with_params(i, &[], &mut false, &mut Ident::new("O", Span::call_site()), &mut syn::Generics::default());
    }
    
    fn visit_type_mut(&mut self, ty: &mut syn::Type) {
        if let Type::TraitObject(type_trait_object) = ty {
            for bound in &mut type_trait_object.bounds {
                if let TypeParamBound::Trait(trait_bound) = bound {
                    if trait_bound.lifetimes.is_some() {
                        let params = Vec::new();
                        let mut new_param_added = false;
                        let mut new_param_name = Ident::new("O", Span::call_site());
                        let mut dummy_generics = syn::Generics::default();
                        self.visit_trait_bound_mut_with_params(trait_bound, &params, &mut new_param_added, &mut new_param_name, &mut dummy_generics);
                    }
                }
            }
        }
        syn::visit_mut::visit_type_mut(self, ty);
    }
}

impl ProjectionHRTBMutatorVisitor {
    fn visit_trait_bound_mut_with_params(&mut self, trait_bound: &mut syn::TraitBound, existing_params: &[Ident], new_param_added: &mut bool, new_param_name: &mut Ident, generics: &mut syn::Generics) {
        if trait_bound.lifetimes.is_some() {
            let mut visitor = ProjectionVisitor {
                existing_params: existing_params.to_vec(),
                new_param_added: *new_param_added,
                new_param_name: new_param_name.clone(),
                generics,
                need_new_param: false,
            };
            visitor.visit_path_mut(&mut trait_bound.path);
            
            if visitor.need_new_param && !*new_param_added {
                generics.params.insert(0, parse_quote!(O));
                *new_param_added = true;
                *new_param_name = Ident::new("O", Span::call_site());
            }
        }
    }
}

struct ProjectionVisitor<'a> {
    existing_params: Vec<Ident>,
    new_param_added: bool,
    new_param_name: Ident,
    generics: &'a mut syn::Generics,
    need_new_param: bool,
}

impl<'a> VisitMut for ProjectionVisitor<'a> {
    fn visit_path_mut(&mut self, path: &mut syn::Path) {
        for segment in &mut path.segments {
            if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                for arg in &mut args.args {
                    if let GenericArgument::Type(ty) = arg {
                        self.visit_type_mut(ty);
                    }
                }
            }
        }
        
        if !path.segments.is_empty() {
            let last_segment = path.segments.last_mut().unwrap();
            if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                for arg in &mut args.args {
                    if let GenericArgument::Type(ty) = arg {
                        if let Type::Path(type_path) = ty {
                            if type_path.path.segments.len() == 1 {
                                let seg = &type_path.path.segments[0];
                                if seg.ident == "Self" && !self.existing_params.is_empty() {
                                    let mut rng = thread_rng();
                                    if let Some(existing_param) = self.existing_params.choose(&mut rng) {
                                        type_path.path.segments[0].ident = existing_param.clone();
                                    } else if !self.new_param_added {
                                        self.need_new_param = true;
                                        type_path.path.segments[0].ident = self.new_param_name.clone();
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        
        syn::visit_mut::visit_path_mut(self, path);
    }
    
    fn visit_type_path_mut(&mut self, type_path: &mut syn::TypePath) {
        if type_path.path.segments.len() == 1 {
            let seg = &type_path.path.segments[0];
            if seg.ident == "Self" && !self.existing_params.is_empty() {
                let mut rng = thread_rng();
                if let Some(existing_param) = self.existing_params.choose(&mut rng) {
                    type_path.path.segments[0].ident = existing_param.clone();
                } else if !self.new_param_added {
                    self.need_new_param = true;
                    type_path.path.segments[0].ident = self.new_param_name.clone();
                }
            }
        }
        syn::visit_mut::visit_type_path_mut(self, type_path);
    }
}