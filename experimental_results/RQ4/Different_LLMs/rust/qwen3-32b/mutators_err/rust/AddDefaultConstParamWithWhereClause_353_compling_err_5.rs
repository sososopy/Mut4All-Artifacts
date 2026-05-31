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
        let mut to_modify = Vec::new();

        // First pass: collect indices and necessary data
        for (index, item) in file.items.iter().enumerate() {
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
                        to_modify.push( (index, const_name, true) );
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
                        to_modify.push( (index, const_name, false) );
                    }
                }
            }
        }

        // Second pass: apply modifications
        for (original_index, const_name, is_struct) in to_modify {
            // Insert the const item
            let default_name = Ident::new("DEFAULT", Span::call_site());
            let default_type = if is_struct {
                // get the type from the struct's params
                let item = &file.items[original_index + 1];
                if let syn::Item::Struct(struct_item) = item {
                    struct_item.generics.params.iter()
                        .find(|p| matches!(p, GenericParam::Const(_)))
                        .and_then(|p| {
                            if let GenericParam::Const(c) = p {
                                Some(c.ty.clone())
                            } else {
                                None
                            }
                        })
                        .unwrap()
                } else {
                    unreachable!()
                }
            } else {
                // get the type from the enum's params
                let item = &file.items[original_index + 1];
                if let syn::Item::Enum(enum_item) = item {
                    enum_item.generics.params.iter()
                        .find(|p| matches!(p, GenericParam::Const(_)))
                        .and_then(|p| {
                            if let GenericParam::Const(c) = p {
                                Some(c.ty.clone())
                            } else {
                                None
                            }
                        })
                        .unwrap()
                } else {
                    unreachable!()
                }
            };
            let default_value = parse_quote!(42);
            let default_const = ItemConst {
                attrs: Vec::new(),
                vis: syn::Visibility::Inherited,
                const_token: Default::default(),
                ident: default_name.clone(),
                colon_token: Default::default(),
                ty: Box::new(default_type),
                eq_token: Default::default(),
                expr: Box::new(default_value),
                semi_token: Default::default(),
                generics: Default::default(),
            };
            file.items.insert(original_index, Item::Const(default_const));

            // Modify the struct or enum's generics
            let modified_item = &mut file.items[original_index + 1];
            if is_struct {
                if let syn::Item::Struct(struct_item) = modified_item {
                    for param in &mut struct_item.generics.params {
                        if let GenericParam::Const(const_param) = param {
                            if const_param.ident == const_name {
                                const_param.default = Some(parse_quote!(DEFAULT));
                            }
                        }
                    }
                    let len_expr = Expr::Path(ExprPath {
                        qself: None,
                        path: SynPath::from(const_name.clone()),
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
            } else {
                if let syn::Item::Enum(enum_item) = modified_item {
                    for param in &mut enum_item.generics.params {
                        if let GenericParam::Const(const_param) = param {
                            if const_param.ident == const_name {
                                const_param.default = Some(parse_quote!(DEFAULT));
                            }
                        }
                    }
                    let len_expr = Expr::Path(ExprPath {
                        qself: None,
                        path: SynPath::from(const_name.clone()),
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
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a default const parameter using a const item in the same scope and adds a where clause that references the const parameter in a const expression. This exploits potential issues in the compiler's substitution logic during specialization checks by combining default const parameters and where clauses with array length constraints."
    }
}.