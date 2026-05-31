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

pub struct Replace_Const_Default_With_Self_Reference_113;

impl Mutator for Replace_Const_Default_With_Self_Reference_113 {
    fn name(&self) -> &str {
        "Replace_Const_Default_With_Self_Reference_113"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut modified = false;
                for param in &mut item_struct.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Some(default) = &const_param.default {
                            const_param.default = Some(parse_quote!(#const_param.ident));
                            modified = true;
                        }
                    }
                }
                if modified {
                    let struct_name = &item_struct.ident;
                    let generics = &item_struct.generics;
                    let mut predicates = if let Some(where_clause) = &generics.where_clause {
                        where_clause.predicates.clone()
                    } else {
                        Punctuated::new()
                    };
                    for param in &item_struct.generics.params {
                        if let GenericParam::Const(const_param) = param {
                            let param_name = &const_param.ident;
                            predicates.push(parse_quote!(#struct_name<#param_name>:));
                        }
                    }
                    item_struct.generics.where_clause = Some(WhereClause {
                        where_token: Default::default(),
                        predicates,
                    });
                }
            }
            if let Item::Enum(item_enum) = item {
                let mut modified = false;
                for param in &mut item_enum.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Some(default) = &const_param.default {
                            const_param.default = Some(parse_quote!(#const_param.ident));
                            modified = true;
                        }
                    }
                }
                if modified {
                    let enum_name = &item_enum.ident;
                    let generics = &item_enum.generics;
                    let mut predicates = if let Some(where_clause) = &generics.where_clause {
                        where_clause.predicates.clone()
                    } else {
                        Punctuated::new()
                    };
                    for param in &item_enum.generics.params {
                        if let GenericParam::Const(const_param) = param {
                            let param_name = &const_param.ident;
                            predicates.push(parse_quote!(#enum_name<#param_name>:));
                        }
                    }
                    item_enum.generics.where_clause = Some(WhereClause {
                        where_token: Default::default(),
                        predicates,
                    });
                }
            }
            if let Item::Fn(item_fn) = item {
                let mut modified = false;
                for param in &mut item_fn.sig.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Some(default) = &const_param.default {
                            const_param.default = Some(parse_quote!(#const_param.ident));
                            modified = true;
                        }
                    }
                }
                if modified {
                    let fn_name = &item_fn.sig.ident;
                    let generics = &item_fn.sig.generics;
                    let mut predicates = if let Some(where_clause) = &generics.where_clause {
                        where_clause.predicates.clone()
                    } else {
                        Punctuated::new()
                    };
                    for param in &item_fn.sig.generics.params {
                        if let GenericParam::Const(const_param) = param {
                            let param_name = &const_param.ident;
                            predicates.push(parse_quote!(#fn_name<#param_name>:));
                        }
                    }
                    item_fn.sig.generics.where_clause = Some(WhereClause {
                        where_token: Default::default(),
                        predicates,
                    });
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets generic const parameters with default values in structs, enums, and functions. It replaces the default value with a self-referential expression using the parameter's identifier. Additionally, it adds or extends a where clause with a predicate referencing the item instantiated with the same parameter, creating a cyclic dependency. This transformation aims to trigger out-of-range substitution errors during instantiation and well-formedness checking, testing the compiler's handling of self-referential defaults and where clause constraints."
    }
}