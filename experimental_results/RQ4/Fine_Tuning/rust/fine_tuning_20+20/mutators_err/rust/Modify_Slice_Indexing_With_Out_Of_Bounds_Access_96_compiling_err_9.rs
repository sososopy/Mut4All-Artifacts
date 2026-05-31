use proc_macro2::{Span, *};
use quote::*;
use rand::{seq::SliceRandom, thread_rng, Rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
use syn::{
    parse_quote,
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

pub struct Modify_Slice_Indexing_With_Out_Of_Bounds_Access_96;

impl Mutator for Modify_Slice_Indexing_With_Out_Of_Bounds_Access_96 {
    fn name(&self) -> &str {
        "Modify_Slice_Indexing_With_Out_Of_Bounds_Access_96"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(func) = impl_item {
                        for stmt in &mut func.block.stmts {
                            if let Stmt::Local(local) = stmt {
                                if let Some(LocalInit { expr, .. }) = &local.init {
                                    if let Expr::Call(expr_call) = expr.as_ref() {
                                        if let Expr::Path(ExprPath { path, .. }) =
                                            expr_call.func.as_ref()
                                        {
                                            if path.is_ident("copy_from_slice") {
                                                let mut new_args = Punctuated::new();
                                                for arg in &expr_call.args {
                                                    if let Expr::Reference(expr_ref) = arg {
                                                        if let Expr::Field(expr_field) =
                                                            expr_ref.expr.as_ref()
                                                        {
                                                            if let Member::Named(ident) =
                                                                &expr_field.member
                                                            {
                                                                if ident == "buffer" {
                                                                    new_args.push(parse_quote!(
                                                                        &self.buffer[self.buffer.len()..self.buffer.len() + 1]
                                                                    ));
                                                                    continue;
                                                                }
                                                            }
                                                        }
                                                    }
                                                    new_args.push(arg.clone());
                                                }
                                                *expr_call = ExprCall {
                                                    attrs: expr_call.attrs.clone(),
                                                    func: expr_call.func.clone(),
                                                    paren_token: expr_call.paren_token,
                                                    args: new_args,
                                                };
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "Identify slice indexing expressions within function bodies and modify them to access an out-of-bounds range, specifically by replacing `self.buffer[..]` with `self.buffer[self.buffer.len()..self.buffer.len() + 1]`, to trigger boundary check errors and test the compiler's robustness against out-of-bounds slice operations."
    }
}