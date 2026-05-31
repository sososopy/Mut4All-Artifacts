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

pub struct Modify_Array_Length_In_Structs_659;

impl Mutator for Modify_Array_Length_In_Structs_659 {
    fn name(&self) -> &str {
        "Modify_Array_Length_In_Structs_659"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                if let Some((_, generics)) = item_struct.generics.params.iter().enumerate().find(|(_, param)| {
                    if let GenericParam::Const(_) = param {
                        true
                    } else {
                        false
                    }
                }) {
                    if let syn::Fields::Unnamed(fields) = &mut item_struct.fields {
                        for field in &mut fields.unnamed {
                            if let Type::Array(type_array) = &mut field.ty {
                                if let Expr::Call(expr_call) = &mut type_array.len {
                                    if let Expr::Path(expr_path) = &*expr_call.func {
                                        if expr_path.path.segments.last().unwrap().ident == "get" {
                                            expr_call.func = Box::new(Expr::Path(ExprPath {
                                                attrs: vec![],
                                                qself: None,
                                                path: parse_quote!(N.get().wrapping_add(1)),
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
    fn chain_of_thought(&self) -> &str {
        ""
    }
}