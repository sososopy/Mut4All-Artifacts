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

pub struct Incomplete_Generic_Type_Parameters_in_Transmutability_Checks_468;

impl Mutator for Incomplete_Generic_Type_Parameters_in_Transmutability_Checks_468 {
    fn name(&self) -> &str {
        "Incomplete_Generic_Type_Parameters_in_Transmutability_Checks_468"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                let mut generic_names = HashSet::new();
                for param in &item_struct.generics.params {
                    if let syn::GenericParam::Type(type_param) = param {
                        generic_names.insert(type_param.ident.clone());
                    }
                }

                let mut has_field_with_generic = false;
                for field in &item_struct.fields {
                    let ty = &field.ty;
                    let mut checker = GenericParamChecker {
                        generic_names: generic_names.clone(),
                        found: false,
                    };
                    syn::visit::visit_type(&mut checker, ty);
                    if checker.found {
                        has_field_with_generic = true;
                        break;
                    }
                }

                if has_field_with_generic && !item_struct.generics.params.is_empty() {
                    let mut params: Vec<_> = item_struct.generics.params.into_iter().collect();
                    params.remove(0);
                    item_struct.generics.params = Punctuated::from_iter(params.into_iter());
                }
            } else if let syn::Item::Fn(item_fn) = item {
                let mut generic_names = HashSet::new();
                for param in &item_fn.sig.generics.params {
                    if let syn::GenericParam::Type(type_param) = param {
                        generic_names.insert(type_param.ident.clone());
                    }
                }

                let mut has_param_or_return_with_generic = false;

                for input in &item_fn.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        let ty = &pat_type.ty;
                        let mut checker = GenericParamChecker {
                            generic_names: generic_names.clone(),
                            found: false,
                        };
                        syn::visit::visit_type(&mut checker, ty);
                        if checker.found {
                            has_param_or_return_with_generic = true;
                            break;
                        }
                    }
                }

                if !has_param_or_return_with_generic {
                    if let ReturnType::Type(_, ty) = &item_fn.sig.output {
                        let mut checker = GenericParamChecker {
                            generic_names: generic_names.clone(),
                            found: false,
                        };
                        syn::visit::visit_type(&mut checker, ty);
                        if checker.found {
                            has_param_or_return_with_generic = true;
                        }
                    }
                }

                if has_param_or_return_with_generic && !item_fn.sig.generics.params.is_empty() {
                    let mut params: Vec<_> = item_fn.sig.generics.params.into_iter().collect();
                    params.remove(0);
                    item_fn.sig.generics.params = Punctuated::from_iter(params.into_iter());
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets structs and generic functions with fields or parameters referencing their generic parameters. It removes a generic parameter from the declaration while retaining it in the field or return type, creating unbound type references. This forces the compiler to mishandle transmutability checks during codegen, potentially triggering ICEs when resolving internal references or constraints."
    }
}

struct GenericParamChecker {
    generic_names: HashSet<syn::Ident>,
    found: bool,
}

impl<'ast> Visit<'ast> for GenericParamChecker {
    fn visit_ident(&mut self, i: &syn::Ident) {
        if self.generic_names.contains(i) {
            self.found = true;
        }
    }
}