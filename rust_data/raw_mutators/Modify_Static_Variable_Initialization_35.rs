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

pub struct Modify_Static_Variable_Initialization_35;

impl Mutator for Modify_Static_Variable_Initialization_35 {
    fn name(&self) -> &str {
        "Modify_Static_Variable_Initialization_35"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Static(ref mut static_item) = item {
                if let Type::Path(TypePath { path: ref type_path, .. }) = *static_item.ty {
                    if type_path.segments.iter().any(|seg| seg.ident == "AtomicUsize") {
                        if let Expr::Call(ref mut expr_call) = *static_item.expr {
                            if let Some(Expr::Lit(expr_lit)) = expr_call.args.first_mut() {
                                if let syn::Lit::Int(ref mut lit_int) = expr_lit.lit {
                                    *lit_int = syn::LitInt::new("0.0", lit_int.span());
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator identifies static variables of type AtomicUsize and modifies their initialization expression to introduce a type mismatch by changing an integer literal to a floating-point literal. This aims to expose type-related issues during compilation."
    }
}