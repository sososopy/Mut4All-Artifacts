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

pub struct Replace_Const_Generic_Bound_Expression_408;

impl Mutator for Replace_Const_Generic_Bound_Expression_408 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_Bound_Expression_408"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ConstGenericBoundVisitor {
            rng: thread_rng(),
            mutations_applied: 0,
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait implementations with where clauses containing const generic expression bounds. It transforms boolean expressions within braced blocks to equivalent forms (e.g., swapping comparison operands, adding logical negation) while preserving semantic equivalence. This aims to trigger different const evaluation paths in MIR and expose bugs in the handling of unevaluated consts, particularly in const generic contexts."
    }
}

struct ConstGenericBoundVisitor {
    rng: rand::rngs::ThreadRng,
    mutations_applied: usize,
}

impl VisitMut for ConstGenericBoundVisitor {
    fn visit_item_impl_mut(&mut self, node: &mut syn::ItemImpl) {
        if let Some(where_clause) = &mut node.generics.where_clause {
            for predicate in &mut where_clause.predicates {
                if let syn::WherePredicate::Type(type_pred) = predicate {
                    self.mutate_type_predicate(type_pred);
                }
            }
        }
        syn::visit_mut::visit_item_impl_mut(self, node);
    }

    fn visit_impl_item_mut(&mut self, node: &mut syn::ImplItem) {
        if let syn::ImplItem::Method(method) = node {
            if let Some(where_clause) = &mut method.sig.generics.where_clause {
                for predicate in &mut where_clause.predicates {
                    if let syn::WherePredicate::Type(type_pred) = predicate {
                        self.mutate_type_predicate(type_pred);
                    }
                }
            }
        }
        syn::visit_mut::visit_impl_item_mut(self, node);
    }
}

impl ConstGenericBoundVisitor {
    fn mutate_type_predicate(&mut self, type_pred: &mut syn::PredicateType) {
        if let syn::Type::Path(type_path) = &type_pred.bounded_ty {
            if let Some(segment) = type_path.path.segments.last() {
                if let syn::PathArguments::AngleBracketed(args) = &mut segment.arguments {
                    for arg in &mut args.args {
                        if let syn::GenericArgument::Const(const_arg) = arg {
                            self.mutate_const_argument(const_arg);
                        }
                    }
                }
            }
        }
    }

    fn mutate_const_argument(&mut self, const_arg: &mut syn::Expr) {
        match const_arg {
            syn::Expr::Block(block) => {
                if let Some(stmt) = block.block.stmts.last_mut() {
                    if let syn::Stmt::Expr(expr, _) = stmt {
                        self.mutate_boolean_expression(expr);
                    }
                }
            }
            _ => {}
        }
    }

    fn mutate_boolean_expression(&mut self, expr: &mut syn::Expr) {
        match expr {
            syn::Expr::Binary(binary) => {
                if self.rng.gen_bool(0.5) {
                    self.swap_comparison_operands(binary);
                } else {
                    self.negate_comparison(binary);
                }
                self.mutations_applied += 1;
            }
            syn::Expr::Unary(unary) => {
                if let syn::UnOp::Not(_) = unary.op {
                    if let syn::Expr::Binary(inner_binary) = &mut *unary.expr {
                        self.swap_comparison_operands(inner_binary);
                        *expr = syn::Expr::Binary(inner_binary.clone());
                    }
                }
            }
            _ => {}
        }
    }

    fn swap_comparison_operands(&mut self, binary: &mut syn::ExprBinary) {
        match binary.op {
            syn::BinOp::Lt(_) => {
                binary.op = syn::BinOp::Gt(syn::token::Gt {
                    spans: [binary.op.span(), binary.op.span()],
                });
                std::mem::swap(&mut binary.left, &mut binary.right);
            }
            syn::BinOp::Gt(_) => {
                binary.op = syn::BinOp::Lt(syn::token::Lt {
                    spans: [binary.op.span(), binary.op.span()],
                });
                std::mem::swap(&mut binary.left, &mut binary.right);
            }
            syn::BinOp::Le(_) => {
                binary.op = syn::BinOp::Ge(syn::token::Ge {
                    spans: [binary.op.span(), binary.op.span()],
                });
                std::mem::swap(&mut binary.left, &mut binary.right);
            }
            syn::BinOp::Ge(_) => {
                binary.op = syn::BinOp::Le(syn::token::Le {
                    spans: [binary.op.span(), binary.op.span()],
                });
                std::mem::swap(&mut binary.left, &mut binary.right);
            }
            syn::BinOp::Eq(_) | syn::BinOp::Ne(_) => {
                std::mem::swap(&mut binary.left, &mut binary.right);
            }
            _ => {}
        }
    }

    fn negate_comparison(&mut self, binary: &mut syn::ExprBinary) {
        let new_op = match binary.op {
            syn::BinOp::Lt(_) => syn::BinOp::Ge(syn::token::Ge {
                spans: [binary.op.span(), binary.op.span()],
            }),
            syn::BinOp::Gt(_) => syn::BinOp::Le(syn::token::Le {
                spans: [binary.op.span(), binary.op.span()],
            }),
            syn::BinOp::Le(_) => syn::BinOp::Gt(syn::token::Gt {
                spans: [binary.op.span(), binary.op.span()],
            }),
            syn::BinOp::Ge(_) => syn::BinOp::Lt(syn::token::Lt {
                spans: [binary.op.span(), binary.op.span()],
            }),
            syn::BinOp::Eq(_) => syn::BinOp::Ne(syn::token::Ne {
                spans: [binary.op.span(), binary.op.span()],
            }),
            syn::BinOp::Ne(_) => syn::BinOp::Eq(syn::token::EqEq {
                spans: [binary.op.span(), binary.op.span()],
            }),
            _ => return,
        };
        
        let new_expr = syn::Expr::Unary(syn::ExprUnary {
            attrs: Vec::new(),
            op: syn::UnOp::Not(syn::token::Not {
                spans: [binary.span()],
            }),
            expr: Box::new(syn::Expr::Binary(syn::ExprBinary {
                attrs: binary.attrs.clone(),
                left: binary.left.clone(),
                op: new_op,
                right: binary.right.clone(),
            })),
        });
        
        *binary = if let syn::Expr::Binary(new_binary) = new_expr {
            new_binary
        } else {
            return;
        };
    }
}