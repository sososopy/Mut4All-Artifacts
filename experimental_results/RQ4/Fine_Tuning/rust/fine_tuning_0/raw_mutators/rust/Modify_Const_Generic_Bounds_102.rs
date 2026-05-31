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
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Modify_Const_Generic_Bounds_102;

impl Mutator for Modify_Const_Generic_Bounds_102 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Bounds_102"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(first_generic_param) = func.sig.generics.params.first() {
                    if let syn::GenericParam::Const(_) = first_generic_param {
                        let generics = &mut func.sig.generics;
                        generics.params.push(parse_quote!(const M: usize));
                        func.sig.generics.where_clause = Some(WhereClause {
                            where_token: Default::default(),
                            predicates: {
                                let mut predicates = Punctuated::new();
                                predicates.push(syn::WherePredicate::Type(PredicateType {
                                    lifetimes: None,
                                    bounded_ty: syn::Type::Array(syn::TypeArray {
                                        bracket_token: Default::default(),
                                        elem: Box::new(syn::Type::Path(TypePath {
                                            qself: None,
                                            path: syn::Path::from(Ident::new("i32", Span::call_site())),
                                        })),
                                        len: syn::Expr::Binary(syn::ExprBinary {
                                            attrs: vec![],
                                            left: Box::new(syn::Expr::Path(syn::ExprPath {
                                                attrs: vec![],
                                                qself: None,
                                                path: syn::Path::from(Ident::new("N", Span::call_site())),
                                            })),
                                            op: syn::BinOp::Add(token::Add { spans: [Span::call_site()] }),
                                            right: Box::new(syn::Expr::Path(syn::ExprPath {
                                                attrs: vec![],
                                                qself: None,
                                                path: syn::Path::from(Ident::new("M", Span::call_site())),
                                            })),
                                        }),
                                    }),
                                    colon_token: Default::default(),
                                    bounds: {
                                        let mut bounds = Punctuated::new();
                                        bounds.push(syn::TypeParamBound::Trait(TraitBound {
                                            paren_token: None,
                                            modifier: TraitBoundModifier::None,
                                            lifetimes: None,
                                            path: syn::Path::from(Ident::new("Sized", Span::call_site())),
                                        }));
                                        bounds
                                    },
                                }));
                                predicates
                            },
                        });
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions using const generics by introducing an additional const generic and a complex where clause. This aims to create potential conflicts or constraints by combining const generics in arithmetic expressions, challenging the compiler's ability to handle const generic interactions and constraints."
    }
}