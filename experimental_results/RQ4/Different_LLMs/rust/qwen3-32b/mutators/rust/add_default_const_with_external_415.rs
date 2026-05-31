use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemConst, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Add_Default_Const_With_External_415;

impl Mutator for Add_Default_Const_With_External_415 {
    fn name(&self) -> &str {
        "Add_Default_Const_With_External_415"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut existing_consts = Vec::new();
        for item in &file.items {
            if let Item::Const(const_item) = item {
                existing_consts.push((const_item.ty.clone(), const_item.ident.clone()));
            }
        }

        let mut insertions = Vec::new();
        let mut default_count = 0;

        for (i, item) in file.items.iter_mut().enumerate() {
            if let Item::Struct(struct_item) = item {
                let mut const_param = None;
                for param in &struct_item.generics.params {
                    if let GenericParam::Const(c) = param {
                        if c.default.is_none() {
                            const_param = Some(c.clone());
                            break;
                        }
                    }
                }

                if let Some(const_param) = const_param {
                    let mut existing_const_name = None;
                    for (ty, name) in &existing_consts {
                        if ty.to_token_stream().to_string() == const_param.ty.to_token_stream().to_string() {
                            existing_const_name = Some(name.clone());
                            break;
                        }
                    }

                    if let Some(name) = existing_const_name {
                        if let Some(c) = struct_item.generics.params.iter_mut().find_map(|p| {
                            if let GenericParam::Const(c) = p {
                                if c.default.is_none() {
                                    Some(c)
                                } else {
                                    None
                                }
                            } else {
                                None
                            }
                        }) {
                            let default_expr = Expr::Path(ExprPath {
                                attrs: vec![],
                                qself: None,
                                path: SynPath::from(name.clone()),
                            });
                            c.default = Some(default_expr);
                        }
                    } else {
                        default_count += 1;
                        let const_name = Ident::new(&format!("DEFAULT_{}", default_count), Span::call_site());
                        let const_type = &const_param.ty;
                        let const_value = match const_type {
                            Type::Path(type_path) => {
                                if let Some(segment) = type_path.path.segments.last() {
                                    match segment.ident.to_string().as_str() {
                                        "usize" => parse_quote!(64),
                                        "u32" => parse_quote!(42),
                                        "i32" => parse_quote!(0),
                                        "bool" => parse_quote!(true),
                                        "char" => parse_quote!('x'),
                                        _ => parse_quote!(0),
                                    }
                                } else {
                                    parse_quote!(0)
                                }
                            },
                            _ => parse_quote!(0),
                        };
                        let new_const = ItemConst {
                            attrs: vec![],
                            vis: parse_quote!(pub),
                            const_token: Default::default(),
                            ident: const_name.clone(),
                            colon_token: Default::default(),
                            ty: Box::new(const_type.clone()),
                            eq_token: Default::default(),
                            expr: Box::new(const_value),
                            semi_token: Default::default(),
                            generics: Default::default(),
                        };
                        insertions.push((i, new_const));
                        if let Some(c) = struct_item.generics.params.iter_mut().find_map(|p| {
                            if let GenericParam::Const(c) = p {
                                if c.default.is_none() {
                                    Some(c)
                                } else {
                                    None
                                }
                            } else {
                                None
                            }
                        }) {
                            let default_expr = Expr::Path(ExprPath {
                                attrs: vec![],
                                qself: None,
                                path: SynPath::from(const_name),
                            });
                            c.default = Some(default_expr);
                        }
                    }
                }
            }
        }

        // Insert new constants in reverse order to avoid index shifting
        insertions.sort_by(|a, b| b.0.cmp(&a.0));
        for (i, new_const) in insertions {
            file.items.insert(i, Item::Const(new_const));
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds a default value to a struct's const generic parameter by referencing an existing const in the same scope or creating a new one. It introduces complex substitution scenarios by leveraging external const variables, challenging the compiler's handling of const generics and default parameters in generic contexts."
    }
}