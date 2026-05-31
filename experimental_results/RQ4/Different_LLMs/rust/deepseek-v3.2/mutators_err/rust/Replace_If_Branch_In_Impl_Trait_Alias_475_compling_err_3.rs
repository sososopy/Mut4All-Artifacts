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

pub struct Replace_If_Branch_In_Impl_Trait_Alias_475;

impl Mutator for Replace_If_Branch_In_Impl_Trait_Alias_475 {
    fn name(&self) -> &str {
        "Replace_If_Branch_In_Impl_Trait_Alias_475"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let mut visitor = ReplaceIfBranchVisitor {
            rng: &mut rng,
            mutated: false,
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions with return types that are type aliases for impl Trait. For each if expression within such a function that has both then and else branches, it randomly selects one branch and replaces its contents with an empty block {}, provided the branch is not already empty. This transformation tests the compiler's type inference for opaque return types when control flow branches are altered, potentially triggering bugs in suggestion diagnostics or trait resolution."
    }
}

struct ReplaceIfBranchVisitor<'a, R: Rng> {
    rng: &'a mut R,
    mutated: bool,
}

impl<'a, R: Rng> VisitMut for ReplaceIfBranchVisitor<'a, R> {
    fn visit_item_fn_mut(&mut self, i: &mut ItemFn) {
        let mut has_impl_trait_alias_return = false;
        if let ReturnType::Type(_, return_type) = &i.sig.output {
            if let Type::Path(TypePath { path, .. }) = &**return_type {
                for item in i.block.stmts.iter() {
                    if let Stmt::Item(Item::Type(type_item)) = item {
                        if type_item.ident == path.segments.last().unwrap().ident {
                            if let Type::ImplTrait(_) = *type_item.ty {
                                has_impl_trait_alias_return = true;
                                break;
                            }
                        }
                    }
                }
            }
        }
        if has_impl_trait_alias_return {
            self.visit_block_mut(&mut i.block);
        }
        syn::visit_mut::visit_item_fn_mut(self, i);
    }

    fn visit_expr_if_mut(&mut self, expr_if: &mut ExprIf) {
        if let (Some(then_branch), Some(else_branch)) = (&mut expr_if.then_branch, &mut expr_if.else_branch) {
            let then_is_empty = matches!(&**then_branch, Expr::Block(block) if block.block.stmts.is_empty());
            let else_is_empty = matches!(&*else_branch.1, Expr::Block(block) if block.block.stmts.is_empty());
            if !then_is_empty || !else_is_empty {
                let branch_to_empty = self.rng.gen_bool(0.5);
                if branch_to_empty && !then_is_empty {
                    *then_branch = Box::new(Expr::Block(ExprBlock {
                        attrs: Vec::new(),
                        label: None,
                        block: parse_quote!({}),
                    }));
                    self.mutated = true;
                } else if !branch_to_empty && !else_is_empty {
                    else_branch.1 = Box::new(Expr::Block(ExprBlock {
                        attrs: Vec::new(),
                        label: None,
                        block: parse_quote!({}),
                    }));
                    self.mutated = true;
                }
            }
        }
        syn::visit_mut::visit_expr_if_mut(self, expr_if);
    }
}