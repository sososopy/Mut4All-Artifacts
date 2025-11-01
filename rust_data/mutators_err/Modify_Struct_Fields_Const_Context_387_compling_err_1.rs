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

pub struct Modify_Struct_Fields_Const_Context_387;

impl Mutator for Modify_Struct_Fields_Const_Context_387 {
    fn name(&self) -> &str {
        "Modify_Struct_Fields_Const_Context_387"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if has_const_param_ty(&item_struct.attrs) {
                    for field in &mut item_struct.fields {
                        if let Type::Path(TypePath { path, .. }) = &field.ty {
                            if let Some(ident) = path.get_ident() {
                                if ident == "i32" {
                                    field.ty = parse_quote!(u32);
                                } else if ident == "i64" {
                                    field.ty = parse_quote!(u64);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator identifies structs implementing the ConstParamTy trait and modifies the type of a field within these structs. Specifically, it changes integer types like i32 to u32 and i64 to u64. This subtle change maintains compatibility but can cause different behaviors in constant evaluation due to differences in signedness and size. The focus is on modifying the struct fields used as constant parameters without introducing compile-time errors unrelated to const param usage."
    }
}

fn has_const_param_ty(attrs: &[Attribute]) -> bool {
    attrs.iter().any(|attr| {
        if let Ok(meta) = attr.parse_meta() {
            if let Meta::List(meta_list) = meta {
                return meta_list.path.is_ident("derive")
                    && meta_list.nested.iter().any(|nested_meta| {
                        if let NestedMeta::Meta(Meta::Path(path)) = nested_meta {
                            return path.is_ident("ConstParamTy");
                        }
                        false
                    });
            }
        }
        false
    })
}