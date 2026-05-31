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

pub struct Modify_Enum_Variant_Patterns_155;

impl Mutator for Modify_Enum_Variant_Patterns_155 {
    fn name(&self) -> &str {
        "Modify_Enum_Variant_Patterns_155"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.ident == "main" {
                    continue;
                }
                let mut visitor = EnumPatternVisitor::default();
                visitor.visit_item_fn(item_fn);
                if let Some((enum_path, variant_name)) = visitor.target {
                    let mut replacer = EnumPatternReplacer {
                        enum_path,
                        variant_name,
                        replaced: false,
                    };
                    replacer.visit_item_fn_mut(item_fn);
                }
            }
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(impl_fn) = impl_item {
                        let mut visitor = EnumPatternVisitor::default();
                        visitor.visit_impl_item_fn(impl_fn);
                        if let Some((enum_path, variant_name)) = visitor.target {
                            let mut replacer = EnumPatternReplacer {
                                enum_path,
                                variant_name,
                                replaced: false,
                            };
                            replacer.visit_impl_item_fn_mut(impl_fn);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets enum pattern matching in non-main functions and methods. It identifies a match arm using a specific enum variant and replaces it with a pattern that binds a variable, ensuring the variable is utilized within the arm. This transformation stresses the compiler's pattern matching logic, especially in handling opaque types and subtyping, by introducing potential mismatches and variable bindings that require type inference and pattern resolution."
    }
}

#[derive(Default)]
struct EnumPatternVisitor {
    target: Option<(SynPath, Ident)>,
}

impl<'ast> Visit<'ast> for EnumPatternVisitor {
    fn visit_expr_match(&mut self, node: &'ast ExprMatch) {
        if self.target.is_some() {
            return;
        }
        for arm in &node.arms {
            if let Pat::Path(pat_path) = &arm.pat {
                if let Some(variant_ident) = pat_path.path.segments.last().map(|seg| &seg.ident) {
                    self.target = Some((pat_path.path.clone(), variant_ident.clone()));
                    return;
                }
            }
        }
    }
}

struct EnumPatternReplacer {
    enum_path: SynPath,
    variant_name: Ident,
    replaced: bool,
}

impl VisitMut for EnumPatternReplacer {
    fn visit_expr_match_mut(&mut self, node: &mut ExprMatch) {
        if self.replaced {
            return;
        }
        for arm in &mut node.arms {
            if let Pat::Path(pat_path) = &arm.pat {
                if pat_path.path == self.enum_path {
                    let var_ident = Ident::new("y", Span::call_site());
                    arm.pat = Pat::TupleStruct(PatTupleStruct {
                        attrs: Vec::new(),
                        path: self.enum_path.clone(),
                        pat: PatTuple {
                            attrs: Vec::new(),
                            paren_token: Paren::default(),
                            elems: {
                                let mut elems = Punctuated::new();
                                elems.push(Pat::Ident(PatIdent {
                                    attrs: Vec::new(),
                                    by_ref: None,
                                    mutability: None,
                                    ident: var_ident.clone(),
                                    subpat: None,
                                }));
                                elems
                            },
                        },
                    });
                    let var_expr = Expr::Path(ExprPath {
                        attrs: Vec::new(),
                        qself: None,
                        path: SynPath::from(var_ident),
                    });
                    let process_expr = Expr::Call(ExprCall {
                        attrs: Vec::new(),
                        func: Box::new(Expr::Path(ExprPath {
                            attrs: Vec::new(),
                            qself: None,
                            path: SynPath::from(Ident::new("process", Span::call_site())),
                        })),
                        paren_token: Paren::default(),
                        args: {
                            let mut args = Punctuated::new();
                            args.push(var_expr);
                            args
                        },
                    });
                    arm.body = Box::new(Expr::Block(ExprBlock {
                        attrs: Vec::new(),
                        label: None,
                        block: Block {
                            brace_token: token::Brace::default(),
                            stmts: vec![Stmt::Expr(process_expr, None)],
                        },
                    }));
                    self.replaced = true;
                    return;
                }
            }
        }
    }
}