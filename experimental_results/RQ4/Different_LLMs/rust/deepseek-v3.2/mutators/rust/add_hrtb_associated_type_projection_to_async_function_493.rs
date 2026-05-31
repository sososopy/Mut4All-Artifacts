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

pub struct Add_HRTB_Associated_Type_Projection_To_Async_Function_493;

impl Mutator for Add_HRTB_Associated_Type_Projection_To_Async_Function_493 {
    fn name(&self) -> &str {
        "Add_HRTB_Associated_Type_Projection_To_Async_Function_493"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut types = HashSet::new();
        let mut traits = HashSet::new();
        
        struct TypeCollector<'a> {
            types: &'a mut HashSet<String>,
            traits: &'a mut HashSet<String>,
        }
        
        impl<'a> Visit<'a> for TypeCollector<'a> {
            fn visit_type_path(&mut self, node: &'a TypePath) {
                let type_name = node.path.segments.iter().map(|seg| seg.ident.to_string()).collect::<Vec<_>>().join("::");
                self.types.insert(type_name);
                syn::visit::visit_type_path(self, node);
            }
            
            fn visit_trait_bound(&mut self, node: &'a TraitBound) {
                let trait_name = node.path.segments.iter().map(|seg| seg.ident.to_string()).collect::<Vec<_>>().join("::");
                self.traits.insert(trait_name);
                syn::visit::visit_trait_bound(self, node);
            }
        }
        
        let mut collector = TypeCollector {
            types: &mut types,
            traits: &mut traits,
        };
        collector.visit_file(file);
        
        let primitive_types = vec!["i32", "u32", "i64", "u64", "f32", "f64", "bool", "char", "usize", "isize"];
        for ty in primitive_types {
            types.insert(ty.to_string());
        }
        
        let primitive_traits = vec!["Copy", "Clone", "Send", "Sync", "Sized", "Debug"];
        for tr in primitive_traits {
            traits.insert(tr.to_string());
        }
        
        let mut rng = thread_rng();
        
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.asyncness.is_none() {
                    continue;
                }
                
                if item_fn.sig.generics.params.is_empty() {
                    continue;
                }
                
                if item_fn.sig.generics.where_clause.is_none() {
                    continue;
                }
                
                let where_clause = item_fn.sig.generics.where_clause.as_mut().unwrap();
                
                let available_types: Vec<String> = types.iter().cloned().collect();
                if available_types.is_empty() {
                    continue;
                }
                
                let selected_type = available_types.choose(&mut rng).unwrap();
                let type_ident = if selected_type.contains("::") {
                    let segments: Vec<&str> = selected_type.split("::").collect();
                    let last_segment = segments.last().unwrap();
                    Ident::new(last_segment, Span::call_site())
                } else {
                    Ident::new(selected_type, Span::call_site())
                };
                
                let available_traits: Vec<String> = traits.iter().cloned().collect();
                if available_traits.is_empty() {
                    continue;
                }
                
                let selected_trait = available_traits.choose(&mut rng).unwrap();
                let trait_ident = if selected_trait.contains("::") {
                    let segments: Vec<&str> = selected_trait.split("::").collect();
                    let last_segment = segments.last().unwrap();
                    Ident::new(last_segment, Span::call_site())
                } else {
                    Ident::new(selected_trait, Span::call_site())
                };
                
                let new_predicate: WherePredicate = parse_quote! {
                    for<'a> <#type_ident as FnOnce<(&'a mut #type_ident,)>>::Output: #trait_ident + 'a
                };
                
                where_clause.predicates.push(new_predicate);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets async functions with where clauses and type parameters. It introduces a higher-ranked trait bound that projects an associated type from FnOnce on a concrete type, constraining that associated type with a trait bound tied to the lifetime. This creates nonsensical bounds (since primitive types don't implement FnOnce) that may trigger compiler inconsistencies during implied bounds computation and coroutine handling, particularly with HRTBs and associated type projections."
    }
}