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

pub struct Recursive_ImplTrait_Return_With_Borrowed_Input_156;

impl Mutator for Recursive_ImplTrait_Return_With_Borrowed_Input_156 {
    fn name(&self) -> &str {
        "Recursive_ImplTrait_Return_With_Borrowed_Input_156"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if let ReturnType::Type(_, return_type) = &func.sig.output {
                    if let Type::ImplTrait(_) = &**return_type {
                        if let Some(first_arg) = func.sig.inputs.first() {
                            if let FnArg::Typed(pat_type) = first_arg {
                                if let Pat::Ident(pat_ident) = &*pat_type.pat {
                                    let arg_name = &pat_ident.ident;
                                    let new_block = parse_quote! {
                                        {
                                            return #func.sig.ident(#arg_name);
                                        }
                                    };
                                    *func.block = new_block;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions returning `impl Trait` with borrowed input arguments. It replaces the function body with a recursive call to itself using the first argument, creating a non-universal lifetime scenario. This forces the compiler to resolve the opaque return type in a recursive context, potentially exposing bugs in lifetime inference and opaque type resolution."
    }
}