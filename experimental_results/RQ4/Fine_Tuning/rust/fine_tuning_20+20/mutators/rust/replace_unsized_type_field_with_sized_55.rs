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

pub struct Replace_Unsized_Type_Field_With_Sized_55;

impl Mutator for Replace_Unsized_Type_Field_With_Sized_55 {
    fn name(&self) -> &str {
        "Replace_Unsized_Type_Field_With_Sized_55"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut has_unsized_field = false;
                for field in &item_struct.fields {
                    if let Type::Path(type_path) = &field.ty {
                        if type_path.path.is_ident("str") {
                            has_unsized_field = true;
                            break;
                        }
                    }
                    if let Type::Slice(_) = &field.ty {
                        has_unsized_field = true;
                        break;
                    }
                }
                if has_unsized_field {
                    let new_fields = item_struct
                        .fields
                        .iter()
                        .map(|field| {
                            if let Type::Path(type_path) = &field.ty {
                                if type_path.path.is_ident("str") {
                                    let new_ty: Type = parse_quote!([u8; 32]);
                                    return syn::Field {
                                        attrs: field.attrs.clone(),
                                        vis: field.vis.clone(),
                                        ident: field.ident.clone(),
                                        colon_token: field.colon_token,
                                        ty: new_ty,
                                        mutability: field.mutability.clone(),
                                    };
                                }
                            }
                            if let Type::Slice(slice) = &field.ty {
                                let elem = &slice.elem;
                                let new_ty: Type = parse_quote!([#elem; 10]);
                                return syn::Field {
                                    attrs: field.attrs.clone(),
                                    vis: field.vis.clone(),
                                    ident: field.ident.clone(),
                                    colon_token: field.colon_token,
                                    ty: new_ty,
                                    mutability: field.mutability.clone(),
                                };
                            }
                            field.clone()
                        })
                        .collect::<Vec<_>>();
                    item_struct.fields = match &item_struct.fields {
                        syn::Fields::Named(_) => syn::Fields::Named(syn::FieldsNamed {
                            brace_token: Default::default(),
                            named: new_fields.into_iter().collect(),
                        }),
                        syn::Fields::Unnamed(_) => syn::Fields::Unnamed(syn::FieldsUnnamed {
                            paren_token: Default::default(),
                            unnamed: new_fields.into_iter().collect(),
                        }),
                        syn::Fields::Unit => syn::Fields::Unit,
                    };
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets struct definitions containing unsized type fields, such as `str` or slice types `[T]`. It replaces these unsized fields with sized equivalents: `str` fields are converted to `[u8; 32]`, and slice fields are converted to fixed-size arrays like `[T; 10]`. This transformation ensures the struct becomes fully sized, preventing potential compiler issues related to size and alignment calculations for unsized types."
    }
}