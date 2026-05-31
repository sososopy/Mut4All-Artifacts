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

pub struct Incorrect_Use_Of_Generic_Type_Parameters_150;

impl Mutator for Incorrect_Use_Of_Generic_Type_Parameters_150 {
    fn name(&self) -> &str {
        "Incorrect_Use_Of_Generic_Type_Parameters_150"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                if func.sig.generics.params.is_empty() {
                    continue;
                }
                let mut has_generic_params = false;
                for input in &func.sig.inputs {
                    if let syn::FnArg::Typed(pat_type) = input {
                        if let syn::Type::Path(type_path) = &*pat_type.ty {
                            if type_path
                                .path
                                .segments
                                .iter()
                                .any(|seg| seg.arguments.is_empty())
                            {
                                has_generic_params = true;
                                break;
                            }
                        }
                    }
                }
                if !has_generic_params {
                    if let syn::ReturnType::Type(_, ty) = &func.sig.output {
                        if let syn::Type::Path(type_path) = &**ty {
                            if type_path
                                .path
                                .segments
                                .iter()
                                .any(|seg| seg.arguments.is_empty())
                            {
                                has_generic_params = true;
                            }
                        }
                    }
                }
                if !has_generic_params {
                    continue;
                }
                let mut undeclared_generic_type = None;
                for param in &func.sig.generics.params {
                    if let syn::GenericParam::Type(type_param) = param {
                        undeclared_generic_type = Some(type_param.ident.clone());
                        break;
                    }
                }
                if undeclared_generic_type.is_none() {
                    continue;
                }
                let undeclared_ident = undeclared_generic_type.unwrap();
                for input in &mut func.sig.inputs {
                    if let syn::FnArg::Typed(pat_type) = input {
                        if let syn::Type::Path(type_path) = &*pat_type.ty {
                            if type_path
                                .path
                                .segments
                                .iter()
                                .any(|seg| seg.arguments.is_empty())
                            {
                                pat_type.ty = Box::new(syn::Type::Path(syn::TypePath {
                                    qself: None,
                                    path: syn::Path::from(undeclared_ident.clone()),
                                }));
                                return;
                            }
                        }
                    }
                }
                if let syn::ReturnType::Type(_, ty) = &mut func.sig.output {
                    if let syn::Type::Path(type_path) = &**ty {
                        if type_path
                            .path
                            .segments
                            .iter()
                            .any(|seg| seg.arguments.is_empty())
                        {
                            *ty = Box::new(syn::Type::Path(syn::TypePath {
                                qself: None,
                                path: syn::Path::from(undeclared_ident),
                            }));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with generic type parameters by replacing a correctly used generic type in the function signature with an undeclared one. It specifically modifies either the parameter type or return type to use a generic type parameter that is not declared in the function's generics list. This transformation exploits potential weaknesses in the compiler's handling of generic type resolution and error reporting, aiming to trigger internal inconsistencies or ICEs related to generics."
    }
}