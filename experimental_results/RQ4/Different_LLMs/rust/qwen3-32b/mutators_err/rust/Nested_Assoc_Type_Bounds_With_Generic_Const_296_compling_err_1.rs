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

pub struct Nested_Assoc_Type_Bounds_With_Generic_Const_296;

impl Mutator for Nested_Assoc_Type_Bounds_With_Generic_Const_296 {
    fn name(&self) -> &str {
        "Nested_Assoc_Type_Bounds_With_Generic_Const_296"
    }
    fn mutate(&self, file: &mut syn::File) {
        // Check for required traits and add if missing
        let mut has_u = false;
        let mut has_v = false;
        for item in &file.items {
            if let syn::Item::Trait(trait_item) = item {
                if trait_item.ident == "U" {
                    has_u = true;
                } else if trait_item.ident == "V" {
                    has_v = true;
                }
            }
        }
        if !has_u {
            let u_trait = parse_quote! {
                trait U<'a> {
                    type B;
                }
            };
            file.items.insert(0, syn::Item::Trait(u_trait));
        }
        if !has_v {
            let v_trait = parse_quote! {
                trait V<'a, 'b> {
                }
            };
            file.items.insert(0, syn::Item::Trait(v_trait));
        }

        // Process each function
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }

                // Replace const parameters with generic expressions
                let mut new_params = Punctuated::new();
                for param in &func.sig.generics.params {
                    if let syn::GenericParam::Const(const_param) = param {
                        let ident = &const_param.ident;
                        let ty = &const_param.ty;
                        let expr = parse_quote! { T::<0>::A as u8 };
                        let new_const = syn::GenericParam::Const(syn::ConstGenericParam {
                            ident: ident.clone(),
                            colon_token: Some(Default::default()),
                            ty: ty.clone(),
                            eq_token: Some(Default::default()),
                            default: Some(Box::new(expr)),
                            ..Default::default()
                        });
                        new_params.push(new_const);
                    } else {
                        new_params.push(param.clone());
                    }
                }
                func.sig.generics.params = new_params;

                // Add nested associated type bound to where clause
                let outer_bound = parse_quote! {
                    T<A: for<'a> U<'a, B: for<'b> V<'a, 'b>>>
                };
                let predicate = syn::WherePredicate::Type(syn::PredicateType {
                    lifetimes: None,
                    bounded_ty: Box::new(syn::Type::Path(syn::TypePath {
                        qself: None,
                        path: syn::Path::from(Ident::new("T", Span::call_site())),
                    })),
                    colon_token: Default::default(),
                    bounds: Punctuated::from_iter(vec![syn::TypeParamBound::Trait(outer_bound)]),
                });

                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    where_clause.predicates.push(predicate);
                } else {
                    let mut where_clause = syn::WhereClause {
                        where_token: Default::default(),
                        predicates: Punctuated::new(),
                    };
                    where_clause.predicates.push(predicate);
                    func.sig.generics.where_clause = Some(where_clause);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}