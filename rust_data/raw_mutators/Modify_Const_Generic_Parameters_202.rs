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

pub struct Modify_Const_Generic_Parameters_202;

impl Mutator for Modify_Const_Generic_Parameters_202 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Parameters_202"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            match item {
                Item::Struct(item_struct) => {
                    if let Some(generics) = &mut item_struct.generics.params.first_mut() {
                        if let GenericParam::Const(const_param) = generics {
                            if let Type::Path(type_path) = &const_param.ty {
                                if type_path.path.segments.last().unwrap().ident != "i32" {
                                    const_param.ty = parse_quote! { i32 };
                                }
                            }
                        }
                    }
                }
                Item::Fn(item_fn) => {
                    if let Some(generics) = &mut item_fn.sig.generics.params.first_mut() {
                        if let GenericParam::Const(const_param) = generics {
                            if let Type::Path(type_path) = &const_param.ty {
                                if type_path.path.segments.last().unwrap().ident != "i32" {
                                    const_param.ty = parse_quote! { i32 };
                                }
                            }
                        }
                    }
                }
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator identifies struct and function definitions using const generics with non-supported types, such as user-defined types, and modifies them to use supported types like integers, booleans, or characters. This is intended to test the compiler's handling of const generics by exploiting current limitations and incomplete features."
    }
}