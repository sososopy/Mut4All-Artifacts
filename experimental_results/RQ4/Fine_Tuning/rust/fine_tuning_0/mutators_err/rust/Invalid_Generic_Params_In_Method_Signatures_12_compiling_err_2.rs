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

pub struct Invalid_Generic_Params_In_Method_Signatures_12;

impl Mutator for Invalid_Generic_Params_In_Method_Signatures_12 {
    fn name(&self) -> &str {
        "Invalid_Generic_Params_In_Method_Signatures_12"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        if let syn::ReturnType::Type(_, ref mut ty) = method.sig.output {
                            if let Type::Path(type_path) = &**ty {
                                if let Some(last_segment) = type_path.path.segments.last_mut() {
                                    if last_segment.ident == "Iter" || last_segment.ident == "IterMut" {
                                        if let PathArguments::AngleBracketed(angle_bracketed) = &mut last_segment.arguments {
                                            for arg in &mut angle_bracketed.args {
                                                if let GenericArgument::Type(arg_type) = arg {
                                                    *arg_type = parse_quote!(NonExistentType);
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
        }
    }

    fn chain_of_thought(&self) -> &str {
        "This mutator targets methods returning iterator types within impl blocks, modifying their generic parameters to an invalid type. By doing so, it aims to test the compiler's handling of incorrect generic parameters in method signatures, potentially leading to ICEs or error messages related to type resolution."
    }
}