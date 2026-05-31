use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemImpl, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Replace_Const_Param_With_Function_MethodCall_433;

impl Mutator for Replace_Const_Param_With_Function_MethodCall_433 {
    fn name(&self) -> &str {
        "Replace_Const_Param_With_Function_MethodCall_433"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut available_functions = Vec::new();
        let mut trait_methods = Vec::new();

        // Collect functions without type parameters
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.generics.params.iter().all(|p| !matches!(p, GenericParam::Type(_))) {
                    available_functions.push(func.sig.ident.clone());
                }
            }
        }

        // Collect trait methods
        for item in &mut file.items {
            if let syn::Item::Impl(impl_item) = item {
                if let Some((_, trait_path, _)) = &impl_item.trait_ {
                    for impl_item in &mut impl_item.items {
                        if let syn::ImplItem::Fn(method) = impl_item {
                            trait_methods.push((impl_item.span(), impl_item, method, trait_path, &impl_item.self_ty));
                        }
                    }
                }
            }
        }

        // Replace const parameters with function names
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident != "main" {
                    for param in &mut func.sig.generics.params {
                        if let syn::GenericParam::Const(c) = param {
                            if c.default.is_none() && !available_functions.is_empty() {
                                let func_name = &available_functions[0];
                                let expr_path = syn::ExprPath {
                                    attrs: vec![],
                                    qself: None,
                                    path: syn::Path::from(syn::Ident::new(&func_name.to_string(), proc_macro2::Span::call_site())),
                                };
                                c.default = Some(syn::Expr::Path(expr_path));
                            }
                        }
                    }
                }
            } else if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        for param in &mut method.sig.generics.params {
                            if let syn::GenericParam::Const(c) = param {
                                if c.default.is_none() && !available_functions.is_empty() {
                                    let func_name = &available_functions[0];
                                    let expr_path = syn::ExprPath {
                                        attrs: vec![],
                                        qself: None,
                                        path: syn::Path::from(syn::Ident::new(&func_name.to_string(), proc_macro2::Span::call_site())),
                                    };
                                    c.default = Some(syn::Expr::Path(expr_path));
                                }
                            }
                        }
                    }
                }
            }
        }

        // Insert function pointer method call
        if !trait_methods.is_empty() {
            let (span, impl_item, method, trait_path, self_ty) = &trait_methods[0];
            let method_name = &method.sig.ident;

            // Find a function to insert the code into
            for item in &mut file.items {
                if let syn::Item::Fn(func) = item {
                    if func.sig.ident != "main" {
                        // Create the path for the trait method
                        let path = parse_quote! { <#self_ty as #trait_path>::#method_name };

                        // Create the let statement
                        let stmt1 = syn::Stmt::Local(syn::Local {
                            attrs: vec![],
                            let_token: Default::default(),
                            pat: syn::Pat::Ident(syn::PatIdent {
                                attrs: vec![],
                                ident: syn::Ident::new("f", proc_macro2::Span::call_site()),
                                subpat: None,
                                mutability: None,
                                by_ref: None,
                            }),
                            init: Some(syn::LocalInit {
                                eq_token: syn::token::Eq::default(),
                                expr: Box::new(syn::Expr::Path(syn::ExprPath {
                                    attrs: vec![],
                                    qself: None,
                                    path: path.clone(),
                                })),
                            }),
                            semi_token: Default::default(),
                        });

                        // Create the method call statement
                        let stmt2 = syn::Stmt::Expr(syn::Expr::MethodCall(syn::ExprMethodCall {
                            attrs: vec![],
                            receiver: Box::new(syn::Expr::Path(syn::ExprPath {
                                attrs: vec![],
                                qself: None,
                                path: syn::Path::from(syn::Ident::new("f", proc_macro2::Span::call_site())),
                            })),
                            dot_token: Default::default(),
                            method: syn::Ident::new("bar", proc_macro2::Span::call_site()),
                            turbofish: None,
                            paren_token: Default::default(),
                            args: syn::punctuated::Punctuated::new(),
                        }), None);

                        // Add the statements to the function's block
                        func.block.stmts.push(stmt1);
                        func.block.stmts.push(stmt2);
                        break;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces const parameters with existing function names and invokes methods on function pointers obtained via trait method calls. This transformation targets invalid const argument usage and improper method resolution on function pointers, stressing the compiler's handling of min_generic_const_args and type-dependent definitions."
    }
}