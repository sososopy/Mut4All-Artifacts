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

pub struct Alter_Function_Signature_With_Nested_Associated_Type_219;

impl Mutator for Alter_Function_Signature_With_Nested_Associated_Type_219 {
    fn name(&self) -> &str {
        "Alter_Function_Signature_With_Nested_Associated_Type_219"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(ref mut func) = item {
                if let Some(FnArg::Typed(PatType { ty, .. })) = func.sig.inputs.first_mut() {
                    if let Type::Path(TypePath { path, .. }) = &mut **ty {
                        if let Some(last_segment) = path.segments.last_mut() {
                            if let PathArguments::AngleBracketed(ref mut args) = last_segment.arguments {
                                if let Some(GenericArgument::Type(Type::Path(TypePath { path: nested_path, .. }))) = args.args.first_mut() {
                                    if let Some(last_nested_segment) = nested_path.segments.last_mut() {
                                        if let PathArguments::AngleBracketed(ref mut nested_args) = last_nested_segment.arguments {
                                            nested_args.args.push(GenericArgument::Type(Type::Path(TypePath {
                                                path: nested_path.clone(),
                                                qself: None,
                                            })));
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
        "The mutation operator targets function signatures that involve associated types or lifetimes. It deepens the nesting of associated types within the function's signature, specifically aiming to create a deep nesting structure. This is done by modifying the type arguments of the function's parameters, introducing complexity in how lifetimes and associated types are resolved. The mutation emphasizes the relationship of bound regions and potential conflicts with lifetimes, potentially triggering compiler issues."
    }
}