use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;
use syn::visit::Visit;

pub struct Add_Const_Unit_Generic_With_Default_And_WhereClause_113;

impl Mutator for Add_Const_Unit_Generic_With_Default_And_WhereClause_113 {
    fn name(&self) -> &str {
        "Add_Const_Unit_Generic_With_Default_And_WhereClause_113"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                if has_const_generics(&item_struct.generics) {
                    let new_param = parse_quote!(const C: () = {});
                    item_struct.generics.params.push(new_param);
                    
                    let struct_ident = &item_struct.ident;
                    let bounded_ty = create_bounded_ty(struct_ident);
                    let predicate = create_predicate(bounded_ty);
                    add_where_clause(&mut item_struct.generics, predicate);
                }
            } else if let syn::Item::Enum(item_enum) = item {
                if has_const_generics(&item_enum.generics) {
                    let new_param = parse_quote!(const C: () = {});
                    item_enum.generics.params.push(new_param);
                    
                    let enum_ident = &item_enum.ident;
                    let bounded_ty = create_bounded_ty(enum_ident);
                    let predicate = create_predicate(bounded_ty);
                    add_where_clause(&mut item_enum.generics, predicate);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds a const generic parameter of unit type with a default value and a where clause referencing the same type with the unit const. This targets substitution logic for const generics with unit types and defaults, leveraging the where clause to force compiler evaluation of the substituted const value. It applies to structs/enums with at least one const generic parameter."
    }
}

fn has_const_generics(generics: &syn::Generics) -> bool {
    generics.params.iter().any(|param| matches!(param, GenericParam::Const(_)))
}

fn create_bounded_ty(ident: &Ident) -> Type {
    Type::Path(TypePath {
        qself: None,
        path: syn::Path {
            leading_colon: None,
            segments: {
                let mut segments = Punctuated::new();
                segments.push(PathSegment {
                    ident: ident.clone(),
                    arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                        colon2_token: None,
                        lt_token: token::Lt::default(),
                        args: {
                            let mut args = Punctuated::new();
                            args.push(GenericArgument::Const(parse_quote!({})));
                            args
                        },
                        gt_token: token::Gt::default(),
                    }),
                });
                segments
            },
        },
    })
}

fn create_predicate(bounded_ty: Type) -> WherePredicate {
    let colon_token = token::Colon::default();
    let bounds = {
        let mut bounds = Punctuated::new();
        bounds.push(syn::TypeParamBound::Trait(syn::TraitBound {
            paren_token: None,
            modifier: syn::TraitBoundModifier::None,
            lifetimes: None,
            path: syn::Path {
                leading_colon: None,
                segments: {
                    let mut segs = Punctuated::new();
                    segs.push(syn::PathSegment {
                        ident: Ident::new("Sized", Span::call_site()),
                        arguments: syn::PathArguments::None,
                    });
                    segs
                },
            },
        }));
        bounds
    };
    WherePredicate::Type(syn::PredicateType {
        lifetimes: None,
        bounded_ty,
        colon_token,
        bounds,
    })
}

fn add_where_clause(generics: &mut syn::Generics, predicate: WherePredicate) {
    let mut where_clause = generics.where_clause.take().unwrap_or_else(|| WhereClause {
        where_token: token::Where::default(),
        predicates: Punctuated::new(),
    });
    where_clause.predicates.push(predicate);
    generics.where_clause = Some(where_clause);
}