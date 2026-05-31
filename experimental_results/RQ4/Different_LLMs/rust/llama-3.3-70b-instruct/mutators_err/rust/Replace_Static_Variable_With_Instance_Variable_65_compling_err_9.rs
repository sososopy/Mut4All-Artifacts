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

pub struct Replace_Static_Variable_With_Instance_Variable_65;

impl Mutator for Replace_Static_Variable_With_Instance_Variable_65 {
    fn name(&self) -> &str {
        "Replace_Static_Variable_With_Instance_Variable_65"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut static_vars = Vec::new();

        // First, collect all static variables
        for item in &file.items {
            if let Item::Static(static_item) = item {
                static_vars.push((static_item.ident.clone(), static_item.ty.clone()));
            }
        }

        // Then, replace static variables with instance variables
        for item in &mut file.items {
            if let Item::Struct(struct_item) = item {
                for field in &mut struct_item.fields {
                    if let syn::Field::Named(field_named) = field {
                        for (static_var_name, static_var_ty) in &static_vars {
                            if field_named.ident == *static_var_name {
                                field_named.ty = static_var_ty.clone();
                            }
                        }
                    }
                }
            }

            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        for (static_var_name, static_var_ty) in &static_vars {
                            if local.pat.to_token_stream().to_string() == static_var_name.to_string() {
                                local.init = Some(LocalInit {
                                    attrs: vec![],
                                    pat: local.pat.clone(),
                                    expr: Expr::Path(ExprPath {
                                        attrs: vec![],
                                        qself: None,
                                        path: SynPath {
                                            leading_colon: None,
                                            segments: Punctuated::from_iter(vec![PathSegment {
                                                ident: static_var_name.clone(),
                                                arguments: PathArguments::None,
                                            }]),
                                        },
                                    }),
                                });
                            }
                        }
                    }
                }
            }

            if let Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Fn(func) = impl_item {
                        for stmt in &mut func.block.stmts {
                            if let Stmt::Local(local) = stmt {
                                for (static_var_name, static_var_ty) in &static_vars {
                                    if local.pat.to_token_stream().to_string() == static_var_name.to_string() {
                                        local.init = Some(LocalInit {
                                            attrs: vec![],
                                            pat: local.pat.clone(),
                                            expr: Expr::Path(ExprPath {
                                                attrs: vec![],
                                                qself: None,
                                                path: SynPath {
                                                    leading_colon: None,
                                                    segments: Punctuated::from_iter(vec![PathSegment {
                                                        ident: static_var_name.clone(),
                                                        arguments: PathArguments::None,
                                                    }]),
                                                },
                                            }),
                                        });
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        // Finally, remove static variables
        file.items.retain(|item| {
            if let Item::Static(_) = item {
                false
            } else {
                true
            }
        });
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets static variables and replaces them with instance variables. It applies to all occurrences of static variables in the code. This transformation changes the scope and lifetime of the variable, which can affect the behavior of the program, especially when trait objects and cross-references are involved."
    }
}