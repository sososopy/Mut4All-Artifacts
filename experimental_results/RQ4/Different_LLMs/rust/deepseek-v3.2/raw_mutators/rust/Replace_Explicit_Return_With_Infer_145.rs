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

pub struct Replace_Explicit_Return_With_Infer_145;

impl Mutator for Replace_Explicit_Return_With_Infer_145 {
    fn name(&self) -> &str {
        "Replace_Explicit_Return_With_Infer_145"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, _) = &item_fn.sig.output {
                    if item_fn.sig.ident != "main" {
                        item_fn.sig.output = ReturnType::Type(Default::default(), Box::new(Type::Infer(TypeInfer { underscore_token: Default::default() })));
                    }
                }
            }
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(func) = impl_item {
                        if let ReturnType::Type(_, _) = &func.sig.output {
                            func.sig.output = ReturnType::Type(Default::default(), Box::new(Type::Infer(TypeInfer { underscore_token: Default::default() })));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces explicit function return types with an underscore placeholder `_`. This forces the compiler to infer the return type from the function body, potentially leading to unexpected internal type representations during variance or borrow-checking computations. It targets functions with non-unit return types, excluding trait declarations and external signatures where placeholders are syntactically invalid."
    }
}