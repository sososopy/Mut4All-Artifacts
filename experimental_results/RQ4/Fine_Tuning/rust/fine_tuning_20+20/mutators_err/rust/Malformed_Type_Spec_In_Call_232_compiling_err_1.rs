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

pub struct Malformed_Type_Spec_In_Call_232;

impl Mutator for Malformed_Type_Spec_In_Call_232 {
    fn name(&self) -> &str {
        "Malformed_Type_Spec_In_Call_232"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut visitor = TypeSpecCallVisitor::default();
                visitor.visit_block_mut(&mut item_fn.block);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets function call expressions with complex type specifications, particularly those using '::' for type qualification. It replaces valid type paths with malformed constructs like 'Layout::(x: !)' to mimic unexpected token sequences and trigger parsing errors, exploiting potential weaknesses in the compiler's handling of type specifications."
    }
}

#[derive(Default)]
struct TypeSpecCallVisitor {
    target_indices: Vec<usize>,
}

impl VisitMut for TypeSpecCallVisitor {
    fn visit_expr_call_mut(&mut self, node: &mut ExprCall) {
        let mut target_indices = Vec::new();
        for (index, arg) in node.args.iter().enumerate() {
            if let Expr::Path(expr_path) = arg {
                if expr_path.path.segments.len() > 1 {
                    target_indices.push(index);
                }
            }
        }
        self.target_indices = target_indices;
        syn::visit_mut::visit_expr_call_mut(self, node);
    }

    fn visit_block_mut(&mut self, node: &mut Block) {
        for stmt in &mut node.stmts {
            self.visit_stmt_mut(stmt);
        }
    }

    fn visit_stmt_mut(&mut self, node: &mut Stmt) {
        match node {
            Stmt::Local(local) => self.visit_local_mut(local),
            Stmt::Item(item) => self.visit_item_mut(item),
            Stmt::Expr(expr, _) => self.visit_expr_mut(expr),
            Stmt::Macro(mac) => self.visit_macro_mut(mac),
        }
    }

    fn visit_expr_mut(&mut self, node: &mut Expr) {
        match node {
            Expr::Array(expr) => self.visit_expr_array_mut(expr),
            Expr::Assign(expr) => self.visit_expr_assign_mut(expr),
            Expr::AssignOp(expr) => self.visit_expr_assign_op_mut(expr),
            Expr::Async(expr) => self.visit_expr_async_mut(expr),
            Expr::Await(expr) => self.visit_expr_await_mut(expr),
            Expr::Binary(expr) => self.visit_expr_binary_mut(expr),
            Expr::Block(expr) => self.visit_expr_block_mut(expr),
            Expr::Box(expr) => self.visit_expr_box_mut(expr),
            Expr::Break(expr) => self.visit_expr_break_mut(expr),
            Expr::Call(expr) => {
                if !self.target_indices.is_empty() {
                    let mut new_args = Punctuated::new();
                    for (index, arg) in expr.args.iter().enumerate() {
                        if self.target_indices.contains(&index) {
                            new_args.push(parse_quote!(Layout::(x: !)));
                        } else {
                            new_args.push(arg.clone());
                        }
                    }
                    expr.args = new_args;
                }
                self.visit_expr_call_mut(expr);
            }
            Expr::Cast(expr) => self.visit_expr_cast_mut(expr),
            Expr::Closure(expr) => self.visit_expr_closure_mut(expr),
            Expr::Const(expr) => self.visit_expr_const_mut(expr),
            Expr::Continue(expr) => self.visit_expr_continue_mut(expr),
            Expr::Field(expr) => self.visit_expr_field_mut(expr),
            Expr::ForLoop(expr) => self.visit_expr_for_loop_mut(expr),
            Expr::Group(expr) => self.visit_expr_group_mut(expr),
            Expr::If(expr) => self.visit_expr_if_mut(expr),
            Expr::Index(expr) => self.visit_expr_index_mut(expr),
            Expr::Infer(expr) => self.visit_expr_infer_mut(expr),
            Expr::Let(expr) => self.visit_expr_let_mut(expr),
            Expr::Lit(expr) => self.visit_expr_lit_mut(expr),
            Expr::Loop(expr) => self.visit_expr_loop_mut(expr),
            Expr::Macro(expr) => self.visit_expr_macro_mut(expr),
            Expr::Match(expr) => self.visit_expr_match_mut(expr),
            Expr::MethodCall(expr) => self.visit_expr_method_call_mut(expr),
            Expr::Paren(expr) => self.visit_expr_paren_mut(expr),
            Expr::Path(expr) => self.visit_expr_path_mut(expr),
            Expr::Range(expr) => self.visit_expr_range_mut(expr),
            Expr::Reference(expr) => self.visit_expr_reference_mut(expr),
            Expr::Repeat(expr) => self.visit_expr_repeat_mut(expr),
            Expr::Return(expr) => self.visit_expr_return_mut(expr),
            Expr::Struct(expr) => self.visit_expr_struct_mut(expr),
            Expr::Try(expr) => self.visit_expr_try_mut(expr),
            Expr::TryBlock(expr) => self.visit_expr_try_block_mut(expr),
            Expr::Tuple(expr) => self.visit_expr_tuple_mut(expr),
            Expr::Type(expr) => self.visit_expr_type_mut(expr),
            Expr::Unary(expr) => self.visit_expr_unary_mut(expr),
            Expr::Unsafe(expr) => self.visit_expr_unsafe_mut(expr),
            Expr::While(expr) => self.visit_expr_while_mut(expr),
            Expr::Yield(expr) => self.visit_expr_yield_mut(expr),
            Expr::Verbatim(_) => {}
            #[cfg(syn_no_non_exhaustive)]
            _ => unreachable!(),
            #[cfg(not(syn_no_non_exhaustive))]
            _ => {}
        }
    }
}