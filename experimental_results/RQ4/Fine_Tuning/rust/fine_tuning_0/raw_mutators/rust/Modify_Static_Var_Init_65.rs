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

pub struct Modify_Static_Var_Init_65;

impl Mutator for Modify_Static_Var_Init_65 {
    fn name(&self) -> &str {
        "Modify_Static_Var_Init_65"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Static(mut static_item) = item {
                if let Type::Path(type_path) = &*static_item.ty {
                    if type_path.path.segments.last().unwrap().ident == "AtomicUsize" {
                        if let Expr::Call(expr_call) = &mut *static_item.expr {
                            if let Expr::Path(expr_path) = &*expr_call.args[0] {
                                if expr_path.path.segments.last().unwrap().ident == "new" {
                                    expr_call.args[0] = parse_quote! { 0.0 };
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets static variables of type `AtomicUsize` and modifies their initialization by introducing a type mismatch. This transformation aims to provoke compiler errors during constant evaluation by replacing the expected integer initialization with a floating-point number, testing the compiler's robustness in handling type mismatches in static contexts."
    }
}