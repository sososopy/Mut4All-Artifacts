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

pub struct Const_Parameter_Misplacement_403;

impl Mutator for Const_Parameter_Misplacement_403 {
    fn name(&self) -> &str {
        "Const_Parameter_Misplacement_403"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if item_struct.generics.params.is_empty() {
                    continue;
                }
                let mut has_const_generic = false;
                for param in &item_struct.generics.params {
                    if matches!(param, GenericParam::Const(_)) {
                        has_const_generic = true;
                        break;
                    }
                }
                if !has_const_generic {
                    continue;
                }
                let mut type_params = Vec::new();
                let mut const_params = Vec::new();
                for param in &item_struct.generics.params {
                    match param {
                        GenericParam::Type(type_param) => {
                            type_params.push(type_param.clone());
                        }
                        GenericParam::Const(const_param) => {
                            const_params.push(const_param.clone());
                        }
                        _ => {}
                    }
                }
                if type_params.is_empty() || const_params.is_empty() {
                    continue;
                }
                let mut new_params = Punctuated::new();
                for param in type_params {
                    new_params.push(GenericParam::Const(parse_quote! {
                        #param
                    }));
                }
                for param in const_params {
                    new_params.push(GenericParam::Type(parse_quote! {
                        #param
                    }));
                }
                item_struct.generics.params = new_params;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets structs with const generic parameters. It swaps the positions of const and type parameters in the struct's generic parameter list. By doing so, it aims to disrupt the expected order of generics, potentially causing the compiler to misinterpret the parameter roles and leading to errors in type checking or code generation related to const generics."
    }
}