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

pub struct Swap_Nested_Anonymous_ADT_141;

impl Mutator for Swap_Nested_Anonymous_ADT_141 {
    fn name(&self) -> &str {
        "Swap_Nested_Anonymous_ADT_141"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                for field in &mut item_struct.fields {
                    if let Some(Type::Struct(type_struct)) = find_anonymous_struct(&field.ty) {
                        if let Some(Type::Union(type_union)) = find_anonymous_union(&field.ty) {
                            if rng.gen_bool(0.5) {
                                swap_struct_union(&mut field.ty, type_struct, type_union);
                            } else {
                                insert_extra_nesting(&mut field.ty, type_struct, type_union);
                            }
                        } else {
                            inject_trivial_union(&mut field.ty);
                        }
                    } else if let Some(Type::Union(type_union)) = find_anonymous_union(&field.ty) {
                        if let Some(Type::Struct(type_struct)) = find_anonymous_struct(&field.ty) {
                            if rng.gen_bool(0.5) {
                                swap_struct_union(&mut field.ty, type_struct, type_union);
                            } else {
                                insert_extra_nesting(&mut field.ty, type_struct, type_union);
                            }
                        } else {
                            inject_trivial_struct(&mut field.ty);
                        }
                    }
                }
            }
            if let Item::Union(item_union) = item {
                for field in &mut item_union.fields {
                    if let Some(Type::Struct(type_struct)) = find_anonymous_struct(&field.ty) {
                        if let Some(Type::Union(type_union)) = find_anonymous_union(&field.ty) {
                            if rng.gen_bool(0.5) {
                                swap_struct_union(&mut field.ty, type_struct, type_union);
                            } else {
                                insert_extra_nesting(&mut field.ty, type_struct, type_union);
                            }
                        } else {
                            inject_trivial_union(&mut field.ty);
                        }
                    } else if let Some(Type::Union(type_union)) = find_anonymous_union(&field.ty) {
                        if let Some(Type::Struct(type_struct)) = find_anonymous_struct(&field.ty) {
                            if rng.gen_bool(0.5) {
                                swap_struct_union(&mut field.ty, type_struct, type_union);
                            } else {
                                insert_extra_nesting(&mut field.ty, type_struct, type_union);
                            }
                        } else {
                            inject_trivial_struct(&mut field.ty);
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

fn find_anonymous_struct(ty: &Box<Type>) -> Option<&Type> {
    match &**ty {
        Type::Struct(type_struct) if type_struct.fields.iter().any(|f| f.ident.is_none()) => Some(&**ty),
        Type::Struct(type_struct) => {
            for field in &type_struct.fields {
                if let Some(found) = find_anonymous_struct(&field.ty) {
                    return Some(found);
                }
            }
            None
        },
        Type::Union(type_union) => {
            for field in &type_union.fields {
                if let Some(found) = find_anonymous_struct(&field.ty) {
                    return Some(found);
                }
            }
            None
        },
        _ => None,
    }
}

fn find_anonymous_union(ty: &Box<Type>) -> Option<&Type> {
    match &**ty {
        Type::Union(type_union) if type_union.fields.iter().any(|f| f.ident.is_none()) => Some(&**ty),
        Type::Struct(type_struct) => {
            for field in &type_struct.fields {
                if let Some(found) = find_anonymous_union(&field.ty) {
                    return Some(found);
                }
            }
            None
        },
        Type::Union(type_union) => {
            for field in &type_union.fields {
                if let Some(found) = find_anonymous_union(&field.ty) {
                    return Some(found);
                }
            }
            None
        },
        _ => None,
    }
}

fn swap_struct_union(ty: &mut Box<Type>, struct_type: &Type, union_type: &Type) {
    let mut new_ty = ty.clone();
    if let Type::Struct(ref mut outer_struct) = *new_ty {
        for field in &mut outer_struct.fields {
            if let Type::Struct(ref mut inner_struct) = *field.ty {
                for inner_field in &mut inner_struct.fields {
                    if &**inner_field.ty == union_type {
                        inner_field.ty = Box::new(struct_type.clone());
                    }
                }
            } else if let Type::Union(ref mut inner_union) = *field.ty {
                for inner_field in &mut inner_union.fields {
                    if &**inner_field.ty == struct_type {
                        inner_field.ty = Box::new(union_type.clone());
                    }
                }
            }
        }
    } else if let Type::Union(ref mut outer_union) = *new_ty {
        for field in &mut outer_union.fields {
            if let Type::Struct(ref mut inner_struct) = *field.ty {
                for inner_field in &mut inner_struct.fields {
                    if &**inner_field.ty == union_type {
                        inner_field.ty = Box::new(struct_type.clone());
                    }
                }
            } else if let Type::Union(ref mut inner_union) = *field.ty {
                for inner_field in &mut inner_union.fields {
                    if &**inner_field.ty == struct_type {
                        inner_field.ty = Box::new(union_type.clone());
                    }
                }
            }
        }
    }
    *ty = new_ty;
}

fn insert_extra_nesting(ty: &mut Box<Type>, struct_type: &Type, union_type: &Type) {
    let mut new_ty = ty.clone();
    if let Type::Struct(ref mut outer_struct) = *new_ty {
        for field in &mut outer_struct.fields {
            if let Type::Struct(ref mut inner_struct) = *field.ty {
                let new_struct = Type::Struct(TypeStruct {
                    struct_token: token::Struct::default(),
                    fields: Fields::Named(Punctuated::from_iter(vec![
                        Field {
                            attrs: vec![],
                            ident: None,
                            colon_token: Some(token::Colon::default()),
                            ty: Box::new(union_type.clone()),
                        },
                    ])),
                });
                inner_struct.fields.push(Field {
                    attrs: vec![],
                    ident: None,
                    colon_token: Some(token::Colon::default()),
                    ty: Box::new(new_struct),
                });
            }
        }
    } else if let Type::Union(ref mut outer_union) = *new_ty {
        for field in &mut outer_union.fields {
            if let Type::Union(ref mut inner_union) = *field.ty {
                let new_union = Type::Union(TypeUnion {
                    union_token: token::Union::default(),
                    fields: Fields::Named(Punctuated::from_iter(vec![
                        Field {
                            attrs: vec![],
                            ident: None,
                            colon_token: Some(token::Colon::default()),
                            ty: Box::new(struct_type.clone()),
                        },
                    ])),
                });
                inner_union.fields.push(Field {
                    attrs: vec![],
                    ident: None,
                    colon_token: Some(token::Colon::default()),
                    ty: Box::new(new_union),
                });
            }
        }
    }
    *ty = new_ty;
}

fn inject_trivial_union(ty: &mut Box<Type>) {
    let trivial_union = Type::Union(TypeUnion {
        union_token: token::Union::default(),
        fields: Fields::Named(Punctuated::from_iter(vec![
            Field {
                attrs: vec![],
                ident: Some(Ident::new("x", Span::call_site())),
                colon_token: Some(token::Colon::default()),
                ty: Box::new(Type::Path(TypePath {
                    qself: None,
                    path: SynPath::from(Ident::new("u8", Span::call_site())),
                })),
            },
        ])),
    });
    if let Type::Struct(ref mut type_struct) = **ty {
        type_struct.fields.push(Field {
            attrs: vec![],
            ident: None,
            colon_token: Some(token::Colon::default()),
            ty: Box::new(trivial_union),
        });
    } else if let Type::Union(ref mut type_union) = **ty {
        type_union.fields.push(Field {
            attrs: vec![],
            ident: None,
            colon_token: Some(token::Colon::default()),
            ty: Box::new(trivial_union),
        });
    }
}

fn inject_trivial_struct(ty: &mut Box<Type>) {
    let trivial_struct = Type::Struct(TypeStruct {
        struct_token: token::Struct::default(),
        fields: Fields::Named(Punctuated::from_iter(vec![
            Field {
                attrs: vec![],
                ident: Some(Ident::new("x", Span::call_site())),
                colon_token: Some(token::Colon::default()),
                ty: Box::new(Type::Path(TypePath {
                    qself: None,
                    path: SynPath::from(Ident::new("u8", Span::call_site())),
                })),
            },
        ])),
    });
    if let Type::Struct(ref mut type_struct) = **ty {
        type_struct.fields.push(Field {
            attrs: vec![],
            ident: None,
            colon_token: Some(token::Colon::default()),
            ty: Box::new(trivial_struct),
        });
    } else if let Type::Union(ref mut type_union) = **ty {
        type_union.fields.push(Field {
            attrs: vec![],
            ident: None,
            colon_token: Some(token::Colon::default()),
            ty: Box::new(trivial_struct),
        });
    }
}