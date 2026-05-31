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

pub struct Modify_Const_Generic_With_Lifetime_104;

impl Mutator for Modify_Const_Generic_With_Lifetime_104 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_With_Lifetime_104"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(lifetime) = func.sig.generics.params.iter().find_map(|param| {
                    if let GenericParam::Lifetime(lt) = param {
                        Some(lt.lifetime.clone())
                    } else {
                        None
                    }
                }) {
                    let mut visitor = ConstExprVisitor {
                        lifetime,
                        modified: false,
                    };
                    visitor.visit_item_fn_mut(func);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies constant expressions used as generic parameters in functions with const generics. It introduces a lifetime-bound reference to the expression, potentially affecting const evaluation and triggering bugs in const generics."
    }
}

struct ConstExprVisitor {
    lifetime: Lifetime,
    modified: bool,
}

impl VisitMut for ConstExprVisitor {
    fn visit_expr_call_mut(&mut self, i: &mut ExprCall) {
        if self.modified {
            return;
        }
        for arg in &mut i.args {
            if let Expr::Block(expr_block) = arg {
                expr_block.block.stmts.insert(0, Stmt::Local(Local {
                    attrs: Vec::new(),
                    let_token: token::Let { span: Span::call_site() },
                    pat: Pat::Type(PatType {
                        attrs: Vec::new(),
                        pat: Box::new(Pat::Wild(PatWild {
                            attrs: Vec::new(),
                            underscore_token: token::Underscore { span: Span::call_site() },
                        })),
                        colon_token: token::Colon { spans: [Span::call_site()] },
                        ty: Box::new(Type::Reference(TypeReference {
                            and_token: token::And { spans: [Span::call_site()] },
                            lifetime: Some(self.lifetime.clone()),
                            mutability: None,
                            elem: Box::new(Type::Tuple(TypeTuple {
                                paren_token: token::Paren { span: Span::call_site() },
                                elems: Punctuated::new(),
                            })),
                        })),
                    }),
                    init: None,
                    semi_token: token::Semi { spans: [Span::call_site()] },
                }));
                self.modified = true;
            }
        }
        visit_mut::visit_expr_call_mut(self, i);
    }
}