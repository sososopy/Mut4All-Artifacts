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

pub struct Insert_Slice_OutOfBounds_Length_Index_76;

impl Mutator for Insert_Slice_OutOfBounds_Length_Index_76 {
    fn name(&self) -> &str {
        "Insert_Slice_OutOfBounds_Length_Index_76"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Const(item_const) = item {
                let mut visitor = ConstBlockVisitor::new();
                visitor.visit_block_mut(&mut item_const.block);
                for (arr_name, arr_len) in visitor.arrays {
                    let new_stmt: Stmt = parse_quote! {
                        unsafe {
                            let ptr = #arr_name.as_ptr().add(#arr_name.len());
                            let fat = std::ptr::slice_from_raw_parts(ptr, usize::MAX);
                            let _ = (*fat)[usize::MAX - 1];
                        }
                    };
                    item_const.block.stmts.push(new_stmt);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const blocks containing arrays or slices. It inserts unsafe code that creates a pointer one past the end of the array, constructs a slice with usize::MAX length, and attempts to index it at usize::MAX - 1. This triggers potential CTFE overflow and out-of-bounds slice creation, testing the compiler's const evaluation safety and slice validation."
    }
}

struct ConstBlockVisitor {
    arrays: Vec<(Ident, usize)>,
}

impl ConstBlockVisitor {
    fn new() -> Self {
        Self { arrays: Vec::new() }
    }
}

impl VisitMut for ConstBlockVisitor {
    fn visit_stmt_mut(&mut self, stmt: &mut Stmt) {
        if let Stmt::Local(local) = stmt {
            if let Pat::Ident(pat_ident) = &local.pat {
                if let Some(init) = &local.init {
                    if let Expr::Array(expr_array) = &*init.expr {
                        self.arrays.push((pat_ident.ident.clone(), expr_array.elems2.len()));
                    }
                }
            }
        }
        syn::visit_mut::visit_stmt_mut(self, stmt);
    }
}