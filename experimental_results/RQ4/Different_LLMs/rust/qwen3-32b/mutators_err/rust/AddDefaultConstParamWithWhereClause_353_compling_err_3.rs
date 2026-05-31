use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemStruct, ItemEnum, ItemConst, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct AddDefaultConstParamWithWhereClause_353;

impl Mutator for AddDefaultConstParamWithWhereClause_353 {
    fn name(&self) -> &str {
        "AddDefaultConstParamWithWhereClause_353"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(struct_item) = item {
                let mut has_const = false;
                let mut first_const_name = None;
                for param in &struct_item.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        has_const = true;
                        first_const_name = Some(const_param.ident.clone());
                        break;
                    }
                }
                if has_const {
                    if let Some(const_name) = first_const_name {
                        let default_name = Ident::new("DEFAULT", Span::call_site());
                        let default_type = struct_item.generics.params.iter()
                            .find(|p| matches!(p, GenericParam::Const(_)))
                            .and_then(|p| {
                                if let GenericParam::Const(c) = p {
                                    Some(c.ty.clone())
                                } else {
                                    None
                                }
                            })
                            .unwrap();
                        let default_value = parse_quote!(42);
                        let default_const = ItemConst {
                            attrs: Vec::new(),
                            vis: syn::Visibility::Inherited,
                            const_token: Default::default(),
                            ident: default_name,
                            colon_token: Default::default(),
                            ty: Box::new(default_type),
                            eq_token: Default::default(),
                            expr: Box::new(default_value),
                            semi_token: Default::default(),
                            generics: Default::default(),
                        };
                        let index = file.items.iter().position(|x| x == item).unwrap();
                        file.items.insert(index, Item::Const(default_const));
                        for param in &mut struct_item.generics.params {
                            if let GenericParam::Const(const_param) = param {
                                if const_param.ident == const_name {
                                    const_param.default = Some(parse_quote!(DEFAULT));
                                }
                            }
                        }
                        let len_expr = Expr::Path(ExprPath {
                            qself: None,
                            path: SynPath::from(const_name),
                            attrs: Vec::new(),
                        });
                        let array_ty = Type::Array(TypeArray {
                            bracket_token: token::Bracket::default(),
                            elem: Box::new(Type::Tuple(TypeTuple {
                                paren_token: token::Paren::default(),
                                elems: Punctuated::new(),
                            })),
                            len: len_expr,
                            semi_token: Default::default(),
                        });
                        let where_pred = WherePredicate::Type(PredicateType {
                            lifetimes: None,
                            bounded_ty: array_ty,
                            colon_token: token::Colon::default(),
                            bounds: Punctuated::new(),
                        });
                        if let Some(where_clause) = &mut struct_item.generics.where_clause {
                            where_clause.predicates.push(where_pred);
                        } else {
                            struct_item.generics.where_clause = Some(WhereClause {
                                where_token: token::Where::default(),
                                predicates: {
                                    let mut preds = Punctuated::new();
                                    preds.push(where_pred);
                                    preds
                                },
                            });
                        }
                    }
                }
            } else if let syn::Item::Enum(enum_item) = item {
                let mut has_const = false;
                let mut first_const_name = None;
                for param in &enum_item.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        has_const = true;
                        first_const_name = Some(const_param.ident.clone());
                        break;
                    }
                }
                if has_const {
                    if let Some(const_name) = first_const_name {
                        let default_name = Ident::new("DEFAULT", Span::call_site());
                        let default_type = enum_item.generics.params.iter()
                            .find(|p| matches!(p, GenericParam::Const(_)))
                            .and_then(|p| {
                                if let GenericParam::Const(c) = p {
                                    Some(c.ty.clone())
                                } else {
                                    None
                                }
                            })
                            .unwrap();
                        let default_value = parse_quote!(42);
                        let default_const = ItemConst {
                            attrs: Vec::new(),
                            vis: syn::Visibility::Inherited,
                            const_token: Default::default(),
                            ident: default_name,
                            colon_token: Default::default(),
                            ty: Box::new(default_type),
                            eq_token: Default::default(),
                            expr: Box::new(default_value),
                            semi_token: Default::default(),
                            generics: Default::default(),
                        };
                        let index = file.items.iter().position(|x| x == item).unwrap();
                        file.items.insert(index, Item::Const(default_const));
                        for param in &mut enum_item.generics.params {
                            if let GenericParam::Const(const_param) = param {
                                if const_param.ident == const_name {
                                    const_param.default = Some(parse_quote!(DEFAULT));
                                }
                            }
                        }
                        let len_expr = Expr::Path(ExprPath {
                            qself: None,
                            path: SynPath::from(const_name),
                            attrs: Vec::new(),
                        });
                        let array_ty = Type::Array(TypeArray {
                            bracket_token: token::Bracket::default(),
                            elem: Box::new(Type::Tuple(TypeTuple {
                                paren_token: token::Paren::default(),
                                elems: Punctuated::new(),
                            })),
                            len: len_expr,
                            semi_token: Default::default(),
                        });
                        let where_pred = WherePredicate::Type(PredicateType {
                            lifetimes: None,
                            bounded_ty: array_ty,
                            colon_token: token::Colon::default(),
                            bounds: Punctuated::new(),
                        });
                        if let Some(where_clause) = &mut enum_item.generics.where_clause {
                            where_clause.predicates.push(where_pred);
                        } else {
                            enum_item.generics.where_clause = Some(WhereClause {
                                where_token: token::Where::default(),
                                predicates: {
                                    let mut preds = Punctuated::new();
                                    preds.push(where_pred);
                                    preds
                                },
                            });
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a default const parameter using a const item in the same scope and adds a where clause that references the const parameter in a const expression. This exploits potential issues in the compiler's substitution logic during specialization checks by combining default const parameters and where clauses with array length constraints."
    }
}