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

pub struct Alter_Static_Variable_Declaration_74;

impl Mutator for Alter_Static_Variable_Declaration_74 {
    fn name(&self) -> &str {
        "Alter_Static_Variable_Declaration_74"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Static(item_static) = item {
                let ty_str = item_static.ty.to_token_stream().to_string();
                if ty_str.contains("Wrapper") {
                    if item_static.mutability.is_some() {
                        item_static.mutability = None;
                    } else {
                        item_static.mutability = Some(token::Mut {
                            span: Span::call_site(),
                        });
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets static variables that use external types or involve unsafe operations, such as those relying on FFI. It alters the mutability of these static variables, switching between mutable and immutable states. This transformation challenges the compiler's handling of static variable mutability, especially in the context of safety guarantees and const evaluation, potentially exposing issues with borrow checking, synchronization, and code generation."
    }
}