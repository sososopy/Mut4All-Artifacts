use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashMap, collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
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

pub struct Replace_Const_Param_With_AssocType_288;

impl Mutator for Replace_Const_Param_With_AssocType_288 {
    fn name(&self) -> &str {
        "Replace_Const_Param_With_AssocType_288"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut const_types = HashSet::new();
        let mut collector = TraitCollector::new();
        collector.visit_file(file);
        
        for item in &file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                for param in &func.sig.generics.params {
                    if let GenericParam::Const(c) = param {
                        let ty_str = c.ty.to_token_stream().to_string();
                        const_types.insert(ty_str);
                    }
                }
            }
        }

        let mut trait_map = HashMap::new();
        let mut counter = 0;
        for ty_str in &const_types {
            let trait_name = Ident::new(&format!("Helper_288_{}", counter), Span::call_site());
            counter += 1;
            
            let ty: Type = parse_quote!(#ty_str);
            let new_trait = parse_quote!(
                trait #trait_name {
                    type Assoc;
                }
            );
            file.items.push(Item::Trait(new_trait));
            
            let new_impl = parse_quote!(
                impl #trait_name for #ty {
                    type Assoc = #ty;
                }
            );
            file.items.push(Item::Impl(new_impl));
            
            trait_map.insert(ty_str.clone(), trait_name);
        }

        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                
                let mut has_type_param = false;
                let mut type_param_name = Ident::new("T", Span::call_site());
                
                let mut type_params = Vec::new();
                for param in &func.sig.generics.params {
                    if let GenericParam::Type(tp) = param {
                        type_params.push(tp.ident.clone());
                    }
                }
                
                if type_params.is_empty() {
                    let mut suffix = 0;
                    while type_params.iter().any(|tp| tp.to_string() == type_param_name.to_string()) {
                        type_param_name = Ident::new(&format!("T{}", suffix), Span::call_site());
                        suffix += 1;
                    }
                    func.sig.generics.params.push(parse_quote!(#type_param_name));
                    has_type_param = true;
                }

                for param in &mut func.sig.generics.params {
                    if let GenericParam::Const(c) = param {
                        let original_type_str = c.ty.to_token_stream().to_string();
                        let trait_name = trait_map.get(&original_type_str).unwrap().clone();
                        
                        let assoc_type_path = parse_quote!(
                            <T as #trait_name>::Assoc
                        );
                        c.ty = assoc_type_path;
                        
                        if func.sig.generics.where_clause.is_none() {
                            func.sig.generics.where_clause = Some(WhereClause {
                                where_token: token::Where::default(),
                                predicates: Punctuated::new(),
                            });
                        }
                        
                        let where_clause = func.sig.generics.where_clause.as_mut().unwrap();
                        let predicate = parse_quote!(
                            T: #trait_name
                        );
                        where_clause.predicates.push(predicate);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces const parameter types with associated types from newly created traits. It introduces nested trait references to stress the compiler's normalization logic, potentially exposing issues in handling const parameters with ADT types through trait indirection."
    }
}

struct TraitCollector {
    traits: Vec<(Ident, String)>,
}

impl TraitCollector {
    fn new() -> Self {
        Self { traits: Vec::new() }
    }
}

impl<'ast> Visit<'ast> for TraitCollector {
    fn visit_item_trait(&mut self, i: &ItemTrait) {
        for item in &i.items {
            if let TraitItem::Type(ty) = item {
                let trait_name = i.ident.clone();
                if let Some(default) = &ty.default {
                    let assoc_type_str = default.1.to_token_stream().to_string();
                    self.traits.push((trait_name, assoc_type_str));
                }
            }
        }
    }
}