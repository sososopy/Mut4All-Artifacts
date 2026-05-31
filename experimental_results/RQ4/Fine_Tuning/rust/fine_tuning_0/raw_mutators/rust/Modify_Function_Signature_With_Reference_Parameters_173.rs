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

pub struct Modify_Function_Signature_With_Reference_Parameters_173;

impl Mutator for Modify_Function_Signature_With_Reference_Parameters_173 {
    fn name(&self) -> &str {
        "Modify_Function_Signature_With_Reference_Parameters_173"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Reference(type_reference) = &mut *pat_type.ty {
                            if type_reference.mutability.is_some() {
                                type_reference.mutability = None;
                            } else {
                                type_reference.mutability = Some(token::Mut {
                                    span: Span::call_site(),
                                });
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets function definitions with reference parameters, altering their mutability. By switching between mutable and immutable references, the transformation tests the compiler's handling of reference mutability, especially in contexts involving inline assembly or `extern \"C\"` functions."
    }
}