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

pub struct Modify_Default_Type_Of_Const_Generic_Parameter_381;

impl Mutator for Modify_Default_Type_Of_Const_Generic_Parameter_381 {
    fn name(&self) -> &str {
        "Modify_Default_Type_Of_Const_Generic_Parameter_381"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut has_const_generic = false;
                for param in &item_struct.generics.params {
                    if let GenericParam::Const(_) = param {
                        has_const_generic = true;
                        break;
                    }
                }
                if !has_const_generic {
                    continue;
                }
                if let Some(where_clause) = &item_struct.generics.where_clause {
                    for predicate in &where_clause.predicates {
                        if let WherePredicate::Type(predicate_type) = predicate {
                            if let Type::Path(type_path) = &predicate_type.bounded_ty {
                                if type_path.qself.is_none()
                                    && type_path.path.leading_colon.is_none()
                                    && type_path.path.segments.len() == 1
                                    && type_path.path.segments[0].ident == "Self"
                                {
                                    return;
                                }
                            }
                        }
                    }
                }
                let mut has_default = false;
                let mut has_modified = false;
                for param in &item_struct.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if const_param.default.is_some() {
                            has_default = true;
                            break;
                        }
                    }
                }
                if !has_default {
                    return;
                }
                for param in &mut item_struct.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Some(default) = &mut const_param.default {
                            if let Expr::Path(expr_path) = default {
                                if expr_path.path.segments.len() == 1 {
                                    let ident = expr_path.path.segments[0].ident.clone();
                                    let new_expr: Expr = parse_quote!(#ident);
                                    *default = new_expr;
                                    has_modified = true;
                                    break;
                                }
                            }
                        }
                    }
                }
                if has_modified {
                    let mut new_items = Vec::new();
                    for item in &file.items {
                        if let Item::Fn(item_fn) = item {
                            if item_fn.sig.ident == "main" {
                                continue;
                            }
                        }
                        new_items.push(item.clone());
                    }
                    file.items = new_items;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutator targets structs with const generic parameters that have default values. It modifies the default value to a type name, such as `bool`, which is not a valid constant expression. This transformation aims to expose compiler weaknesses in handling const generic defaults that are improperly specified as types rather than values, particularly in scenarios lacking a main function for direct execution."
    }
}