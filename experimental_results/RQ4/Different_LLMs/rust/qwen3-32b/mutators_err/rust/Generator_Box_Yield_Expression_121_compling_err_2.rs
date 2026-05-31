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

pub struct Generator_Box_Yield_Expression_121;

impl Mutator for Generator_Box_Yield_Expression_121 {
    fn name(&self) -> &str {
        "Generator_Box_Yield_Expression_121"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = YieldToBox;
        syn::visit_mut::visit_file_mut(&mut visitor, file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets generator closures by replacing `yield` expressions with `box yield` expressions. This introduces a boxed yield, creating a type mismatch in the generator's state machine. It stresses the compiler's MIR validation logic by forcing it to handle boxed and unboxed yields, potentially exposing bugs in generator state transitions and type inference for coroutine outputs."
    }
}

struct YieldToBox;

impl<'ast> VisitMut for YieldToBox {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Yield(yield_expr) = expr {
            let yield_expr_ref = &*yield_expr;
            let span = yield_expr_ref.yield_token.span;
            let inner_expr = Expr::Yield(*yield_expr); // Take ownership
            let box_expr = parse_quote! { box #inner_expr };
            *expr = box_expr;
        } else {
            syn::visit_mut::visit_expr_mut(self, expr);
        }
    }
}