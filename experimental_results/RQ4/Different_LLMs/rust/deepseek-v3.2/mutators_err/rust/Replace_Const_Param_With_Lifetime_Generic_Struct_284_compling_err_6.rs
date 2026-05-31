use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, GenericParam::Const, Ident,
    Item, ItemFn, ItemStruct, ItemMacro, Local, Pat, PatType, Path as SynPath,
    PathArguments, PathSegment, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
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

pub struct Replace_Const_Param_With_Lifetime_Generic_Struct_284;

impl Mutator for Replace_Const_Param_With_Lifetime_Generic_Struct_284 {
    fn name(&self) -> &str {
        "Replace_Const_Param_With_Lifetime_Generic_Struct_284"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut lifetime_generic_structs = Vec::new();
        for item in &file.items {
            if let Item::Struct(item_struct) = item {
                if item_struct.generics.params.iter().any(|param| matches!(param, GenericParam::Lifetime(_))) {
                    lifetime_generic_structs.push(item_struct.ident.clone());
                }
            }
        }
        if lifetime_generic_structs.is_empty() {
            return;
        }
        let mut rng = thread_rng();
        let target_struct = lifetime_generic_structs.choose(&mut rng).unwrap();
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                for param in &mut item_struct.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Type::Path(type_path) = &const_param.ty {
                            if type_path.path.segments.len() == 1 {
                                let segment = &type_path.path.segments[0];
                                if segment.arguments.is_none() {
                                    const_param.ty = Type::Path(TypePath {
                                        qself: None,
                                        path: SynPath {
                                            leading_colon: None,
                                            segments: Punctuated::from_iter(vec![PathSegment {
                                                ident: target_struct.clone(),
                                                arguments: PathArguments::None,
                                            }]),
                                        },
                                    });
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            if let Item::Enum(item_enum) = item {
                for param in &mut item_enum.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Type::Path(type_path) = &const_param.ty {
                            if type_path.path.segments.len() == 1 {
                                let segment = &type_path.path.segments[0];
                                if segment.arguments.is_none() {
                                    const_param.ty = Type::Path(TypePath {
                                        qself: None,
                                        path: SynPath {
                                            leading_colon: None,
                                            segments: Punctuated::from_iter(vec![PathSegment {
                                                ident: target_struct.clone(),
                                                arguments: PathArguments::None,
                                            }]),
                                        },
                                    });
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
        let mut has_feature = false;
        for item in &file.items {
            if let Item::Macro(item_macro) = item {
                if item_macro.mac.path.segments.len() == 1 && item_macro.mac.path.segments[0].ident == "feature" {
                    has_feature = true;
                    break;
                }
            }
        }
        if !has_feature {
            file.items.insert(0, parse_quote! {
                #![feature(adt_const_params)]
            });
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies structs generic over lifetimes in the seed program. It then replaces the type of a const generic parameter in another struct or enum definition with one of these lifetime-generic structs, but without providing lifetime arguments. This creates an incomplete generic type in a const parameter context, mimicking the bug report that triggers an ICE. The operator also ensures the adt_const_params feature is enabled if not already present."
    }
}