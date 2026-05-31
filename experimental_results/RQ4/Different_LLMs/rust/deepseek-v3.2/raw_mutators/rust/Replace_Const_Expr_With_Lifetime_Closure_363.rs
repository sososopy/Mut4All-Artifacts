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

pub struct Replace_Const_Expr_With_Lifetime_Closure_363;

impl Mutator for Replace_Const_Expr_With_Lifetime_Closure_363 {
    fn name(&self) -> &str {
        "Replace_Const_Expr_With_Lifetime_Closure_363"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ConstExprVisitor {
            changed: false,
            lifetime_param: None,
            generic_type: None,
        };
        visitor.visit_file_mut(file);
        if !visitor.changed {
            return;
        }
        let lifetime = visitor.lifetime_param.unwrap_or_else(|| Lifetime::new("'a", Span::call_site()));
        let generic_type = visitor.generic_type.unwrap_or_else(|| Type::Path(TypePath {
            qself: None,
            path: SynPath::from(Ident::new("u32", Span::call_site())),
        }));
        let mut inner_visitor = InnerVisitor {
            lifetime,
            generic_type,
        };
        inner_visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets generic const expressions within the nightly-only generic_const_exprs feature. It identifies functions with generic lifetime parameters and const expressions used as generic arguments. The operator replaces a simple const expression with a block containing a closure that captures the lifetime parameter, creating a complex const expression involving a closure with captured lifetime-dependent types. This transformation aims to trigger normalization issues and const evaluation bugs in the compiler while preserving the original constant value for type compatibility."
    }
}

struct ConstExprVisitor {
    changed: bool,
    lifetime_param: Option<Lifetime>,
    generic_type: Option<Type>,
}

impl VisitMut for ConstExprVisitor {
    fn visit_item_fn_mut(&mut self, i: &mut ItemFn) {
        for param in &i.sig.generics.params {
            match param {
                GenericParam::Lifetime(lp) => {
                    self.lifetime_param = Some(lp.lifetime.clone());
                }
                GenericParam::Type(tp) => {
                    self.generic_type = Some(Type::Path(TypePath {
                        qself: None,
                        path: SynPath::from(tp.ident.clone()),
                    }));
                }
                _ => {}
            }
        }
        if self.lifetime_param.is_some() {
            self.changed = true;
        }
        syn::visit_mut::visit_item_fn_mut(self, i);
    }
}

struct InnerVisitor {
    lifetime: Lifetime,
    generic_type: Type,
}

impl VisitMut for InnerVisitor {
    fn visit_generic_argument_mut(&mut self, ga: &mut GenericArgument) {
        if let GenericArgument::Const(expr) = ga {
            if let Expr::Lit(_) = &**expr {
                let new_expr: Expr = parse_quote!({
                    |x: & #self.lifetime #self.generic_type| { x };
                    #expr
                });
                *expr = Box::new(new_expr);
            }
        }
        syn::visit_mut::visit_generic_argument_mut(self, ga);
    }
}