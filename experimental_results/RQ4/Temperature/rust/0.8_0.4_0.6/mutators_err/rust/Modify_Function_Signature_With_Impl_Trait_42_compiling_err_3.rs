use proc_macro2::{Span, TokenStream};
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
use syn::{
    parse_quote, BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument,
    GenericParam, Ident, Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType,
    Path as SynPath, PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type,
    TypeImplTrait, TypeParamBound, TypePath, punctuated::Punctuated, spanned::Spanned, token,
    token::Comma, token::Paren, token::Plus, visit::Visit, visit_mut::VisitMut,
};

use crate::mutator::Mutator;

pub struct Modify_Function_Signature_With_Impl_Trait_42;

impl Mutator for Modify_Function_Signature_With_Impl_Trait_42 {
    fn name(&self) -> &str {
        "Modify_Function_Signature_With_Impl_Trait_42"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Path(type_path) = &*pat_type.ty {
                            if let Some(segment) = type_path.path.segments.last() {
                                let trait_bound = match segment.ident.to_string().as_str() {
                                    "str" => "AsRef<str>",
                                    "i32" => "Into<i32>",
                                    _ => continue,
                                };
                                let trait_bound: TypeParamBound = parse_quote!(#trait_bound);
                                pat_type.ty = Box::new(Type::ImplTrait(TypeImplTrait {
                                    impl_token: token::Impl { span: Span::call_site() },
                                    bounds: {
                                        let mut bounds = Punctuated::new();
                                        bounds.push(trait_bound);
                                        bounds
                                    },
                                }));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator modifies function signatures by replacing explicit parameter types with `impl Trait` syntax, where the trait is a common trait bound implemented by the original type. This transformation aims to challenge the compiler's type inference and trait resolution mechanisms, especially in complex generic scenarios."
    }
}