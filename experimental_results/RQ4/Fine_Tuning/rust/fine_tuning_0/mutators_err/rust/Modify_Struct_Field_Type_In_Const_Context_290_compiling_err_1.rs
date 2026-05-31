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

pub struct Modify_Struct_Field_Type_In_Const_Context_290;

impl Mutator for Modify_Struct_Field_Type_In_Const_Context_290 {
    fn name(&self) -> &str {
        "Modify_Struct_Field_Type_In_Const_Context_290"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                if item_struct.attrs.iter().any(|attr| attr.path.is_ident("derive")) {
                    if item_struct.attrs.iter().any(|attr| attr.tokens.to_string().contains("ConstParamTy")) {
                        for field in &mut item_struct.fields {
                            if let syn::Type::Path(type_path) = &mut field.ty {
                                if type_path.path.segments.last().unwrap().ident == "i32" {
                                    type_path.path.segments.last_mut().unwrap().ident = Ident::new("u32", Span::call_site());
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets structs implementing the ConstParamTy trait, modifying the type of a field from i32 to u32. This subtle change in signedness and size can affect constant evaluation, potentially revealing differences in behavior without causing direct compile-time errors unrelated to const param usage."
    }
}