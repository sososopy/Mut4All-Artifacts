use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemUnion, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Swap_Nested_Anonymous_ADT_141;

impl Mutator for Swap_Nested_Anonymous_ADT_141 {
    fn name(&self) -> &str {
        "Swap_Nested_Anonymous_ADT_141"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                for field in &mut item_struct.fields.iter_mut() {
                    if let Some(type_path) = find_anonymous_struct(&Box::new(field.ty.clone())) {
                        if let Some(type_path2) = find_anonymous_union(&Box::new(field.ty.clone())) {
                            if rng.gen_bool(0.5) {
                                swap_struct_union(&mut Box::new(field.ty.clone()), &type_path, &type_path2);
                            } else {
                                insert_extra_nesting(&mut Box::new(field.ty.clone()), &type_path, &type_path2);
                            }
                        } else {
                            inject_trivial_union(&mut Box::new(field.ty.clone()));
                        }
                    } else if let Some(type_path2) = find_anonymous_union(&Box::new(field.ty.clone())) {
                        if let Some(type_path) = find_anonymous_struct(&Box::new(field.ty.clone())) {
                            if rng.gen_bool(0.5) {
                                swap_struct_union(&mut Box::new(field.ty.clone()), &type_path, &type_path2);
                            } else {
                                insert_extra_nesting(&mut Box::new(field.ty.clone()), &type_path, &type_path2);
                            }
                        } else {
                            inject_trivial_struct(&mut Box::new(field.ty.clone()));
                        }
                    }
                }
            }
            if let Item::Union(item_union) = item {
                for field in &mut item_union.fields.named.iter_mut() {
                    if let Some(type_path) = find_anonymous_struct(&Box::new(field.ty.clone())) {
                        if let Some(type_path2) = find_anonymous_union(&Box::new(field.ty.clone())) {
                            if rng.gen_bool(0.5) {
                                swap_struct_union(&mut Box::new(field.ty.clone()), &type_path, &type_path2);
                            } else {
                                insert_extra_nesting(&mut Box::new(field.ty.clone()), &type_path, &type_path2);
                            }
                        } else {
                            inject_trivial_union(&mut Box::new(field.ty.clone()));
                        }
                    } else if let Some(type_path2) = find_anonymous_union(&Box::new(field.ty.clone())) {
                        if let Some(type_path) = find_anonymous_struct(&Box::new(field.ty.clone())) {
                            if rng.gen_bool(0.5) {
                                swap_struct_union(&mut Box::new(field.2.clone()), &type_path, &type_path2);
                            } else {
                                insert_extra_nesting(&mut Box::new(field.ty.clone()), &type_path, &type_path2);
                            }
                        } else {
                            inject_trivial_struct(&mut Box::new(field.ty.clone()));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets nested anonymous structs and unions within named structs or unions, specifically when using the unnamed_fields feature. It modifies the nesting structure by swapping the order of an anonymous struct and an anonymous union within a field, or by introducing an additional level of nesting that mixes these anonymous types. The change is made within existing field declarations; no new fields are added. The goal is to create complex nested anonymous ADT patterns that stress the compiler's field uniqueness checking logic. If the seed lacks a suitable anonymous union or struct to swap, the operator injects a trivial anonymous type using a primitive type already present in the program."
    }
}

fn find_anonymous_struct(ty: &Box<Type>) -> Option<Type> {
    match &**ty {
        Type::Path(type_path) if type_path.path.segments.iter().any(|seg| seg.ident == "struct") => Some(*ty.clone()),
        Type::Path(type_path) => {
            for seg in &type_path.path.segments {
                if let PathArguments::AngleBracketed(args) = &seg.arguments {
                    for arg in &args.args {
                        if let GenericArgument::Type(ty_arg) = arg {
                            if let Some(found) = find_anonymous_struct(&Box::new(ty_arg.clone())) {
                                return Some(found);
                            }
                        }
                    }
                }
            }
            None
        },
        _ => None,
    }
}

fn find_anonymous_union(ty: &Box<Type>) -> Option<Type> {
    match &**ty {
        Type::Path(type_path) if type_path.path.segments.iter().any(|seg| seg.ident == "union") => Some(*ty.clone()),
        Type::Path(type_path) => {
            for seg in &type_path.path.segments {
                if let PathArguments::AngleBracketed(args) = &seg.arguments {
                    for arg in &args.args {
                        if let GenericArgument::Type(ty_arg) = arg {
                            if let Some(found) = find_anonymous_union(&Box::new(ty_arg.clone())) {
                                return Some(found);
                            }
                        }
                    }
                }
            }
            None
        },
        _ => None,
    }
}

fn swap_struct_union(ty: &mut Box<Type>, struct_type: &Type, union_type: &Type) {
    let mut new_ty = ty.clone();
    if let Type::Path(ref mut outer_path) = *new_ty {
        for seg in &mut outer_path.path.segments {
            if let PathArguments::AngleBracketed(args) = &mut seg.arguments {
                for arg in &mut args.args {
                    if let GenericArgument::Type(ty_arg) = arg {
                        if let Type::Path(ref mut inner_path) = *ty_arg {
                            for inner_seg in &mut inner_path.path.segments {
                                if let PathArguments::AngleBracketed(inner_args) = &mut inner_seg.arguments {
                                    for inner_arg in &mut inner_args.args {
                                        if let GenericArgument::Type(inner_ty_arg) = inner_arg {
                                            if inner_ty_arg == union_type {
                                                *inner_ty_arg = struct_type.clone();
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
    *ty = new_ty;
}

fn insert_extra_nesting(ty: &mut Box<Type>, struct_type: &Type, union_type: &Type) {
    let mut new_ty = ty.clone();
    if let Type::Path(ref mut outer_path) = *new_2 {
        for seg in &mut outer_path.path.segments {
            if let PathArguments::AngleBracketed(args) = &mut seg.arguments {
                for arg in &mut args.args {
                    if let GenericArgument::Type(ty_arg) = arg {
                        if let Type::Path(ref mut inner_path) = *ty_arg {
                            let new_struct = Type::Path(TypePath {
                                qself: None,
                                path: parse_quote!(struct),
                            });
                            inner_path.path.segments.push(syn::PathSegment {
                                ident: Ident::new("field", Span::call_site()),
                                arguments: PathArguments::AngleBracketed(syn::AngleBracketedGenericArguments {
                                    colon2_token: None,
                                    lt_token: token::Lt::default(),
                                    args: Punctuated::from_iter(vec![
                                        GenericArgument::Type(new_struct),
                                    ]),
                                    gt_token: token::Gt::default(),
                                }),
                            });
                        }
                    }
                }
            }
        }
    }
    *ty = new_ty;
}

fn inject_trivial_union(ty: &mut Box<Type>) {
    let trivial_union = Type::Path(TypePath {
        qself: None,
        path: parse_quote!(union),
    });
    if let Type::Path(ref mut type_path) = **ty {
        type_path.path.segments.push(syn::PathSegment {
            ident: Ident::new("field", Span::call_site()),
            arguments: PathArguments::AngleBracketed(syn::AngleBracketedGenericArguments {
                colon2_token: None,
                lt_token: token::Lt::default(),
                args: Punctuated::from_iter(vec![
                    GenericArgument::Type(trivial_union),
                ]),
                gt_token: token::Gt::default(),
            }),
        });
    }
}

fn inject_trivial_struct(ty: &mut Box<Type>) {
    let trivial_struct = Type::Path(TypePath {
        qself: None,
        path: parse_quote!(struct),
    });
    if let Type::Path(ref mut type_path) = **ty {
        type_path.path.segments.push(syn::PathSegment {
            ident: Ident::new("field", Span::call_site()),
            arguments: PathArguments::AngleBracketed(syn::AngleBracketedGenericArguments {
                colon2_token: None,
                lt_token: token::Lt::default(),
                args: Punctuated::from_iter(vec![
                    GenericArgument::Type(trivial_struct),
                ]),
                gt_token: token::Gt::default(),
            }),
        });
    }
}