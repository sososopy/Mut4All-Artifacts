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

pub struct Modify_Const_Trait_Bound_32;

impl Mutator for Modify_Const_Trait_Bound_32 {
    fn name(&self) -> &str {
        "Modify_Const_Trait_Bound_32"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                let has_const_modifier = item_impl.defaultness.is_some();
                let mut should_modify = false;

                // Decide whether to add or remove the `const` keyword
                if has_const_modifier {
                    should_modify = thread_rng().gen_bool(0.5);
                } else {
                    should_modify = thread_rng().gen_bool(0.5);
                }

                if should_modify {
                    if has_const_modifier {
                        // Remove `const` keyword
                        item_impl.defaultness = None;
                    } else {
                        // Add `const` keyword
                        item_impl.defaultness = Some(syn::Default::Const(token::Const {
                            span: Span::call_site(),
                        }));
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets trait implementations and modifies the presence of the `const` keyword. By randomly deciding to add or remove the `const` keyword, it tests the compiler's handling of const trait implementations, which are currently unstable, aiming to expose potential issues in the Rust compiler's support for const contexts."
    }
}