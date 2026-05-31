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

pub struct Modify_Adt_Const_Params_287;

impl Mutator for Modify_Adt_Const_Params_287 {
    fn name(&self) -> &str {
        "Modify_Adt_Const_Params_287"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if let syn::ReturnType::Type(_, ref mut ty) = func.sig.output {
                            if let Type::Reference(type_reference) = &**ty {
                                if let Type::Path(type_path) = &*type_reference.elem {
                                    if type_path.path.segments.last().unwrap().ident == "MyStr" {
                                        // Change return type to &'static str
                                        *ty = Box::new(Type::Reference(syn::TypeReference {
                                            and_token: token::And { spans: [Span::call_site()] },
                                            lifetime: Some(syn::Lifetime::new("'static", Span::call_site())),
                                            mutability: None,
                                            elem: Box::new(Type::Path(syn::TypePath {
                                                qself: None,
                                                path: syn::Path::from(Ident::new("str", Span::call_site())),
                                            })),
                                        }));

                                        // Modify function body to return the input directly
                                        let input_ident = if let FnArg::Typed(pat_type) = &func.sig.inputs[0] {
                                            if let Pat::Ident(pat_ident) = &*pat_type.pat {
                                                &pat_ident.ident
                                            } else {
                                                continue;
                                            }
                                        } else {
                                            continue;
                                        };
                                        let new_expr: Expr = parse_quote! { #input_ident };
                                        func.block.stmts.clear();
                                        func.block.stmts.push(Stmt::Expr(new_expr));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets structs implementing traits with custom const functions using ADT constant parameters. It modifies the return type of such functions from a specific reference type to a generic `&'static str`, altering the function body to directly return the input. This change aims to exploit potential inconsistencies in ADT constant parameter handling, increasing the likelihood of triggering compiler errors or unexpected behavior."
    }
}