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

pub struct Move_Associated_Type_To_Where_Clause_15;

impl Mutator for Move_Associated_Type_To_Where_Clause_15 {
    fn name(&self) -> &str {
        "Move_Associated_Type_To_Where_Clause_15"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(enum_item) = item {
                let mut lifetimes: Vec<Lifetime> = Vec::new();
                for param in &enum_item.generics.params {
                    if let GenericParam::Lifetime(lifetime_param) = param {
                        lifetimes.push(lifetime_param.lifetime.clone());
                    }
                }
                
                let mut visitor = AssocTypeVisitor {
                    lifetimes: &lifetimes,
                    has_mutation: false,
                    where_clause: enum_item.generics.where_clause.clone(),
                    new_lifetime: None,
                };
                
                visitor.visit_item_enum_mut(enum_item);
                
                if visitor.has_mutation {
                    if let Some(lifetime) = visitor.new_lifetime {
                        if !lifetimes.iter().any(|lt| lt.ident == lifetime.ident) {
                            enum_item.generics.params.push(GenericParam::Lifetime(LifetimeParam {
                                attrs: Vec::new(),
                                lifetime: lifetime.clone(),
                                colon_token: None,
                                bounds: Punctuated::new(),
                            }));
                        }
                    }
                    
                    if let Some(where_clause) = visitor.where_clause {
                        enum_item.generics.where_clause = Some(where_clause);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets enum variants containing associated type projections. It moves the projection's trait bound to the enum's where clause, introducing or reusing lifetime parameters. This transformation normalizes associated types in enum layouts, potentially triggering internal compiler errors in trait resolution and associated type handling during enum layout computation."
    }
}

struct AssocTypeVisitor<'a> {
    lifetimes: &'a Vec<Lifetime>,
    has_mutation: bool,
    where_clause: Option<WhereClause>,
    new_lifetime: Option<Lifetime>,
}

impl<'a> VisitMut for AssocTypeVisitor<'a> {
    fn visit_type_mut(&mut self, ty: &mut Type) {
        if let Type::Path(type_path) = ty {
            if let Some(qself) = &type_path.qself {
                if let Type::Path(inner_type) = &*qself.ty {
                    let mut segments = inner_type.path.segments.iter();
                    if let Some(first_seg) = segments.next() {
                        let type_ident = first_seg.ident.clone();
                        
                        let mut trait_path = type_path.path.clone();
                        trait_path.segments.pop();
                        
                        if !trait_path.segments.is_empty() {
                            let lifetime_to_use = if !self.lifetimes.is_empty() {
                                self.lifetimes[0].clone()
                            } else {
                                let new_lt = Lifetime::new("'a", Span::call_site());
                                self.new_lifetime = Some(new_lt.clone());
                                new_lt
                            };
                            
                            let ref_type: Type = parse_quote!(&#lifetime_to_use #inner_type);
                            
                            let new_projection: Type =  parse_quote!(<#ref_type as #trait_path>::#type_ident);
                            *ty = new_projection;
                            
                            let where_predicate: WherePredicate = parse_quote!(#ref_type: #trait_path);
                            
                            if let Some(where_clause) = &mut self.where_clause {
                                where_clause.predicates.push(where_predicate);
                            } else {
                                let mut predicates = Punctuated::new();
                                predicates.push(where_predicate);
                                self.where_clause = Some(WhereClause {
                                    where_token: Default::default(),
                                    predicates,
                                });
                            }
                            
                            self.has_mutation = true;
                        }
                    }
                }
            }
        }
        
        syn::visit_mut::visit_type_mut(self, ty);
    }
}