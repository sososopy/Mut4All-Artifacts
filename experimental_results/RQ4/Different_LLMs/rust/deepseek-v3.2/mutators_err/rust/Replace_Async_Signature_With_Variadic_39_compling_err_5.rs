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

pub struct Replace_Async_Signature_With_Variadic_39;

impl Mutator for Replace_Async_Signature_With_Variadic_39 {
    fn name(&self) -> &str {
        "Replace_Async_Signature_With_Variadic_39"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if !item_fn.sig.asyncness.is_some() {
                    continue;
                }
                let lifetime_count = item_fn.sig.generics.params.iter().filter(|p| matches!(p, GenericParam::Lifetime(_))).count();
                if lifetime_count == 0 {
                    continue;
                }
                let regular_param_count = item_fn.sig.inputs.iter().filter(|arg| matches!(arg, FnArg::Typed(_))).count();
                if regular_param_count == 0 {
                    continue;
                }
                let is_variadic = item_fn.sig.inputs.iter().any(|arg| matches!(arg, FnArg::Receiver(_)));
                if is_variadic {
                    continue;
                }
                let last_input = item_fn.sig.inputs.last();
                if let Some(FnArg::Typed(pat_type)) = last_input {
                    let comma_token = token::Comma::default();
                    let variadic_arg = FnArg::Typed(PatType {
                        attrs: Vec::new(),
                        pat: Box::new(Pat::Wild(syn::PatWild {
                            attrs: Vec::new(),
                            underscore_token: token::Underscore::default(),
                        })),
                        colon_token: token::Colon::default(),
                        ty: Box::new(Type::Verbatim(quote! { ... })),
                    });
                    item_fn.sig.inputs.push_punct(comma_token);
                    item_fn.sig.inputs.push(variadic_arg);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets async function declarations with multiple named lifetime parameters and at least one regular parameter. It changes the function signature to be C-variadic by appending a variadic argument ... after the last parameter, while keeping the original lifetime parameters unchanged. This introduces a variadic argument list that mismatches the lifetime count, potentially triggering index out of bounds errors in the compiler's handling of variadic async functions. The mutation does not introduce new types or variables; it only modifies the signature."
    }
}