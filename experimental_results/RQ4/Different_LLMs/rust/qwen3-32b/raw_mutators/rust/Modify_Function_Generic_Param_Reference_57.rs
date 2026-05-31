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

pub struct Modify_Function_Generic_Param_Reference_57;

impl Mutator for Modify_Function_Generic_Param_Reference_57 {
    fn name(&self) -> &str {
        "Modify_Function_Generic_Param_Reference_57"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                for generic_param in &mut item_struct.generics.params {
                    if let syn::GenericParam::Type(type_param) = generic_param {
                        for bound in &mut type_param.bounds {
                            if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                if let Some(segment) = trait_bound.path.segments.iter().last() {
                                    let is_fn_trait = segment.ident == "Fn" || segment.ident == "FnMut" || segment.ident == "FnOnce";
                                    if is_fn_trait {
                                        if let syn::PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                            if let Some(first_arg) = args.args.iter_mut().next() {
                                                if let syn::GenericArgument::Type(ty) = first_arg {
                                                    match ty {
                                                        syn::Type::Reference(ref mut ref_ty) => {
                                                            ref_ty.mutability = match ref_ty.mutability {
                                                                Some(_) => None,
                                                                None => Some(token::Mut {
                                                                    span: proc_macro2::Span::call_site(),
                                                                }),
                                                            };
                                                        },
                                                        _ => {
                                                            let original_ty = std::mem::take(ty);
                                                            *ty = syn::Type::Reference(Box::new(syn::TypeReference {
                                                                and_token: token::And::default(),
                                                                mutability: None,
                                                                elem_ty: Box::new(original_ty),
                                                            }));
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            } else if let syn::Item::Trait(item_trait) = item {
                for generic_param in &mut item_trait.generics.params {
                    if let syn::GenericParam::Type(type_param) = generic_param {
                        for bound in &mut type_param.bounds {
                            if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                if let Some(segment) = trait_bound.path.segments.iter().last() {
                                    let is_fn_trait = segment.ident == "Fn" || segment.ident == "FnMut" || segment.ident == "FnOnce";
                                    if is_fn_trait {
                                        if let syn::PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                            if let Some(first_arg) = args.args.iter_mut().next() {
                                                if let syn::GenericArgument::Type(ty) = first_arg {
                                                    match ty {
                                                        syn::Type::Reference(ref mut ref_ty) => {
                                                            ref_ty.mutability = match ref_ty.mutability {
                                                                Some(_) => None,
                                                                None => Some(token::Mut {
                                                                    span: proc_macro2::Span::call_site(),
                                                                }),
                                                            };
                                                        },
                                                        _ => {
                                                            let original_ty = std::mem::take(ty);
                                                            *ty = syn::Type::Reference(Box::new(syn::TypeReference {
                                                                and_token: token::And::default(),
                                                                mutability: None,
                                                                elem_ty: Box::new(original_ty),
                                                            }));
                                                        }
                                                    }
                                                }
                                            }
                                        }
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
        "The mutation operator modifies the first parameter of function-type generic bounds in structs and traits by adding an additional reference level or toggling mutability. This stresses the compiler's handling of reference types in generic function signatures, potentially exposing issues in trait resolution, reference coercion, and const evaluation under complex generic constraints."
    }
}