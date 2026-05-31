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

pub struct Change_Const_Type_Mutator_394;

impl Mutator for Change_Const_Type_Mutator_394 {
    fn name(&self) -> &str {
        "Change_Const_Type_Mutator_394"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(struct_item) = item {
                if let Some(param) = struct_item.generics.params.first_mut() {
                    if let GenericParam::Const(param) = param {
                        let new_type = match &param.ty {
                            Type::Path(TypePath {
                                qself: None,
                                path: SynPath {
                                    leading_colon: None,
                                    segments,
                                },
                            }) => {
                                let segment = segments.first().unwrap();
                                match segment.ident.to_string().as_str() {
                                    "i64" => parse_quote!(usize),
                                    "usize" => parse_quote!(i64),
                                    _ => continue,
                                }
                            }
                            _ => continue,
                        };
                        param.ty = Box::new(new_type);
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The Change_Const_Type_Mutator_394 mutation operator changes the type of constant parameters in struct definitions. It targets generic const parameters and swaps their types between i64 and usize. This transformation aims to test the compiler's handling of constant type declarations and generic type constraints, potentially revealing bugs in type checking or const evaluation."
    }
}