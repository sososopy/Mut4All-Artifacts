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

pub struct Function_Signature_Trait_Bounds_121;

impl Mutator for Function_Signature_Trait_Bounds_121 {
    fn name(&self) -> &str {
        "Function_Signature_Trait_Bounds_121"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut has_trait_bounds = false;
                for param in &mut item_fn.sig.generics.params {
                    if let GenericParam::Type(type_param) = param {
                        for bound in &mut type_param.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                has_trait_bounds = true;
                                if !item_fn.sig.constness.is_some() {
                                    item_fn.sig.constness = Some(token::Const::default());
                                }
                                trait_bound.modifier = TraitBoundModifier::MaybeConst(token::TildeConst {
                                    span: Span::call_site(),
                                });
                            }
                        }
                    }
                }
                if has_trait_bounds {
                    item_fn.sig.constness = Some(token::Const::default());
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets function parameter trait bounds by introducing `~const` in contexts where it is not valid. It modifies the function signature to be a `const fn` if not already, ensuring that the mutation affects the const context."
    }
}