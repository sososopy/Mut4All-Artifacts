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

pub struct Insert_Const_Generics_In_Closure_Binder_115;

impl Mutator for Insert_Const_Generics_In_Closure_Binder_115 {
    fn name(&self) -> &str {
        "Insert_Const_Generics_In_Closure_Binder_115"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ClosureVisitor {
            const_param_name: Ident::new("N", Span::call_site()),
            const_param_type: Type::Path(TypePath {
                qself: None,
                path: SynPath::from(Ident::new("i32", Span::call_site())),
            }),
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator inserts a const generic parameter into the for<...> binder of closure expressions. It targets closures that have either no binder or only lifetime parameters in their binder. The operator adds a const parameter (e.g., const N: i32) to the binder, either creating a new binder or extending an existing one. This transformation tests the compiler's handling of const generics in closure contexts, potentially triggering type resolution errors, binder validation issues, or unexpected behavior in generic closure inference."
    }
}

struct ClosureVisitor {
    const_param_name: Ident,
    const_param_type: Type,
}

impl VisitMut for ClosureVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Closure(closure) = expr {
            match &closure.bounds {
                Some(BoundLifetimes { lifetimes, .. }) => {
                    if lifetimes.iter().all(|lp| lp.lifetime.ident.to_string().starts_with("'")) {
                        let new_param = GenericParam::Const(syn::ConstParam {
                            const_token: token::Const::default(),
                            ident: self.const_param_name.clone(),
                            colon_token: token::Colon::default(),
                            ty: self.const_param_type.clone(),
                        });
                        let mut new_lifetimes = lifetimes.clone();
                        new_lifetimes.push(LifetimeParam {
                            attrs: Vec::new(),
                            lifetime: Lifetime::new("'a", Span::call_site()),
                            colon_token: None,
                        });
                        closure.bounds = Some(BoundLifetimes {
                            lifetimes: new_lifetimes,
                            ..closure.bounds.clone().unwrap()
                        });
                        if let Some(bounds) = &closure.bounds {
                            let mut params = Vec::new();
                            for lp in &bounds.lifetimes {
                                params.push(GenericParam::Lifetime(lp.clone()));
                            }
                            params.push(new_param);
                            closure.bounds = Some(BoundLifetimes {
                                lifetimes: bounds.lifetimes.clone(),
                                ..bounds.clone()
                            });
                        }
                    }
                }
                None => {
                    let new_param = GenericParam::Const(syn::ConstParam {
                        const_token: token::Const::default(),
                        ident: self.const_param_name.clone(),
                        colon_token: token::Colon::default(),
                        ty: self.const_param_type.clone(),
                    });
                    closure.bounds = Some(BoundLifetimes {
                        lifetimes: Punctuated::new(),
                        for_token: token::For::default(),
                        lt_token: token::Lt::default(),
                        gt_token: token::Gt::default(),
                    });
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}