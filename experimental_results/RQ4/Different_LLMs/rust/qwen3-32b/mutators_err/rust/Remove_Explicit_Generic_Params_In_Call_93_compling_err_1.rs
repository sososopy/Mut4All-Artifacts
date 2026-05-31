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

pub struct Remove_Explicit_Generic_Params_In_Call_93;

impl Mutator for Remove_Explicit_Generic_Params_In_Call_93 {
    fn name(&self) -> &str {
        "Remove_Explicit_Generic_Params_In_Call_93"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct CallVisitor;
        impl VisitMut for CallVisitor {
            fn visit_expr_call_mut(&mut self, call: &mut ExprCall) {
                if let Expr::Path(path) = &mut *call.func {
                    for segment in &mut path.path.segments {
                        segment.arguments = PathArguments::None;
                    }
                }
                visit::visit_expr_call_mut(self, call);
            }
        }
        let mut visitor = CallVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator removes explicit generic parameters from function calls where the return type depends on an associated constant. This exploits the compiler's type inference for const parameters involving associated constants, potentially exposing bugs in const generic evaluation when type parameters are elided but required for evaluating [(); T::VALUE]: Sized constraints."
    }
}