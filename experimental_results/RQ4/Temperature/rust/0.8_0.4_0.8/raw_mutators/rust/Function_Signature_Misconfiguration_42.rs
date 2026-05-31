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

pub struct Function_Signature_Misconfiguration_42;

impl Mutator for Function_Signature_Misconfiguration_42 {
    fn name(&self) -> &str {
        "Function_Signature_Misconfiguration_42"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let mut new_inputs = Punctuated::new();
                for input in &func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        let mut new_pat_type = pat_type.clone();
                        new_pat_type.ty = Box::new(Type::Path(TypePath {
                            qself: None,
                            path: parse_quote!(TypeX impl TraitA for<T> TraitB<'_, T = impl TraitC + '_>),
                        }));
                        new_inputs.push(FnArg::Typed(new_pat_type));
                    } else {
                        new_inputs.push(input.clone());
                    }
                }
                func.sig.inputs = new_inputs;
                if let ReturnType::Type(_, ref mut ty) = func.sig.output {
                    *ty = Box::new(Type::Path(TypePath {
                        qself: None,
                        path: parse_quote!(impl TraitC),
                    }));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}