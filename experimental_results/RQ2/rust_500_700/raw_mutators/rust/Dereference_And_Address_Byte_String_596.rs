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

pub struct Dereference_And_Address_Byte_String_596;

impl Mutator for Dereference_And_Address_Byte_String_596 {
    fn name(&self) -> &str {
        "Dereference_And_Address_Byte_String_596"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut visitor = ByteStringVisitor;
                visitor.visit_block_mut(&mut item_fn.block);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets byte string literals and transforms them by applying the dereference and address-of operations. This transformation aims to expose potential bugs in the compiler's handling of such operations, particularly in relation to LLVM's optimization and code generation processes."
    }
}

struct ByteStringVisitor;

impl VisitMut for ByteStringVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        match expr {
            Expr::Lit(expr_lit) => {
                if let syn::Lit::ByteStr(_) = &expr_lit.lit {
                    *expr = parse_quote!(&*#expr);
                }
            }
            _ => visit_mut::visit_expr_mut(self, expr),
        }
    }
}