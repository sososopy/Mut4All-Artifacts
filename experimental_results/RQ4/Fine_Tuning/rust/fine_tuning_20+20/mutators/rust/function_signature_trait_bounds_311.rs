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

pub struct Function_Signature_Trait_Bounds_311;

impl Mutator for Function_Signature_Trait_Bounds_311 {
    fn name(&self) -> &str {
        "Function_Signature_Trait_Bounds_311"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut modified = false;
                for param in &mut item_fn.sig.generics.params {
                    if let GenericParam::Type(type_param) = param {
                        for bound in &mut type_param.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                trait_bound.modifier =
                                    TraitBoundModifier::Maybe(syn::token::Question::default());
                                modified = true;
                            }
                        }
                    }
                }
                if modified {
                    item_fn.sig.constness = Some(syn::token::Const::default());
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with generic parameters that have trait bounds. It modifies the trait bounds by introducing the `~const` modifier, which is not valid in this context. Additionally, it ensures the function is a `const fn` if any trait bounds are altered. This transformation is designed to stress the compiler's handling of trait bounds in const contexts, potentially triggering errors or unexpected behavior related to trait resolution and const analysis."
    }
}