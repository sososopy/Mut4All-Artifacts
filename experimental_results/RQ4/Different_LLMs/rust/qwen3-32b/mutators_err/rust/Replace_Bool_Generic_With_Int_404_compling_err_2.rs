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

pub struct Replace_Bool_Generic_With_Int_404;

impl Mutator for Replace_Bool_Generic_With_Int_404 {
    fn name(&self) -> &str {
        "Replace_Bool_Generic_With_Int_404"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct ReplaceBoolGenericArgVisitor;
        impl VisitMut for ReplaceBoolGenericArgVisitor {
            fn visit_expr_call_mut(&mut self, call: &mut ExprCall) {
                if let Expr::Path(func_path) = &mut *call.func {
                    for segment in &mut func_path.path.segments {
                        if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                            for arg in &mut args.args {
                                if let GenericArgument::Expr(expr) = arg {
                                    if let Expr::Lit(lit) = &**expr {
                                        if let Lit::Bool(_) = lit.lit {
                                            *arg = Box::new(GenericArgument::Const(Box::new(
                                                parse_quote!(0u8),
                                            )));
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                syn::visit_mut::visit_expr_call_mut(self, call);
            }
        }
        let mut visitor = ReplaceBoolGenericArgVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces boolean literal generic arguments with an integer literal (0u8), creating type mismatches in contexts expecting a bool. This tests the compiler's handling of generic const expressions and ensures type mismatches trigger errors rather than ICEs."
    }
}