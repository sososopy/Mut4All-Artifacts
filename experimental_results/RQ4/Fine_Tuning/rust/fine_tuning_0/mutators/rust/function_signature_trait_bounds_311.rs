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

pub struct Function_Signature_Trait_Bounds_311;

impl Mutator for Function_Signature_Trait_Bounds_311 {
    fn name(&self) -> &str {
        "Function_Signature_Trait_Bounds_311"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }

                // Ensure the function is a const fn
                func.sig.constness.get_or_insert(token::Const {
                    span: Span::call_site(),
                });

                // Iterate over the generic parameters
                for param in &mut func.sig.generics.params {
                    if let GenericParam::Type(type_param) = param {
                        // Modify trait bounds by introducing `~const`
                        for bound in &mut type_param.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                trait_bound.modifier = TraitBoundModifier::None;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with generic parameters having trait bounds. It modifies these bounds by introducing `~const`, ensuring the function is a `const fn`. This transformation aims to test the compiler's handling of trait bounds in const contexts, potentially leading to errors or unexpected behavior."
    }
}