use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet,
    default,
    fs,
    ops::Range,
    panic,
    path::Path,
    process::Command,
};
use syn::{
    BoundLifetimes,
    Expr,
    ExprCall,
    ExprPath,
    File,
    FnArg,
    GenericArgument,
    GenericParam,
    Ident,
    Item,
    ItemFn,
    ItemStruct,
    Lifetime,
    LifetimeParam,
    Local,
    Pat,
    PatType,
    Path as SynPath,
    PathArguments,
    ReturnType,
    Stmt,
    TraitBound,
    TraitBoundModifier,
    Type,
    TypeImplTrait,
    TypeParamBound,
    TypePath,
    parse_quote,
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

pub struct Substitute_Lifetime_Parameter_266;

impl Mutator for Substitute_Lifetime_Parameter_266 {
    fn name(&self) -> &str {
        "Substitute_Lifetime_Parameter_266"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(impl_item) = item {
                if let Some(generics) = &impl_item.generics {
                    if let Some(lifetimes) = &generics.lifetimes {
                        let lifetimes: Vec<LifetimeParam> = lifetimes.params.iter().cloned().collect();
                        for impl_item in &mut impl_item.items {
                            if let ImplItem::Fn(func) = impl_item {
                                if let Some(generics) = &mut func.sig.generics {
                                    for param in &mut generics.params {
                                        if let GenericParam::Lifetime(lifetime) = param {
                                            if let Some(replacement) = lifetimes.choose(&mut thread_rng()) {
                                                lifetime.lifetime = replacement.lifetime.clone();
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
        "The mutation operator substitutes lifetime parameters in trait implementations with other lifetime parameters declared in the same scope or introduces new lifetime parameters. This transformation tests the compiler's handling of lifetime parameters in trait implementations and can help expose bugs related to lifetime parameter usage."
    }
}