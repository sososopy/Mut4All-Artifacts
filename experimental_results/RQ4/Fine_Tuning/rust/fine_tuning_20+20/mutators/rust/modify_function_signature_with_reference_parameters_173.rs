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

pub struct Modify_Function_Signature_With_Reference_Parameters_173;

impl Mutator for Modify_Function_Signature_With_Reference_Parameters_173 {
    fn name(&self) -> &str {
        "Modify_Function_Signature_With_Reference_Parameters_173"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                for input in &mut item_fn.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Reference(type_reference) = &*pat_type.ty {
                            let mutability = type_reference.mutability;
                            let new_reference = if mutability.is_some() {
                                Type::Reference(TypeReference {
                                    and_token: token::And::default(),
                                    lifetime: type_reference.lifetime.clone(),
                                    mutability: None,
                                    elem: type_reference.elem.clone(),
                                })
                            } else {
                                Type::Reference(TypeReference {
                                    and_token: token::And::default(),
                                    lifetime: type_reference.lifetime.clone(),
                                    mutability: Some(token::Mut::default()),
                                    elem: type_reference.elem.clone(),
                                })
                            };
                            pat_type.ty = Box::new(new_reference);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with reference parameters, altering their mutability by toggling between immutable and mutable references. It challenges the compiler's handling of reference mutability, particularly in contexts like inline assembly, where precise type semantics are crucial."
    }
}