use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident, Item, ItemFn,
    ItemStruct, Local, Pat, PatType, Path as SynPath, PathArguments, ReturnType, Stmt, TraitBound,
    TraitBoundModifier, Type, TypeImplTrait, TypeParamBound, TypePath, parse_quote,
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

pub struct Replace_DynStar_With_Trait_Object_33;

impl Mutator for Replace_DynStar_With_Trait_Object_33 {
    fn name(&self) -> &str {
        "Replace_DynStar_With_Trait_Object_33"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let syn::ReturnType::Type(_, ref mut ty) = func.sig.output {
                    if let Type::TraitObject(type_trait_object) = &**ty {
                        if type_trait_object.dyn_token.is_some() {
                            let bounds = &type_trait_object.bounds;
                            *ty = Box::new(Type::Path(TypePath {
                                qself: None,
                                path: parse_quote!(Box<dyn #bounds>),
                            }));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets instances where `dyn*` is used in return types. It replaces `dyn*` with `Box<dyn Trait>`, converting dynamic pointers into boxed trait objects. This transformation tests the compiler's handling of trait objects and dynamic typing, potentially exposing bugs related to type erasure and dynamic dispatch."
    }
}