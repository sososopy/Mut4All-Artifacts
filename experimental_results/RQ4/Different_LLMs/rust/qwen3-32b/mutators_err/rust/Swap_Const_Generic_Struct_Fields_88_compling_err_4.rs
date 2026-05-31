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

pub struct Swap_Const_Generic_Struct_Fields_88;

impl Mutator for Swap_Const_Generic_Struct_Fields_88 {
    fn name(&self) -> &str {
        "Swap_Const_Generic_Struct_Fields_88"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(struct_def) = item {
                let has_const_generics = struct_def.generics.params.iter().any(|param| {
                    matches!(param, GenericParam::Const(_))
                });
                if !has_const_generics {
                    continue;
                }

                let in_unsafe_context = false;

                if let syn::Fields::Named(ref mut named) = struct_def.fields {
                    let fields = &mut named.named;
                    let mut swapped = false;
                    for i in 0..fields.len() - 1 {
                        let field1 = &fields[i];
                        let field2 = &fields[i + 1];
                        if let Type::Array(array1) = &field1.ty {
                            if let Type::Array(array2) = &field2.ty {
                                let expr1_str = array1.len.to_token_stream().to_string();
                                let expr2_str = array2.len.to_token_stream().to_string();
                                if expr1_str != expr2_str {
                                    let mut vec_fields: Vec<_> = fields.clone().into_iter().collect();
                                    vec_fields.swap(i, i + 1);
                                    *fields = Punctuated::from_iter(vec_fields.into_iter());
                                    swapped = true;
                                    break;
                                }
                            }
                        }
                    }
                    if !swapped {
                        for i in 0..fields.len() - 1 {
                            let field1 = &fields[i];
                            let field2 = &fields[i + 1];
                            let is_unit = |ty: &Type| {
                                matches!(ty, Type::Tuple(tuple) if tuple.elems.is_empty())
                            };
                            if !is_unit(&field1.ty) && !is_unit(&field2.ty) {
                                let mut vec_fields: Vec<_> = fields.clone().into_iter().collect();
                                vec_fields.swap(i, i + 1);
                                *fields = Punctuated::from_iter(vec_fields.into_iter());
                                swapped = true;
                                break;
                            }
                        }
                    }
                } else if let syn::Fields::Unnamed(ref mut unnamed) = struct_def.fields {
                    let fields = &mut unnamed.unnamed;
                    let mut swapped = false;
                    for i in 0..fields.len() - 1 {
                        let field1 = &fields[i];
                        let field2 = &fields[i + 1];
                        if let Type::Array(array1) = &field1.ty {
                            if let Type::Array(array2) = &field2.ty {
                                let expr1_str = array1.len.to_token_stream().to_string();
                                let expr2_str = array2.len.to_token_stream().to_string();
                                if expr1_str != expr2_str {
                                    let mut vec_fields: Vec<_> = fields.clone().into_iter().collect();
                                    vec_fields.swap(i, i + 1);
                                    *fields = Punctuated::from_iter(vec_fields.into_iter());
                                    swapped = true;
                                    break;
                                }
                            }
                        }
                    }
                    if !swapped {
                        for i in 0..fields.len() - 1 {
                            let field1 = &fields[i];
                            let field2 = &fields[i + 1];
                            let is_unit = |ty: &Type| {
                                matches!(ty, Type::Tuple(tuple) if tuple.elems.is_empty())
                            };
                            if !is_unit(&field1.ty) && !is_unit(&field2.ty) {
                                let mut vec_fields: Vec<_> = fields.clone().into_iter().collect();
                                vec_fields.swap(i, i + 1);
                                *fields = Punctuated::from_iter(vec_fields.into_iter());
                                swapped = true;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}